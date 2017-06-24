#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "../Core/Network.h"


class Eqnsys {
public:
  map<char, Equation<double> > eqns;
};


class Path {
public:
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
  fVec3 emitterVelocity;
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
/*
class BubbleManager
{
private:
  std::list<Bubble> bubbles;
  bool didDetectPassively(Bubble bubble) const;
  bool didDetectActively(Bubble bubble) const;
  bool isSureFire(Bubble bubble) const;
public:
  void interactWithShip(Ship& ship);
  void add(Bubble bubble);
  void move();
  void out() const; //for test
};*/

#endif
