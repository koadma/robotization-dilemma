#pragma once

#include "Object.h"

class Engine : public Object {       //Order of serialisation
private:
  keyframe<value<mpssVec3>> _accelRequest;
  keyframe<value<mpssVec3>> _accel;
public:
  Engine(Drone* parentShip, uint64_t ID) :
  Object(parentShip, ID)
  {}

  int type() { return Type::Engine; }

  power_type_W getDisplayMaxPower(time_type_s time) {
    return _maxUseablePower.getAt(time);
  }
  power_type_W getDisplayPower(time_type_s time) {
    return _usedPower.getAt(time);
  }

  mpssVec3 getAccel(time_type_s time) {
    return _accel.getAt(time)();
  }

  power_type_W accel2power(mpssVec3 acc) {
    return acc.sqrlen();
  }
  acc_type_mperss power2accel(power_type_W pow) {
    return sqrt(pow);
  }

#ifdef M_SERVER
  bool loadV(xml_node<>* data, time_type_s time);
  void energyCallbackV(time_type_s t, Game* g);
  void setTargetAccel(time_type_s time, mpssVec3 acc, Game* g);
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