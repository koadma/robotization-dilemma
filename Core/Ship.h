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
  
  power_type_W getGeneratedShipPower(time_type_s time);
  power_type_W getUnusedShipPower(time_type_s time);
  power_type_W getUsedShipPower(time_type_s time);
  energy_type_J getMaxShipEnergy(time_type_s time);
  energy_type_J getStoredShipEnergy(time_type_s time);
  energy_type_J useEnergy(time_type_s time, energy_type_J amount);
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount);
  void refreshEnergy(time_type_s time);
  Event* runOut(); //when will storage run out

  int getHealth(time_type_s time);
  int getMaxHealth(time_type_s time);

  void sightMovement(Movement& m, time_type_s time);

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

  Ship(uint32_t _ID) {

    Object* no = new ::Generator({ 100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 0));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Sensor({ -100,0,0 }, 1000, 100, 1000, 100000, mix(_ID, 1));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Engine({ 0,173.2f ,0 }, 1000, 100, 1000, 100000, {0, 0, 0}, mix(_ID, 2));
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Laser({ 0,-173.2f ,0 }, 1000, 100, 1000, mix(_ID, 3));
    no->parentShip = this;

    objects.push_back(no);

    Movement m;
    m.pos.z = 1000.0 * _ID;
    mov.addFrame(0, m);
  }

#ifdef M_SERVER
  NetworkS* connectedClient;

  //int makeMove(DataElement* data);
  void newTurn(int id);

  bool packetRecv(DataElement *Data, int Id, NetworkS* thisptr);

  void collectPath(list<Path*> &addTo, float time);
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

  void drawSightings(float camcx, float camcy, float camcz, float d);
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