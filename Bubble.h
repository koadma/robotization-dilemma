#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <iostream>
#include <vector>
#include "Point.h"
#include "Ship.h"
#include "constants.h"

class Bubble
{
public:
  enum Type {Active, Passive, Reflection};
  Point origin;
  unsigned int emitter;
  Point emitterVelocity;
  int visibility;
  int age; //radius = age * ROUND_TIME * SOL
  Type btype;
  unsigned int receiver; //only for reflection, would be cleaner with virtual methods
  friend std::ostream& operator<<(std::ostream& os, const Bubble& b); // only for test
};

class BubbleManager
{
private:
  std::vector<Bubble> bubbles;
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
