#pragma once

#include "Object.h"

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

  int type() { return Type::Sensor; }

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