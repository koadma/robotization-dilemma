#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <utility>
#include "WinManager.h"
#include "helperFunctions.h"
#include "constants.h"
#include "Point.h"
#include "Ship.h"
#include "Game.h"

using namespace std;

void Game::askToContinue(int nextPlayer) const
{
  terminalClear();
  cout << nextPlayer << ". játékos köre következik!" << endl;
  cout << "Nyomj entert, ha kezdődhet." << endl;
  waitForEnter();
}

void Game::giveInformation(int currentPlayer) const
{
  cout << "Kör eleji információk:" << endl;
  cout << roundNumber << ". kör" << endl << endl;
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
  cout << "Kör lejátszása." << endl;
  
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
    cout << "Az " << winManager.getWinner() << ". játékos nyert!" << endl;
  } else
  {
    cout << "Mindenki elpusztult a csatában!" << endl;
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
    ships.push_back(Ship(Point().randomize(), playerID));
  }
  mainGameLoop();
}

int main()
{
  srand(time(NULL));
  string inp;
  do
  {
    unsigned int numOfShips;
    cout << "Hajók száma: ";
    cin >> numOfShips;
    Game game(numOfShips);
    cout << "Akarsz új játékot kezdeni? (y - igen, bármi más - nem)" << endl;
    cin >> inp;
  } while (inp == "y");
  return 0;
}
