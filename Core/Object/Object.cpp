#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

Object::Object(Drone* parentShip, uint64_t ID) {
  _ID = ID;
  _parentShip = parentShip;
  
  _maxGeneratedPower.addFrame(-2, power_type_W(0));
  _requestedPower.addFrame(-2, power_type_W(0));
  _maxUseablePower.addFrame(-2, power_type_W(0));
  _maxStorage.addFrame(-2, energy_type_J(0));
  _usedPower.addFrame(-2, power_type_W(0));
  _selfUsedPower.addFrame(-2, power_type_W(0));
  _generatedPower.addFrame(-2, power_type_W(0));

  _energySystem=_parentShip->energySystem.addVertex(0, 0, 0, 0, 0, -1);
}

power_type_W Object::getMaxGeneratedPower(time_type_s time) {
  return _maxGeneratedPower.getAt(time);
}
power_type_W Object::getGeneratedPower(time_type_s time) {
  return _generatedPower.getAt(time);
}
power_type_W Object::getMaxUseablePower(time_type_s time) {
  return _maxUseablePower.getAt(time);
}
power_type_W Object::getRequestedPower(time_type_s time) {
  return _requestedPower.getAt(time);
}
power_type_W Object::getUsedPower(time_type_s time) {
  return _usedPower.getAt(time);
}
power_type_W Object::getSelfUsedPower(time_type_s time) {
  return _selfUsedPower.getAt(time);
}
energy_type_J Object::getMaxEnergy(time_type_s time) {
  return _maxStorage.getAt(time);
}
energy_type_J Object::getStoredEnergy(time_type_s time) {
  return _energySystem->_val.getAt(time);
}
void Object::maxGeneratedPowerChange(time_type_s time, power_type_W power) {
  _maxGeneratedPower.addFrame(time, power);
  _energySystem->_goal = -power;
}
void Object::requestedPowerChange(time_type_s time, power_type_W power) {
  power = maxv(0.0,minv(power, getMaxUseablePower(time)));
  _requestedPower.addFrame(time, power);
  _energySystem->_goal = power;
}
void Object::maxUseablePowerChange(time_type_s time, power_type_W power) {
  _maxUseablePower.addFrame(time, power);
  requestedPowerChange(time, getRequestedPower(time));
}
void Object::maxStorageChange(time_type_s time, energy_type_J energy) {
  _maxStorage.addFrame(time, energy);
  _energySystem->_maxVal = energy;
  _energySystem->_maxCharge = 100000;
  _energySystem->_maxDrain = -100000;
  //energyStoredChange(time, min(energy, getStoredEnergy(time)));
}
#ifdef M_SERVER
void Object::changeSelfBubble(Bubble* newSelf, Game* g) {
  if (_selfBubble) {
    _selfBubble->after = newSelf;
    newSelf->before = _selfBubble;
  }
  _selfBubble = newSelf;
  g->add(_selfBubble);
}
void Object::reflectBubble(Bubble* what, time_type_s time, Game* g) {
  energy_type_J newEnergy = _radius*_radius*PI*what->getFlux(time, getMovement(time).pos);
  if(high(what->originID) == high(_ID)) {
    return;
  }
  if (newEnergy <= BUBBLE_REMOVE) {
    newEnergy = 0; //Low energy bubble only reflected as end.
    if(what->before == NULL) { //If no prev: delete
      return;
    } else if (what->before->energy < BUBBLE_REMOVE) { //If prev was also low energy: delete
      return;
    }
  }
  if (what->bsource == Bubble_Chat) {
    return;
  }

  Bubble* reflected = new Bubble();
  reflected->constrains.push_back(constrain(constrain::include, { 1, 0, 0 }, -2)); //Include all directions
  reflected->bsource = what->bsource;
  reflected->btype = what->btype;
  reflected->emitter = getMovement(time);
  reflected->energy = newEnergy;
  reflected->gEmissionTime = time;
  reflected->originID = _ID;

  if(reflected->btype == BubbleType::Bubble_Row_Border) {
    auto it = reflection.find(what->before);
    if (it != reflection.end()) {
      reflected->before = it->second;
      it->second->after = reflected;
    }
  }
  reflection[what] = reflected;
  g->add(reflected);
}
energy_type_J Object::useEnergy(time_type_s time, energy_type_J amount, Game* g) {
  return -_parentShip->energyUpdate(time, g, this, -amount);
}
energy_type_J Object::chargeEnergy(time_type_s time, energy_type_J amount, Game* g) {
  return _parentShip->energyUpdate(time, g, this, amount);
}
void Object::maxGeneratedPowerChange(time_type_s time, power_type_W power, Game* g) {
  maxGeneratedPowerChange(time, power);
  _parentShip->energyUpdate(time, g);
}
void Object::requestedPowerChange(time_type_s time, power_type_W power, Game* g) {
  requestedPowerChange(time, power);
  _parentShip->energyUpdate(time, g);
}
void Object::maxUseablePowerChange(time_type_s time, power_type_W power, Game* g) {
  maxUseablePowerChange(time, power);
  _parentShip->energyUpdate(time, g);
}
/*void Object::energyStoredChange(time_type_s time, energy_type_J energy, Game* g) {
  energyStoredChange(time, energy);
  _parentShip->energyUpdate(time, g);
}*/
void Object::maxStorageChange(time_type_s time, energy_type_J energy, Game* g) {
  maxStorageChange(time, energy);
  _parentShip->energyUpdate(time, g);
}
#endif


Movement Object::getMovement(time_type_s time) {
  Movement m = _parentShip->mov.getAt(time);
  m.pos = m.pos + _relativePos;
  m.radius = _radius;
  return m;
}
list< pair<time_type_s, pair<Object*, Path*>>> Object::intersect(Path* p) {
  list< pair<time_type_s, pair<Object*, Path*>>> res;
  auto it = _parentShip->mov._frames.begin();
  while (it != _parentShip->mov._frames.end()) {
    Movement m = it->second;
    m.pos += _relativePos;
    m.radius = _radius;
    vector<time_type_s> times = p->intersect(&m);
    for (auto&& itt : times) {
      auto nit = it;
      ++nit;
      if (it->first <= itt && (nit == _parentShip->mov._frames.end() || itt < nit->first)) {
        res.push_back({ itt,{ this, p } });
      }
    }
    ++it;
  }
  return res;
}
void Object::getStatus(DataElement* data) {
  DataElement* typee = new DataElement();
  typee->_core->fromType<int>(type());
  data->addChild(typee);

  DataElement* rele = new DataElement();
  _relativePos.get(rele);
  data->addChild(rele);

  DataElement* maxe = new DataElement();
  _maxHealth.get(maxe);
  data->addChild(maxe);

  DataElement* hele = new DataElement();
  _health.get(hele);
  data->addChild(hele);

  DataElement* rade = new DataElement();
  rade->_core->fromType<distance_type_m>(_radius);
  data->addChild(rade);

  DataElement* ide = new DataElement();
  ide->_core->fromType<uint64_t>(_ID);
  data->addChild(ide);

  DataElement* name = new DataElement();
  name->_core->fromType<string>(_name);
  data->addChild(name);

  DataElement* mgpe = new DataElement();
  _maxGeneratedPower.get(mgpe);
  data->addChild(mgpe);

  DataElement* gpe = new DataElement();
  _generatedPower.get(gpe);
  data->addChild(gpe);

  DataElement* mupe = new DataElement();
  _maxUseablePower.get(mupe);
  data->addChild(mupe);

  DataElement* rpe = new DataElement();
  _requestedPower.get(rpe);
  data->addChild(rpe);

  DataElement* upe = new DataElement();
  _usedPower.get(upe);
  data->addChild(upe);

  DataElement* supe = new DataElement();
  _selfUsedPower.get(supe);
  data->addChild(supe);

  DataElement* mse = new DataElement();
  _maxStorage.get(mse);
  data->addChild(mse);

  DataElement* ese = new DataElement();
  _energySystem->_val.get(ese);
  data->addChild(ese);

  DataElement* vire = new DataElement();
  getVStatus(vire);
  data->addChild(vire);
}
void Object::setStatus(DataElement* data) {
  _relativePos.set(data->_children[1]);

  _maxHealth.set(data->_children[2]);

  _health.set(data->_children[3]);

  _radius = data->_children[4]->_core->toType<float>();

  _ID = data->_children[5]->_core->toType<uint64_t>();

  _name = data->_children[6]->_core->toType<string>();

  _maxGeneratedPower.set(data->_children[7]);

  _generatedPower.set(data->_children[8]);

  _maxUseablePower.set(data->_children[9]);

  _requestedPower.set(data->_children[10]);

  _usedPower.set(data->_children[11]);

  _selfUsedPower.set(data->_children[12]);

  _maxStorage.set(data->_children[13]);

  _energySystem->_val.set(data->_children[14]);

  setVStatus(data->_children[15]);
}

void Object::getVStatus(DataElement* data) {

}
void Object::setVStatus(DataElement* data) {

}

Object::~Object() {

}

void Object::collectEvents(list<StateChange*> &addTo, time_type_s time) {

}

#ifdef M_CLIENT
void Object::setSidebarElement(string filename) {
  bool reset = true;
  if (selectedo == NULL || selectedo->type() != type()) {
    reset = true;
  }
  selectedo = this;
  if (reset) {
    Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), filename);

  }
}
void Object::setSidebar() {
  LOG LERROR GRAPHICS "Unimplemented type " << type() END;
}
list< pair<time_type_s, pair<Object*, Path*>>> Object::getIntersect(vec3<double> ori, vec3<double> dir) {
  Shot p;
  p.origin = ori;
  p.origintime = 0;
  p.vel = dir;
  Movement m;
  m.pos = _relativePos /*+ _parentShip->mov.pos*/;
  m.vel = vel_type_mpers(0);
  m.acc = acc_type_mperss(0);
  m.gTimeStamp = 0;
  m.radius = _radius;
  list< pair<time_type_s, pair<Object*, Path*>>> res;
  vector<time_type_s> times = intersectPaths(&m, &p);
  for (auto&& it : times) {
    res.push_back({ it,{ this, &p } });
  }
  return res;
}
void Object::drawObject(float camcx, float camcy, float camcz, float d, time_type_s time, bool worldView) {
  if(worldView) {
    drawObjectVirt(camcx, camcy, camcz, d, time, worldView);
  } else {
    glTranslated(_relativePos.x.toDouble(), _relativePos.y.toDouble(), _relativePos.z.toDouble());
    setColor(0xffdf0000 + int(0xdf * _health.getAt(time) / float(_maxHealth.getAt(time))) + 0x100 * int(0xdf * _health.getAt(time) / float(_maxHealth.getAt(time))));
    glutSolidSphere(_radius.toDouble(), 20, 20);
    glTranslated(-_relativePos.x.toDouble(), -_relativePos.y.toDouble(), -_relativePos.z.toDouble());
    drawObjectVirt(camcx, camcy, camcz, d, time, worldView);
  }
}
#endif

#ifdef M_SERVER
void Object::energyCallback(time_type_s t, Game* g) {
  if (_energySystem->_delta > 0) {
    _generatedPower.addFrame(t, power_type_W(0));
    _usedPower.addFrame(t, _energySystem->_delta);
    _selfUsedPower.addFrame(t, _energySystem->_firstDelta);
  }
  if (_energySystem->_delta <= 0) {
    _generatedPower.addFrame(t, -_energySystem->_delta);
    _usedPower.addFrame(t, power_type_W(0));
    _selfUsedPower.addFrame(t, power_type_W(0));
  }
  energyCallbackV(t, g);
}
void Object::getPath(time_type_s time, Path* p, Game* g) {
  cout << "Path type: " << p->type() << ", origin:" << p->originID << ", target " << _ID << endl; 
  if ((p->type() == Path::PathTypeShot) && (p->originID != _ID)) {
    Shot* ps = (Shot*)p;
    _health.addFrame(time,
      value<int>(max(
        0,
        _health.getAt(time) - int((ps->energy / 50).toDouble())
        ))); //TODO BETTER
    ps->energy -= 10000;
    ps->energy = max(0, ps->energy);
  }
  if (p->type() == Path::PathTypeBubble) {
    reflectBubble((Bubble*)p, time, g);
  }
  getPathVirt(time, p, g);
}
bool Object::loadEnergy(xml_node<>* data, time_type_s time) {
  maxStorageChange(time, 0);
  _energySystem->_maxCharge = 0;
  _energySystem->_maxDrain = 0;
  maxUseablePowerChange(time, 0);
  maxGeneratedPowerChange(time, 0);

  if(data) {
    xml_node<>* elem;
    elem = data->first_node("max_storage");
    if (elem) {
      maxStorageChange(time, strTo<energy_type_J>(elem->value()));
    }

    elem = data->first_node("max_charge");
    if (elem) {
      _energySystem->_maxCharge = abs(strTo<power_type_W>(elem->value()));
    }

    elem = data->first_node("max_drain");
    if (elem) {
      _energySystem->_maxDrain = -abs(strTo<power_type_W>(elem->value()));
    }

    elem = data->first_node("max_useable_power");
    if (elem) {
      maxUseablePowerChange(time, abs(strTo<power_type_W>(elem->value())));
    }

    elem = data->first_node("max_generated_power");
    if (elem) {
      maxGeneratedPowerChange(time, abs(strTo<power_type_W>(elem->value())));
    }
  }
  return true;
}
bool Object::load(xml_node<>* data, time_type_s time) {
  xml_node<>* elem;

  elem = data->first_node("name");
  if (!elem) {
    return false;
  }
  _name = elem->value();

  elem = data->first_node("radius");
  if (!elem) {
    return false;
  }
  _radius = strTo<distance_type_m>(elem->value());

  elem = data->first_node("pos");
  if (!elem) {
    return false;
  }
  else {
    _relativePos.load(elem);
  }

  elem = data->first_node("max_health");
  if (!elem) {
    return false;
  }
  _maxHealth._frames.clear();
  _maxHealth.addFrame(time, strTo<int>(elem->value()));

  elem = data->first_node("health");
  _health._frames.clear();
  if (!elem) {
    _health.addFrame(time, _maxHealth.getAt(time));
  } else {  
    _health.addFrame(time, strTo<int>(elem->value()));
  }

  elem = data->first_node("energy");
  if (!loadEnergy(elem, time)) {
    return false;
  }
  
  return loadV(data, time);
}
#endif