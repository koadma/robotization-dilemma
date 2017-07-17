#ifndef __GAME_H__
#define __GAME_H__

#include "WinManager.h"

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
  set<Event*, EventSort> events;

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
  /*void collectPath(list<Path*> &addTo, float time) {
    for (auto it : drones) {
#ifdef M_SERVER
      it->collectPath(paths, time);
#endif
    }
  }*/
  void calcIntersect(Object* object);
  void calcIntersect(Drone* drone);
  void calcIntersect(Path* path);
  //void calcIntersect();
  void simulate(float from, float till);
};

extern Game* game;

/*
class Game
{
private:
  unsigned int numOfShips;
  std::vector<Ship> ships;
  int roundNumber = 0;
  WinManager winManager;
  BubbleManager bubbles;
  std::vector<int> projectiles; 

  void askToContinue(int nextPlayer) const;
  void giveInformation(int currentPlayer) const;
  void mainGameLoop();
  void playRound();
  void manageBubbles();
  void manageProjectiles();
  void moveShips();
  void manageDetections();
  void giveWinScreen();
public:
  Game(unsigned int numOfShips);
  Game();
};
*/
#endif
