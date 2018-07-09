
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
      LOG INFO GAME "Event " << *it2 << ": " << (*it2)->type_name() << " deleted with path " << path << " at " << (*it2)->_time END;
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
    list< pair<time_type_s, pair<Object*, Path*>>> inters = object->intersect(itb);
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
    list< pair<time_type_s, pair<Object*, Path*>>> inters = (*its)->intersect(path);
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

NetBinder* Game::addShip() {
  int nid = drones.size();
  NetBinder* ship = new NetBinder(nid, shipStarts[nid], "html/Ships/Test/");
  drones.push_back(ship);
  string nCode;
  do {
    nCode = randomHexString(16);
  } while (shipAuth.count(nCode));
  shipAuth.insert({nCode, ship});
  return ship;
}
Drone* Game::addDrone(mVec3 pos) {
///TODO: Fix // id only valid assuming no delets
  int nid = drones.size();
  Drone* drone = new Drone();
  drones.push_back(drone);
  return drone;
}
void Game::add(Path* path) {
  calcIntersect(path);
  paths.push_back(path);
  LOG INFO GAME "Path " << path << ": " << path->type() << " added" END;
}
void Game::add(Event* evt) {
  LOG INFO GAME "Event " << evt << " added at " << evt->_time << ", type " << evt->type_name() END;
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
void Game::simulate(time_type_s from, time_type_s till) {
  //recalcIntersects();
  LOG INFO GAME "SIM " << from << " " << till END;
  auto it = events.begin();
  while (it != events.end() && (*it)->_time < till) {
    if(from <= (*it)->_time) {
      from = (*it)->_time;
      auto it2 = it;
      LOG INFO GAME "Event " << *it << ": " << (*it)->type_name() << " processed at " << (*it)->_time END;
      (*it)->apply(this);
      LOG INFO GAME "Done" END;
      ++it;
      delete *it2;
      events.erase(it2);
    }
    else {
      auto it2 = it;
      ++it;
      LOG INFO GAME "Event " << *it2 << ": " << (*it2)->type_name() << " deleted at " << (*it2)->_time END;
      events.erase(it2);
    }
  }

  //Clean bubbles
  auto pit = paths.begin();
  while(pit != paths.end()) {
    bool land = false;
    if ((*pit)->type() == Path::PathTypeBubble) {
      land = true;
      auto dit = drones.begin();
      while (dit != drones.end() && land) {
        auto oit = (*dit)->objects.begin();
        while (oit != (*dit)->objects.end() && land) {
          bool nand = ((Bubble*)(*pit))->isWellIn((*oit)->getMovement(till).pos, (*oit)->getMovement(till).radius, till);
          if (!nand) {
            LOG INFO GAME "Test failed at " << (*oit)->getId() << "(" << ((*oit)->getMovement(till).pos) << "), at " << till << " bubble size of " << (till - ((Bubble*)(*pit))->gEmissionTime) * SOL << " from " << ((Bubble*)(*pit))->emitter.getAt(((Bubble*)(*pit))->gEmissionTime).pos << endl;
          }
          land = land && nand;
          ++oit;
        }
        ++dit;
      }
      if (land) {
        auto dpit = pit;
        ++pit;
        removeIntersect(*dpit);
        LOG INFO GAME "Path " << *dpit << ": " << (*dpit)->type() << " removed" END;
        paths.erase(dpit);
      }
    }
    if (!land) {
      ++pit;
    }
  }

  newTurn();
}

////////////////////////////////////////////

