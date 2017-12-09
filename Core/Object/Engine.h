#pragma once

#include "Object.h"

class Engine : public Object {       //Order of serialisation
private:
  
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

#ifdef M_SERVER
  void updateEnergy(time_type_s t);
#endif

  void setTargetAccel(time_type_s time, mpssVec3 acc);
  void setComponent(time_type_s time, int c, acc_type_mperss val);

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};