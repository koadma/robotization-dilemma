#pragma once
#include "Sighting.h"

class Object;
class Drone;

#ifdef M_SERVER
class Game;
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
    EvTThermalRadiation = 6,
    EvTBatteryDrain = 7,
    EvTSensorPing = 8
  };
  time_type_s _time;

  virtual int type();
  
#ifdef M_SERVER
  virtual void apply(Game *g);
#endif
};
class Collision : public Event {
public:
  Path* _p;
  Object* _o;

  int type();
  
#ifdef M_SERVER
  void apply(Game *g);
#endif

};
class BatteryDrain : public Event {
  Drone* _d;

  int type();

#ifdef M_SERVER
  void apply(Game *g);
#endif

};
class StateChange : public Event {
public:
  Object* _o;

  virtual int type();
  virtual void getV(DataElement* data);
  void get(DataElement* data);

#ifdef M_SERVER
  virtual void apply(Game *g);
  void set(DataElement* data, Game* game);
  virtual void setV(DataElement* data, Game* game);
#endif
};
class EngineAcc : public StateChange {
public:
  mpssVec3 _acc;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class SensorPow : public StateChange {
public:
  power_type_W _power;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class LaserShot : public StateChange {
public:
  sVec3 _dir;
  energy_type_J _energy;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class ThermalRadiation : public StateChange {
public:
  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class SensorPing : public StateChange {
public:
  energy_type_J _energy;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
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

class Object {
protected:
  uint64_t _ID;
  mVec3 _relativePos;
  int _maxHealth;
  keyframe<value<int> > _health;
  distance_type_m _radius;
  string _name;
public:

  Object(mVec3 relativePos, int maxHealth, distance_type_m radius, int health, uint64_t ID) {
    _relativePos = relativePos;
    _maxHealth = maxHealth;
    _health.addFrame(0, health);
    _radius = radius;
	  _ID = ID;
  }

  Drone* parentShip;

  uint64_t getId() {
    return _ID;
  }

  const enum Type {
    Ship = 1,
    Shield = 2,
    Sensor = 3,
    Computer = 4,
    Generator = 5,
    Storage = 6,
    Engine = 7,
    Laser = 8
  };
  virtual int type() { throw 1; return 0; }
  string name() { return _name; }
  virtual power_type_W getGeneratedPower(time_type_s time) { return 0;}
  virtual power_type_W getUsedPower(time_type_s time) {return 0;}
  virtual power_type_W getMaxPower(time_type_s time) {return 0;}
  virtual energy_type_J getMaxEnergy(time_type_s time) {return 0;}
  virtual energy_type_J getStoredEnergy(time_type_s time) {return 0;}
  virtual energy_type_J useEnergy(time_type_s time, energy_type_J amount){return amount;} //no change
  virtual energy_type_J chargeEnergy(time_type_s time, energy_type_J amount) {
    return amount;
  } //no change

  int getHealth(time_type_s time) {
    return _health.getAt(time)();
  }
  int getMaxHealth(time_type_s time) {
    return _maxHealth;
  }

  virtual mpssVec3 getAccel(time_type_s time) {
    return {0,0,0};
  }

  Movement getMovement(time_type_s time);

#ifdef M_CLIENT
  list< pair<double, pair<Object*, Path*>>> getIntersect(vec3<double> ori, vec3<double> dir);
  virtual void setSidebarElement(string filename);
  virtual void setSidebar();
  virtual void drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time) {}
  void drawObject(float camcx, float camcy, float camcz, float d, time_type_s time);
#endif

  virtual void collectEvents(list<StateChange*> &addTo, time_type_s time);

#ifdef M_SERVER
  virtual void getPathVirt(time_type_s time, Path* p) {

  }
  void getPath(time_type_s time, Path* p, Game* g);
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  virtual void getVStatus(DataElement* data);
  virtual void setVStatus(DataElement* data);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p);

  ~Object();
};
class Sensor : public Object {       //Order of serialisation
private:
  keyframe<value<power_type_W>> _power;
  power_type_W _maxPower;
  energy_type_J _tempE;
  list<tuple<time_type_s, energy_type_J, bool>> _pings; //time, energy, eventCrate
public:
  Sensor(mVec3 relativePos, int maxHealth, distance_type_m radius, int health, power_type_W maxPower, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _power.addFrame(0, 0);
    _maxPower = maxPower;
    _name = "Sensor";
  }

  int type() {return Type::Sensor;}

  power_type_W getUsedPower(time_type_s time) { return _power.getAt(time)(); }
  power_type_W getMaxPower(time_type_s time) { return _maxPower; }

  void setPower(time_type_s time, power_type_W val) {
    _power.addFrame(time, max(min(val, _maxPower), 0.0));
  }
  void setEnergy(energy_type_J val) {
    _tempE = val;
  }
  void ping(time_type_s time) {
    _pings.push_back(make_tuple(time, _tempE, false));
  }


#ifdef M_SERVER
  void getPathVirt(time_type_s time, Path* p);
#endif

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};
class Engine : public Object {       //Order of serialisation
private:
  power_type_W _maxPower;
  keyframe<value<mpssVec3>> _accel;
public:
  Engine(mVec3 relativePos, int maxHealth, distance_type_m radius, int health, power_type_W maxPower, mpssVec3 accel, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _accel.addFrame(0, accel);
    _maxPower = maxPower;
    _name = "Engine";

  }

  int type() { return Type::Engine; }
  power_type_W getUsedPower(time_type_s time) { return _accel.getAt(time)().sqrlen(); }
  power_type_W getMaxPower(time_type_s time) { return _maxPower; }

  mpssVec3 getAccel(time_type_s time) {
    return _accel.getAt(time)();
  }

  void setAccel(time_type_s time, mpssVec3 acc);
  void setComponent(time_type_s time, int c, acc_type_mperss val);

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};
class Laser : public Object {       //Order of serialisation
private:
  list<tuple<time_type_s,energy_type_J, sVec3, bool>> _shots; //time, energy, direction, eventCrated
  //pair<energy_type_J, sVec3> _shot;
  keyframe<value<energy_type_J>> _energyStored;
  keyframe<value<energy_type_J>> _maxStorage;

  energy_type_J _tempE;
  sVec3 _tempD;
public:
  Laser(mVec3 relativePos, int maxHealth, distance_type_m radius, int health, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _name = "Laser";
    _energyStored.addFrame(0, 0);
    _maxStorage.addFrame(0, 0);
  }

  int type() { return Type::Laser; }

  energy_type_J getMaxEnergy(time_type_s time) { return 0; }
  energy_type_J getStoredEnergy(time_type_s time) { return 0; }
  energy_type_J useEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() - amount;
    energy_type_J remain = max(0.0, -nenergy);
    _energyStored.addFrame(time, max(0.0, nenergy)); //cant go below 0 energy
    return max(0.0, remain);
  }
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() + amount;
    energy_type_J extra = nenergy - _maxStorage.getAt(time)();
    _energyStored.addFrame(time, min(_maxStorage.getAt(time)(), _energyStored.getAt(time)())); //cant go below 0 energy
    return max(0.0, extra);
  }

  void setComponent(int index, scalar_type comp) {
    _tempD[index] = comp;
  }
  void setDir(sVec3 dir) {
    _tempD = dir;
  }
  void setEnergy(energy_type_J val) {
    _tempE = val;
  }

  void shoot(time_type_s time) {
    _shots.push_back(make_tuple(time, _tempE, _tempD * (SOL / _tempD.length()), false));
  }

#ifdef M_CLIENT
  void setSidebar();
  void drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time);
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};
class Generator : public Object {       //Order of serialisation
private:
  keyframe<value<power_type_W>> _maxPower;
  keyframe<value<energy_type_J>> _energyStored;
  keyframe<value<energy_type_J>> _maxStorage;
public:
  Generator(mVec3 relativePos, int maxHealth, float radius, int health, float maxPower, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _maxPower.addFrame(0, maxPower);
    _name = "Generator";
    _energyStored.addFrame(0, 0);
    _maxStorage.addFrame(0, 0);
  }
  int type() { return Type::Generator; }
  
  power_type_W getGeneratedPower(time_type_s time) { return _maxPower.getAt(time)(); }
  power_type_W getMaxPower(time_type_s time) { return _maxPower.getAt(time)(); }
  energy_type_J getMaxEnergy(time_type_s time) { return _maxStorage.getAt(time)(); }
  energy_type_J getStoredEnergy(time_type_s time) { return _energyStored.getAt(time)(); }
  energy_type_J useEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() - amount;
    energy_type_J remain = max(0.0,-nenergy);
    _energyStored.addFrame(time, max(0.0, nenergy)); //cant go below 0 energy
    return max(0.0, remain); 
  }
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() + amount;
    energy_type_J extra = nenergy - _maxStorage.getAt(time)();
    _energyStored.addFrame(time, min(_maxStorage.getAt(time)(), _energyStored.getAt(time)())); //cant go below 0 energy
    return max(0.0, extra);
  }

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};

///############################################///
//                     Ship                     //
///############################################///

class Drone {
public:
  keyframe<Movement> mov;
  long int _droneID;
  list<Object*> objects;
  list<Sighting*> sightings;

  time_type_s lastEvtTime;

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    auto it = objects.begin();
    while (it != objects.end()) {
      res.splice(res.end(), (*it)->intersect(p));
      ++it;
    }
    return res;
  }

  Object* getObject(uint64_t oid) {
    for (auto it : objects) {
      if (it->getId() == oid) {
        return it;
      }
    }
    return NULL;
  }
  
  power_type_W getGeneratedShipPower(time_type_s time);
  power_type_W getUnusedShipPower(time_type_s time);
  power_type_W getUsedShipPower(time_type_s time);
  energy_type_J getMaxShipEnergy(time_type_s time);
  energy_type_J getStoredShipEnergy(time_type_s time);
  energy_type_J useEnergy(time_type_s time, energy_type_J amount);
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount);
  void refreshEnergy(time_type_s time);
  Event* runOut(); //when will storage run out

  int getHealth(time_type_s time);
  int getMaxHealth(time_type_s time);

  void sightMovement(Movement& m, time_type_s time);

  mpssVec3 getAccel(time_type_s time) {
    mpssVec3 sum = { 0,0,0 };
    for (auto it : objects) {
      sum += it->getAccel(time);
    }
    return sum;
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

    Movement m;
    m.pos.z = 1000.0 * _ID;
    mov.addFrame(0, m);
  }

#ifdef M_SERVER
  NetworkS* connectedClient;

  //int makeMove(DataElement* data);
  void newTurn(int id);

  bool packetRecv(DataElement *Data, int Id, NetworkS* thisptr);

  void collectPath(list<Path*> &addTo, float time);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();
  void newTurn(int id);

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

  void selectSighting(vec3<double> ori, vec3<double> dir);

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

bool surefire(keyframe<Movement>& me, keyframe<Movement>& enemy, time_type_s when, sVec3 &direction);

#ifdef M_CLIENT
extern Ship* ship;
extern time_type_s timeNow;
extern Object* selectedo;
extern Sighting* selecteds;
#endif