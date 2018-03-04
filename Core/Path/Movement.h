#pragma once

#include "Path.h"

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

class constrain {
private:
  bool _include;
  sVec3 _dir;
  double _cosphi;
public:
  enum {
    include = true,
    exclude = false
  };
  void verify(bool& current, sVec3 toVerify) {
    toVerify.norm();
    bool in = dot(toVerify, _dir) >= _cosphi;
    //If should be included and is in -> its current

    //If it was current, to exclude, and not in -> its in
    //If it was current, to include, and not in -> its in
    //Sum: If it was current, and not in -> its in;
    current = (_include and in) or (current and not in);
  }
  constrain(bool sign, sVec3 dir, double cosphi) : _include(sign), _dir(dir.norm()), _cosphi(cosphi) { }
};

enum BubbleSource {
  Bubble_Ping = 1,
  Bubble_Thermal = 2,
  Bubble_Chat = 3
};
enum BubbleType {
  Bubble_Pulse = 0,
  Bubble_Row_Border = 1
};
