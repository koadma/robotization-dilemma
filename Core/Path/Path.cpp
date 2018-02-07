#include "Path.h"

int Path::type() {
  return PathTypePath;
}
int Path::etype() {
  return EqnTypeUndefined;
}
Eqnsys Path::getEquations(bool b) {//approximate
  throw 1;
  return Eqnsys();
}
bool Path::verify(time_type_s time, mVec3 pos) {
  return true;
}
vector<time_type_s> Path::intersect(Path* other) {
  return intersectPaths(this, other);
}

vector<double> intersectPaths(Path* lhs, Path* rhs) {
  Eqnsys impleq; //
  Eqnsys expleq; //
  if (lhs->etype() == Path::EqnTypeExplicit && rhs->etype() == Path::EqnTypeExplicit) {
    throw 1;
  }
  if (lhs->etype() == Path::EqnTypeExplicit && rhs->etype() == Path::EqnTypeApproxable) {
    impleq = rhs->getEquations(false);
    expleq = lhs->getEquations(false);
  }
  if (lhs->etype() == Path::EqnTypeExplicit && rhs->etype() == Path::EqnTypeImplicit) {
    impleq = rhs->getEquations(false);
    expleq = lhs->getEquations(false);
  }
  if (lhs->etype() == Path::EqnTypeApproxable && rhs->etype() == Path::EqnTypeExplicit) {
    impleq = lhs->getEquations(false);
    expleq = rhs->getEquations(false);
  }
  if (lhs->etype() == Path::EqnTypeApproxable && rhs->etype() == Path::EqnTypeApproxable) {
    impleq = lhs->getEquations(false);
    expleq = rhs->getEquations(true);
  }
  if (lhs->etype() == Path::EqnTypeApproxable && rhs->etype() == Path::EqnTypeImplicit) {
    impleq = rhs->getEquations(false);
    expleq = lhs->getEquations(true);
  }
  if (lhs->etype() == Path::EqnTypeImplicit && rhs->etype() == Path::EqnTypeExplicit) {
    impleq = lhs->getEquations(false);
    expleq = rhs->getEquations(false);
  }
  if (lhs->etype() == Path::EqnTypeImplicit && rhs->etype() == Path::EqnTypeApproxable) {
    impleq = lhs->getEquations(false);
    expleq = rhs->getEquations(true);
  }
  if (lhs->etype() == Path::EqnTypeImplicit && rhs->etype() == Path::EqnTypeImplicit) {
    throw 1;
  }

  if (impleq.eqns.size() == 1) {
    auto it = expleq.eqns.begin();
    while (it != expleq.eqns.end()) {
      impleq.eqns.begin()->second.substitute(it->second, it->first);
      ++it;
    }
  }

  return impleq.eqns.begin()->second.getPolynomial('t').solve();
}
