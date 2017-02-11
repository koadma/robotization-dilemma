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

using namespace std;

struct Bubble
{
  enum Type {Active, Passive, Reflection};
  Point origin;
  int emitter;
  Point emitterVelocity;
  int visibility;
  int age; //radius = age * ROUND_TIME * SOL
  Type btype;
  int receiver; //only for reflection, would be cleaner with virtual methods
  friend ostream& operator<<(ostream& os, const Bubble& b) // only for test
  {
    switch (b.btype)
    {
      case Active:
      cout << "Aktív buborék!" << endl;
      break;

      case Passive:
      cout << "Passzív buborék!" << endl;
      break;

      case Reflection:
      cout << "Visszaverődő buborék!" << endl;
      break;
    }
    if (b.btype == Reflection)
    {
      cout << "Tudja fogadni: " << b.receiver << ". játékos." << endl;
    }
    cout << "Középpont: " << b.origin << endl;
    cout << "Keltő játékos: " << b.emitter << endl;
    cout << "Keltő játékos sebssége keltéskor: " << b.emitterVelocity << endl;
    cout << "Láthatóság: " << b.visibility << endl;
    cout << "Kora: " << b.age << " kör."; // no endl
  }
};


class Game
{
  unsigned int numOfShips;
  vector<Ship> ships;
  int roundNumber = 0;
  WinManager winManager; //bug: 2 people reach origo at the same time
  vector<Bubble> bubbles;
  vector<int> projectiles; //ugly
  //(projectiles[3]==-1) means no projectile targets the 3. player

  void askToContinue(int nextPlayer)
  {
    terminalClear();
    cout << nextPlayer << ". játékos köre következik!" << endl;
    cout << "Nyomj entert, ha kezdődhet." << endl;
    waitForEnter();
  }
  
  void giveInformation(int currentPlayer)
  {
    cout << "Kör eleji információk:" << endl;
    cout << roundNumber << ". kör" << endl << endl;
    cout << ships[currentPlayer] << endl << endl;
    ships[currentPlayer].giveSensorData();
    cout << "----------------------" << endl;
  }

  
  void manageBubbles()
  {
    //create bubbles
    for (int player=0; player<numOfShips; player++)
    {
      const Ship& ship = ships[player];
      if (not ship.isDestroyed())
      {
        //passive bubble
        int vis = ship.getVisibility(); 
        bubbles.push_back(Bubble
        {
          ship.getPlace(),    //place 
          player,             //emitter
          ship.getVelocity(), //emitterVelocity
          vis,                //visibility 
          0,                  //age 
          Bubble::Passive,    //btype
          0                   //receiver
        });
        
        //active bubble
        if (ship.getSensorRadiation() > 0)
        {
          bubbles.push_back(Bubble
          {
            ship.getPlace(),
            player,
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
    for (Bubble& bubble : bubbles)
    {
      bubble.age++;
      bubble.visibility--;
    }
  }

  void manageProjectiles()
  {
    //firing(creating projectiles)
    for (int player = 0; player<numOfShips; player++)
    {
      const Ship& ship = ships[player];
      if(ship.didFire())
      {
        int target = ship.getAim();
        int oldT = projectiles[target];
        int newT = floor( distance(ships[player].getPlace(), ships[target].getPlace()) / SOL );
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

  
  void moveShips()
  {
    for (int player = 0; player<numOfShips; player++)
    {
      if (not ships[player].isDestroyed())
      {
        ships[player].move(ROUND_TIME);
        //reaching origo == win
        if (ships[player].getPlace().length() <= ships[player].getHullRadius()) //can go over, if the ship is fast
        {
          winManager.win(player);
        }
      }
    }
  }
  
  //Did the ship detected the Bubble(active or passive)?
  bool didDetectPassively(Bubble bubble, int player)
  {
    //todo: insert radar equation
    return bubble.visibility >= PASSIVE_SENSOR_MINVIS; //placeholder
  }

  //Did the ship detected the reflection bubble?
  bool didDetectActively(Bubble bubble, int player)
  {
    //todo
    return bubble.visibility >= ACTIVE_SENSOR_MINVIS; //placeholder
  }

  //Can the ship hit the target?
  bool isSureFire(Bubble bubble, int player)
  {
    //todo: insert equation
    return true; //placeholder
  }

  void manageDetections()
  {
  //todo: a bubble can be destroyed, when every player detects it
    for (int player = 0; player < numOfShips; player++)
    {
      ships[player].flushSensorData();
      for (Bubble bubble : bubbles)
      {
        //I dont want self-detections.
        if (bubble.emitter == player)
        {
          continue;
        }
        int dist = distance(bubble.origin, ships[player].getPlace());
        if (dist < bubble.age*SOL*ROUND_TIME && 
            dist > (bubble.age-1)*SOL*ROUND_TIME) // collision was in this round
        {
          //passive sensor
          if (bubble.btype == Bubble::Active || bubble.btype == Bubble::Passive)
          {
            if (didDetectPassively(bubble, player))
            {
              ships[player].sense(SensorDataElement
              {
                SensorDataElement::Passive, //detectType
                bubble.emitter,         //player
                bubble.origin,          //place
                bubble.emitterVelocity, //velocity
                Point(),//moveObject(bubble.origin, bubble.emitterVelocity, {0, 0, 0}, ROUND_TIME, 0).first, 
                isSureFire(bubble, player) //sureFire?
              });
            }
          }

          //active sensor
          if (bubble.btype == Bubble::Reflection && bubble.receiver == player)
          {
            if (didDetectActively(bubble, player))
            {
              ships[player].sense(SensorDataElement
              {
                SensorDataElement::Active,
                bubble.emitter,
                bubble.origin,
                bubble.emitterVelocity,
                Point(),//placeHolder
                isSureFire(bubble, player)
              });
            }
          }

          //create reflection
          if (bubble.btype == Bubble::Active)
          {
            bubbles.push_back(Bubble
            {
              ships[player].getPlace(),       //place 
              player,                         //emitter
              ships[player].getVelocity(),    //emitterVelocity
              bubble.visibility,              //visibility 
              0,                              //age 
              Bubble::Reflection,             //btype
              bubble.emitter                  //receiver
            }); 
          }
        }
      }
    }
  }

  void playRound()
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
  
  void giveWinScreen()
  {
    if (winManager.hasWinner())
    {
      cout << "Az " << winManager.getWinner() << ". játékos nyert!" << endl;
    } else
    {
      cout << "Mindenki elpusztult a csatában!" << endl;
    }
  }

  void adminPhase()
  {
    for (auto bubble : bubbles)
    {
      cout << bubble << endl << endl;
    }
  }

  void mainGameLoop()
  {
    do
    {
      adminPhase(); // ONLY FOR TEST
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

public:

  Game(unsigned int numOfShips) : 
  numOfShips(numOfShips), winManager(numOfShips), projectiles(numOfShips, -1)
  {
    for (int i=0; i<numOfShips; i++) 
    {
      ships.push_back(Ship(Point().randomize(), i));
    }
    mainGameLoop();
  }
};

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
