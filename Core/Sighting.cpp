#include "Sighting.h"


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
/*
int Sighting::getLastSmaller(time_type_s t)
{
  int first = 0, last = int(keyframes.size()) - 1;
  while (first <= last)
  {
    int mid = (first + last) / 2;
    if (keyframes[mid]->gTimeStamp >= t)
      last = mid - 1;
    else
      first = mid + 1;
  }
  return first - 1 < 0 ? -1 : first - 1;
}
Movement Sighting::estimatePos(time_type_s t, vel_type_mpers maxVelocity) {
  int id = getLastSmaller(t);
  if (id == -1) {
    id = 0;
  }
  if (id < keyframes.size()) {
    return keyframes[id]->getAt(t, maxVelocity);
  }
  else {
    throw 1;
    return Movement();
  }
}
#ifdef M_CLIENT
void Sighting::drawSighting(float camcx, float camcy, float camcz, float d, vel_type_mpers maxVel) {
  glLineWidth(2.0f);

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (keyframes.size()) {
    time_type_s t = keyframes[0]->gTimeStamp;

    while (t < keyframes[keyframes.size() - 1]->gTimeStamp + ROUND_TIME) {
      Movement estpos = estimatePos(t, maxVel);
      glVertex3d(estpos.pos.x, estpos.pos.y, estpos.pos.z);
      t += ROUND_TIME * 0.1f;
    }
  }

  glEnd();

  //drawPointingVector(camcx, camcy, camcz, d);
}
#endif
void Sighting::getSighting(DataElement* data) {
  keyframes.get(data);
}
void Sighting::setSighting(DataElement* data) {
  keyframes.set(data);
}
Sighting::~Sighting() {

}
*/

#ifdef M_CLIENT
void Sighting::drawSighting(float camcx, float camcy, float camcz, float d, vel_type_mpers maxVel, time_type_s time) {
  glLineWidth(2.0f);

  glBegin(GL_LINES);
  if (keyframes.size()) {
    time_type_s t = keyframes.getFirst();

    while (t < keyframes.getLast() + ROUND_TIME) {
      Movement estpos = getAt(t);
      if (t < keyframes.getLast()) {
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      else {
        glColor3f(0.2f, 1.0f, 0.2f);
      }
      glVertex3d(estpos.pos.x, estpos.pos.y, estpos.pos.z);
      t += ROUND_TIME * 0.1f;
    }
  }
  
  glEnd();

  if (keyframes.size() && keyframes.getFirst() < time) {
    Movement now = keyframes.getAt(time);

    glTranslated(now.pos.x, now.pos.y, now.pos.z);
    glColor3f(0.0f, 1.0f, 1.0f);
    glutSolidSphere(now.radius, 20, 20);
    glTranslated(-now.pos.x, -now.pos.y, -now.pos.z);
  }
}
#endif