#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <iostream>
#include <vector>
#include "..\Core\Point.h"
#include "Ship.h"
#include "..\Core\constants.h"

class Bubble
{
public:
  enum Type {Active, Passive, Reflection};
  fVec3 origin;
  unsigned int emitter;
  fVec3 emitterVelocity;
  int visibility;
  /*  float energy = 6000000000.0f;
  float visibility(float t) {
    if (t < 0.1f) {
      if (t < 0) {
        return 0;
      }
      else {
        return energy;
      }
    }
    return energy / ((SOL * t) * (SOL * t));
  }*/
  int age; //radius = age * ROUND_TIME * SOL
  Type btype;
  unsigned int receiver; //only for reflection, would be cleaner with virtual methods
  friend std::ostream& operator<<(std::ostream& os, const Bubble& b); // only for test
};

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
};

#endif
