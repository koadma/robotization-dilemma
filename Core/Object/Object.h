#pragma once

#include "../Resources.h"

class Drone;

class Object {
protected:
  uint64_t _ID;
  mVec3 _relativePos;
  int _maxHealth;
  keyframe<value<int> > _health;
  distance_type_m _radius;
  string _name;
  FlowVertex<energy_type_J, power_type_W, time_type_s>* _energySystem;

  keyframe<value<power_type_W>> _maxGeneratedPower;
  keyframe<value<power_type_W>> _generatedPower;
  keyframe<value<power_type_W>> _maxUseablePower;
  keyframe<value<power_type_W>> _requestedPower;
  keyframe<value<power_type_W>> _usedPower;
  keyframe<value<energy_type_J>> _maxStorage;
  keyframe<value<energy_type_J>> _energyStored;
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
  power_type_W getMaxGeneratedPower(time_type_s time);
  power_type_W getGeneratedPower(time_type_s time);
  power_type_W getMaxUseablePower(time_type_s time);
  power_type_W getRequestedPower(time_type_s time);
  power_type_W getUsedPower(time_type_s time);
  energy_type_J getMaxEnergy(time_type_s time);
  energy_type_J getStoredEnergy(time_type_s time);
  energy_type_J useEnergy(time_type_s time, energy_type_J amount);
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount);
  void maxGeneratedPowerChange(time_type_s time, power_type_W power);
  void requestedPowerChange(time_type_s time, power_type_W power);
  void energyStoredChange(time_type_s time, energy_type_J energy);
  void maxStorageChange(time_type_s time, energy_type_J energy);

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
  virtual void updateEnergy(time_type_s t) {}
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  virtual void getVStatus(DataElement* data);
  virtual void setVStatus(DataElement* data);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p);

  ~Object();
};