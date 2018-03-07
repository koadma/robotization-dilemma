#pragma once

#include "../Resources.h"

class Drone;

class Object {
protected:
  uint64_t _ID;
  mVec3 _relativePos;
  distance_type_m _radius;
  string _name;
  keyframe<value<int> > _maxHealth;
  keyframe<value<int> > _health;
#ifdef M_SERVER
  Bubble* _selfBubble = NULL; //used to store current continous bubble
  map<Bubble*, Bubble*> reflection;
#endif
public:
  keyframe<value<power_type_W>> _maxGeneratedPower;
  keyframe<value<power_type_W>> _generatedPower;
  keyframe<value<power_type_W>> _maxUseablePower;
  keyframe<value<power_type_W>> _requestedPower;
  keyframe<value<power_type_W>> _usedPower;
  keyframe<value<power_type_W>> _selfUsedPower; //Not for battery charge
  keyframe<value<energy_type_J>> _maxStorage;
  
  Object(Drone* parentShip, uint64_t ID);

  Drone* _parentShip;
  FlowVertex<energy_type_J, Fraction, time_type_s>* _energySystem;

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
  power_type_W getSelfUsedPower(time_type_s time);
  energy_type_J getMaxEnergy(time_type_s time);
 
  energy_type_J getStoredEnergy(time_type_s time);

#ifdef M_SERVER
  void changeSelfBubble(Bubble* newSelf, Game* g);
  void reflectBubble(Bubble* what, time_type_s time, Game* g);

  energy_type_J useEnergy(time_type_s time, energy_type_J amount, Game* g);
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount, Game* g);

  void maxGeneratedPowerChange(time_type_s time, power_type_W power, Game* g);
  void maxUseablePowerChange(time_type_s time, power_type_W power, Game* g);
  void requestedPowerChange(time_type_s time, power_type_W power, Game* g);
  void maxStorageChange(time_type_s time, energy_type_J energy, Game* g);
  //void energyStoredChange(time_type_s time, energy_type_J energy, Game* g);
#endif
  void maxGeneratedPowerChange(time_type_s time, power_type_W power);
  void maxUseablePowerChange(time_type_s time, power_type_W power);
  void requestedPowerChange(time_type_s time, power_type_W power);
  void maxStorageChange(time_type_s time, energy_type_J energy);
  //void energyStoredChange(time_type_s time, energy_type_J energy);

  virtual power_type_W getDisplayMaxPower(time_type_s time) { ///Max generatable power, max usable, whatever applicable
    return 0;
  }
  virtual power_type_W getDisplayPower(time_type_s time) {
    return 0;
  }
  virtual energy_type_J getDisplayMaxEnergy(time_type_s time) { ///Max stored energy, used in an operation, whatever applicable
    return 0;
  }
  virtual energy_type_J getDisplayEnergy(time_type_s time) {
    return 0;
  }

  int getHealth(time_type_s time) {
    return _health.getAt(time)();
  }
  int getMaxHealth(time_type_s time) {
    return _maxHealth.getAt(time)();
  }

  virtual mpssVec3 getAccel(time_type_s time) {
    return{ 0,0,0 };
  }

  Movement getMovement(time_type_s time);

#ifdef M_CLIENT
  list< pair<double, pair<Object*, Path*>>> getIntersect(vec3<double> ori, vec3<double> dir);
  virtual void setSidebarElement(string filename);
  virtual void setSidebar();
  virtual void drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time, bool worldView) {}
  void drawObject(float camcx, float camcy, float camcz, float d, time_type_s time, bool worldView);
#endif

  virtual void collectEvents(list<StateChange*> &addTo, time_type_s time);

#ifdef M_SERVER
  virtual void getPathVirt(time_type_s time, Path* p, Game* g) {

  }
  void getPath(time_type_s time, Path* p, Game* g);

  bool loadEnergy(xml_node<>* data, time_type_s time);
  bool load(xml_node<>* data, time_type_s time);
  virtual bool loadV(xml_node<>* data, time_type_s time) { return true; }
  virtual void energyCallbackV(time_type_s t, Game* g) {}
  void energyCallback(time_type_s t, Game* g);
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  virtual void getVStatus(DataElement* data);
  virtual void setVStatus(DataElement* data);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p);

  ~Object();
};