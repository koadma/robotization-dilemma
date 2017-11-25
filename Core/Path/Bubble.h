#pragma once

#include "Movement.h"

class Bubble : public Path
{
public:
  enum Type {
    Ping = 1,
    Thermal = 2,
    Chat = 3
  };
  mVec3 origin;
  time_type_s gEmissionTime;
  Movement emitter;
  energy_type_J energy = 1.0f;
  string data = "";
  en_flux_type_Jpermm getFlux(time_type_s t) {
    if (t <= gEmissionTime) {
      return 0;
    }
    return energy / (4 * PI * (SOL * (t - gEmissionTime)) * (SOL * (t - gEmissionTime)));
  }
  Type btype;
  int etype() {
    return EqnTypeImplicit;
  }
  virtual int type() {
    return PathTypeBubble;
  }
  Eqnsys getEquations(bool b) {//approximate
    Eqnsys res;
    if (b) {
      throw 1; //Cant approxiamte bubble
    }
    else {
      res.eqns['t'] =
        Equation<double>(vector<pair<double, string> >{{ 1, "x" }, { -origin.x, "" }}) * Equation<double>(vector<pair<double, string> >{{ 1, "x" }, { -origin.x, "" }}) +
        Equation<double>(vector<pair<double, string> >{{ 1, "y" }, { -origin.y, "" }}) * Equation<double>(vector<pair<double, string> >{{ 1, "y" }, { -origin.y, "" }}) +
        Equation<double>(vector<pair<double, string> >{{ 1, "z" }, { -origin.z, "" }}) * Equation<double>(vector<pair<double, string> >{{ 1, "z" }, { -origin.z, "" }}) +
        Equation<double>(vector<pair<double, string> >{{ 1, "t" }, { -gEmissionTime, "" }}) * Equation<double>(vector<pair<double, string> >{{ 1, "t" }, { -gEmissionTime, "" }}) * SOL * SOL * (-1.0);
    }
    return res;
  }
  bool isWellIn(mVec3 point, distance_type_m radius, time_type_s time) {
    distance_type_m brad = (time - gEmissionTime) * SOL;
    return (2 * (brad + radius)) > ((point - origin).length());
  }
  //unsigned int receiver; //only for reflection, would be cleaner with virtual methods
  //friend std::ostream& operator<<(std::ostream& os, const Bubble& b); // only for test
 
};

