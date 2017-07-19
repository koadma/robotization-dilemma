#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "../Core/Network.h"

class Eqnsys {
public:
  map<char, Equation<double> > eqns;
};

class Path {
public:
  long int originID;
  static const enum PathType {
    PathTypePath = 0,
    PathTypeShot = 1,
    PathTypeBubble = 2,
    PathTypeMovement = 3
  };
  static const enum EqnType {
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

class Movement : public Path { //Order of serialisation
public:
  virtual int type() {
    return PathTypeMovement;
  }
  int etype() {
    return EqnType::EqnTypeApproxable;
  }
  float gTimeStamp = 0;        //0
  fVec3 pos = fVec3(0);        //1
                               //float posUncertainty = 0;
  fVec3 vel = fVec3(0);        //2
                               //float velUncertainty = 0;
  fVec3 acc = fVec3(0);        //3
                               //float accUncertainty = 0;
                               //int type;                    //4
  string pathData;             //5
  double radius;               //6
  Eqnsys getEquations(bool b) {//approximate
    Eqnsys res;
    if (!b) { //Parametric curve
      Eqnsys resh;

      resh.eqns['a'] = Equation<double>({ { acc.x / 2.0f, "tt" },{ vel.x, "t" } ,{ pos.x, "" },{ -1, "a" } });
      resh.eqns['b'] = Equation<double>({ { acc.y / 2.0f, "tt" },{ vel.y, "t" } ,{ pos.y, "" },{ -1, "b" } });
      resh.eqns['c'] = Equation<double>({ { acc.z / 2.0f, "tt" },{ vel.z, "t" } ,{ pos.z, "" },{ -1, "c" } });


      res.eqns['t'] =
        Equation<double>({ { 1, "x" },{ -1, "a" } }) * Equation<double>({ { 1, "x" },{ -1, "a" } }) +
        Equation<double>({ { 1, "y" },{ -1, "b" } }) * Equation<double>({ { 1, "y" },{ -1, "b" } }) +
        Equation<double>({ { 1, "z" },{ -1, "c" } }) * Equation<double>({ { 1, "z" },{ -1, "c" } }) +
        Equation<double>({ { -radius*radius, "" } });
      res.eqns['t'].substitute(resh.eqns['a'], 'a');
      res.eqns['t'].substitute(resh.eqns['b'], 'b');
      res.eqns['t'].substitute(resh.eqns['c'], 'c');
    }
    else { //Moving sphere
      res.eqns['x'] = Equation<double>({ { acc.x / 2.0f, "tt" },{ vel.x, "t" } ,{ pos.x, "" },{ -1, "x" } });
      res.eqns['y'] = Equation<double>({ { acc.y / 2.0f, "tt" },{ vel.y, "t" } ,{ pos.y, "" },{ -1, "y" } });
      res.eqns['z'] = Equation<double>({ { acc.z / 2.0f, "tt" },{ vel.z, "t" } ,{ pos.z, "" },{ -1, "z" } });
    }
    return res;
  }
  /*float reachMaxVelIn(float maxVelocity, bool& will) {
  //-1: No acceleration, cant be solved.
  //0 .. 2: number of solutions
  int numOfSolut;
  float sol[2];
  float a = pow(acc.length(), 2);
  float b = 2 * (vel.x*acc.x + vel.y*acc.y + vel.z*acc.z);
  float c = pow(vel.length(), 2) - pow(maxVelocity, 2); //solve (a+vt)^2 = vmax^2 for t
  if (a == 0)
  {
  will = false;
  return 0;
  } else {
  will = true;
  solve2(a, b, c, sol, numOfSolut); //calculate time of reacing maximum velocity
  if(numOfSolut < 2) {
  throw 1;
  return 0;
  } else {
  return max(sol[0], sol[1]);
  }
  }
  throw 1;
  return 0;
  }*/
  Movement getAt(float gTime, float maxVelocity);
  /*Movement goForwardTo(float time, float maxVelocity) {
  Movement res;
  res.timestamp = time;
  float dt = time - timestamp;
  bool will = false;
  float reachMaxT = reachMaxVelIn(maxVelocity, will);
  if (!will) {
  res.pos = pos + vel * dt;
  res.vel = vel;
  res.acc = 0;
  }
  else
  {
  if (reachMaxT < 0)
  {
  throw 1;
  }
  if (reachMaxT > dt)
  {
  res.pos = pos + vel * dt + acc*(pow(dt, 2) / 2);
  res.vel = vel + acc*dt;
  res.acc = acc;
  }
  else
  {
  fVec3 startVelocity = vel;
  res.vel = vel + acc*reachMaxT;
  res.pos = pos + startVelocity*dt + vel*(dt - reachMaxT) + acc*(pow(reachMaxT, 2) / 2);
  res.acc = 0;
  }
  }
  //timestamp = time;
  }
  Movement goBackTo(float time, float maxVelocity) {
  Movement res;
  res.timestamp = time;
  float dt = time - timestamp;
  res.pos = pos + vel * dt + acc*(pow(dt, 2) / 2);
  res.vel = vel + acc*dt;
  res.acc = acc;
  }*/

  void get(DataElement* data);
  void set(DataElement* data);

  ~Movement();
};

class Bubble : public Path
{
public:
  static enum Type {
    Ping = 1,
    Thermal = 2,
    Chat = 3
  };
  fVec3 origin;
  float gEmissionTime;
  //unsigned int emitter;
  Movement emitter;
  //int visibility;
  float energy = 1.0f;
  float visibility(float t) {
    if (t <= 0) {
      return 0;
    }
    return energy / ((SOL * t) * (SOL * t));
  }
  //int age; //radius = age * ROUND_TIME * SOL
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
  //unsigned int receiver; //only for reflection, would be cleaner with virtual methods
  //friend std::ostream& operator<<(std::ostream& os, const Bubble& b); // only for test
 
};

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
      throw 1; //Cant approxiamte bubble
    }
    else {
      res.eqns['x'] = Equation<double>({ { vel.x, "t" }, { -vel.x * origintime, "" } ,{ origin.x, "" },{ -1, "x" } });
      res.eqns['y'] = Equation<double>({ { vel.y, "t" }, { -vel.y * origintime, "" } ,{ origin.y, "" },{ -1, "y" } });
      res.eqns['z'] = Equation<double>({ { vel.z, "t" }, { -vel.z * origintime, "" } ,{ origin.z, "" },{ -1, "z" } });
    }
    return res;
  }
};

#endif
