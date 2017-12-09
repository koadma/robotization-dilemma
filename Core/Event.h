#pragma once

#include "Sighting.h"

class Object;
class Drone;
class Game;

class Event {
public:
  enum Type {
    EvTEvent = 0,
    EvTCollision = 1,
    EvTStateChange = 2,
    EvTEngineAcc = 3,
    EvTSensorPow = 4,
    EvTLaserShot = 5,
    EvTThermalRadiation = 6,
    EvTBatteryDrain = 7,
    EvTSensorPing = 8
  };
  time_type_s _time;

  virtual int type();

#ifdef M_SERVER
  virtual void apply(Game *g);
#endif
};
class Collision : public Event {
public:
  Path* _p;
  Object* _o;

  int type();

#ifdef M_SERVER
  void apply(Game *g);
#endif

};
class BatteryDrain : public Event {
public:
  Drone* _d;

  int type();

#ifdef M_SERVER
  void apply(Game *g);
#endif

};
class StateChange : public Event {
public:
  Object* _o;

  virtual int type();
  virtual void getV(DataElement* data);
  void get(DataElement* data);

#ifdef M_SERVER
  virtual void apply(Game *g);
  void set(DataElement* data, Game* game);
  virtual void setV(DataElement* data, Game* game);
#endif
};
class EngineAcc : public StateChange {
public:
  mpssVec3 _acc;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class SensorPow : public StateChange {
public:
  power_type_W _power;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class LaserShot : public StateChange {
public:
  sVec3 _dir;
  energy_type_J _energy;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class ThermalRadiation : public StateChange {
public:
  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};
class SensorPing : public StateChange {
public:
  energy_type_J _energy;

  int type();
  void getV(DataElement* data);

#ifdef M_SERVER
  void apply(Game *g);
  void setV(DataElement* data, Game* game);
#endif
};


struct EventSort {
  bool operator ()(const Event* lhs, const Event* rhs) const
  {
    return lhs->_time < rhs->_time;
  }
};
