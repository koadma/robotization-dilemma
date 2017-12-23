#pragma once

#include "Object/Battery.h"
#include "Object/Computer.h"
#include "Object/Engine.h"
#include "Object/Generator.h"
#include "Object/Hangar.h"
#include "Object/Laser.h"
#include "Object/Sensor.h"

class Drone;

#ifdef M_SERVER
class Game;
#endif

class Drone {
public:
  keyframe<Movement> mov;
  long int _droneID;
  list<Object*> objects;
  list<Sighting*> sightings;

  FlowGraph<energy_type_J, Fraction, time_type_s> energySystem;

  enum ShipClass {
    SCError = 0,
    SCTerrainCruiser = 1,
  };
  ShipClass shipClass;

  string graphicsName;

  static ShipClass ShipClassFromStr(string s) {
    if (s == "TerrainCruiser") {
      return SCTerrainCruiser;
    }
    return SCError;
  }
  static string ShipClassToStr(ShipClass s) {
    switch (s) {
      case SCError:
        return "Error";
        break;
      case SCTerrainCruiser:
        return "TerrainCruiser";
        break;
    }
    return "Error";
  }

  time_type_s lastEvtTime;

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    auto it = objects.begin();
    while (it != objects.end()) {
      res.splice(res.end(), (*it)->intersect(p));
      ++it;
    }
    return res;
  }

  Object* getObject(uint64_t oid) {
    for (auto it : objects) {
      if (it->getId() == oid) {
        return it;
      }
    }
    return NULL;
  }

  power_type_W getMaxGeneratedPower(time_type_s time);
  power_type_W getGeneratedPower(time_type_s time);
  power_type_W getMaxUseablePower(time_type_s time);
  power_type_W getRequestedPower(time_type_s time);
  power_type_W getUsedPower(time_type_s time);
  energy_type_J getMaxEnergy(time_type_s time);
  energy_type_J getStoredEnergy(time_type_s time);
  
#ifdef M_SERVER
  void energyUpdate(time_type_s time, Game* g); //To do energy simulations. Callback for energy run out objects
  void energyCallback(time_type_s, Game* g);
#endif

  int getHealth(time_type_s time);
  int getMaxHealth(time_type_s time);

  void sightMovement(Movement& m, time_type_s time, bool autofire);

  mpssVec3 getAccel(time_type_s time) {
    mpssVec3 sum = { 0,0,0 };
    for (auto it : objects) {
      sum += it->getAccel(time);
    }
    return sum;
  }
};
class Ship : public Drone {
private:
  bool canMove = false; //is the player open to moving / are we waiting for a move.
public:
  Ship(uint32_t _ID, mVec3 _pos);
  Ship(uint32_t _ID);
  
  void load(uint32_t _ID, mVec3 _pos);

#ifdef M_SERVER
  NetworkS* connectedClient;

  //int makeMove(DataElement* data);
  void newTurn(int id);

  bool packetRecv(DataElement *Data, int Id, NetworkS* thisptr);

  void collectPath(list<Path*> &addTo, float time);

  bool loadShip(string filename);
  bool loadShip(xml_node<>* data);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();
  void newTurn(int id);

  void setSidebar();
  void setSidebar(vec3<double> ori, vec3<double> dir) {
    list< pair<double, pair<Object*, Path*>>> inters;

    auto it = objects.begin();

    while (it != objects.end()) {
      list< pair<double, pair<Object*, Path*>>> temp = (*it)->getIntersect(ori, dir);
      inters.splice(inters.end(), temp);
      ++it;
     }

     inters.sort();

     if(inters.size()) {
       inters.begin()->second.first->setSidebar();
     } else {
       setSidebar();
     }
  }

  void selectSighting(vec3<double> ori, vec3<double> dir);

  void drawSightings(float camcx, float camcy, float camcz, float d, OpenGLData data);
  void drawObjects(float camcx, float camcy, float camcz, float d, bool b = false);

  bool packetRecv(DataElement *Data, int Id, NetworkC* thisptr);
#endif

  void getStatus(DataElement* data);
  void setStatus(DataElement* data);

  void getSightings(DataElement* data);
  void setSightings(DataElement* data);

  void clearObjects();
  void clearSightings();

  ~Ship();
};

bool surefire(keyframe<Movement>& me, keyframe<Movement>& enemy, time_type_s when, sVec3 &direction);

#ifdef M_CLIENT
extern Ship* ship;
extern time_type_s timeNow;
extern Object* selectedo;
extern Sighting* selecteds;
#endif