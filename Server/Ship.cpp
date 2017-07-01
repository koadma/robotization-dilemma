#include "Ship.h"
#include "Game.h"

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

  DataElement* vire = new DataElement();
  getVStatus(vire);
  data->addChild(vire);
}
void Object::setStatus(DataElement* data) {
  _relativePos.set(data->_children[1]);

  _maxHealth = data->_children[2]->_core->toType<int>();

  _health = data->_children[3]->_core->toType<int>();

  _radius = data->_children[4]->_core->toType<float>();

  setVStatus(data->_children[5]);
}

void Object::getVStatus(DataElement* data) {

}
void Object::setVStatus(DataElement* data) {

}
void Sensor::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  enee->_core->fromType<int>(_energy);
  data->addChild(enee);

  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxEnergy);
  data->addChild(maxe);
}
void Sensor::setVStatus(DataElement* data) {
  _energy = data->_children[0]->_core->toType<float>();
  _maxEnergy = data->_children[1]->_core->toType<float>();
}
void Engine::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxEnergy);
  data->addChild(maxe);

  DataElement* acce = new DataElement();
  _accel.get(acce);
  data->addChild(acce);
}
void Engine::setVStatus(DataElement* data) {
  _maxEnergy = data->_children[0]->_core->toType<float>();
  _accel.set(data->_children[1]);
}
void Generator::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxEnergy);
  data->addChild(maxe);
}
void Generator::setVStatus(DataElement* data) {
  _maxEnergy = data->_children[0]->_core->toType<float>();
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
    if (ran1() < _health / float(_maxHealth)) { //
      if (ran1() < 1 / (1 + pow(E, -(reinterpret_cast<Bubble*>(p)->energy * _energy)))) { //detect
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
  setColor(0xffff0000 + int(0xff * _health / float(_maxHealth)) + 0x100 * int(0xff * _health / float(_maxHealth)));
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
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyLabel"))->text = " / " + to_string(_maxEnergy, 0);
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyInput"))->text = to_string(_energy, 2);
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
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text = "Max output: " + to_string(_maxEnergy, 0);
  
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
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectEngineSidebarEnergyLabel"))->text = to_string(usedEnergy(), 3) + " / " + to_string(_maxEnergy, 0);
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
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectShipSidebarEnergyLabel"))->text = to_string(reinterpret_cast<::Ship*>(this)->getSpentShipEnergy()) + " / " + to_string(reinterpret_cast<::Ship*>(this)->getTotalShipEnergy());

}
void Ship::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/ship_settings.html");
}

#endif

Object::~Object() {

}

#ifdef M_SERVER
void Object::collectPath(list<Path*> &addTo, float time) {

}
void Sensor::collectPath(list<Path*> &addTo, float time) {
 
}
void Engine::collectPath(list<Path*> &addTo, float time) {
  Bubble* p = new Bubble();
  p->btype = Bubble::Thermal;
  p->energy = usedEnergy();
  p->origin = parentShip->mov.pos + _relativePos;
  p->emitter = parentShip->mov;
  p->emitter.pos += _relativePos;
  p->gEmissionTime = time;
  addTo.push_back(p);
}
void Laser::collectPath(list<Path*> &addTo, float time) {
  Shot* p = new Shot();
  p->energy = _shot.first;
  p->origin = parentShip->mov.pos + _relativePos;
  p->origintime = time;
  p->vel = _shot.second;
  addTo.push_back(p);
}
void Generator::collectPath(list<Path*> &addTo, float time) {

}
#endif

#ifdef M_SERVER
int Ship::makeMove(DataElement *Data) {
  if (!Data->_children.size()) {
    return 1;
  }
  switch (Data->_children[0]->_core->toType<int>()) {
  case CommandAccel:
    if (Data->_children.size() < 4) {
      return 1;
    }
    //_accel = fVec3(strTo<int>(data[1]), strTo<int>(data[2]), strTo<int>(data[3]));
    return 0;
    break;
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

      int res = makeMove(Data);

      DataElement* de = new DataElement();
      de->_core->fromType<int>(res);
      connectedClient->SendData(de, PacketCommand);
    }
    break;
  case PacketCommit:
    if (canMove) {
      canMove = false;
      setSightings(Data->_children[0]);
      setStatus(Data->_children[1]);
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
    it->collectPath(addTo, time);
  }
}
#endif
#ifdef M_CLIENT
void Ship::commit() {
  if(canMove) {
    canMove = false;

    DataElement* de = new DataElement();
    DataElement* se = new DataElement();
    DataElement* oe = new DataElement();
    getSightings(se);
    getStatus(oe);
    de->addChild(se);
    de->addChild(oe);
    connectedServer->SendData(de, PacketCommit);
  }
}
void Ship::newTurn(int id) {
  canMove = true;
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
float Ship::getTotalShipEnergy() {
  float res = 0;
  auto it = objects.begin();
  while (it != objects.end()) {
    res += (*it)->prodEnergy();
    ++it;
  }
  return res;
}
float Ship::getSpentShipEnergy() {
  float res = 0;
  auto it = objects.begin();
  while (it != objects.end()) {
    res += (*it)->usedEnergy();
    ++it;
  }
  return res;
}
float Ship::getRemainingShipEnergy() {
  return getTotalShipEnergy()-getSpentShipEnergy();
}
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
      nObj = new Sensor(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Computer:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Generator:
      nObj = new Generator(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Storage:
      //nObj = new Shield(fVec3(), 0, 0, 0, 0);
      break;
    case Object::Type::Engine:
      nObj = new Engine(fVec3(), 0, 0, 0, 0, fVec3());
      break;
    case Object::Type::Laser:
      nObj = new Laser(fVec3(), 0, 0, 0);
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

/*void shipPacketRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship) {
    ship->packetRecv(Data, Id, DataLen, thisptr);
  }

int Ship::getSpentEnergy() const
{
  return command.accel.length()+(command.didFire?100:0)+command.sensorEnergy;
}

ostream& operator<<(ostream& os, const Command& c)
{
  cout << "Gyorsulas: " << c.accel << endl;
  cout << "Tuzel-e: " << (c.didFire?"igen":"nem") << endl;
  if (c.didFire)
  {
    cout << "Cel: " << c.aim << endl;
  }
  cout << "Sensor energia: " <<c. sensorEnergy; //no endl
  return os;
}

ostream& operator<<(ostream& os, const SensorDataElement& sde)
{
  cout << ((sde.detectType == SensorDataElement::Active)?"Aktiv":"Passziv") << " szenzor jelzett!" << endl;
  cout << "Jatekos: " << sde.player << endl;
  cout << "Hely: " << sde.place << endl;
  cout << "Sebesseg: " << sde.velocity << endl;
  cout << "Josolt hely: " << sde.nextPlace << endl;
  cout << "Biztos-e a talalat: " << (sde.sureFire?"igen":"nem"); // no endl
  return os;
}

void Object::move(fVec3 accel, float time)
{
  float solut[2];
  int numOfSolut;
  float a = pow(accel.length(), 2);
  float b = 2*(velocity.x*accel.x + velocity.y*accel.y + velocity.z*accel.z);
  float c = pow(velocity.length(), 2) - pow(maxVelocity, 2); //solve (a+vt)^2 = vmax^2 for t
  if (a == 0)
  {
    velocity = velocity;
    place = place + velocity*time;
  } else
  {
    solve2(a, b, c, solut, numOfSolut); //calculate time of reacing maximum velocity
    float reachMaxT = max(solut[0], solut[1]);
    if (reachMaxT < 0 || numOfSolut != 0) 
    {
      throw 1;
    }
    if (reachMaxT > time)
    {
      place = place + velocity * time + accel*(pow(time, 2)/2);
      velocity = velocity + accel*time;
    } else
    {
      fVec3 startVelocity = velocity;
      velocity = velocity + accel*reachMaxT;
      place = place + startVelocity*time + velocity*(time-reachMaxT) + accel*(pow(reachMaxT, 2)/2);
    }
  }
}

void Ship::flushSensorData() 
{
  sensorData = vector<SensorDataElement>();
}

void Ship::giveSensorData() const
{
  if (not destroyed)
  {
    for (auto sde : sensorData)
    {
      cout << sde << endl << endl;
    }
  }
}

void Ship::getCommand()
//with Command class
{
  cout << "Adj meg parancsokat!" << endl;
  bool gettingInput = true;
  while (gettingInput)
  {
    string input;
    getline(cin, input);
    if (input == "" or input == "\n")
    {
      continue;
    }
    vector<string> cmd = tokenize(input);
    //parse (move x y z, aim x, sensor energy, data) 
    if (cmd[0] == "move")
    {
      if (cmd.size() != 4)
      {
        cout << "Rossz szamu szo!" << endl;
      } else
      {
        command.accel = {strTo<float>(cmd[1]), strTo<float>(cmd[2]), strTo<float>(cmd[3])};
        if (command.accel.length() > maxAcceleration)
        {
          cout << "Tul nagy gyorsulast adtal meg!" << endl;
          cout << "A hajod " << maxAcceleration << " gyorsulasra kepes!" << endl;
          command.accel = {0, 0, 0};
        } 
      }
    } else if (cmd[0] == "aim")
    {
      if (cmd.size() != 2)
      {
        cout << "Rossz szamu szo!" << endl;
      } else
      {
        if (cmd[1] == "off")
        {
          command.didFire = false;
        } else
        {
          int target = strTo<float>(cmd[1]);
          //Is the aiming sure?
          bool valid = false;
          for (auto sde : sensorData)
          {
            if (sde.player == target && sde.sureFire)
            {
              valid = true;
            }
          }
          if (valid)
          {
            command.didFire = true;
            command.aim = strTo<float>(cmd[1]);
          } else
          {
            cout << "Nem biztos a loves!" << endl;
          }
        }
      }
    } else if(cmd[0] == "sensor")
    {
      if (cmd.size() == 2)
      {
        command.sensorEnergy = strTo<float>(cmd[1]);
        if (strTo<float>(cmd[1]) > maxSensorEnergy)
        {
          cout << "Tul nagy szenzor energiat adtal meg!" << endl;
          cout << "A hajod " << maxSensorEnergy << "-re kepes!" << endl;
          command.sensorEnergy = 0;
        }
      } else
      {
        cout << "Rossz szamu szo!" << endl;
      }
    } else if (cmd[0] == "data")
    {
      cout << command << endl;
    } else if (input == "over")
    {
      if (getSpentEnergy() <= maxGeneratorEnergy)
      {
        gettingInput = false;
      } else
      {
        cout << getSpentEnergy() << " energiat szeretnel felhasznalni." << endl;
        cout << "A hajod generatora maximum " << maxGeneratorEnergy << "-re kepes." << endl;
      }
    } else
    {
      cout << "Ismeretlen parancs!" << endl;
    }
  }
}

bool Ship::didFire() const
{
  return command.didFire;
}

int Ship::getAim() const
{
  return command.aim;
}

void Ship::sense(SensorDataElement sde)
{
  sensorData.push_back(sde);
}

int Ship::getVisibility() const
{
  return floor(command.accel.length())+(command.didFire?10:0)+(command.sensorEnergy); // placeholder
}

int Ship::getSensorRadiation() const
{
  return command.sensorEnergy; //placeholder
}

void Ship::move(float time)
{
  this->Object::move(command.accel, time);
}

bool Ship::isDestroyed() const
{
  return destroyed;
}

void Ship::destroy() 
{
  destroyed = true;
}

fVec3 Ship::getPlace() const
{
  return place;
}

fVec3 Ship::getVelocity() const
{
  return velocity;
}

int Ship::getHullRadius() const
{
  return hullRadius;
}


unsigned int Ship::getOwner() const
{
  return owner;
}

ostream& operator<<(ostream& os, const Ship& s)
{
  if (s.destroyed)
  {
    os << "A hajo megsemmisult!"; // no endl
  } else
  {
    os << "Helyvektor: " << s.place << endl;
    os << "Sebessegvektor: " << s.velocity; // no endl
  }
  return os;
}
fpVec3 Movement::getPosPolynomial() {
return
(acc * 0.5f) * PolynomialF(
vector<float>(1, -gTimeStamp)
) *PolynomialF( vector<float>(1, -gTimeStamp ) ) +
vel * PolynomialF( vector<float>( 1, -gTimeStamp ) ) +
pos * PolynomialF( vector<float>(1) );
}
float reachMaxVelIn(float maxVelocity, bool& will) {
//-1: No acceleration, cant be solved.
//0 .. 2: number of solutions
int numOfSolut;
float sol[2];
float a = pow(acc.length(), 2);
float b = 2 * (vel.x*acc.x + vel.y*acc.y + vel.z*acc.z);
float c = pow(vel.length(), 2) - pow(maxVelocity, 2); //solve (a+vt)^2 = vmax^2 for t
if (a == 0)
{
will = false;
return 0;
} else {
will = true;
solve2(a, b, c, sol, numOfSolut); //calculate time of reacing maximum velocity
if(numOfSolut < 2) {
throw 1;
return 0;
} else {
return max(sol[0], sol[1]);
}
}
throw 1;
return 0;
}Movement goForwardTo(float time, float maxVelocity) {
Movement res;
res.timestamp = time;
float dt = time - timestamp;
bool will = false;
float reachMaxT = reachMaxVelIn(maxVelocity, will);
if (!will) {
res.pos = pos + vel * dt;
res.vel = vel;
res.acc = 0;
}
else
{
if (reachMaxT < 0)
{
throw 1;
}
if (reachMaxT > dt)
{
res.pos = pos + vel * dt + acc*(pow(dt, 2) / 2);
res.vel = vel + acc*dt;
res.acc = acc;
}
else
{
fVec3 startVelocity = vel;
res.vel = vel + acc*reachMaxT;
res.pos = pos + startVelocity*dt + vel*(dt - reachMaxT) + acc*(pow(reachMaxT, 2) / 2);
res.acc = 0;
}
}
//timestamp = time;
}
Movement goBackTo(float time, float maxVelocity) {
Movement res;
res.timestamp = time;
float dt = time - timestamp;
res.pos = pos + vel * dt + acc*(pow(dt, 2) / 2);
res.vel = vel + acc*dt;
res.acc = acc;
}//CHANGE!!!!!!!!!!!!!!!!
float Movement::intersect(Bubble &b, float maxVelocity) {
//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE
//solve c^2 (t-t_e)^2 = (px + vx t - ex)^2 + ...
PolynomialF p =
SOL*SOL*PolynomialF{ vector<float> {1, -b.gEmissionTime } }*PolynomialF{ vector<float> {1, -b.gEmissionTime } }-
PolynomialF{ vector<float> { vel.x, pos.x - b.origin.x - vel.x*gTimeStamp } } *PolynomialF{ vector<float> { vel.x, pos.x - b.origin.x - vel.x*gTimeStamp } }-
PolynomialF{ vector<float> { vel.y, pos.y - b.origin.y - vel.y*gTimeStamp } } *PolynomialF{ vector<float> { vel.y, pos.y - b.origin.y - vel.y*gTimeStamp } }-
PolynomialF{ vector<float> { vel.z, pos.z - b.origin.z - vel.z*gTimeStamp } } *PolynomialF{ vector<float> { vel.z, pos.z - b.origin.z - vel.z*gTimeStamp } };
float sol[2];
int numOfSols;
solve2(p.Coefficient[2], p.Coefficient[1], p.Coefficient[0], sol, numOfSols);
if (numOfSols != 2) {
throw 1;
return 0;
}
else {
return max(sol[0], sol[1]);
}
}
float Movement::intersect(Shot &l, float radius) {
//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE
PolynomialF p =
vel.x * PolynomialF{ vector<float> { vel.x, vel.x*gTimeStamp + pos.x } } +
vel.y * PolynomialF{ vector<float> { vel.y, vel.y*gTimeStamp + pos.y } } +
vel.z * PolynomialF{ vector<float> { vel.z, vel.z*gTimeStamp + pos.z } } -
PolynomialF{ vector<float> { l.vel.x, l.vel.x*l.time + l.origin.x } } -
PolynomialF{ vector<float> { l.vel.y, l.vel.y*l.time + l.origin.y } } -
PolynomialF{ vector<float> { l.vel.z, l.vel.z*l.time + l.origin.z } };
return -p.Coefficient[0] / p.Coefficient[1];
}
*/

#ifdef M_CLIENT
Ship* ship;
#endif
Object* selected;