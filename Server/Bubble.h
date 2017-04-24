#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "..\Core\Network.h"

class Bubble
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
  //unsigned int receiver; //only for reflection, would be cleaner with virtual methods
  friend std::ostream& operator<<(std::ostream& os, const Bubble& b); // only for test
 
};

class Shot {
public:
  fVec3 origin;
  float time;
  fVec3 vel;
  float energy;
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
