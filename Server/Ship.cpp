#include "Ship.h"

#ifdef M_SERVER
#include "Game.h"
#endif

#ifdef M_SERVER
void Event::apply(Game *g) {
  throw 1;
}
void Collision::apply(Game *g) {
  _o->getPath(_p);
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
  _o->parentShip->moveShip(_time);
  reinterpret_cast<Engine*>(_o)->setComponent(0, _acc.x);
  reinterpret_cast<Engine*>(_o)->setComponent(1, _acc.y);
  reinterpret_cast<Engine*>(_o)->setComponent(2, _acc.z);
  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info
  g->removeIntersect(_o->parentShip);
  g->calcIntersect(_o->parentShip); //recalculate ship related future intersections
}
void EngineAcc::setV(DataElement* data, Game* game) {
  _acc.set(data->_children[0]);
}
void SensorPow::apply(Game *g) {
  reinterpret_cast<Sensor*>(_o)->setPower(_power);
  _o->parentShip->refreshEnergy(_time); //recalculate ship energy info
  //g.removeIntersect(_o->parentShip);
  //g.calcIntersect(_o->parentShip); //recalculate ship related future intersections
}
void SensorPow::setV(DataElement* data, Game* game) {
  _power = data->_children[0]->_core->toType<float>();
}
void LaserShot::apply(Game *g) {
  Shot* s = new Shot();
  s->energy = _energy - _o->useEnergy(_energy);
  s->origin = _o->getMovement().getAt(_time, SOL).pos;
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
  b->emitter = _o->getMovement();
  b->energy = _o->getUsedPower();
  b->gEmissionTime = _time;
  b->origin = _o->getMovement().getAt(_time, SOL).pos;
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
  timee->_core->fromType<float>(_time);
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
  powe->_core->fromType<float>(_power);
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
  enee->_core->fromType<float>(_energy);
  data->addChild(enee);
}
int ThermalRadiation::type() {
  return EvTThermalRadiation;
}
void ThermalRadiation::getV(DataElement* data) {

}

using namespace std;

Movement Object::getMovement() {
  Movement m = parentShip->mov;
  m.pos = m.pos + _relativePos;
  return m;
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
  hele->_core->fromType<int>(_health);
  data->addChild(hele);

  DataElement* rade = new DataElement();
  rade->_core->fromType<int>(_radius);
  data->addChild(rade);

  DataElement* ide = new DataElement();
  ide->_core->fromType<uint64_t>(_ID);
  data->addChild(ide);

  DataElement* vire = new DataElement();
  getVStatus(vire);
  data->addChild(vire);
}
void Object::setStatus(DataElement* data) {
  _relativePos.set(data->_children[1]);

  _maxHealth = data->_children[2]->_core->toType<int>();

  _health = data->_children[3]->_core->toType<int>();

  _radius = data->_children[4]->_core->toType<float>();

  _ID = data->_children[5]->_core->toType<uint64_t>();

  setVStatus(data->_children[6]);
}
void Object::getVStatus(DataElement* data) {

}
void Object::setVStatus(DataElement* data) {

}
void Sensor::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  enee->_core->fromType<int>(_power);
  data->addChild(enee);

  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxPower);
  data->addChild(maxe);
}
void Sensor::setVStatus(DataElement* data) {
  _power = data->_children[0]->_core->toType<float>();
  _maxPower = data->_children[1]->_core->toType<float>();
}
void Engine::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxPower);
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
  maxe->_core->fromType<int>(_maxPower);
  data->addChild(maxe);
}
void Generator::setVStatus(DataElement* data) {
  _maxPower = data->_children[0]->_core->toType<float>();
}
void Laser::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  enee->_core->fromType<float>(_shot.first);
  data->addChild(enee);

  DataElement* dire = new DataElement();
  _shot.second.get(dire);
  data->addChild(dire);
}
void Laser::setVStatus(DataElement* data) {
  _shot.first = data->_children[0]->_core->toType<float>();
  _shot.second.set(data->_children[1]);
}

void Sensor::getPathVirt(Path* p) {
  if (p->type() == Path::PathTypeBubble) {
    float h = _health / float(_maxHealth);
    if (ran1() < 2 * (1 - 1 / (1 + h))) {
      float e = reinterpret_cast<Bubble*>(p)->energy / _power;
      if (ran1() < 1 / (1 + pow(2, (1 / e - e) * 3))) { //detect
        cout << "Detected" << endl;
        Sighting* s = new Sighting();
        Movement* m = new Movement();
        *m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
        s->keyframes.push_back(m);
        parentShip->sightings.push_back(s);
      }
    }
  }
}

#ifdef M_CLIENT
void Object::setSidebarElement() {
  cout << "Unimplemented type " << type() << endl;
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
  for (int i = 0; i < times.size(); i++) {
    res.push_back({ times[i],{ this, &p } });
  }
  return res;
}
void Object::drawObject(float camcx, float camcy, float camcz, float d) {
  glTranslatef(_relativePos.x, _relativePos.y, _relativePos.z);
  setColor(0xffdf0000 + int(0xff * _health / float(_maxHealth)) + 0x100 * int(0xdf * _health / float(_maxHealth)));
  glutSolidSphere(_radius, 20, 20);
  glTranslatef(- _relativePos.x, - _relativePos.y, - _relativePos.z);
}

void Sensor::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = true;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth);
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyLabel"))->text = " / " + to_string(_maxPower, 0);
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyInput"))->text = to_string(_power, 2);
}
void Sensor::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/sensor_settings.html");
}
void Generator::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = true;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth);
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text = "Max output: " + to_string(_maxPower, 0);
  
}
void Generator::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/generator_settings.html");
}
void Engine::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = true;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectEngineSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth, 0);
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectEngineSidebarEnergyLabel"))->text = to_string(getUsedPower(), 3) + " / " + to_string(_maxPower, 0);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputX"))->text = to_string(_accel.x, 2);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputY"))->text = to_string(_accel.y, 2);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputZ"))->text = to_string(_accel.z, 2);
   
}
void Engine::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/engine_settings.html");
}
void Laser::setSidebar() {
  bool reset = true;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectLaserSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth, 0);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarEnergyInput"))->text = to_string(_shot.first, 3);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputX"))->text = to_string(_shot.second.x, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputY"))->text = to_string(_shot.second.y, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarAccInputZ"))->text = to_string(_shot.second.z, 2);
}
void Laser::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/laser_settings.html");
}
void Ship::setSidebar() {
  selected = NULL;
  setSidebarElement();
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectShipSidebarEnergyLabel"))->text = to_string(reinterpret_cast<::Ship*>(this)->getUsedShipPower()) + " / " + to_string(reinterpret_cast<::Ship*>(this)->getGeneratedShipPower());

}
void Ship::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/ship_settings.html");
}

#endif

Object::~Object() {

}

void Object::collectEvents(list<StateChange*> &addTo, float time) {

}
void Sensor::collectEvents(list<StateChange*> &addTo, float time) {
  SensorPow* ev = new SensorPow();
  ev->_power = _power;
  ev->_o = this;
  ev->_time = time;
  addTo.push_back(ev);
}
void Engine::collectEvents(list<StateChange*> &addTo, float time) {
  EngineAcc* ev = new EngineAcc();
  ev->_acc = _accel;
  ev->_o = this;
  ev->_time = time;
  addTo.push_back(ev);
}
void Laser::collectEvents(list<StateChange*> &addTo, float time) {
  LaserShot* ev = new LaserShot();
  ev->_dir = _shot.second;
  ev->_energy = _shot.first;
  ev->_o = this;
  ev->_time = time;
  addTo.push_back(ev);
}
void Generator::collectEvents(list<StateChange*> &addTo, float time) {
  ThermalRadiation* ev = new ThermalRadiation();
  ev->_o = this;
  addTo.push_back(ev);
}

float Drone::getGeneratedShipPower() {
  float sum = 0;
  for(auto it : objects) {
    sum += it->getGeneratedPower();
  }
  return sum;
}
float Drone::getUnusedShipPower() {
  return getGeneratedShipPower() - getUsedShipPower();
}
float Drone::getUsedShipPower() {
  float sum = 0;
  for (auto it : objects) {
    sum += it->getUsedPower();
  }
  return sum;
}
float Drone::getMaxShipEnergy() {
  float sum = 0;
  for (auto it : objects) {
    sum += it->getMaxEnergy();
  }
  return sum;
}
float Drone::getStoredShipEnergy() {
  float sum = 0;
  for (auto it : objects) {
    sum += it->getStoredEnergy();
  }
  return sum;
}

float Drone::useEnergy(float amount, float time) {
  refreshEnergy(time);
  for (auto it : objects) {
    amount = it->useEnergy(amount);
  }
  return amount;
}
float Drone::chargeEnergy(float amount) {
  for (auto it : objects) {
    amount = it->chargeEnergy(amount);
  }
  return amount;
}
void Drone::refreshEnergy(float time) {
  chargeEnergy(getUnusedShipPower() * (time - lastEnergyCalcTime));
  lastEnergyCalcTime = time;
}
Event* Drone::runOut() {
  BatteryDrain* ev = new BatteryDrain();
  ev->_time = lastEnergyCalcTime - getStoredShipEnergy() / getUnusedShipPower();
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
      
      for(auto it : Data->_children) {
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
      it->collectEvents(evs, time);
    }

    for(auto it : evs) {
      DataElement* ev = new DataElement();
      it->get(ev);
      events->addChild(ev);
    }

    connectedServer->SendData(events, PacketCommit);
  }
}
void Ship::newTurn(int id) {
  canMove = true;
  time = (id - 1) * ROUND_TIME;
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
    glTranslatef(mov.pos.x, mov.pos.y, mov.pos.z);
  }
  while (it != objects.end()) {
    (*it)->drawObject(camcx, camcy, camcz, d);
    ++it;
  }
  if (b) {
    glTranslatef(-mov.pos.x, -mov.pos.y, -mov.pos.z);
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
    it->getSighting(ne);
    data->_children.push_back(ne);
  }
}
void Ship::setSightings(DataElement* data) {
  clearSightings();

  for (DataElement* it : data->_children) {
    Sighting* nSig = new Sighting;
    nSig->setSighting(it);
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
#endif
Object* selected;