#pragma once

#ifdef M_SERVER
#include "Bubble.h"
#endif
#ifdef M_CLIENT
#include "../robotization dilemma/RenderOut.h"
#endif

vector<double> intersectPaths(Path* lhs, Path* rhs);

//typedef keyframe<unpoint<Movement*>> Sighting;

/*
class Sighting {
public:
  keyframe<unpoint<Movement*>> keyframes;
  int getLastSmaller(time_type_s t);
  Movement estimatePos(time_type_s t, vel_type_mpers maxVelocity);

#ifdef M_CLIENT
  void drawSighting(float camcx, float camcy, float camcz, float d, vel_type_mpers maxVel);
#endif

  void getSighting(DataElement* data);
  void setSighting(DataElement* data);

  ~Sighting();
};*/


class Sighting {
private:
  keyframe<Movement, Movement, time_type_s> keyframes;
public:
  void addFrame(time_type_s& _time, Movement& _val) {
    keyframes.addFrame(_time, _val);
  }

 /* typename set<pair<time_type_s, Movement> >::iterator search(time_type_s& _time) {
    keyframes.search(_time);
  }*/
  time_type_s getFirst() {
    return keyframes.getFirst();
  }
  Movement getAt(time_type_s _time) {
    return keyframes.getAt(_time);
  }
  void get(DataElement* data) {
    keyframes.get(data);
  }
  void set(DataElement* data) {
    keyframes.set(data);
  }

#ifdef M_CLIENT
  void drawSighting(float camcx, float camcy, float camcz, float d, vel_type_mpers maxVel);
#endif
};