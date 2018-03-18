#pragma once

#include "../Core/Path/Bubble.h"
#include "../Core/Path/Shot.h"
#include "../Core/Path/Movement.h"


//typedef keyframe<unpoint<Movement*>> Sighting;

class SightedMovement : public Movement {
public:
  Path* original;
  SightedMovement() {

  }
  SightedMovement(Movement m) : Movement(m) {

  }
  SightedMovement(Movement m, Path* p) : Movement(m), original(p) {

  }
};

class Sighting {
public:
  keyframe<SightedMovement, SightedMovement, time_type_s> _keyframes;
  bool _closed;
  time_type_s _closetime;
  void addFrame(time_type_s& time, SightedMovement& val) {
    _keyframes.addFrame(time, val);
    _closed = false;
  }


  time_type_s getFirst() {
    return _keyframes.getFirst();
  }
  SightedMovement getAt(time_type_s time) {
    return _keyframes.getAt(time);
  }
  bool tryClose(Path* p, time_type_s t) {
    if(_keyframes.size()) {
      auto it = _keyframes._frames.end();
      --it;
      if (it->second.original == p) {
        _closed = true;
        _closetime = t;
        return true;
      }
    }
    return false;
  }
  void get(DataElement* data) {
    DataElement* keye = new DataElement();
    _keyframes.get(keye);
    data->addChild(keye);

    DataElement* close = new DataElement();
    vGFunc(_closed, close);
    data->addChild(close);

    DataElement* timee = new DataElement();
    vGFunc(_closetime, timee);
    data->addChild(timee);
  }
  void set(DataElement* data) {
    _keyframes.set(data->_children[0]);
    vSFunc(_closed, data->_children[1]);
    vSFunc(_closetime, data->_children[2]);

  }
  pair<distance_type_m, bool> closest(Movement* p) {
    priority_queue_smallest<distance_type_m> intersects;
    if(_keyframes.size()) {
      auto it = _keyframes._frames.begin();
      auto nit = it;
      while (it != _keyframes._frames.end()) {
        ++nit;
        vector<time_type_s> temp = intersectPaths(&it->second, p, true);
        for (auto && tit : temp) {
          if ((it == _keyframes._frames.begin() || it->first <= tit) && (nit == _keyframes._frames.end() || tit <= nit->first)) {
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

  result(longDouble inputx, longDouble inputy, longDouble inputz, bool inansw) : dir(inputx, inputy, inputz), answ(inansw) {};
  result(sVec3 idir, bool inansw) : dir(idir), answ(inansw) {};
};

result coordinates(mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, acc_type_mperss amax, distance_type_m d, time_type_s tpassed);

//automata visszaloveshez a koordianatak, ez meg az eszleles pillanataban megtortenik
result surefire1(time_type_s obsTime, mpsVec3 vTarget, mVec3 sTarget, time_type_s now, mVec3 sShip, acc_type_mperss amax, distance_type_m d);

//azt szamoljuk ki hogy az elozo korben tortent eszlelesre a mostani mozgassorok közben meddig biztos a talalat
//vector<result> surefire2(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, keyframe<Movement>& movements, double utime, double round);

//result surefire3(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, keyframe<Movement>& movements, double utime, double qtime, double round);
