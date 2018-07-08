#include "Ship.h"

#ifdef M_SERVER
#include "../Server/Game.h"
#endif

using namespace std;

#ifdef M_CLIENT
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

/*power_type_W Drone::getGeneratedShipPower(time_type_s time) {
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
}*/

power_type_W Drone::getMaxGeneratedPower(time_type_s time) {
  power_type_W sum = 0;
  for (auto&& it : objects) {
    sum += it->getMaxGeneratedPower(time);
  }
  return sum;
}
power_type_W Drone::getGeneratedPower(time_type_s time) {
  power_type_W sum = 0;
  for (auto&& it : objects) {
    sum += it->getGeneratedPower(time);
  }
  return sum;
}
power_type_W Drone::getMaxUseablePower(time_type_s time) {
  power_type_W sum = 0;
  for (auto&& it : objects) {
    sum += it->getMaxUseablePower(time);
  }
  return sum;
}
power_type_W Drone::getRequestedPower(time_type_s time) {
  power_type_W sum = 0;
  for (auto&& it : objects) {
    sum += it->getRequestedPower(time);
  }
  return sum;
}
power_type_W Drone::getUsedPower(time_type_s time) {
  power_type_W sum = 0;
  for (auto&& it : objects) {
    sum += it->getUsedPower(time);
  }
  return sum;
}
energy_type_J Drone::getMaxEnergy(time_type_s time) {
  energy_type_J sum = 0;
  for (auto&& it : objects) {
    sum += it->getMaxEnergy(time);
  }
  return sum;
}
energy_type_J Drone::getStoredEnergy(time_type_s time) {
  energy_type_J sum = 0;
  for (auto&& it : objects) {
    sum += it->getStoredEnergy(time);
  }
  return sum;
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

Ship::Ship(uint32_t _ID) {
  _droneID = _ID;
}
Ship::Ship(uint32_t _ID, mVec3 _pos, string filename) {
  load(_ID, _pos, filename);
}
Ship::Ship(uint32_t _ID, string filename) {
  load(_ID, { 0,0,0 }, filename);
}

void Ship::load(uint32_t _ID, mVec3 _pos, string filename) {
  energySystem._lastUpdate = -0.01;
  _droneID = _ID;

  xml_document<> doc;
  std::ifstream file(filename + "objects.xml");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);
  xml_node<> *data = doc.first_node("ship");
  buffer.clear();

  int id = 0;

  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling()) {

    /*Object* go = new ::Generator(this, mix(_ID, 0));//, { 100,0,0 }, 1000, 100, 1000, 1000000);
    objects.push_back(go);

    Object* so = new ::Sensor(this, mix(_ID, 1));//, { -100,0,0 }, 1000, 100, 1000, 100000);
    objects.push_back(so);

    Object* eo = new ::Engine(this, mix(_ID, 2));//, { 0,173.2f ,0 }, 1000, 100, 1000, 10000);
    objects.push_back(eo);

    Object* lo = new ::Laser(this, mix(_ID, 3));//, { 0,-173.2f ,0 }, 1000, 100, 1000, 100000);
    objects.push_back(lo);*/

    string s = pElem->name();
    Object* no = NULL;

    if(s == "Generator") {
      no = new ::Generator(this, mix(_ID, id));
    }
    if (s == "Sensor") {
      no = new ::Sensor(this, mix(_ID, id));
    }
    if (s == "Engine") {
      no = new ::Engine(this, mix(_ID, id));
    }
    if (s == "Laser") {
      no = new ::Laser(this, mix(_ID, id));
    }
#ifdef M_SERVER
    if(no != NULL) {
      if (no->load(pElem, 0)) {
        ++id;
        objects.push_back(no);
      } else {
        delete no;
      }
    }
#endif
  }

  file = ifstream(filename + "energySystem.xml");
  buffer << file.rdbuf();
  file.close();
  content = string(buffer.str());
  doc.parse<0>(&content[0]);
  data = doc.first_node("cables");

  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling()) {
    energySystem.addEdge(
      getObject(pElem->first_node("from")->value())->_energySystem,
      getObject(pElem->first_node("to")->value())->_energySystem,
      Fraction(pElem->first_node("capacity")->value())
    );
  }

  /*energySystem.addSymmetricEdge(go->_energySystem, so->_energySystem, 1000000);
  energySystem.addSymmetricEdge(go->_energySystem, eo->_energySystem, 1000000);
  energySystem.addSymmetricEdge(go->_energySystem, lo->_energySystem, 1000000);*/

  Movement m;
  m.pos = _pos;
  mov.addFrame(0, m);
}

#ifdef M_SERVER
Sighting* Drone::sightPath(Bubble* p, time_type_s time, Game* g, bool closed, bool autofire) {
  cout << "Sighting" << endl;
  Sighting* s = NULL;
  if (closed) {
    for (auto&& it : sightings) {
      if(it->tryClose(p, time)) {
        return it;
      }
    }
    return NULL;
  }
  priority_queue_smallest<pair<distance_type_m, Sighting*>> closest;
  for (auto&& it : sightings) {
    pair<distance_type_m, bool> res = it->closest(&(p->emitter));
    if(res.second) {
      closest.push({ res.first,it });
    }
  }

  if (closest.size()) {
    pair<distance_type_m, Sighting*> elem = closest.top();
    if (elem.first < 10) {
      elem.second->addFrame(time, SightedMovement(p->emitter, p));
      s = elem.second;
    }
    else {
      s = new Sighting();
      s->addFrame(time, SightedMovement(p->emitter, p));
      sightings.push_back(s);
    }
  }
  else {
    s = new Sighting();
    s->addFrame(time, SightedMovement(p->emitter, p));
    sightings.push_back(s);
  }
  if (autofire) {
    for (auto&& it : objects) {
      if (it->type() == Object::Laser) {
        Laser* lit = (Laser*)it;
        sVec3 dir;
        bool sf = surefire(mov, s->_keyframes, time, lit->_relativePos, dir);
        cout << "Returning fire" << endl;
        lit->shoot(time, 100000, dir, g);
      }
    }
  }
  return s;
}
energy_type_J Drone::energyUpdate(time_type_s time, Game* game, Object* chg, energy_type_J chgval) {
  FlowVertex<energy_type_J, Fraction, time_type_s>* vertex = NULL;
  if(chg != NULL) {
    vertex = chg->_energySystem;
  }
  pair<vector<time_type_s>, energy_type_J> runOut = energySystem.goTo(time, vertex, chgval);
  game->updateDroneBatteryEvents(this, runOut.first);
  energyCallback(time, game);
  return runOut.second;
}
void Drone::energyCallback(time_type_s time, Game* game) {
  for(auto&& it : objects) {
    it->energyCallback(time, game);
  }
}

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
        case Event::Type::EvTSensorAutofire:
          nObj = new SensorAutofire();
          break;
        }
        if (nObj != NULL) {
          nObj->set(it, game);
          game->add(nObj);
        }
        else {
          LOG LERROR "Wrong event type: " << typ END;
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
void Ship::collectPath(list<Path*> &addTo, time_type_s time) {
  for (auto it : objects) {
    //it->collectPath(addTo, time);
  }
}
bool Ship::loadShip(string filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  xml_document<> doc;
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  return loadShip(doc.first_node("ship"));
}
bool Ship::loadShip(xml_node<>* data) {
  time_type_s loadTime = -1;

  xml_node<>* elem;

  elem = data->first_node("shipclass");
  if(!elem) {
    return false;
  }
  shipClass = ShipClassFromStr(elem->value());

  elem = data->first_node("graphics");
  if (!elem) {
    return false;
  }
  graphicsName = ShipClassFromStr(elem->value());

  int id = 0;

  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling()) {
    string name = pElem->name();
    Object* o = NULL;
    if (name == "Generator") {
      o = new Generator(this, id);
    }
    if (name == "Sensor") {
      o = new Sensor(this, id);
    }
    if (name == "Engine") {
      o = new Engine(this, id);
    }
    if (name == "Laser") {
      o = new Laser(this, id);
    }
    if(o != NULL) {
      if (!o->load(pElem, loadTime)) {
        return false;
      }
      objects.push_back(o);
      ++id;
    } else {
      LOG LERROR "Wrong object type: " END;
      return false;
    }
  }
  return true;
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
  glutPostRedisplay();
}
void Ship::drawSightings(float camcx, float camcy, float camcz, float d, OpenGLData data) {
  if(sightings.size()) {
    mVec3 pos = mov.getAt(timeNow).pos;

    distance_type_m maxD = 0;

    /*for (auto&& it : sightings) {
    maxD = max(maxD, (it->getAt(timeNow).pos - pos).length());
    }*/

    for (auto&& it : sightings) {
      it->drawSighting({ camcx, camcy, camcz }, d, SOL, timeNow, /*maxD,*/ data, it == selecteds);
    }
  }
}
void Ship::drawObjects(float camcx, float camcy, float camcz, float d, bool worldView) {
  if(mov.size()) {
    if(worldView) {
      setColor(hexToInt("ff00ff00"));
      glLineWidth(2.0);
      glBegin(GL_LINE_STRIP);
      glVertex3d(camcx/d, camcy / d, camcz / d);
      glVertex3d(
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.x)/d,
        (camcy / d),
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.z)/d
        );
      glVertex3d(
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.x) / d,
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.y) / d,
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.z) / d
        );
      glEnd();
      glTranslated(
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.x) / d,
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.y) / d,
        to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.z) / d
        );
      glutSolidSphere(ShipSize, 20,20);
      glTranslated(
        -to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.x) / d,
        -to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.y) / d,
        -to_doubleT<double, distance_type_m>(mov.getAt(timeNow).pos.z) / d
        );
      for (auto&& it : objects) {
        it->drawObject(camcx, camcy, camcz, d, timeNow, worldView);
      }
    } else {
      for(auto&& it : objects) {
        it->drawObject(camcx, camcy, camcz, d, timeNow, worldView);
      }
    }
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
void Ship::selectSighting(vec3<double> ori, vec3<double> dir, double d) {
  list<pair<time_type_s, Sighting*>> inters;

  Shot p;
  p.origin = ori;
  p.origintime = 0;
  p.vel = dir;

  auto it = sightings.begin();

  while (it != sightings.end()) {
    if((*it)->getFirst() < timeNow) {
      Movement m = (*it)->getAt(timeNow);
      m.pos.x /= d;
      m.pos.y /= d;
      m.pos.z /= d;
      m.radius = SightingSize;
      m.acc = { 0,0,0 };
      m.vel = { 0,0,0 };
      vector<time_type_s> temp = intersectPaths(&m, &p);
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
      nObj = new Sensor(this, 0);
      break;
    case Object::Type::Computer:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Generator:
      nObj = new Generator(this, 0);
      break;
    case Object::Type::Storage:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Engine:
      nObj = new Engine(this, 0);
      break;
    case Object::Type::Laser:
      nObj = new Laser(this, 0);
      break;
    }
    if(nObj != NULL) {
      nObj->setStatus(*it);
      objects.push_back(nObj);
    } else {
      LOG LERROR "Wrong object type: " << typ END;
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
    Sighting* nSig = new Sighting();
    nSig->set(it);
    sightings.push_back(nSig);
  }
}
void Ship::clearObjects() {
  while (objects.size()) {
    delete *(objects.begin());
    objects.erase(objects.begin());
  }

#ifdef M_CLIENT
  Graphics::deleteElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")));
#endif // M_CLIENT


}
void Ship::clearSightings() {
  while (sightings.size()) {
    delete *(sightings.begin());
    sightings.erase(sightings.begin());
  }
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

bool surefire(keyframe<Movement>& me, keyframe<Movement>& enemy, time_type_s when, mVec3 posShift, sVec3 &direction) {
  //direction.randomize(100);
  //return fmodf(when, 0.5)<0.25;
  Movement m = me.getAt(when);
  Movement e = (--enemy._frames.end())->second;
  time_type_s t = (--enemy._frames.end())->first;
  result res = surefire1(t, e.vel, e.pos, when, m.pos + posShift, MAX_ACCEL, e.radius);
  direction = res.dir;
  return res.answ;
}

bool surefire(keyframe<Movement>& me, keyframe<SightedMovement>& enemy, time_type_s when, mVec3 posShift, sVec3 &direction) {
  Movement m = me.getAt(when);
  Movement e = (--enemy._frames.end())->second;
  time_type_s t = (--enemy._frames.end())->first;
  result res = surefire1(t, e.vel, e.pos, when, m.pos + posShift, MAX_ACCEL, e.radius);
  direction = res.dir;
  return res.answ;
}

#ifdef M_CLIENT
Ship* ship;
time_type_s timeNow;
Object* selectedo;
Sighting* selecteds;
#endif
