
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
  waitingFor = drones.size();
  for(auto&& it : drones) {
    ((Ship*)it)->newTurn(turnId);
  }
}
void Game::tryGameStart() {
  if (targetPlayerCount == drones.size()) { //reached target number of players
    startGame();
  }
}
void Game::startGame() {
  state = Waiting;
  for (auto&& it : drones) {
    it->energyUpdate(0, this);
  }
  newTurn();
}

void Game::remove(Path* path) {
  auto it = events.begin();

  while (it != events.end()) {
    if (!(*it)->_isApplying && (*it)->type() == Event::EvTCollision && (reinterpret_cast<Collision*>(*it))->_p == path) {
      auto it2 = it;
      ++it;
      events.erase(it2);
    }
    else {
      ++it;
    }
  }
}

void Game::removeIntersect(Object* object) {
  auto it = events.begin();
  while (it != events.end()) {
    if (!(*it)->_isApplying && (*it)->type() == Event::EvTCollision && (reinterpret_cast<Collision*>(*it))->_o == object) {
      auto it2 = it;
      ++it;
      events.erase(it2);
    }
    else {
      ++it;
    }
  }
}
void Game::removeIntersect(Drone* drone) {
  auto it = events.begin();
  while (it != events.end()) {
    if (!(*it)->_isApplying && (*it)->type() == Event::EvTCollision && (reinterpret_cast<Collision*>(*it))->_o->_parentShip == drone) {
      auto it2 = it;
      ++it;
      events.erase(it2);
    }
    else {
      ++it;
    }
  }
}
void Game::removeIntersect(Path* path) {
  auto it = events.begin();

  while (it != events.end()) {
    if (!(*it)->_isApplying && (*it)->type() == Event::EvTCollision && (reinterpret_cast<Collision*>(*it))->_p == path) {
      auto it2 = it;
      ++it;
      events.erase(it2);
    }
    else {
      ++it;
    }
  }
}

void Game::removeBatteryEvents(Drone * drone) {
  auto it = events.begin();

  while (it != events.end()) {
    if (!(*it)->_isApplying && (*it)->type() == Event::EvTBatteryDrain && (reinterpret_cast<BatteryDrain*>(*it))->_d == drone) {
      auto it2 = it;
      ++it;
      events.erase(it2);
    }
    else {
      ++it;
    }
  }
}

void Game::calcIntersect(Object* object) {
  for (auto&& itb : paths) {
    list< pair<double, pair<Object*, Path*>>> inters = object->intersect(itb);
    for (auto it : inters) {
      Collision* coll = new Collision();
      coll->_time = it.first;
      coll->_o = it.second.first;
      coll->_p = it.second.second;
      add(coll);
    }
  }
}
void Game::calcIntersect(Drone* drone) {
  for(auto&& ito : drone->objects) {
    calcIntersect(ito);
  }
}
void Game::calcIntersect(Path* path) {
  auto its = drones.begin();
  while (its != drones.end()) {
    list< pair<double, pair<Object*, Path*>>> inters = (*its)->intersect(path);
    for (auto&& it : inters) {
      Collision* coll = new Collision();
      coll->_time = it.first;
      coll->_o = it.second.first;
      coll->_p = it.second.second;
      add(coll);
    }
    ++its;
  }
}

Ship* Game::addShip() {
  int nid = drones.size();
  Ship* ship = new Ship(nid, shipStarts[nid], "html/Ships/Test/");
  drones.push_back(ship);
  string nCode;
  do {
    nCode = randomHexString(16);
  } while (shipAuth.count(nCode));
  shipAuth.insert({nCode, ship});
  return ship;
}
Drone* Game::addDrone(mVec3 pos) {
///TODO: Fix
  int nid = drones.size();
  Drone* drone = new Drone();
  drones.push_back(drone);
  return drone;
}
void Game::add(Path* path) {
  calcIntersect(path);
  paths.push_back(path);
}
void Game::add(Event* evt) {
  cout << "Event added at " << evt->_time << ", type " << evt->type_name() << endl;
  events.insert(evt);
}

void Game::recalcIntersects() {
  for (auto&& it : drones) {
    recalcIntersects(it);
  }
}
void Game::recalcIntersects(Drone* drone) {
  removeIntersect(drone);
  calcIntersect(drone);
}
void Game::recalcIntersects(Object* object) {
  removeIntersect(object);
  calcIntersect(object);
}
void Game::recalcIntersects(Path* path) {
  removeIntersect(path);
  calcIntersect(path);
}
void Game::updateDroneBatteryEvents(Drone * drone, vector<time_type_s> evts) {
  removeBatteryEvents(drone);
  sort(evts.begin(), evts.end());
  if (evts.size()) {
    BatteryDrain* evt = new BatteryDrain();
    evt->_d = drone;
    evt->_time = evts.front();
    add(evt);
  }
}
void Game::simulate(float from, float till) {
  //recalcIntersects();
  cout << "SIM " << from << " " << till << endl;
  auto it = events.begin();
  while (it != events.end() && (*it)->_time < till) {
    if(from <= (*it)->_time) {
      auto it2 = it;
      cout << "PROCESSING EVENT " << (*it)->type_name() << " TIME " << (*it)->_time << endl;
      (*it)->apply(this);
      cout << "PROCESSED EVENT" << endl;
      ++it;
      events.erase(it2);
    }
    else {
      auto it2 = it;
      ++it;
      cout << "DEL EVENT " << (*it2)->type_name() << " TIME " << (*it2)->_time << endl;
      events.erase(it2);
    }
  }

  //Clean bubbles
  auto pit = paths.begin();
  while(pit != paths.end()) {
    bool land = true;
    if ((*pit)->type() == Path::PathTypeBubble) {
      auto dit = drones.begin();
      while (dit != drones.end() && land) {
        auto oit = (*dit)->objects.begin();
        while (oit != (*dit)->objects.end() && land) {
          land = land && !((Bubble*)(*pit))->isWellIn((*oit)->getMovement(till).pos, (*oit)->getMovement(till).radius, till);
          ++oit;
        }
        ++dit;
      }
    }
    if (land) {
      auto dpit = pit;
      ++pit;
      removeIntersect(*dpit);
      paths.erase(dpit);
    } else {
      ++pit;
    }
  }

  newTurn();
}

////////////////////////////////////////////

