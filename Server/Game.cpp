
#include "Game.h"

Game* game;

using namespace std;

void Game::moveMade() {
  waitingFor--;
  if (waitingFor == 0) {
    simulate((turnId-1) * ROUND_TIME, turnId * ROUND_TIME);
  }
}
void Game::newTurn() {
  turnId++;
  waitingFor = ships.size();
  auto it = ships.begin();
  while (it != ships.end()) {
    (*it)->newTurn(turnId);
    ++it;
  }
}
void Game::startGame() {
  state = Waiting;
  newTurn();
}
void Game::addShip(Ship* ship) {
  ships.push_back(ship);
  if (targetPlayerCount == ships.size()) { //reached target number of players
    startGame();
  }
}
void Game::removeIntersect(Drone* drone) {
  auto it = intersects.begin();

  while (it != intersects.end()) {
    if (it->second.first->parentShip == drone) {
      auto it2 = it;
      ++it;
      intersects.erase(it2);
    }
    else {
      ++it;
    }
  }
}
void Game::removeIntersect(Bubble* bubble) {
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
void Game::calcIntersect() {
  
  intersects.clear(); //reset intersections
  auto itb = paths.begin();
  while (itb != paths.end()) {
    auto its = ships.begin();
    while (its != ships.end()) {
      list< pair<double, pair<Object*, Path*>>> inters = (*its)->intersect(*itb);
      intersects.insert(inters.begin(), inters.end());
      ++its;
    }
    ++itb;
  }
}
void Game::simulate(float from, float till) {
  for (auto it : ships) {
    it->setAccel();
  }
  collectPath(paths, from);
  calcIntersect();
  auto it = intersects.begin();
  while (it != intersects.end() && it->first < till) {
    if(from <= it->first) {
      it->second.first->getPath(it->second.second);
    }
    ++it;
  }
  for (auto it : ships) {
    it->moveShip(till);
  }
  newTurn();
}

////////////////////////////////////////////

