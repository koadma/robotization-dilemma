#include "Ship.h"
#include "Game.h"

using namespace std;

Movement Object::getMovement() {
  Movement m = parentShip->mov;
  m.pos = m.pos + _relativePos;
  return m;
}
void Object::getStatus(unsigned char** data, int &DataLen) {
  vector<pair<unsigned char*, int> > status;
  unsigned char* c;
  int l;

  //_relativePos
  _relativePos.get(&c, l);
  status.push_back({c, l});

  //type
  serialize(type(), &c, l);
  status.push_back({ c, l });

  //maxHealth
  serialize(_maxHealth, &c, l);
  status.push_back({ c, l });

  //health
  serialize(_health, &c, l);
  status.push_back({ c, l });

  //radius
  serialize(_radius, &c, l);
  status.push_back({ c, l });

  concat(status, data, DataLen);
}
void Object::setStatus(unsigned char* data, int DataLen) {
  vector<pair<unsigned char*, int> > status;
  split(data, DataLen, status);

  //_relativePos
  _relativePos.set(status[0].first, status[0].second);

  //type
  //type = deserializei(status[1].first, status[1].second);

  //maxHealth
  _maxHealth = deserializei(status[2].first, status[2].second);

  //health
  _health = deserializei(status[3].first, status[3].second);

  //radius
  _radius = deserializef(status[4].first, status[4].second);
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
  m.pos = _relativePos + parentShip->mov.pos;
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
#endif
Object::~Object() {

}

#ifdef M_CLIENT
void Sensor::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = false;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth);
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyLabel"))->text = " / " + to_string(_maxEnergy);
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyInput"))->text = to_string(_energy);
}
void Sensor::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/sensor_settings.html");
}
void Generator::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = false;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth);
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text = "Max output: " + to_string(_maxEnergy);
  
}
void Generator::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/generator_settings.html");
}
void Engine::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  bool reset = false;
  if (selected == NULL || selected->type() != type()) {
    reset = true;
  }
  selected = this;
  if (reset) {
    setSidebarElement();
  }

    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectEngineSidebarHealth"))->text = "Health: " + to_string(_health) + " / " + to_string(_maxHealth);
    reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectEngineSidebarEnergyLabel"))->text = to_string(usedEnergy()) + " / " + to_string(_maxEnergy);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputX"))->text = to_string(_accel.x);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputY"))->text = to_string(_accel.y);
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputZ"))->text = to_string(_accel.z);
   
}
void Engine::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/engine_settings.html");
}
void Ship::setSidebar() {
  //cout << type << " clicked " << health/float(maxHealth) << endl;
  setSidebarElement();

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectShipSidebarEnergyLabel"))->text = to_string(reinterpret_cast<::Ship*>(this)->getSpentShipEnergy()) + " / " + to_string(reinterpret_cast<::Ship*>(this)->getTotalShipEnergy());

}
void Ship::setSidebarElement() {
  Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/ship_settings.html");
}
#endif

#ifdef M_SERVER
int Ship::makeMove(vector<string> & data) {
  if (!data.size()) {
    return 1;
  }
  switch (strTo<int>(data[0])) {
  case CommandAccel:
    if (data.size() < 4) {
      return 1;
    }
    //_accel = fVec3(strTo<int>(data[1]), strTo<int>(data[2]), strTo<int>(data[3]));
    return 0;
    break;
  }
  
}
void Ship::newTurn(int id) {
  canMove = true;

  connectedClient->SendData<int>(id, PacketNewRound);
}
bool Ship::packetRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr) {
  int l;
  unsigned char* c;

  switch (Id) {
  case PacketCommand:
    if (canMove) {
      vector<string> args = tokenize(string(reinterpret_cast<char*>(Data)), ';');
      int res = makeMove(args);
      connectedClient->SendData<int>(res, PacketCommand);
    }
    break;
  case PacketCommit:
    if (canMove) {
      canMove = false;
      game->moveMade();
    }
    else {

    }
    break;
  case PacketSensor:
    getSightings(&c, l);
    connectedClient->SendData(c, PacketSensor, l);
    break;
  case PacketCommandHistory: //

    break;
  case PacketShipData:
    getStatus(&c, l);
    connectedClient->SendData(c, PacketShipData, l);
    break;
  }
  return 0;
}
#endif
#ifdef M_CLIENT
void Ship::commit() {
  if(canMove) {
    canMove = false;
    connectedServer->SendData("a", PacketCommit, 1);
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
void Ship::drawObjects(float camcx, float camcy, float camcz, float d) {
  auto it = objects.begin();

  while (it != objects.end()) {
    (*it)->drawObject(camcx, camcy, camcz, d);
    ++it;
  }
}
bool Ship::packetRecv(unsigned char *Data, int Id, int DataLen, NetworkC* thisptr) {
  switch (Id) {
  case PacketNewRound:
    newTurn(strTo<int>(string(reinterpret_cast<char*>(Data))));
  case PacketGameOver:
    createMainMenu();
    break;
  case PacketCommand:
    //createMainMenu();
    break;
  case PacketSensor:
    setSightings(Data, DataLen);
    glutPostRedisplay();
    break;
  case PacketShipData:
    setStatus(Data, DataLen);
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
void Ship::getStatus(unsigned char** data, int &DataLen) {
  vector<pair<unsigned char* , int> > status;
  auto it = objects.begin();

  while (it != objects.end()) {
    unsigned char* d;
    int i;
    (*it)->getStatus(&d, i);
    status.push_back({d,i});
    ++it;
  }
  concat(status, data, DataLen);
}
void Ship::setStatus(unsigned char* data, int DataLen) {
  vector<pair<unsigned char*, int> > status;
  split(data, DataLen, status);

  clearObjects();

  /*for (int i = 0; i < status.size(); i++) {
    Object* nObj = new Object();
    nObj->setStatus(status[i].first, status[i].second);
    nObj->parentShip = this;
    objects.push_back(nObj);
  }*/
  throw 1;
  ///TODO REDO
}
void Ship::getSightings(unsigned char** data, int &DataLen) {
  vector<pair<unsigned char*, int> > status;
  auto it = sightings.begin();

  while (it != sightings.end()) {
    unsigned char* d;
    int i;
    (*it)->getSighting(&d, i);
    status.push_back({ d,i });
    ++it;
  }
  concat(status, data, DataLen);
}
void Ship::setSightings(unsigned char* data, int DataLen) {
  vector<pair<unsigned char*, int> > sigh;
  split(data, DataLen, sigh);

  clearSightings();

  for (int i = 0; i < sigh.size(); i++) {
    Sighting* nObj = new Sighting();
    nObj->setSighting(sigh[i].first, sigh[i].second);
    sightings.push_back(nObj);
  }
}
void Ship::clearObjects() {
  auto it = objects.begin();

  while (it != objects.end()) {
    (*it)->~Object();
    delete *it;
  }

  objects.clear();
}
void Ship::clearSightings() {
  auto it = sightings.begin();

  while (it != sightings.end()) {
    (*it)->~Sighting();
    delete *it;
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