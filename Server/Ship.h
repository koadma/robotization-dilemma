#ifndef __SHIP_H__
#define __SHIP_H__

#include <iostream>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include <vector>
#include "..\Core\Point.h"
#include "..\Core\constants.h"
#include "..\Core\helperFunctions.h"

struct Command
{
  public:
  fVec3 accel = fVec3(0);
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
  fVec3 place;
  fVec3 velocity;
  fVec3 nextPlace;
  bool sureFire;
  friend std::ostream& operator<<(std::ostream& os, const SensorDataElement& sde);
};

class Object
{
protected:
  fVec3 place;
  fVec3 velocity;
  int maxVelocity;

public:
  Object(fVec3 place, fVec3 velocity, int maxVelocity) : 
    place(place), velocity(velocity), maxVelocity(maxVelocity) {}
  void move(fVec3 accel, float time);
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
  Ship(fVec3 place, int owner) : 
    Object(place, place * ((-1)*INITIAL_VELOCITY/place.length()), INITIAL_MAX_VELOCITY),
    owner(owner) {}
  void getCommand();
  void flushSensorData();
  void sense(SensorDataElement sde);
  void giveSensorData() const;
  void move(float time);
  void destroy();
  int getAim() const;
  fVec3 getPlace() const;
  fVec3 getVelocity() const;
  int getHullRadius() const;
  bool isDestroyed() const;
  int getSensorRadiation() const;
  int getVisibility() const;
  bool didFire() const;
  unsigned int getOwner() const;
  friend std::ostream& operator<<(std::ostream& os, const Ship& s);
};


#endif
