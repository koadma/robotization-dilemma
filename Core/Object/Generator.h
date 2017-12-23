#pragma once

#include "Object.h"

class Generator : public Object {       //Order of serialisation
private:

public:
  Generator(Drone* parentShip, uint64_t ID) :
    Object(parentShip, ID)
  {}
  Generator(Drone* parentShip, uint64_t ID, mVec3 relativePos, int maxHealth, float radius, int health, float maxPower) :
  Object(parentShip, ID, relativePos, maxHealth, radius, health) {
    maxGeneratedPowerChange(0, maxPower);
    _name = "Generator";
    energyStoredChange(0, 0);
    maxStorageChange(0, 0);
  }
  int type();

  power_type_W getDisplayMaxPower(time_type_s time) { ///Max generatable power, max usable, whatever applicable
    return _maxGeneratedPower.getAt(time)();
  }
  power_type_W getDisplayPower(time_type_s time) {
    return _generatedPower.getAt(time)();
  }

#ifdef M_SERVER
  bool loadV(xml_node<>* data) {
    xml_node<>* elem;

    elem = data->first_node("shipclass");
    if (!elem) {
      return false;
    }
    
  }
#endif

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};