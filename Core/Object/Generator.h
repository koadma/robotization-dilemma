#pragma once

#include "Object.h"

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

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};