#ifndef __SHIP_H__
#define __SHIP_H__

#include "../Server/Bubble.h"

class Collision;

class Movement {
public:
  float gTimeStamp = 0;
  fVec3 pos = fVec3(0);
  //float posUncertainty = 0;
  fVec3 vel = fVec3(0);
  //float velUncertainty = 0;
  fVec3 acc = fVec3(0);
  //float accUncertainty = 0;
  int type;
  string data;
  fpVec3 getPosPolynomial() {
    return
    (acc * 0.5f) * PolynomialF{vector<float>{1, -gTimeStamp}} * PolynomialF{ vector<float>{1, -gTimeStamp }} +
    vel * PolynomialF{ vector<float>{ 1, -gTimeStamp }} +
    pos * PolynomialF{ vector<float>{1}};
  }
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
  Movement goTo(float gTime, float maxVelocity) {
    /*if (time < timestamp) {
      return goBackTo(time, maxVelocity);
    }
    else {
      return goForwardTo(time, maxVelocity);
    }*/
    Movement m = *this;
    m.pos = pos + vel*gTime + acc*pow(gTime,2)/2.0f;
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

  //CHANGE!!!!!!!!!!!!!!!!
  float intersect(Bubble &b, float maxVelocity) {
    //CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE
    //solve c^2 (t-t_e)^2 = (px + vx t - ex)^2 + ...
    PolynomialF p =
      SOL*SOL*PolynomialF{ vector<float> {1, -b.gEmissionTime }}*PolynomialF{ vector<float> {1, -b.gEmissionTime }}-
      PolynomialF{ vector<float> { vel.x, pos.x - b.origin.x - vel.x*gTimeStamp }} * PolynomialF{ vector<float> { vel.x, pos.x - b.origin.x - vel.x*gTimeStamp }}-
      PolynomialF{ vector<float> { vel.y, pos.y - b.origin.y - vel.y*gTimeStamp }} * PolynomialF{ vector<float> { vel.y, pos.y - b.origin.y - vel.y*gTimeStamp }}-
      PolynomialF{ vector<float> { vel.z, pos.z - b.origin.z - vel.z*gTimeStamp }} * PolynomialF{ vector<float> { vel.z, pos.z - b.origin.z - vel.z*gTimeStamp }};
    float sol[2];
    int numOfSols;
    solve2(p.Coefficient[2], p.Coefficient[1], p.Coefficient[0],sol,numOfSols);
    if (numOfSols != 2) {
      throw 1;
      return 0;
    }
    else {
      return max(sol[0], sol[1]);
    }
  }
  float intersect(Shot &l, float radius) {
  //CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE//CHANGE
    PolynomialF p =
      vel.x * PolynomialF{ vector<float> { vel.x, vel.x*gTimeStamp + pos.x }} +
      vel.y * PolynomialF{ vector<float> { vel.y, vel.y*gTimeStamp + pos.y }} +
      vel.z * PolynomialF{ vector<float> { vel.z, vel.z*gTimeStamp + pos.z }} -
      PolynomialF{ vector<float> { l.vel.x, l.vel.x*l.time + l.origin.x }} -
      PolynomialF{ vector<float> { l.vel.y, l.vel.y*l.time + l.origin.y }} -
      PolynomialF{ vector<float> { l.vel.z, l.vel.z*l.time + l.origin.z }};
    return -p.Coefficient[0]/p.Coefficient[1];
  }
};

class Sighting {
  vector<pair<float, Movement*> > keyframes; //time - keyframe. Sorted.
  int getLastSmaller(float t)
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
  Movement estimatePos(float t, float maxVelocity) {
    int id = getLastSmaller(t);
    if (id == -1) {
      id = 0;
    }
    if (id < keyframes.size()) {
      return keyframes[id].second->goTo(t, maxVelocity);
    } else {
      throw 1;
      return Movement();
    }
  }
  
};

class Drone;

class Object {
public:
  Drone* parentShip;
  fVec3 relativePos;
  static enum Type {
    Ship = 1,
    Shield = 2,
    Sensor = 3,
    Computer = 4,
    Generator = 5,
    Storage = 6
  };
  int type;
  int maxHealth;
  int health;
  float radius;

  float intersect(Bubble &b);

  float intersect(Shot &b);
};

class Drone : public Object {
public:
  Movement mov;
};

class Ship : public Drone {
public:
  fVec3 accel = fVec3(0);

  list<Object*> objects;
  list<Sighting*> sightings;
#ifdef M_SERVER

  NetworkS* connectedClient;
  bool canMove = false; //is the player open to moving / are we waiting for a move.

  int makeMove(vector<string> & data) {
    if (!data.size()) {
      return 1;
    }
    switch (strTo<int>(data[0])) {
      case CommandAccel:
        if(data.size() < 4) {
          return 1;
        }
        accel = fVec3(strTo<int>(data[1]), strTo<int>(data[2]), strTo<int>(data[3]));
        return 0;
      break;
    }
  }
  void newTurn(int id) {
    canMove = true;

    connectedClient->SendData<int>(id, PacketNewRound);
  }
  void packetRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr) {
    switch (Id) {
      case PacketCommand:
        if (canMove) {
          vector<string> args = tokenize(string(reinterpret_cast<char*>(Data)), ';');
          int res = makeMove(args);
          connectedClient->SendData<int>(res, PacketCommand);
        }
      break;
      case PacketCommit:
        if(canMove) {
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
};

void shipPacketRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship);

/*
struct Command
{
  public:
  fVec3 accel = fVec3(0);
  bool didFire = false;
  unsigned int aim;
  int sensorEnergy = 0;
  friend std::ostream& operator<<(std::ostream& os, const Command& c);
};

struct SensorDataElement
{
  enum Type {Active, Passive};
  Type detectType;
  unsigned int player;
  fVec3 place;
  fVec3 velocity;
  fVec3 nextPlace;
  bool sureFire;
  friend std::ostream& operator<<(std::ostream& os, const SensorDataElement& sde);
};

class Object
{
protected:
  fVec3 place;
  fVec3 velocity;
  int maxVelocity;

public:
  Object(fVec3 place, fVec3 velocity, int maxVelocity) : 
    place(place), velocity(velocity), maxVelocity(maxVelocity) {}
  void move(fVec3 accel, float time);
};

class Ship : public Object
{
private:
  const unsigned int owner;
  int maxAcceleration = INITIAL_MAX_ACC;
  int maxSensorEnergy = INITIAL_MAX_SENSORENERGY;
  int maxGeneratorEnergy = INITIAL_MAX_GENERATORENERGY;
  int hullRadius = INITIAL_HULL_RADIUS;
  bool destroyed = false;
  Command command;
  std::vector<SensorDataElement> sensorData;
  int getSpentEnergy() const;

public:
  Ship(fVec3 place, int owner) : 
    Object(place, place * ((-1)*INITIAL_VELOCITY/place.length()), INITIAL_MAX_VELOCITY),
    owner(owner) {}
  void getCommand();
  void flushSensorData();
  void sense(SensorDataElement sde);
  void giveSensorData() const;
  void move(float time);
  void destroy();
  int getAim() const;
  fVec3 getPlace() const;
  fVec3 getVelocity() const;
  int getHullRadius() const;
  bool isDestroyed() const;
  int getSensorRadiation() const;
  int getVisibility() const;
  bool didFire() const;
  unsigned int getOwner() const;
  friend std::ostream& operator<<(std::ostream& os, const Ship& s);
};*/

class Collision {
public:
  Drone* drone;
  Projectile* proj;
};

#endif
