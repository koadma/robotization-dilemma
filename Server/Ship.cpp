#include "Ship.h"

using namespace std;

void shipPacketRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship) {
  ship->packetRecv(Data, Id, DataLen, thisptr);
}

/*fpVec3 Movement::getPosPolynomial() {
  return
    (acc * 0.5f) * PolynomialF(
    vector<float>(1, -gTimeStamp)
  ) *PolynomialF( vector<float>(1, -gTimeStamp ) ) +
    vel * PolynomialF( vector<float>( 1, -gTimeStamp ) ) +
    pos * PolynomialF( vector<float>(1) );
}*/
/*float reachMaxVelIn(float maxVelocity, bool& will) {
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
}*/
Movement Movement::goTo(float gTime, float maxVelocity) {
  /*if (time < timestamp) {
  return goBackTo(time, maxVelocity);
  }
  else {
  return goForwardTo(time, maxVelocity);
  }*/
  Movement m = *this;
  m.pos = pos + vel*gTime + acc*pow(gTime, 2) / 2.0f;
  m.vel = vel + acc*gTime;
  return m;
}
/*Movement goForwardTo(float time, float maxVelocity) {
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
}*/

/*//CHANGE!!!!!!!!!!!!!!!!
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
int Sighting::getLastSmaller(float t)
{
  int first = 0, last = int(keyframes.size()) - 1;
  while (first <= last)
  {
    int mid = (first + last) / 2;
    if (keyframes[mid].first >= t)
      last = mid - 1;
    else
      first = mid + 1;
  }
  return first - 1 < 0 ? -1 : first - 1;
}
Movement Sighting::estimatePos(float t, float maxVelocity) {
  int id = getLastSmaller(t);
  if (id == -1) {
    id = 0;
  }
  if (id < keyframes.size()) {
    return keyframes[id].second->goTo(t, maxVelocity);
  }
  else {
    throw 1;
    return Movement();
  }
}

Movement Object::getMovement() {
  Movement m = parentShip->mov;
  m.pos = m.pos + relativePos;
  return m;
}

#ifdef M_SERVER

  NetworkS* connectedClient;
  bool canMove = false; //is the player open to moving / are we waiting for a move.

  int Ship::makeMove(vector<string> & data) {
    if (!data.size()) {
      return 1;
    }
    switch (strTo<int>(data[0])) {
    case CommandAccel:
      if (data.size() < 4) {
        return 1;
      }
      accel = fVec3(strTo<int>(data[1]), strTo<int>(data[2]), strTo<int>(data[3]));
      return 0;
      break;
    }
  }
  void Ship::newTurn(int id) {
    canMove = true;

    connectedClient->SendData<int>(id, PacketNewRound);
  }
  void Ship::packetRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr) {
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
      }
      else {

      }
      break;
    case PacketSensor:
      //connectedClient->SendData<list<Sighting*>>(sightings, PacketSensor);
      break;
    case PacketCommandHistory: //

      break;
    case PacketShipData:
      //connectedClient->SendData<Ship*>(this, PacketShipData);
      break;
    }
  }
#endif

void shipPacketRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship);

/*
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
*/