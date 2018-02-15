#pragma once

#ifdef M_CLIENT
#include "../../robotization dilemma/RenderOut.h"
#endif
#ifdef M_SERVER
#include "../Scripts/Scripts.h"
#endif

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
  virtual int type();
  virtual int etype();
  virtual Eqnsys getEquations(bool b);
  virtual bool verify(time_type_s time, mVec3 pos);
  virtual vector<time_type_s> intersect(Path* other);
};

vector<double> intersectPaths(Path* lhs, Path* rhs, bool closest = false);
