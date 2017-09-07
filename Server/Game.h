#pragma once

#include "..\Core\Ship.h"

extern class Game;

class Game {
public:
  Game(int count) {
    targetPlayerCount = count;
  }
  int turnId;
  mutex lock;
  int targetPlayerCount;
  int waitingFor;
  list<Drone*> drones;
  list<Path*> paths;

  //set<pair<double, pair<Object*, Path*> > > intersects;
  multiset<Event*, EventSort> events;

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
  void startGame();
  void addShip(Ship* ship);
  void removeIntersect(Object* object);
  void removeIntersect(Drone* drone);
  void removeIntersect(Path* path);
 
  void calcIntersect(Object* object);
  void calcIntersect(Drone* drone);
  void calcIntersect(Path* path);
  void recalcIntersects();
  void simulate(float from, float till);
};

extern Game* game;