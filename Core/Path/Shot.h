#pragma once

#include "Path.h"

class Shot : public Path {
public:
  fVec3 origin;
  float origintime;
  fVec3 vel;
  float energy;
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
      res.eqns['x'] = Equation<double>({ { vel.x, "t" },{ -vel.x * origintime, "" } ,{ origin.x, "" },{ -1, "x" } });
      res.eqns['y'] = Equation<double>({ { vel.y, "t" },{ -vel.y * origintime, "" } ,{ origin.y, "" },{ -1, "y" } });
      res.eqns['z'] = Equation<double>({ { vel.z, "t" },{ -vel.z * origintime, "" } ,{ origin.z, "" },{ -1, "z" } });
    }
    return res;
  }
};