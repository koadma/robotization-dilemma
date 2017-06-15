#pragma once

#ifdef M_SERVER
#include "../Server/Bubble.h"
#endif
#ifdef M_CLIENT
#include "../robotization dilemma/RenderOut.h"
#endif


vector<double> intersectPaths(Path* lhs, Path* rhs);

class Movement : public Path { //Order of serialisation
public:
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
  int type;                    //4
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
  Movement goTo(float gTime, float maxVelocity);
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

class Sighting {
public:
  vector<Movement*> keyframes; //time - keyframe. Sorted.
  int getLastSmaller(float t);
  Movement estimatePos(float t, float maxVelocity);

#ifdef M_CLIENT
  void drawSighting(float camcx, float camcy, float camcz, float d, float maxVel);
#endif

  void getSighting(DataElement* data);
  void setSighting(DataElement* data);

  void clearKeyframes();
  ~Sighting();
};
