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

Movement Movement::goTo(float gTime, float maxVelocity) {
  /*if (time < timestamp) {
  return goBackTo(time, maxVelocity);
  }
  else {
  return goForwardTo(time, maxVelocity);
  }*/
  Movement m = *this;
  m.pos = pos + vel*gTime + acc*pow(gTime, 2) / 2.0f;
  m.vel = vel + acc*gTime;
  return m;
}
void Movement::get(DataElement* data) {
  DataElement* timee = new DataElement();
  timee->_core->fromType<float>(gTimeStamp);
  data->addChild(timee);

  DataElement* pose = new DataElement();
  pos.set(pose);
  data->addChild(pose);
  
  DataElement* vele = new DataElement();
  vel.set(vele);
  data->addChild(vele);

  DataElement* acce = new DataElement();
  acc.set(acce);
  data->addChild(acce);

  DataElement* typee = new DataElement();
  typee->_core->fromType<int>(type());
  data->addChild(typee);

  DataElement* pathe = new DataElement();
  pathe->_core->fromType<string>(pathData);
  data->addChild(pathe);

  DataElement* rade = new DataElement();
  rade->_core->fromType<double>(radius);
  data->addChild(rade);
}
void Movement::set(DataElement* data) {
  //gTimeStamp
  gTimeStamp = data->_children[0]->_core->toType<float>();

  //pos
  pos.set(data->_children[1]);

  //vel
  vel.set(data->_children[2]);

  //acc
  acc.set(data->_children[3]);

  //type
  //type = data->_children[0]->_core->toType<int>();

  //pathData
  pathData = data->_children[0]->_core->toType<string>();

  //radius
  radius = data->_children[0]->_core->toType<double>();
}
Movement::~Movement() {

}

int Sighting::getLastSmaller(float t)
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
Movement Sighting::estimatePos(float t, float maxVelocity) {
  int id = getLastSmaller(t);
  if (id == -1) {
    id = 0;
  }
  if (id < keyframes.size()) {
    return keyframes[id]->goTo(t, maxVelocity);
  }
  else {
    throw 1;
    return Movement();
  }
}
#ifdef M_CLIENT
void Sighting::drawSighting(float camcx, float camcy, float camcz, float d, float maxVel) {
  auto it = keyframes.begin();

  int id = 0;

  glBegin(GL_LINE_STRIP);

  if (keyframes.size()) {
    float t = keyframes[0]->gTimeStamp;

    while (t < keyframes[keyframes.size() - 1]->gTimeStamp + ROUND_TIME) {
      Movement estpos = estimatePos(t, maxVel);
      glVertex3f(estpos.pos.x, estpos.pos.x, estpos.pos.x);
      t += ROUND_TIME * 0.1f;
    }
  }

  glEnd();

  //drawPointingVector(camcx, camcy, camcz, d);
}
#endif
void Sighting::getSighting(DataElement* data) {
  for (auto it : keyframes) {
    DataElement* ne = new DataElement();
    it->get(ne);
    data->_children.push_back(ne);
  }
}
void Sighting::setSighting(DataElement* data) {
  clearKeyframes();

  for (DataElement* it : data->_children) {
    Movement* nMov = new Movement();
    nMov->set(it);
    keyframes.push_back(nMov);
  }
}
void Sighting::clearKeyframes() {
  auto it = keyframes.begin();

  while (it != keyframes.end()) {
    (*it)->~Movement();
    delete *it;
  }

  keyframes.clear();
}
Sighting::~Sighting() {

}
