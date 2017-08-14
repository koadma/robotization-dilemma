#include "Ship.h"

#ifdef M_SERVER
#include "../Server/Game.h"
#endif

#ifdef M_SERVER
void Event::apply(Game *g) {
  throw 1;
}
void Collision::apply(Game *g) {
  _o->getPath(_time, _p);
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
  //_o->parentShip->moveShip(_time);
  //reinterpret_cast<Engine*>(_o)->setComponent(0, _acc.x);
  //reinterpret_cast<Engine*>(_o)->setComponent(1, _acc.y);
  //reinterpret_cast<Engine*>(_o)->setComponent(2, _acc.z);
  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info
  g->removeIntersect(_o->parentShip);
  g->calcIntersect(_o->parentShip); //recalculate ship related future intersections
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
  _power = data->_children[0]->_core->toType<float>();
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
  
  _energy = data->_children[1]->_core->toType<float>();
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

using namespace std;

Movement Object::getMovement(time_type_s time) {
  Movement m = parentShip->mov.getAt(time);
  m.pos = m.pos + _relativePos;
  return m;
}
list< pair<double, pair<Object*, Path*>>> Object::intersect(Path* p) {
  list< pair<double, pair<Object*, Path*>>> res;
  auto it = parentShip->mov._frames.begin();
  while (it != parentShip->mov._frames.end()) {
    vector<double> times = intersectPaths(p, &(it->second));
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
}
void Sensor::setVStatus(DataElement* data) {
  _power.set(data->_children[0]);
  _maxPower = data->_children[1]->_core->toType<float>();
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
    DataElement* enee = new DataElement();
    DataElement* dire = new DataElement();

    vGFunc<time_type_s>(it.first, timee);
    vGFunc<energy_type_J>(it.second.first, enee);
    it.second.second.get(dire);

    ne->addChild(timee);
    ne->addChild(enee);
    ne->addChild(dire);

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
  for (auto&& it : data->_children[0]->_children) {

    pair<time_type_s, pair<energy_type_J, sVec3> > nVal;

    vSFunc<time_type_s>(nVal.first, it->_children[0]);
    vSFunc<energy_type_J>(nVal.second.first, it->_children[1]);
    nVal.second.second.set(it->_children[2]);
  }

  _energyStored.set(data->_children[1]);
  _maxStorage.set(data->_children[2]);

}

void Sensor::getPathVirt(time_type_s time, Path* p) {
  if (p->type() == Path::PathTypeBubble) {
    float h = _health.getAt(time)() / float(_maxHealth);
    if (ran1() < 2 * (1 - 1 / (1 + h))) {
      float e = reinterpret_cast<Bubble*>(p)->energy / _power.getAt(time)();
      if (ran1() < 1 / (1 + pow(2, (1 / e - e) * 3))) { //detect
        cout << "Detected" << endl;
        Sighting* s = new Sighting();
        Movement m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
        s->addFrame(time, m);
        parentShip->sightings.push_back(s);
      }
    }
  }
}

#ifdef M_CLIENT
void Object::setSidebarElement(string filename) {
  bool reset = true;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
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
void Object::drawObject(float camcx, float camcy, float camcz, float d) {
  glTranslated(_relativePos.x, _relativePos.y, _relativePos.z);
  setColor(0xffdf0000 + int(0xdf * _health.getAt(timeNow)() / float(_maxHealth)) + 0x100 * int(0xdf * _health.getAt(timeNow)() / float(_maxHealth)));
  glutSolidSphere(_radius, 20, 20);
  glTranslated(- _relativePos.x, - _relativePos.y, - _relativePos.z);
}
void Sensor::setSidebar() {
  setSidebarElement("html/sensor_settings.xml");
  
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyLabel"))->text = " / " + to_string(_maxPower, 0);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectSensorSidebarEnergyInput"))->text = to_string(_power.getAt(timeNow)(), 2);
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

  /*reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarEnergyInput"))->text = to_string(_shot.first, 3);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputX"))->text = to_string(_shot.second.x, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputY"))->text = to_string(_shot.second.y, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputZ"))->text = to_string(_shot.second.z, 2);*/
}
void Ship::setSidebar() {
  selected = NULL;
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
    LaserShot* ev = new LaserShot();
    ev->_dir = it.second.second;
    ev->_energy = it.second.first;
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
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
}
void Ship::drawSightings(float camcx, float camcy, float camcz, float d) {
  auto it = sightings.begin();

  while (it != sightings.end()) {
    (*it)->drawSighting(camcx, camcy, camcz, d, SOL);
    ++it;
  }
}
void Ship::drawObjects(float camcx, float camcy, float camcz, float d, bool b) {
  auto it = objects.begin();

  if(b) {
    glTranslatef(mov.getAt(timeNow).pos.x, mov.getAt(timeNow).pos.y, mov.getAt(timeNow).pos.z);
  }
  while (it != objects.end()) {
    (*it)->drawObject(camcx, camcy, camcz, d);
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

#ifdef M_CLIENT
Ship* ship;
time_type_s timeNow;
#endif
Object* selected;
