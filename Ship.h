#ifndef __SHIP_H__
#define __SHIP_H__

#include <iostream>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include <vector>
#include "Point.h"
#include "constants.h"
#include "helperFunctions.h"

struct Command
{
  Point accel = {0, 0, 0};
  bool didFire = false;
  unsigned int aim;
  int sensorEnergy = 0;
  friend std::ostream& operator<<(std::ostream& os, const Command& c);
};

struct SensorDataElement
{
  enum Type {Active, Passive};
  Type detectType;
  unsigned int player;
  Point place;
  Point velocity;
  Point nextPlace;
  bool sureFire;
  friend std::ostream& operator<<(std::ostream& os, const SensorDataElement& sde);
};

class Object
{
protected:
  Point place;
  Point velocity;
  int maxVelocity;

public:
  Object(Point place, Point velocity, int maxVelocity) : 
    place(place), velocity(velocity), maxVelocity(maxVelocity) {}
  void move(Point accel, float time);
};

class Ship : public Object
{
private:
  const unsigned int owner;
  int maxAcceleration = INITIAL_MAX_ACC;
  int maxSensorEnergy = INITIAL_MAX_SENSORENERGY;
  int maxGeneratorEnergy = INITIAL_MAX_GENERATORENERGY;
  int hullRadius = INITIAL_HULL_RADIUS;
  bool destroyed = false;
  Command command;
  std::vector<SensorDataElement> sensorData;
  int getSpentEnergy() const;

public:
  Ship(Point place, int owner) : 
    Object(place, ((-1)*INITIAL_VELOCITY/place.length())*place, INITIAL_MAX_VELOCITY), 
    owner(owner) {}
  void getCommand();
  void flushSensorData();
  void sense(SensorDataElement sde);
  void giveSensorData() const;
  void move(float time);
  void destroy();
  int getAim() const;
  Point getPlace() const;
  Point getVelocity() const;
  int getHullRadius() const;
  bool isDestroyed() const;
  int getSensorRadiation() const;
  int getVisibility() const;
  bool didFire() const;
  unsigned int getOwner() const;
  friend std::ostream& operator<<(std::ostream& os, const Ship& s);
};


#endif
