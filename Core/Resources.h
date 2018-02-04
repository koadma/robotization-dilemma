#pragma once

#include "Event.h"

class ResourceData {
public:
  int _ID;
  string _name;
  string _unit;
  string _derivative;

  template<typename T>
  string format(T val) {
    return to_string(val) + " " + _unit;
  }
  template<typename T>
  string formatDerivaitive(T val) {
    return to_string(val) + " " + _derivative;
  }

  ResourceData(int ID, string name, string unit, string derivative) {
    _ID = ID;
    _name = name;
    _unit = unit;
    _derivative = derivative;
  }
};

extern vector<ResourceData> resources;