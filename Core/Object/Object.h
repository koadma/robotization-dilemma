#pragma once

#include "../Event.h"

class Drone;

class Object {
protected:
  uint64_t _ID;
  mVec3 _relativePos;
  int _maxHealth;
  keyframe<value<int> > _health;
  distance_type_m _radius;
  string _name;
public:

  Object(uint64_t ID) {
    _ID = ID;
  }

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
  virtual power_type_W getGeneratedPower(time_type_s time) { return 0; }
  virtual power_type_W getUsedPower(time_type_s time) { return 0; }
  virtual power_type_W getMaxPower(time_type_s time) { return 0; }
  virtual energy_type_J getMaxEnergy(time_type_s time) { return 0; }
  virtual energy_type_J getStoredEnergy(time_type_s time) { return 0; }
  virtual energy_type_J useEnergy(time_type_s time, energy_type_J amount) { return amount; } //no change
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
    return{ 0,0,0 };
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

  bool load(xml_node<>* data);
  virtual bool loadV(xml_node<>* data) { return false; }
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  virtual void getVStatus(DataElement* data);
  virtual void setVStatus(DataElement* data);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p);

  ~Object();
};