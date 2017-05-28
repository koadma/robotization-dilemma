#ifndef __SHIP_H__
#define __SHIP_H__

#ifdef M_SERVER
#include "../Server/Bubble.h"
#endif
#ifdef M_CLIENT
#include "../robotization dilemma/RenderOut.h"
#endif

vector<double> intersectPaths(Path* lhs, Path* rhs);

class Movement : public Path { //Order of serialisation
public:
  int etype() {
    return EqnType::EqnTypeApproxable;
  }
  float gTimeStamp = 0;        //0
  fVec3 pos = fVec3(0);        //1
  //float posUncertainty = 0;
  fVec3 vel = fVec3(0);        //2
  //float velUncertainty = 0;
  fVec3 acc = fVec3(0);        //3
  //float accUncertainty = 0;
  int type;                    //4
  string pathData;             //5
  double radius;               //6
  Eqnsys getEquations(bool b) {//approximate
    Eqnsys res;
    if (!b) { //Parametric curve
      Eqnsys resh;

      resh.eqns['a'] = Equation<double>({ { acc.x / 2.0f, "tt" },{ vel.x, "t" } ,{ pos.x, "" },{ -1, "a" } });
      resh.eqns['b'] = Equation<double>({ { acc.y / 2.0f, "tt" },{ vel.y, "t" } ,{ pos.y, "" },{ -1, "b" } });
      resh.eqns['c'] = Equation<double>({ { acc.z / 2.0f, "tt" },{ vel.z, "t" } ,{ pos.z, "" },{ -1, "c" } });
     

      res.eqns['t'] =
        Equation<double>({ { 1, "x" },{ -1, "a" } }) * Equation<double>({ { 1, "x" },{ -1, "a" } }) +
        Equation<double>({ { 1, "y" },{ -1, "b" } }) * Equation<double>({ { 1, "y" },{ -1, "b" } }) +
        Equation<double>({ { 1, "z" },{ -1, "c" } }) * Equation<double>({ { 1, "z" },{ -1, "c" } }) +
        Equation<double>({ { -radius*radius, "" } });
      res.eqns['t'].substitute(resh.eqns['a'], 'a');
      res.eqns['t'].substitute(resh.eqns['b'], 'b');
      res.eqns['t'].substitute(resh.eqns['c'], 'c');
    }
    else { //Moving sphere
      res.eqns['x'] = Equation<double>({ { acc.x / 2.0f, "tt" },{ vel.x, "t" } ,{ pos.x, "" },{ -1, "x" } });
      res.eqns['y'] = Equation<double>({ { acc.y / 2.0f, "tt" },{ vel.y, "t" } ,{ pos.y, "" },{ -1, "y" } });
      res.eqns['z'] = Equation<double>({ { acc.z / 2.0f, "tt" },{ vel.z, "t" } ,{ pos.z, "" },{ -1, "z" } });
    }
    return res;
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
  Movement goTo(float gTime, float maxVelocity);
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

  void get(unsigned char** data, int &DataLen);
  void set(unsigned char* data, int DataLen);

  ~Movement();
};

class Sighting {
public:
  vector<Movement*> keyframes; //time - keyframe. Sorted.
  int getLastSmaller(float t);
  Movement estimatePos(float t, float maxVelocity);
  
#ifdef M_CLIENT
  void drawSighting(float camcx, float camcy, float camcz, float d, float maxVel);
#endif

  void getSighting(unsigned char** data, int &DataLen);
  void setSighting(unsigned char* data, int DataLen);

  void clearKeyframes();
  ~Sighting();
};

class Drone;

class Object {       //Order of serialisation
public:
  Drone* parentShip;
  fVec3 relativePos; //0
  static enum Type {
    Ship = 1,
    Shield = 2,
    Sensor = 3,
    Computer = 4,
    Generator = 5,
    Storage = 6
  };
  int type;         //1
  int maxHealth;    //2
  int health;       //3
  float radius;     //4
  float energy;     //5
  float maxEnergy;  //6

  Movement getMovement();

#ifdef M_CLIENT
  list< pair<double, pair<Object*, Path*>>> getIntersect(vec3<double> ori, vec3<double> dir);
  void setSidebar() {
    //cout << type << " clicked " << health/float(maxHealth) << endl;
    if(type == Sensor) {
      Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/sensor_settings.html");
      reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarHealth"))->text = "Health: " + to_string(health) + " / " + to_string(maxHealth);
      reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarEnergyLabel"))->text = " / " + to_string(maxEnergy);
      return;
    }
    if (type == Generator) {
      Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), "html/generator_settings.html");
      reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarHealth"))->text = "Health: " + to_string(health) + " / " + to_string(maxHealth);
      reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text = "Max output: " + to_string(maxEnergy);
      return;
    }
    cout << "Unimplemented type " << type << endl;
  }
  void drawObject(float camcx, float camcy, float camcz, float d);
#endif

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    vector<double> times = intersectPaths(&getMovement(), p);
    for (int i = 0; i < times.size(); i++) {
      res.push_back({ times[i],{ this, p } });
    }
    return res;
  }

  ~Object();
};

class Drone : public Object {
public:
  Movement mov;
};

class Ship : public Drone {
public:
  fVec3 accel = fVec3(0);

  bool canMove = false; //is the player open to moving / are we waiting for a move.

  Ship() {
    Object* no = new Object();
    no->parentShip = this;
    no->relativePos = {100,0,0};
    no->type = Generator;
    no->maxHealth = 1000;
    no->health = 800;
    no->radius = 100;
    no->maxEnergy = 100000;

    objects.push_back(no);

    no = new Object();
    no->parentShip = this;
    no->relativePos = { -100,0,0 };
    no->type = Sensor;
    no->maxHealth = 1000;
    no->health = 600;
    no->radius = 100;
    no->maxEnergy = 100000;
    no->energy = 0;

    objects.push_back(no);
  }

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    auto it = objects.begin();
    while (it != objects.end()) {
      res.splice(res.end(), (*it)->intersect(p));
      ++it;
    }
    return res;
  }

  list<Object*> objects;
  list<Sighting*> sightings;
#ifdef M_SERVER
  NetworkS* connectedClient;

  int makeMove(vector<string> & data);
  void newTurn(int id);
  bool packetRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();

  void newTurn(int id);

  void setSidebar(vec3<double> ori, vec3<double> dir) {
    list< pair<double, pair<Object*, Path*>>> inters;

    auto it = objects.begin();

    while (it != objects.end()) {
      list< pair<double, pair<Object*, Path*>>> temp = (*it)->getIntersect(ori, dir);
      inters.splice(inters.end(), temp);
      ++it;
     }

     inters.sort();

     if(inters.size()) {
       inters.begin()->second.first->setSidebar();
     }
  }

  void drawSightings(float camcx, float camcy, float camcz, float d);
  void drawObjects(float camcx, float camcy, float camcz, float d);

  bool packetRecv(unsigned char *Data, int Id, int DataLen, NetworkC* thisptr);
#endif
  Object* getObject(int type);
  Object* getGenerator();
  Object* getSensor();
  void  setSensorEnergy(float energy);
  float getSensorEnergy();
  float getMaximumSensorEnergy();

  float getTotalShipEnergy();
  float getRemainingShipEnergy();

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);

  void getSightings(unsigned char** data, int &DataLen);
  void setSightings(unsigned char* data, int DataLen);

  void clearObjects();
  void clearSightings();

  ~Ship();
};

//void shipPacketRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr, Ship* ship);

extern Ship* ship;

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

#endif
