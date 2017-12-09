#pragma once

#include "Object.h"

class Generator : public Object {       //Order of serialisation
private:

public:
  Generator(uint64_t ID) : Object(ID) {}
  Generator(mVec3 relativePos, int maxHealth, float radius, int health, float maxPower, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _maxPower.addFrame(0, maxPower);
    _name = "Generator";
    _energyStored.addFrame(0, 0);
    _maxStorage.addFrame(0, 0);
  }
  int type() { return Type::Generator; }

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