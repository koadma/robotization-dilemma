#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "Network.h"

class Eqnsys {
public:
  map<char, Equation<double> > eqns;
};

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

class Movement : public Path { //Order of serialisation
public:
                               //4
  virtual int type() {
    return PathTypeMovement;
  }
  int etype() {
    return EqnType::EqnTypeApproxable;
  }
  time_type_s gTimeStamp = 0;  //0
  mVec3 pos = fVec3(0);        //1
  mpsVec3 vel = fVec3(0);      //2
  mpssVec3 acc = fVec3(0);     //3
  string pathData;             //5
  distance_type_m radius;      //6
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

  Movement getAt(time_type_s gTime, vel_type_mpers maxVelocity) const;
  Movement getAt(time_type_s gTime) const;

  void get(DataElement* data);
  void set(DataElement* data);

  ~Movement();
};

class Bubble : public Path
{
public:
  enum Type {
    Ping = 1,
    Thermal = 2,
    Chat = 3
  };
  mVec3 origin;
  time_type_s gEmissionTime;
  Movement emitter;
  energy_type_J energy = 1.0f;
  en_flux_type_Jpermm getFlux(time_type_s t) {
    if (t <= gEmissionTime) {
      return 0;
    }
    return energy / ((SOL * (t - gEmissionTime)) * (SOL * (t - gEmissionTime)));
  }
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
