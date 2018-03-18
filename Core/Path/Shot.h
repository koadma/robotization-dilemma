#pragma once

#include "Path.h"

class Shot : public Path {
public:
  mVec3 origin;
  time_type_s origintime;
  mpsVec3 vel;
  energy_type_J energy;
  virtual int type() {
    return PathTypeShot;
  }
  int etype() {
    return EqnTypeExplicit;
  }
  Eqnsys getEquations(bool b) {//approximate
    Eqnsys res;
    if (b) {
      throw 1; //Cant approxiamte shot
    }
    else {
      res.eqns['x'] = Equation<longDouble>({ { vel.x, "t" },{ -vel.x * origintime, "" } ,{ origin.x, "" },{ -1, "x" } });
      res.eqns['y'] = Equation<longDouble>({ { vel.y, "t" },{ -vel.y * origintime, "" } ,{ origin.y, "" },{ -1, "y" } });
      res.eqns['z'] = Equation<longDouble>({ { vel.z, "t" },{ -vel.z * origintime, "" } ,{ origin.z, "" },{ -1, "z" } });
    }
    return res;
  }
};