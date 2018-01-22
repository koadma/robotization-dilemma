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

  class constrain {
  private:
    bool _include;
    sVec3 _dir;
    double _cosphi;
  public:
    enum {
      include = true,
      exclude = false
    };
    void verify(bool& current, sVec3 toVerify) {
      toVerify.norm();
      bool in = dot(toVerify, _dir) >= _cosphi;
      //If should be included and is in -> its current

      //If it was current, to exclude, and not in -> its in
      //If it was current, to include, and not in -> its in
      //Sum: If it was current, and not in -> its in;
      current = (_include and in) or (current and not in);
    }
    constrain(bool sign, sVec3 dir, double cosphi) : _include(sign), _dir(dir.norm()), _cosphi(cosphi) { }
  };
  mVec3 origin;
  time_type_s gEmissionTime;
  Movement emitter;
  energy_type_J energy = 1.0f;
  string data = "";
  vector<constrain> constrains;
  en_flux_type_Jpermm getFlux(time_type_s t) {
    if (t <= gEmissionTime) {
      return 0;
    }
    return energy / (4 * PI * (SOL * (t - gEmissionTime)) * (SOL * (t - gEmissionTime)));
  }
  bool verify(time_type_s time, mVec3 pos) {
    bool in = false;
    for (auto&& it : constrains) {
      it.verify(in, pos - origin);
    }
    return in;
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

