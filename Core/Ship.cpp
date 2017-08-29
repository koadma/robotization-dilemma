#include "Ship.h"

#ifdef M_SERVER
#include "../Server/Game.h"
#endif

#ifdef M_SERVER
void Event::apply(Game *g) {
  throw 1;
}
void Collision::apply(Game *g) {
  _o->getPath(_time, _p, g);
}
void BatteryDrain::apply(Game *g) {
  cout << __FILE__ << ":" << __LINE__ << " Ship ran out of energy!" << endl;
}
void StateChange::apply(Game *g) {
  throw 1;
}
void StateChange::set(DataElement* data, Game* game) {
  _o = game->getObject(data->_children[1]->_core->toType<uint64_t>());
  if (_o == NULL) {
    cout << __FILE__ << ":" << __LINE__ << " Unknown Object ID " << data->_children[1]->_core->toType<uint64_t>() << endl;
    throw 1;
  }
  _time = data->_children[2]->_core->toType<float>();

  setV(data->_children[3], game);
}
void StateChange::setV(DataElement* data, Game* game) {

}
void EngineAcc::apply(Game *g) {
  reinterpret_cast<Engine*>(_o)->setAccel(_time, _acc);

  g->removeIntersect(_o->parentShip);
  g->calcIntersect(_o->parentShip); //recalculate ship related future intersections

  ThermalRadiation ev;
  ev._o = _o;
  ev._time = _time;
  ev.apply(game);
}
void EngineAcc::setV(DataElement* data, Game* game) {
  _acc.set(data->_children[0]);
}
void SensorPow::apply(Game *g) {
  reinterpret_cast<Sensor*>(_o)->setPower(_time, _power);
  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info
  //g.removeIntersect(_o->parentShip);
  //g.calcIntersect(_o->parentShip); //recalculate ship related future intersections
}
void SensorPow::setV(DataElement* data, Game* game) {
  _power = data->_children[0]->_core->toType<power_type_W>();
}
void LaserShot::apply(Game *g) {
  Shot* s = new Shot();
  s->energy = _energy/* - _o->useEnergy(_time, _energy)*/;///TODO When energy system is stabilized, enable
  s->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  s->originID = _o->getId();
  s->origintime = _time;
  s->vel = _dir;

  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info

  g->paths.push_back(s);
  g->calcIntersect(s);
}
void LaserShot::setV(DataElement* data, Game* game) {
  _dir.set(data->_children[0]);
  
  _energy = data->_children[1]->_core->toType<energy_type_J>();
}
void ThermalRadiation::apply(Game *g) {
  Bubble* b = new Bubble();
  b->btype = Bubble::Thermal;
  b->emitter = _o->getMovement(_time);
  b->energy = _o->getUsedPower(_time);
  b->gEmissionTime = _time;
  b->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  b->originID = _o->getId();

  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info

  g->paths.push_back(b);
  g->calcIntersect(b);
}
void ThermalRadiation::setV(DataElement* data, Game* game) {

}
void SensorPing::apply(Game *g) {
  Bubble* b = new Bubble();
  b->btype = Bubble::Ping;
  b->emitter = _o->getMovement(_time);
  b->energy = _energy;
  b->gEmissionTime = _time;
  b->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  b->originID = _o->getId();

  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info

  g->paths.push_back(b);
  g->calcIntersect(b);
}
void SensorPing::setV(DataElement* data, Game* game) {
  _energy = data->_children[0]->_core->toType<energy_type_J>();
}
#endif

int Event::type() {
  return EvTEvent;
}
int Collision::type() {
  return EvTCollision;
}
int BatteryDrain::type() {
  return EvTBatteryDrain;
}
int StateChange::type() {
  return EvTStateChange;
}
void StateChange::getV(DataElement* data) {

}
void StateChange::get(DataElement* data) {
  DataElement* ide = new DataElement();
  ide->_core->fromType<uint64_t>(type());
  data->addChild(ide);

  DataElement* oe = new DataElement();
  oe->_core->fromType<uint64_t>(_o->getId());
  data->addChild(oe);

  DataElement* timee = new DataElement();
  timee->_core->fromType<time_type_s>(_time);
  data->addChild(timee);

  DataElement* virte = new DataElement();
  getV(virte);
  data->addChild(virte);
}
int EngineAcc::type() {
  return EvTEngineAcc;
}
void EngineAcc::getV(DataElement* data) {
  DataElement* acce = new DataElement();
  _acc.get(acce);
  data->addChild(acce);
}
int SensorPow::type() {
  return EvTSensorPow;
}
void SensorPow::getV(DataElement* data) {
  DataElement* powe = new DataElement();
  powe->_core->fromType<power_type_W>(_power);
  data->addChild(powe);
}
int LaserShot::type() {
  return EvTLaserShot;
}
void LaserShot::getV(DataElement* data) {
  DataElement* dire = new DataElement();
  _dir.get(dire);
  data->addChild(dire);
  
  DataElement* enee = new DataElement();
  enee->_core->fromType<energy_type_J>(_energy);
  data->addChild(enee);
}
int ThermalRadiation::type() {
  return EvTThermalRadiation;
}
void ThermalRadiation::getV(DataElement* data) {

}
int SensorPing::type() {
  return EvTSensorPing;
}
void SensorPing::getV(DataElement* data) {
  DataElement* enee = new DataElement();
  enee->_core->fromType<energy_type_J>(_energy);
  data->addChild(enee);
}

using namespace std;

Movement Object::getMovement(time_type_s time) {
  Movement m = parentShip->mov.getAt(time);
  m.pos = m.pos + _relativePos;
  m.radius = _radius;
  return m;
}
list< pair<double, pair<Object*, Path*>>> Object::intersect(Path* p) {
  list< pair<double, pair<Object*, Path*>>> res;
  auto it = parentShip->mov._frames.begin();
  while (it != parentShip->mov._frames.end()) {
    Movement m = it->second;
    m.pos += _relativePos;
    m.radius = _radius;
    vector<double> times = intersectPaths(p, &m);
    for (auto&& itt : times) {
      auto nit = it;
      ++nit;
      if (it->first <= itt && (nit == parentShip->mov._frames.end() || itt < nit->first)) {
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
  
  setVStatus(data->_children[7]);
}
void Object::getVStatus(DataElement* data) {

}
void Object::setVStatus(DataElement* data) {

}
void Sensor::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  _power.get(enee);
  data->addChild(enee);

  DataElement* maxe = new DataElement();
  maxe->_core->fromType<power_type_W>(_maxPower);
  data->addChild(maxe);

  DataElement* pinge = new DataElement();
  for (auto&& it : _pings) {
    DataElement* ne = new DataElement();

    DataElement* timee = new DataElement();
    vGFunc<time_type_s>(get<0>(it), timee);
    ne->addChild(timee);

    DataElement* enee = new DataElement();
    vGFunc<energy_type_J>(get<1>(it), enee);
    ne->addChild(enee);

    DataElement* usede = new DataElement();
    vGFunc<bool>(get<2>(it), usede);
    ne->addChild(usede);

    pinge->addChild(ne);
  }
  data->addChild(pinge);
}
void Sensor::setVStatus(DataElement* data) {
  _power.set(data->_children[0]);
  _maxPower = data->_children[1]->_core->toType<float>();

  _pings.clear();

  for (auto&& it : data->_children[2]->_children) {

    tuple<time_type_s, energy_type_J, bool> nVal;

    vSFunc<time_type_s>(get<0>(nVal), it->_children[0]);
    vSFunc<energy_type_J>(get<1>(nVal), it->_children[1]);
    vSFunc<bool>(get<2>(nVal), it->_children[2]);

    _pings.push_back(nVal);
  }
}
void Engine::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  maxe->_core->fromType<power_type_W>(_maxPower);
  data->addChild(maxe);

  DataElement* acce = new DataElement();
  _accel.get(acce);
  data->addChild(acce);
}
void Engine::setVStatus(DataElement* data) {
  _maxPower = data->_children[0]->_core->toType<float>();
  _accel.set(data->_children[1]);
}
void Generator::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  _maxPower.get(maxe);
  data->addChild(maxe);

  DataElement* store = new DataElement();
  _energyStored.get(store);
  data->addChild(store);

  DataElement* mstoe = new DataElement();
  _maxStorage.get(mstoe);
  data->addChild(mstoe);
}
void Generator::setVStatus(DataElement* data) {
  _maxPower.set(data->_children[0]);
  _energyStored.set(data->_children[1]);
  _maxStorage.set(data->_children[2]);
}
void Laser::getVStatus(DataElement* data) {
  DataElement* shote = new DataElement();
  for (auto&& it : _shots) {
    DataElement* ne = new DataElement();
    
    DataElement* timee = new DataElement();
    vGFunc<time_type_s>(get<0>(it), timee); 
    ne->addChild(timee);

    DataElement* enee = new DataElement();
    vGFunc<energy_type_J>(get<1>(it), enee);
    ne->addChild(enee);

    DataElement* dire = new DataElement();
    get<2>(it).get(dire);
    ne->addChild(dire);

    DataElement* usede = new DataElement();
    vGFunc<bool>(get<3>(it), usede);
    ne->addChild(usede);

    shote->addChild(ne);
  }
  data->addChild(shote);

  DataElement* store = new DataElement();
  _energyStored.get(store);
  data->addChild(store);

  DataElement* mstoe = new DataElement();
  _maxStorage.get(mstoe);
  data->addChild(mstoe);
}
void Laser::setVStatus(DataElement* data) {
  _shots.clear();

  for (auto&& it : data->_children[0]->_children) {

    tuple<time_type_s, energy_type_J, sVec3, bool> nVal;

    vSFunc<time_type_s>(get<0>(nVal), it->_children[0]);
    vSFunc<energy_type_J>(get<1>(nVal), it->_children[1]);
    get<2>(nVal).set(it->_children[2]);
    vSFunc<bool>(get<3>(nVal), it->_children[3]);

    _shots.push_back(nVal);
  }

  _energyStored.set(data->_children[1]);
  _maxStorage.set(data->_children[2]);

}

#ifdef M_SERVER
void Object::getPath(time_type_s time, Path* p, Game* g) {
  if ((p->type() == Path::PathTypeShot) && (p->originID != _ID)) {
    _health.addFrame(time,
      value<int>(max(
        0,
        _health.getAt(time)() - int(((Shot*)p)->energy / 50)
        ))); //TODO BETTER
  }
  if ((p->type() == Path::PathTypeBubble) && (high(p->originID) != high(_ID)) && ((Bubble*)p)->getFlux(time) > BUBBLE_REMOVE && ((Bubble*)p)->btype != Bubble::Chat) { //Thermal and ping are reflected
    Bubble* b = new Bubble();
    b->btype = ((Bubble*)p)->btype;
    b->emitter = getMovement(time);
    b->energy = _radius*_radius*PI*((Bubble*)p)->getFlux(time);
    b->gEmissionTime = time;
    b->origin = getMovement(time).pos;
    b->originID = _ID;

    g->calcIntersect(b);
    g->paths.push_back(b);
  }
  getPathVirt(time, p);
}
void Sensor::getPathVirt(time_type_s time, Path* p) {
  if (p->type() == Path::PathTypeBubble) {
    float h = _health.getAt(time)() / float(_maxHealth);
    cout << "SENSOR " << _ID << " EN " << reinterpret_cast<Bubble*>(p)->energy << " FROM " << p->originID << endl;
    if (ran1() < 2 * (1 - 1 / (1 + h))) {
      float e = reinterpret_cast<Bubble*>(p)->energy * _power.getAt(time)();
      if (ran1() < 1 / (1 + pow(2, (1 / e - e) * 3))) { //detect
        cout << "Detected" << endl;
        Sighting* s = new Sighting();
        Movement m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
        parentShip->sightMovement(m, time);
      }
    }
  }
}
#endif

void Engine::setAccel(time_type_s time, mpssVec3 acc) {
  Movement m = parentShip->mov.getAt(time);
  m.acc = parentShip->getAccel(time);

  _accel.addFrame(time, acc);

  m.acc += acc;
  parentShip->mov.addFrame(time, m);

  parentShip->refreshEnergy(time); //recalculate ship energy info
}
void Engine::setComponent(time_type_s time, int c, acc_type_mperss val) {
  mpssVec3 nval = _accel.getAt(time)();
  nval[c] = val;
  setAccel(time, nval);
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
  m.pos = _relativePos /*+ parentShip->mov.pos*/;
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
void Object::drawObject(float camcx, float camcy, float camcz, float d, time_type_s time) {
  glTranslated(_relativePos.x, _relativePos.y, _relativePos.z);
  setColor(0xffdf0000 + int(0xdf * _health.getAt(time)() / float(_maxHealth)) + 0x100 * int(0xdf * _health.getAt(time)() / float(_maxHealth)));
  glutSolidSphere(_radius, 20, 20);
  glTranslated(- _relativePos.x, - _relativePos.y, - _relativePos.z);
  drawObjectVirt(camcx, camcy, camcz, d, time);
}
void Sensor::setSidebar() {
  setSidebarElement("html/sensor_settings.xml");
  
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarPowerLabel"))->text = " / " + to_string(_maxPower, 0);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectSensorSidebarPowerInput"))->text = to_string(_power.getAt(timeNow)(), 2);
}
void Generator::setSidebar() {
  setSidebarElement("html/generator_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text =
    new TextBind<
    TextBindFunc<power_type_W>
    >("Max: %",
      TextBindFunc<power_type_W>(getCurrentMaxPower)
      );
}
void Engine::setSidebar() {
  setSidebarElement("html/engine_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectEngineSidebarEnergyLabel"))->text =
    new TextBind<
    TextBindFunc<power_type_W>,
    TextBindFunc<power_type_W>
    >("% / %",
      TextBindFunc<power_type_W>(getCurrentUsedPower),
      TextBindFunc<power_type_W>(getCurrentMaxPower)
      );
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputX"))->text = to_string(_accel.getAt(timeNow)().x, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputY"))->text = to_string(_accel.getAt(timeNow)().y, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputZ"))->text = to_string(_accel.getAt(timeNow)().z, 2);
}
void Laser::setSidebar() {
  setSidebarElement("html/laser_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectLaserSidebarSurefireLabel"))->text =
    new TextBind<
    TextBindFunc<string>
    >("%",
      TextBindFunc<string>(isSurefire)
      );
}
void Laser::drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time) {
  for (auto&& it : _shots) {
    if (time - ROUND_TIME <= get<0>(it) && get<0>(it) <= time + ROUND_TIME) {
      if(time < get<0>(it)) {
        glColor3f(0.6f, 0.0f, 0.0f);
      } else {
        glColor3f(1.0f, 0.0f, 0.0f);
      }
      glLineWidth(2.0f);
      glBegin(GL_LINES);
      mVec3 p = getMovement(time).pos;
      glVertex3d(p.x, p.y, p.z);
      sVec3 dvec = get<2>(it);
      dvec.norm();
      dvec *= (get<1>(it) / 100.0f + _radius);
      glVertex3d(p.x + dvec.x, p.y + dvec.y, p.z + dvec.z);
      glEnd();
    }
  }
}
void Ship::setSidebar() {
  selectedo = NULL;
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/ship_settings.xml");
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectShipSidebarEnergyLabel"))->text =
    new TextBind<
    TextBindFunc<power_type_W>,
    TextBindFunc<power_type_W>
    >("% / %",
      TextBindFunc<power_type_W>(getCurrentUsedPower),
      TextBindFunc<power_type_W>(getCurrentGeneratedPower)
      );
}
#endif

Object::~Object() {

}

void Object::collectEvents(list<StateChange*> &addTo, time_type_s time) {

}
void Sensor::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for(auto&& it : _power._frames) {
    SensorPow* ev = new SensorPow();
    ev->_power = it.second();
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
  }

  for (auto&& it : _pings) {
    if (time <= get<0>(it) && get<2>(it) == false) {
      SensorPing* ev = new SensorPing();
      ev->_energy = get<1>(it);
      ev->_o = this;
      ev->_time = get<0>(it);
      addTo.push_back(ev);
    }
    get<2>(it) = true;
  }
}
void Engine::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _accel._frames) {
    EngineAcc* ev = new EngineAcc();
    ev->_acc = it.second();
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
  }
}
void Laser::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for(auto&& it : _shots) {
    if(time <= get<0>(it) && get<3>(it) == false) {
      LaserShot* ev = new LaserShot();
      ev->_dir = get<2>(it);
      ev->_energy = get<1>(it);
      ev->_o = this;
      ev->_time = get<0>(it);
      addTo.push_back(ev);
    }
    get<3>(it) = true;
  }
}
void Generator::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  ThermalRadiation* ev = new ThermalRadiation();
  ev->_o = this;
  addTo.push_back(ev);
}

power_type_W Drone::getGeneratedShipPower(time_type_s time) {
  power_type_W sum = 0;
  for(auto&& it : objects) {
    sum += it->getGeneratedPower(time);
  }
  return sum;
}
power_type_W Drone::getUnusedShipPower(time_type_s time) {
  return getGeneratedShipPower(time) - getUsedShipPower(time);
}
power_type_W Drone::getUsedShipPower(time_type_s time) {
  power_type_W sum = 0;
  for (auto it : objects) {
    sum += it->getUsedPower(time);
  }
  return sum;
}
energy_type_J Drone::getMaxShipEnergy(time_type_s time) {
  energy_type_J sum = 0;
  for (auto it : objects) {
    sum += it->getMaxEnergy(time);
  }
  return sum;
}
energy_type_J Drone::getStoredShipEnergy(time_type_s time) {
  energy_type_J sum = 0;
  for (auto it : objects) {
    sum += it->getStoredEnergy(time);
  }
  return sum;
}

energy_type_J Drone::useEnergy(time_type_s time, energy_type_J amount) {
  refreshEnergy(time);
  for (auto it : objects) {
    amount = it->useEnergy(time, amount);
  }
  return amount;
}
energy_type_J Drone::chargeEnergy(time_type_s time, energy_type_J amount) {
  for (auto it : objects) {
    amount = it->chargeEnergy(time, amount);
  }
  return amount;
}
void Drone::refreshEnergy(time_type_s time) {
  chargeEnergy(0, getUnusedShipPower(0) * (time - lastEvtTime));
  lastEvtTime = time;
}
Event* Drone::runOut() {
  BatteryDrain* ev = new BatteryDrain();
  ev->_time = lastEvtTime - getStoredShipEnergy(0) / getUnusedShipPower(0);
  return ev;
}

int Drone::getHealth(time_type_s time) {
  int sum = 0;
  for (auto&& it : objects) {
    sum += it->getHealth(time);
  }
  return sum;
}
int Drone::getMaxHealth(time_type_s time) {
  int sum = 0;
  for (auto&& it : objects) {
    sum += it->getMaxHealth(time);
  }
  return sum;
}

void Drone::sightMovement(Movement& m, time_type_s time) {
  priority_queue<pair<scalar_type, Sighting*>> dists; //
  for (auto&& it : sightings) {
    dists.push({-(it->getAt(time).pos - m.getAt(time).pos).sqrlen() / (m.radius * it->getAt(time).radius), it});
  }
  if (dists.size()) {
    pair<scalar_type, Sighting*> elem = dists.top();
    if(- elem.first < 1) {
      elem.second->addFrame(time, m);
    }
    else {
      Sighting* s = new Sighting();;
      s->addFrame(time, m);
      sightings.push_back(s);
    }
  }
  else {
    Sighting* s = new Sighting();
    s->addFrame(time, m);
    sightings.push_back(s);
  }
}

#ifdef M_SERVER

void Ship::newTurn(int id) {
  canMove = true;

  DataElement* de = new DataElement();
  de->_core->fromType<int>(id);
  connectedClient->SendData(de, PacketNewRound);
}
bool Ship::packetRecv(DataElement *Data, int Id, NetworkS* thisptr) {
  DataElement* rese;
  switch (Id) {
  case PacketCommand:
    if (canMove) {
      
      }
    break;
  case PacketCommit:
    if (canMove) {
      canMove = false;
      
      for(auto&& it : Data->_children) {
        int typ = it->_children[0]->_core->toType<int>();

        StateChange* nObj = NULL;
        switch (typ) {
        case Event::Type::EvTEvent:
          //nObj = new Event();
          break;
        case Event::Type::EvTCollision:
          //nObj = new Collision();
          break;
        case Event::Type::EvTStateChange:
          //nObj = new StateChange();
          break;
        case Event::Type::EvTEngineAcc:
          nObj = new EngineAcc();
          break;
        case Event::Type::EvTSensorPow:
          nObj = new SensorPow();
          break;
        case Event::Type::EvTLaserShot:
          nObj = new LaserShot();
          break;
        case Event::Type::EvTThermalRadiation:
          nObj = new ThermalRadiation();
          break;
        case Event::Type::EvTBatteryDrain:
          //nObj = new BatteryDrain();
          break;
        case Event::Type::EvTSensorPing:
          nObj = new SensorPing();
          break;
        }
        if (nObj != NULL) {
          nObj->set(it, game);
          game->events.insert(nObj);
        }
        else {
          cout << __FILE__ << " " << __LINE__ << ": Wrong type " << typ << endl;
        }
      }

      game->moveMade();
    }
    else {

    }
    break;
  case PacketSensor:
    rese = new DataElement();
    getSightings(rese);
    connectedClient->SendData(rese, PacketSensor);
    break;
  case PacketCommandHistory:

    break;
  case PacketShipData:
    rese = new DataElement();
    getStatus(rese);
    connectedClient->SendData(rese, PacketShipData);
    break;
  }
  return 0;
}
void Ship::collectPath(list<Path*> &addTo, float time) {
  for (auto it : objects) {
    //it->collectPath(addTo, time);
  }
}
#endif
#ifdef M_CLIENT
void Ship::commit() {
  if(canMove) {
    canMove = false;

    DataElement* events = new DataElement();

    list<StateChange*> evs;
    for (auto it : objects) {
      it->collectEvents(evs, lastEvtTime);
    }

    for(auto&& it : evs) {
      DataElement* ev = new DataElement();
      it->get(ev);
      events->addChild(ev);
    }

    connectedServer->SendData(events, PacketCommit);
  }
}
void Ship::newTurn(int id) {
  canMove = true;
  lastEvtTime = (id - 1) * ROUND_TIME;
  renderNewRound(id);
  timeNow = ROUND_TIME*(id - 1);
}
void Ship::drawSightings(float camcx, float camcy, float camcz, float d) {
  auto it = sightings.begin();

  while (it != sightings.end()) {
    (*it)->drawSighting(camcx, camcy, camcz, d, SOL, timeNow, *it == selecteds);
    ++it;
  }
}
void Ship::drawObjects(float camcx, float camcy, float camcz, float d, bool b) {
  auto it = objects.begin();

  if(b) {
    glTranslatef(mov.getAt(timeNow).pos.x, mov.getAt(timeNow).pos.y, mov.getAt(timeNow).pos.z);
  }
  while (it != objects.end()) {
    (*it)->drawObject(camcx, camcy, camcz, d, timeNow);
    ++it;
  }
  if (b) {
    glTranslatef(-mov.getAt(timeNow).pos.x, -mov.getAt(timeNow).pos.y, -mov.getAt(timeNow).pos.z);
  }
}
bool Ship::packetRecv(DataElement *Data, int Id, NetworkC* thisptr) {
  switch (Id) {
  case PacketNewRound:
    int nturn;
    nturn  = Data->_core->toType<int>();
    newTurn(nturn);
    reinterpret_cast<Graphics::ButtonHwnd>(Graphics::getElementById("objectMainGameCommitButton"))->text = "Commit (" + to_string(nturn) + ")";
    thisptr->SendData(new DataElement(), PacketSensor);
    thisptr->SendData(new DataElement(), PacketShipData);
    break;
  case PacketGameOver:
    createMainMenu();
    break;
  case PacketCommand:
    //createMainMenu();
    break;
  case PacketSensor:
    setSightings(Data);
    glutPostRedisplay();
    break;
  case PacketShipData:
    setStatus(Data);
    glutPostRedisplay();
    break;
  case PacketCommandHistory: ///TODO

    break;

  }
  return 0;
}
void Ship::selectSighting(vec3<double> ori, vec3<double> dir) {
  list<pair<double, Sighting*>> inters;

  Shot p;
  p.origin = ori;
  p.origintime = 0;
  p.vel = dir;

  auto it = sightings.begin();

  while (it != sightings.end()) {
    if((*it)->getFirst() < timeNow) {
      Movement m = (*it)->getAt(timeNow);
      m.acc = { 0,0,0 };
      m.vel = { 0,0,0 };
      vector<double> temp = intersectPaths(&m, &p);
      for (auto&& itt : temp) {
        inters.push_back({ itt, *it });
      }
    }
    ++it;
  }

  inters.sort();

  if (inters.size()) {
    selecteds = inters.begin()->second;
    //inters.begin()->second->select();
  }
  else {
    selecteds = NULL;
    //selectSighting();
  }
}

#endif
/*Object* Ship::getObject(int type) {
  auto it = objects.begin();
  while (it != objects.end()) {
    if ((*it)->type == type) {
      return *it;
    }
    ++it;
  }
  return NULL;
}
Object* Ship::getSensor() {
  return getObject(Type::Sensor);
}
Object* Ship::getGenerator() {
  return getObject(Type::Generator);
}
void  Ship::setSensorEnergy(float energy) {
  getSensor()->energy = energy;
}*/

void Ship::getStatus(DataElement* data) {
  DataElement* pose = new DataElement();
  mov.get(pose);
  data->_children.push_back(pose);

  for (auto it : objects) {
    DataElement* ne = new DataElement();
    it->getStatus(ne);
    data->_children.push_back(ne);
  }
}
void Ship::setStatus(DataElement* data) {
  clearObjects();

  mov.set(data->_children[0]);

  auto it = data->_children.begin();
  ++it;

  while(it != data->_children.end()) {
    int typ = (*it)->_children[0]->_core->toType<int>();

    Object* nObj = NULL;
    switch(typ) {
    case Object::Type::Ship:
      //nObj = new Ship(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Shield: 
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Sensor:
      nObj = new Sensor(fVec3(), 0, 0, 0, 0, 0);
      break;
    case Object::Type::Computer:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Generator:
      nObj = new Generator(fVec3(), 0, 0, 0, 0, 0);
      break;
    case Object::Type::Storage:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Engine:
      nObj = new Engine(fVec3(), 0, 0, 0, 0, fVec3(), 0);
      break;
    case Object::Type::Laser:
      nObj = new Laser(fVec3(), 0, 0, 0, 0);
      break;
    }
    if(nObj != NULL) {
      nObj->parentShip = this;
      nObj->setStatus(*it);
      objects.push_back(nObj);
    } else {
      cout << __FILE__ << " " << __LINE__ << ": Wrong type " << typ << endl;
    }

    ++it;
  }

#ifdef M_CLIENT
  setSidebar();
#endif // M_CLIENT


}
void Ship::getSightings(DataElement* data) {
  for (auto it : sightings) {
    DataElement* ne = new DataElement();
    it->get(ne);
    data->_children.push_back(ne);
  }
}
void Ship::setSightings(DataElement* data) {
  clearSightings();

  for (DataElement* it : data->_children) {
    Sighting* nSig = new Sighting;
    nSig->set(it);
    sightings.push_back(nSig);
  }
}
void Ship::clearObjects() {
  auto it = objects.begin();

  while (it != objects.end()) {
    //(*it)->~Object();
    delete *it;
    ++it;
  }

  objects.clear();

#ifdef M_CLIENT
  Graphics::deleteElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")));
#endif // M_CLIENT


}
void Ship::clearSightings() {
  auto it = sightings.begin();

  while (it != sightings.end()) {
    //(*it)->~Sighting();
    delete *it;
    ++it;
  }

  sightings.clear();
}
Ship::~Ship() {
  clearObjects();
  clearSightings();
#ifdef M_SERVER
  connectedClient->~NetworkS();
  delete connectedClient;
#endif
#ifdef M_CLIENT
  connectedServer->~NetworkC();
  delete connectedServer;
#endif

}

bool surefire(keyframe<Movement>& me, keyframe<Movement>& enemy, time_type_s when, sVec3 &direction) {
  //direction.randomize(100);
  //return fmodf(when, 0.5)<0.25;
  Movement m = me.getAt(when);
  Movement e = (--enemy._frames.end())->second;
  time_type_s t = (--enemy._frames.end())->first;
  result res = surefire1(t, e.vel, e.pos, when, m.pos, MAX_ACCEL, e.radius);
  direction = res.dir;
  return res.answ;
}

#ifdef M_CLIENT
Ship* ship;
time_type_s timeNow;
Object* selectedo;
Sighting* selecteds;
#endif
