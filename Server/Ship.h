#ifndef __SHIP_H__
#define __SHIP_H__

#include "Sighting.h"

extern class Object;
extern Object* selected;


#ifdef M_SERVER
extern class Game;
#endif

///############################################///
//                    Events                    //
///############################################///

class Event {
public:
  enum Type {
    EvTEvent = 0,
    EvTCollision = 1,
    EvTStateChange = 2,
    EvTEngineAcc = 3,
    EvTSensorPow = 4,
    EvTLaserShot = 5,
    EvTThermalRadiation = 6
  };
  float _time;
#ifdef M_SERVER
  virtual void apply(Game *g);
#endif
  virtual int type();
};

class Collision : public Event {
public:
  Path* _p;
  Object* _o;
#ifdef M_SERVER
  void apply(Game *g);
#endif
  int type();

};

class ThermalRadiation : public Event {
public:
  Object* _o;
#ifdef M_SERVER
  void apply(Game *g);
#endif
  int type();
};

class StateChange : public Event {
public:
  Object* _o;
#ifdef M_SERVER
  virtual void apply(Game *g);
#endif
  virtual int type();
};

class EngineAcc : public StateChange {
public:
  fVec3 _acc;
#ifdef M_SERVER
  void apply(Game *g);
#endif
  int type();
};

class SensorPow : public StateChange {
public:
  float _power;
#ifdef M_SERVER
  void apply(Game *g);
#endif
  int type();
};

class LaserShot : public StateChange {
public:
  fVec3 _dir;
  float _energy;
#ifdef M_SERVER
  void apply(Game *g);
#endif
  int type();
};

struct EventSort{
  bool operator ()(const Event* lhs, const Event* rhs) const
  {
    return lhs->_time < rhs->_time;
  }
};

///############################################///
//                  Ship parts                  //
///############################################///

class Drone;

class Object {       //Order of serialisation
protected:
  uint64_t _ID;
  fVec3 _relativePos; //1
  int _maxHealth;     //2
  int _health;        //3
  float _radius;      //4
public:

  Object(fVec3 relativePos, int maxHealth, float radius, int health, unsigned long int ID) {
    _relativePos = relativePos;
    _maxHealth = maxHealth;
    _health = health;
    _radius = radius;
	_ID = ID;
  }

  Drone* parentShip;

  uint64_t getId() {
    return _ID;
  }

  static enum Type {
    Ship = 1,
    Shield = 2,
    Sensor = 3,
    Computer = 4,
    Generator = 5,
    Storage = 6,
    Engine = 7,
    Laser = 8
  };
  virtual int type() { throw 1; return 0; }         //0
  virtual float getGeneratedPower() { return 0;}
  virtual float getUsedPower() {return 0;}
  virtual float getMaxEnergy() {return 0;}
  virtual float getStoredEnergy() {return 0;}
  virtual float useEnergy(float amount){return amount;} //no change
  virtual float chargeEnergy(float amount) {
    return amount;
  } //no change

  virtual fVec3 getAccel() {
    return {0,0,0};
  }

  Movement getMovement();

#ifdef M_CLIENT
  list< pair<double, pair<Object*, Path*>>> getIntersect(vec3<double> ori, vec3<double> dir);
  virtual void setSidebarElement();
  virtual void setSidebar();
  void drawObject(float camcx, float camcy, float camcz, float d);
#endif

  virtual void collectEvents(list<Event*> &addTo, float time);

  virtual void getPathVirt(Path* p) {

  }
  void getPath(Path* p) {
    if((p->type() == Path::PathTypeShot) && (((Shot*)p)->originID !=this->_ID)) {
      _health -= (((Shot*)p)->energy)/50; //TODO BETTER
      _health = max(_health, 0);
    }
    getPathVirt(p);
  }

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  virtual void getVStatus(DataElement* data);
  virtual void setVStatus(DataElement* data);

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

class Sensor : public Object {       //Order of serialisation
private:
  float _power;
  float _maxPower;
public:
  Sensor(fVec3 relativePos, int maxHealth, float radius, int health, float maxPower, unsigned long int ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _power = 0;
    _maxPower = maxPower;
  }

  int type() {return Type::Sensor;}

  float getUsedPower() { return _power; }

  void setPower(float val) {
    _power = max(min(val, _maxPower), 0.0f);
  }

  void getPathVirt(Path* p);

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void collectEvents(list<Event*> &addTo, float time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};

class Engine : public Object {       //Order of serialisation
private:
  float _maxPower;
  fVec3 _accel;
public:
  Engine(fVec3 relativePos, int maxHealth, float radius, int health, float maxPower, fVec3 accel, unsigned long int ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _accel = accel;
    _maxPower = maxPower;
  }

  int type() { return Type::Engine; }
  virtual float getUsedPower() { return _accel.sqrlen(); }
  
  fVec3 getAccel() {
    return _accel;
  }

  void setComponent(int c, float val) {
    _accel[c] = val;
  }

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void collectEvents(list<Event*> &addTo, float time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};

class Laser : public Object {       //Order of serialisation
private:
  //vector<pair<float, fVec3> > _shots; //energy, directipn
  pair<float, fVec3> _shot;

public:
  Laser(fVec3 relativePos, int maxHealth, float radius, int health, unsigned long int ID) : Object(relativePos, maxHealth, radius, health, ID) {

  }

  int type() { return Type::Laser; }

  void setComponent(int c, float val) {
    _shot.second[c] = val;
  }
  void setEnergy(float val) {
    _shot.first = val;
  }

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void collectEvents(list<Event*> &addTo, float time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};

class Generator : public Object {       //Order of serialisation
private:
  float _maxPower;
  float _energyStored;
  float _maxStorage;
public:
  Generator(fVec3 relativePos, int maxHealth, float radius, int health, float maxPower, unsigned long int ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _maxPower = maxPower;
  }
  int type() { return Type::Generator; }
  
  float getGeneratedPower() { return _maxPower; }
  float getMaxEnergy() { return _maxStorage; }
  float getStoredEnergy() { return _energyStored; }
  float useEnergy(float amount) {
    _energyStored -= amount;
    float remain = -_energyStored;
    _energyStored = max(0.0f, _energyStored); //cant go below 0 energy
    return max(0.0f, remain); 
  } //no change
  float chargeEnergy(float amount) {
    _energyStored += amount;
    float extra = _energyStored - _maxStorage;
    _energyStored = min(_maxStorage, _energyStored); //cant go below 0 energy
    return max(0.0f, extra);
  } //no change

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void collectEvents(list<Event*> &addTo, float time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};

///############################################///
//                     Ship                     //
///############################################///

class Drone {
public:
  Movement mov;
  long int _droneID;
  list<Object*> objects;
  list<Sighting*> sightings;

  float lastEnergyCalcTime;

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    auto it = objects.begin();
    while (it != objects.end()) {
      res.splice(res.end(), (*it)->intersect(p));
      ++it;
    }
    return res;
  }
  
  float getGeneratedShipPower();
  float getUnusedShipPower();
  float getUsedShipPower();
  float getMaxShipEnergy();
  float getStoredShipEnergy();
  float useEnergy(float amount, float time);
  float chargeEnergy(float amount);
  void refreshEnergy(float time); 
  float runOut(float time); //when will storage run out

  fVec3 getAccel() {
    fVec3 sum = { 0,0,0 };
    for (auto it : objects) {
      sum += it->getAccel();
    }
    return sum;
  }
  void setAccel() {
    mov.acc = getAccel();
  }
  void moveShip(float to) {
    mov = mov.getAt(to, SOL);
  }
};

class Ship : public Drone {
private:
  bool canMove = false; //is the player open to moving / are we waiting for a move.
public:

  Ship(uint32_t _ID) {

    Object* no = new ::Generator({ 100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 0));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Sensor({ -100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 1));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Engine({ 0,173.2f ,0 }, 1000, 100, 1000, 100000, {0, 0, 0}, mix(_ID, 2));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Laser({ 0,-173.2f ,0 }, 1000, 100, 1000, mix(_ID, 3));
    no->parentShip = this;

    objects.push_back(no);
  }

#ifdef M_SERVER
  NetworkS* connectedClient;

  int makeMove(DataElement* data);
  void newTurn(int id);

  bool packetRecv(DataElement *Data, int Id, NetworkS* thisptr);

  void collectPath(list<Path*> &addTo, float time);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();
  void newTurn(int id);

  void setSidebarElement();
  void setSidebar();
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
     } else {
       setSidebar();
     }
  }

  void drawSightings(float camcx, float camcy, float camcz, float d);
  void drawObjects(float camcx, float camcy, float camcz, float d, bool b = false);

  bool packetRecv(DataElement *Data, int Id, NetworkC* thisptr);
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  void getSightings(DataElement* data);
  void setSightings(DataElement* data);

  void clearObjects();
  void clearSightings();

  ~Ship();
};

#ifdef M_CLIENT
extern Ship* ship;
#endif

#endif
