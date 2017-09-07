#pragma once

#include "../Maths/Polynomial.h"

class Path {
public:
  uint64_t originID;
  const enum PathType {
    PathTypePath = 0,
    PathTypeShot = 1,
    PathTypeBubble = 2,
    PathTypeMovement = 3
  };
  const enum EqnType {
    EqnTypeExplicit = 0,
    EqnTypeApproxable = 1,
    EqnTypeImplicit = 2,
    EqnTypeUndefined = 3
  };
  virtual int type() {
    return PathTypePath;
  }
  virtual int etype() {
    return EqnTypeUndefined;
  }
  virtual Eqnsys getEquations(bool b) {//approximate
    throw 1;
    return Eqnsys();
  }
};