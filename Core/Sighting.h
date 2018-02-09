#pragma once

#ifdef M_SERVER
#include "../Core/Path/Bubble.h"
#include "../Core/Path/Shot.h"
#include "../Core/Path/Movement.h"
#endif
#ifdef M_CLIENT
#include "../robotization dilemma/RenderOut.h"
#endif

//typedef keyframe<unpoint<Movement*>> Sighting;

class Sighting {
public:
  keyframe<Movement, Movement, time_type_s> keyframes;
  void addFrame(time_type_s& _time, Movement& _val) {
    keyframes.addFrame(_time, _val);
  }

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
  pair<distance_type_m, bool> closest(Movement* p) {
    priority_queue<distance_type_m, vector<distance_type_m>, greater<distance_type_m>> intersects;
    if(keyframes.size()) {
      auto it = keyframes._frames.begin();
      auto nit = it;
      while (it != keyframes._frames.end()) {
        ++nit;
        vector<time_type_s> temp = intersectPaths(&it->second, p, true);
        for (auto && tit : temp) {
          if ((it == keyframes._frames.begin() || it->first <= tit) && (nit == keyframes._frames.end() || tit <= nit->first)) {
            intersects.push((p->getAt(tit).pos - it->second.getAt(tit).pos).length());
          }
          intersects.push((p->getAt(it->first).pos - it->second.getAt(it->first).pos).length());
        }
        ++it;
      }
    }
    if(intersects.size()) {
      return{ intersects.top(), true };
    }
    return {0, false};
  }

#ifdef M_CLIENT
  mVec3 reCalcPosition(mVec3 position, mVec3 viewCenter, distance_type_m maxDist);

  void drawSighting(mVec3 viewCenter, float d, vel_type_mpers maxVel, time_type_s time, /*distance_type_m maxDist,*/ OpenGLData data, bool selected);
#endif
};

//Surefire maths

class result
{
public:
  bool answ;
  sVec3 dir;

  result(double inputx, double inputy, double inputz, bool inansw) : dir(inputx, inputy, inputz), answ(inansw) {};
  result(sVec3 idir, bool inansw) : dir(idir), answ(inansw) {};
};

result coordinates(mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, acc_type_mperss amax, distance_type_m d, time_type_s tpassed);

//automata visszaloveshez a koordianatak, ez meg az eszleles pillanataban megtortenik
result surefire1(time_type_s obsTime, mpsVec3 vTarget, mVec3 sTarget, time_type_s now, mVec3 sShip, acc_type_mperss amax, distance_type_m d);

//azt szamoljuk ki hogy az elozo korben tortent eszlelesre a mostani mozgassorok közben meddig biztos a talalat
//vector<result> surefire2(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, keyframe<Movement>& movements, double utime, double round);

//result surefire3(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, keyframe<Movement>& movements, double utime, double qtime, double round);
