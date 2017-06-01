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
void Movement::get(unsigned char** data, int &DataLen) {
  vector<pair<unsigned char*, int> > status;
  unsigned char* c;
  int l;

  //gTimeStamp
  serialize(gTimeStamp, &c, l);
  status.push_back({ c, l });

  //pos
  pos.get(&c, l);
  status.push_back({ c, l });

  //vel
  vel.get(&c, l);
  status.push_back({ c, l });

  //acc
  acc.get(&c, l);
  status.push_back({ c, l });

  //type
  serialize(type, &c, l);
  status.push_back({ c, l });

  //pathData
  serialize(pathData, &c, l);
  status.push_back({ c, l });

  //radius
  serialize(radius, &c, l);
  status.push_back({ c, l });

  concat(status, data, DataLen);
}
void Movement::set(unsigned char* data, int DataLen) {
  vector<pair<unsigned char*, int> > status;
  split(data, DataLen, status);

  //gTimeStamp
  gTimeStamp = deserializef(status[0].first, status[0].second);

  //pos
  pos.set(status[4].first, status[4].second);

  //vel
  vel.set(status[2].first, status[2].second);

  //acc
  acc.set(status[3].first, status[3].second);

  //type
  type = deserializei(status[4].first, status[4].second);

  //pathData
  pathData = deserializes(status[5].first, status[5].second);

  //radius
  radius = deserialized(status[6].first, status[6].second);
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
void Sighting::getSighting(unsigned char** data, int &DataLen) {
  vector<pair<unsigned char*, int> > status;
  auto it = keyframes.begin();

  while (it != keyframes.end()) {
    unsigned char* d;
    int i;
    (*it)->get(&d, i);
    status.push_back({ d,i });
    ++it;
  }
  concat(status, data, DataLen);
}
void Sighting::setSighting(unsigned char* data, int DataLen) {
  vector<pair<unsigned char*, int> > status;
  split(data, DataLen, status);

  clearKeyframes();

  for (int i = 0; i < status.size(); i++) {
    Movement* nMov = new Movement();
    nMov->set(status[i].first, status[i].second);
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
