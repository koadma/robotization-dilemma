#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

Object::Object(Drone* parentShip, uint64_t ID) {
  _ID = ID;
  _parentShip = parentShip;
}

Object::Object(Drone* parentShip, uint64_t ID, mVec3 relativePos, int maxHealth, distance_type_m radius, int health) {
  _parentShip = parentShip;
  _relativePos = relativePos;
  _maxHealth = maxHealth;
  _health.addFrame(0, health);
  _radius = radius;
  _ID = ID;
  _energySystem = _parentShip->energySystem.addVertex(0, 0, 0, 0);

  _maxGeneratedPower.addFrame(-2, 0);
  _requestedPower.addFrame(-2, 0);
  _maxUseablePower.addFrame(-2, 0);
  _maxStorage.addFrame(-2, 0);
  _usedPower.addFrame(-2, 0);
  _selfUsedPower.addFrame(-2, 0);
  _generatedPower.addFrame(-2, 0);
}

power_type_W Object::getMaxGeneratedPower(time_type_s time) {
  return _maxGeneratedPower.getAt(time)();
}
power_type_W Object::getGeneratedPower(time_type_s time) {
  return _generatedPower.getAt(time)();
}
power_type_W Object::getMaxUseablePower(time_type_s time) {
  return _maxUseablePower.getAt(time)();
}
power_type_W Object::getRequestedPower(time_type_s time) {
  return _requestedPower.getAt(time)();
}
power_type_W Object::getUsedPower(time_type_s time) {
  return _usedPower.getAt(time)();
}
power_type_W Object::getSelfUsedPower(time_type_s time) {
  return _selfUsedPower.getAt(time)();
}
energy_type_J Object::getMaxEnergy(time_type_s time) {
  return _maxStorage.getAt(time)();
}
energy_type_J Object::getStoredEnergy(time_type_s time) {
  return _energySystem->_val.getAt(time);
}
void Object::maxGeneratedPowerChange(time_type_s time, power_type_W power) {
  _maxGeneratedPower.addFrame(time, power);
  _energySystem->_goal = -power;
}
void Object::requestedPowerChange(time_type_s time, power_type_W power) {
  power = max(0.0,min(power, getMaxUseablePower(time)));
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
list< pair<double, pair<Object*, Path*>>> Object::intersect(Path* p) {
  list< pair<double, pair<Object*, Path*>>> res;
  auto it = _parentShip->mov._frames.begin();
  while (it != _parentShip->mov._frames.end()) {
    Movement m = it->second;
    m.pos += _relativePos;
    m.radius = _radius;
    vector<double> times = intersectPaths(p, &m);
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
  maxe->_core->fromType<int>(_maxHealth);
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

  _maxHealth = data->_children[2]->_core->toType<int>();

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
  cout << "Unimplemented type " << type() << endl;
}
list< pair<double, pair<Object*, Path*>>> Object::getIntersect(vec3<double> ori, vec3<double> dir) {
  Shot p;
  p.origin = ori;
  p.origintime = 0;
  p.vel = dir;
  Movement m;
  m.pos = _relativePos /*+ _parentShip->mov.pos*/;
  m.vel = 0;
  m.acc = 0;
  m.gTimeStamp = 0;
  m.radius = _radius;
  list< pair<double, pair<Object*, Path*>>> res;
  vector<double> times = intersectPaths(&m, &p);
  for (auto&& it : times) {
    res.push_back({ it,{ this, &p } });
  }
  return res;
}
void Object::drawObject(float camcx, float camcy, float camcz, float d, time_type_s time, bool worldView) {
  if(worldView) {
    drawObjectVirt(camcx, camcy, camcz, d, time, worldView);
  } else {
    glTranslated(_relativePos.x, _relativePos.y, _relativePos.z);
    setColor(0xffdf0000 + int(0xdf * _health.getAt(time)() / float(_maxHealth)) + 0x100 * int(0xdf * _health.getAt(time)() / float(_maxHealth)));
    glutSolidSphere(_radius, 20, 20);
    glTranslated(-_relativePos.x, -_relativePos.y, -_relativePos.z);
    drawObjectVirt(camcx, camcy, camcz, d, time, worldView);
  }
}
#endif

#ifdef M_SERVER
void Object::energyCallback(time_type_s t, Game* g) {
  if (_energySystem->_delta > Fraction(0)) {
    _generatedPower.addFrame(t, 0);
    _usedPower.addFrame(t, double(_energySystem->_delta));
    _selfUsedPower.addFrame(t, double(_energySystem->_firstDelta));
  }
  if (_energySystem->_delta < Fraction(0)) {
    _generatedPower.addFrame(t, -double(_energySystem->_delta));
    _usedPower.addFrame(t, 0);
    _selfUsedPower.addFrame(t, 0);
  }
  energyCallbackV(t, g);
}
void Object::getPath(time_type_s time, Path* p, Game* g) {
  if ((p->type() == Path::PathTypeShot) && (p->originID != _ID)) {
    Shot* ps = (Shot*)p;
    _health.addFrame(time,
      value<int>(max(
        0,
        _health.getAt(time)() - int(ps->energy / 50)
        ))); //TODO BETTER
    ps->energy -= 10000;
    ps->energy = max(0, ps->energy);
  }
  if ((p->type() == Path::PathTypeBubble) && (high(p->originID) != high(_ID)) && _radius*_radius*PI*((Bubble*)p)->getFlux(time) > BUBBLE_REMOVE && ((Bubble*)p)->btype != Bubble::Chat) { //Thermal and ping are reflected
    Bubble* b = new Bubble();
    b->constrains.push_back(Bubble::constrain(Bubble::constrain::include, {1, 0, 0}, -2)); //Include all directions
    b->btype = ((Bubble*)p)->btype;
    b->emitter = getMovement(time);
    b->energy = _radius*_radius*PI*((Bubble*)p)->getFlux(time);
    b->gEmissionTime = time;
    b->origin = getMovement(time).pos;
    b->originID = _ID;

    g->calcIntersect(b);
    g->paths.push_back(b);
  }
  getPathVirt(time, p, g);
}
bool Object::load(xml_node<>* data, time_type_s time) {
  xml_node<>* elem;

  elem = data->first_node("health");
  if (!elem) {
    return false;
  }
  _health._frames.clear();
  _health.addFrame(0, strTo<int>(elem->value()));

  elem = data->first_node("max_health");
  if (!elem) {
    return false;
  }
  _health._frames.clear();
  _health.addFrame(0, strTo<int>(elem->value()));

  elem = data->first_node("max_storage");
  _maxStorage._frames.clear();
  if (!elem) {
    maxStorageChange(0, 0);
  } else {
    maxStorageChange(0, strTo<energy_type_J>(elem->value()));
  }

  elem = data->first_node("max_charge");
  if (!elem) {
    _energySystem->_maxCharge = 0;
  } else {
    _energySystem->_maxCharge = Fraction(strTo<power_type_W>(elem->value()));
  }

  elem = data->first_node("max_drain");
  if (!elem) {
    _energySystem->_maxDrain = 0;
  } else {
    _energySystem->_maxDrain = -Fraction(strTo<power_type_W>(elem->value()));
  }

  elem = data->first_node("max_useable_power");
  if (!elem) {
    maxUseablePowerChange(time, 0);
  }
  else {
    maxUseablePowerChange(time, strTo<power_type_W>(elem->value()));
  }

  elem = data->first_node("max_generated_power");
  if (!elem) {
    maxGeneratedPowerChange(time, 0);
  }
  else {
    maxGeneratedPowerChange(time, -strTo<power_type_W>(elem->value()));
  }


}
#endif