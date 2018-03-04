#pragma once

#include "..\Core\Ship.h"

class Game;

class Game {
  int turnId;
  mutex lock;
  int targetPlayerCount;
  int waitingFor;

  list<Drone*> drones;
  list<Path*> paths;
  vector<mVec3> shipStarts;

  multiset<Event*, EventSort> events;
  
  void removeIntersect(Object* object);
  void removeIntersect(Drone* drone);
  void removeIntersect(Path* path);

  void removeBatteryEvents(Drone* drone);

  void calcIntersect(Object* object);
  void calcIntersect(Drone* drone);
  void calcIntersect(Path* path);
public:
  map<string, Ship*> shipAuth;

  Game(int count) {
    targetPlayerCount = count;
    shipStarts = randstartpos(1 * SOL);
  }

  Object* getObject(uint64_t oid) {
    Object* o = NULL;
    auto it = drones.begin();
    while (it != drones.end() && o == NULL) {
      o = (*it)->getObject(oid);
      ++it;
    }
    return o;
  }

  enum State {
    Joining,
    Waiting,
    Simulating,
    Finished
  };
  int state = Joining;
  void moveMade();
  void newTurn();
  void tryGameStart();
  void startGame();

  void add(Path* path);
  void add(Event* evt);
  Drone* addDrone(mVec3 pos);
  Ship* addShip();

  void remove(Path* path);

  void recalcIntersects();
  void recalcIntersects(Drone* drone);
  void recalcIntersects(Object* object);
  void recalcIntersects(Path* path);
  void updateDroneBatteryEvents(Drone* drone, vector<time_type_s> evts);
  void simulate(float from, float till);
};

extern Game* game;