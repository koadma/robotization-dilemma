#pragma once

#include "Movement.h"

class Bubble : public Path
{
private:
  vector<constrain> constrains;
public:
  double solidangle;
  Movement emitter;
  energy_type_J energy = 1.0f;
  string data = "";
  Bubble* before = NULL; //only for btype Row_Border
  Bubble* after = NULL; //only for btype Row_Border
  void setConstrains(vector<constrain> to, double newsolidangle) {
    constrains = to;
    solidangle = newsolidangle;
  }
  void setConstrains(vector<constrain> to) {
    if (to.size() == 1) {
      setConstrains(to, to[0].solidangle());
    } else {
      LOG LERROR MATH "Unable to calculate solid angle" END;
    }
  }
  en_flux_type_Jpermm getFlux(time_type_s t, mVec3 pos) {
    if (t <= gStartTime) {
      LOG LERROR MATH "Asked time before creation" END;
      return 0;
    }
    if (solidangle == 0) {
      LOG LERROR MATH "Solid angle is 0" END;
      return 0;
    }
    bool in = false;
    for (auto&& it : constrains) {
      it.verify(in, pos - emitter.getAt(gStartTime).pos);
    }
    if (in) {
      return energy / (solidangle * (SOL * (t - gStartTime)) * (SOL * (t - gStartTime)));
    }
    return 0;
  }
  BubbleSource bsource;
  BubbleType btype;
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
      mVec3 origin = emitter.getAt(gStartTime).pos;
      res.eqns['t'] =
        Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "x" }, { -origin.x, "" }}) * Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "x" }, { -origin.x, "" }}) +
        Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "y" }, { -origin.y, "" }}) * Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "y" }, { -origin.y, "" }}) +
        Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "z" }, { -origin.z, "" }}) * Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "z" }, { -origin.z, "" }}) +
        Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "t" }, { -gStartTime, "" }}) * Equation<longDouble>(vector<pair<longDouble, string> >{{ 1, "t" }, { -gStartTime, "" }}) * (SOL * SOL * -1.0);
    }
    return res;
  }
  bool isWellIn(mVec3 point, distance_type_m radius, time_type_s at) {
    distance_type_m brad = (at - gStartTime) * SOL;
    return (0.5 * (brad + radius)) > ((point - emitter.getAt(gStartTime).pos).length());
  }

};

