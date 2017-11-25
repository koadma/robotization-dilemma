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

Ship::Ship(uint32_t _ID, mVec3 _pos) {
  load(_ID, _pos);
}
Ship::Ship(uint32_t _ID) {
  load(_ID, { 0,0,0 });
}

void Ship::load(uint32_t _ID, mVec3 _pos) {
  Object* no = new ::Generator({ 100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 0));
  no->parentShip = this;

  objects.push_back(no);

  no = new ::Sensor({ -100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 1));
  no->parentShip = this;

  objects.push_back(no);

  no = new ::Engine({ 0,173.2f ,0 }, 1000, 100, 1000, 100000, { 0, 0, 0 }, mix(_ID, 2));
  no->parentShip = this;

  objects.push_back(no);

  no = new ::Laser({ 0,-173.2f ,0 }, 1000, 100, 1000, mix(_ID, 3));
  no->parentShip = this;

  objects.push_back(no);

  Movement m;
  m.pos = _pos;
  mov.addFrame(0, m);
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
    if (name == "generator") {
      o = new Generator(id);
      if (!o->load(pElem)) {
        return false;
      }
    }
    if(o != NULL) {
      objects.push_back(o);
      ++id;
    }
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
