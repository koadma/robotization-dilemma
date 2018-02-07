#pragma once

#include "Object.h"

class Generator : public Object {       //Order of serialisation
private:

public:
  Generator(Drone* parentShip, uint64_t ID) :
    Object(parentShip, ID)
  {}
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