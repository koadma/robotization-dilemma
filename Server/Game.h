#ifndef __GAME_H__
#define __GAME_H__

#include "WinManager.h"

class Game {
public:
  Game(int count) {
    targetPlayerCount = count;
  }
  int turnId;
  mutex lock;
  int targetPlayerCount;
  int waitingFor;
  list<Ship*> ships;
  list<Bubble*> bubbles;
  list<Shot*> shots;

  set<pair<float, pair<Ship*, Bubble*> > > intersects; 

  enum State {
    Joining,
    Waiting,
    Simulating,
    Finished
  };
  int state = Joining;
  void newTurn() {
    turnId++;
    auto it = ships.begin();
    while(it != ships.end()) {
      (*it)->newTurn(turnId);
      ++it;
    }
  }
  void startGame() {
    state = Waiting;
    turnId = 0;
  }
  void addShip(Ship* ship) {
    ships.push_back(ship);
    if (targetPlayerCount == ships.size()) { //reached target number of players
      startGame();
    }
  }
  void removeIntersect(Drone* drone) {
    auto it = intersects.begin();

    while (it != intersects.end()) {
      if (it->second.first == drone) {
        auto it2 = it;
        ++it;
        intersects.erase(it2);
      }
      else {
        ++it;
      }
    }
  }
  void removeIntersect(Bubble* bubble) {
    auto it = intersects.begin();

    while (it != intersects.end()) {
      if (it->second.second == bubble) {
        auto it2 = it;
        ++it;
        intersects.erase(it2);
      }
      else {
        ++it;
      }
    }
  }
  void calcIntersect() {
    intersects.clear(); //reset intersections
    auto itb = bubbles.begin();
    while (itb != bubbles.end()) {
      auto its = ships.begin();
      while (its != ships.end()) {
        intersects.insert({(*its)->mov.intersect(**itb, SOL) ,{*its, *itb}});
        ++its;
      }
      ++itb;
    }
  }
  void simulate(float till) {
    calcIntersect();
    auto it = intersects.begin();
    while (it != intersects.end() && it->first < till) {
      //it->second.first->shot();
      ++it;
    }
  }
};

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
