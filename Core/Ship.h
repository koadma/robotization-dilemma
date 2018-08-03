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

class LogEntry {
  string _cause;
  string _object;
  time_type_s _time;
  string _event;
};

class Drone {
public:
  keyframe<Movement> mov;
  uint64_t _droneID;
  list<Object*> objects;
  list<Sighting*> sightings;

  list<LogEntry> captainsLog; //Only store user-knowable facts, not all server events!

  FlowGraph<energy_type_J, power_type_W, time_type_s> energySystem;
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

  list< pair<time_type_s, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<time_type_s, pair<Object*, Path*>>> res;
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
  Object* getObject(string name) {
    for (auto it : objects) {
      if (it->name() == name) {
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
  Sighting* sightPath(Bubble* p, time_type_s time, Game* g, bool closed, bool autofire);
  energy_type_J energyUpdate(time_type_s time, Game* g, Object* chg = NULL, energy_type_J chgval = 0); //To do energy simulations.
  void energyCallback(time_type_s, Game* g); // Callback for energy run out objects, or any run of neergy sys
#endif

  int getHealth(time_type_s time);
  int getMaxHealth(time_type_s time); 

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
  string code;
  Ship(uint32_t _ID);
  Ship(uint32_t _ID, mVec3 _pos, string filename);
  Ship(uint32_t _ID, string filename);
  
  void load(uint32_t _ID, mVec3 _pos, string filename);

#ifdef M_SERVER
  NetworkS* connectedClient;

  //int makeMove(DataElement* data);
  void newTurn(int id);

  bool packetRecv(DataElement *Data, int Id, NetworkS* thisptr);

  void collectPath(list<Path*> &addTo, time_type_s time);

  bool loadShip(string filename);
  bool loadShip(xml_node<>* data);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();
  void newTurn(int id);

  void setSidebar();
  void setSidebar(vec3<double> ori, vec3<double> dir) {
    list< pair<time_type_s, pair<Object*, Path*>>> inters;

    auto it = objects.begin();

    while (it != objects.end()) {
      list< pair<time_type_s, pair<Object*, Path*>>> temp = (*it)->getIntersect(ori, dir);
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

  void selectSighting(vec3<double> ori, vec3<double> dir, double d);

  void drawSightings(float camcx, float camcy, float camcz, float d, OpenGLData data);
  void drawObjects(float camcx, float camcy, float camcz, float d, bool worldView);

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

class NetBinder : public Ship {
public:
  NetBinder(uint32_t _ID) : Ship(_ID) {

  }
  NetBinder(uint32_t _ID, mVec3 _pos, string filename) : Ship(_ID, _pos, filename) {

  }
  NetBinder(uint32_t _ID, string filename) : Ship(_ID, filename) {

  }
};

bool surefire(keyframe<Movement>& me, keyframe<Movement>& enemy, time_type_s when, mVec3 posShift, sVec3 &direction);
bool surefire(keyframe<Movement>& me, keyframe<SightedMovement>& enemy, time_type_s when, mVec3 posShift, sVec3 &direction);


#ifdef M_CLIENT
extern Ship* ship;
extern time_type_s timeNow;
extern Object* selectedo;
extern Sighting* selecteds;
#endif