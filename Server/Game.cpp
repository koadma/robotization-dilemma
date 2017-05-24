
#include "Game.h"

Game* game;

using namespace std;
/*
void Game::askToContinue(int nextPlayer) const
{
  terminalClear();
  cout << nextPlayer << ". jatekos kore kovetkezik!" << endl;
  cout << "Nyomj entert, ha kezdodhet." << endl;
  waitForEnter();
}

void Game::giveInformation(int currentPlayer) const
{
  cout << "Kor eleji informaciok:" << endl;
  cout << roundNumber << ". kor" << endl << endl;
  cout << ships[currentPlayer] << endl << endl;
  ships[currentPlayer].giveSensorData();
  cout << "----------------------" << endl;
}


void Game::manageBubbles()
{
  //create bubbles
  for (const Ship& ship : ships)
  {
    if (not ship.isDestroyed())
    {
      //passive bubble
      int vis = ship.getVisibility(); 
      bubbles.add(Bubble
      {
        ship.getPlace(),    //place 
        ship.getOwner(),    //emitter
        ship.getVelocity(), //emitterVelocity
        vis,                //visibility 
        0,                  //age 
        Bubble::Passive,    //btype
        0                   //receiver
      });
      
      //active bubble
      if (ship.getSensorRadiation() > 0)
      {
        bubbles.add(Bubble
        {
          ship.getPlace(),
          ship.getOwner(),
          ship.getVelocity(),
          ship.getSensorRadiation(),   //visibility
          0,                          //age
          Bubble::Active,
          0                         //receiver
        });
      }
    }
  }
  //move bubbles
  bubbles.move();
}

void Game::manageProjectiles()
{
  //firing(creating projectiles)
  for (const Ship& ship : ships)
  {
    if(ship.didFire())
    {
      int target = ship.getAim();
      int oldT = projectiles[target];
      int newT = floor( distance(ship.getPlace(), ship.getPlace()) / SOL );
      if (oldT == -1)
      {
        projectiles[target] = newT;
      } else
      {
        projectiles[target] = min(oldT, newT);
      }
    }
  }
  //move projectiles
  for (int player = 0; player<numOfShips; player++)
  {
    if (projectiles[player] >= 0) //aimed at
    {
      if (projectiles[player] == 0) //reached
      {
        ships[player].destroy();
        winManager.lose(player);
      }
      projectiles[player]--;
    }
  }
}


void Game::moveShips()
{
  for (Ship& ship : ships)
  {
    if (not ship.isDestroyed())
    {
      ship.move(ROUND_TIME);
      //reaching origo == win
      if (ship.getPlace().length() <= ship.getHullRadius()) //can go over, if the ship is fast
      {
        winManager.win(ship.getOwner());
      }
    }
  }
}


void Game::manageDetections()
{
//todo: a bubble can be destroyed, when every player detects it
  for (Ship& ship : ships)
  {
    ship.flushSensorData();
    bubbles.interactWithShip(ship);
  }
}

void Game::playRound()
{
  terminalClear();
  cout << "Kor lejatszasa." << endl;
  
  manageBubbles();
  manageProjectiles();
  moveShips();
  manageDetections();

  waitForEnter();
  terminalClear();
}

void Game::giveWinScreen()
{
  if (winManager.hasWinner())
  {
    cout << "Az " << winManager.getWinner() << ". jatekos nyert!" << endl;
  } else
  {
    cout << "Mindenki elpusztult a csataban!" << endl;
  }
}

void Game::mainGameLoop()
{
  do
  {
    bubbles.out(); // ONLY FOR TEST
    waitForEnter();
    for(int player=0; player<numOfShips; player++)
    {
      askToContinue(player);
      giveInformation(player);
      if (ships[player].isDestroyed())
      {
        waitForEnter();
      } else
      {
        ships[player].getCommand();
      }
    }
    playRound();
    roundNumber++;
  } while (not winManager.isOver());
  giveWinScreen();
}

Game::Game(unsigned int numOfShips) : 
numOfShips(numOfShips), winManager(numOfShips), projectiles(numOfShips, -1)
{
  for (unsigned int playerID=0; playerID<numOfShips; playerID++) 
  {
    ships.push_back(Ship(fVec3().randomize(MAP_SIZE), playerID));
  }
  mainGameLoop();
}*/

void Game::moveMade() {
  waitingFor--;
  if (waitingFor == 0) {
    simulate(turnId * ROUND_TIME);
  }
}
void Game::newTurn() {
  turnId++;
  auto it = ships.begin();
  while (it != ships.end()) {
    (*it)->newTurn(turnId);
    ++it;
  }
}
void Game::startGame() {
  state = Waiting;
  turnId = 0;
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
  auto itb = bubbles.begin();
  while (itb != bubbles.end()) {
    auto its = ships.begin();
    while (its != ships.end()) {
      list< pair<double, pair<Object*, Path*>>> inters = (*its)->intersect(*itb);
      intersects.insert(inters.begin(), inters.end());
      ++its;
    }
    ++itb;
  }
}
void Game::simulate(float till) {
  calcIntersect();
  auto it = intersects.begin();
  while (it != intersects.end() && it->first < till) {
    //it->second.first->shot();
    ++it;
  }
  turnId++;
}

////////////////////////////////////////////

