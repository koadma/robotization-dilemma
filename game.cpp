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

using namespace std;

struct Ship 
{
  Point place;
  Point velocity = {0, 0, 0};
  int owner; //0..numOfShips-1
  int maxVelocity = INITIAL_MAX_VELOCITY;
  int maxAcceleration = INITIAL_MAX_ACC;
  int maxSensorEnergy = INITIAL_MAX_SENSORENERGY;
  int hullRadius = INITIAL_HULL_RADIUS;
  bool isDestroyed = false;
  Ship(Point place, int owner) : place(place), owner(owner), 
    velocity
    ( 
      ((-1)*INITIAL_VELOCITY/place.length())*place
    ) {}
  friend ostream& operator<<(ostream& os, const Ship& s)
  {
    if (s.isDestroyed)
    {
      os << "A hajó megsemmisült!"; // no endl
    } else
    {
      os << "Helyvektor: " << s.place << endl;
      os << "Sebességvektor: " << s.velocity; // no endl
    }
    return os;
  }
};

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
  struct Command
  {
    Point accel = {0, 0, 0};
    bool didFire = false;
    int aim;
    //Point aim = {0, 0, 0};
    int sensorEnergy = 0; // 0..
    friend ostream& operator<<(ostream& os, const Command& c)
    {
      cout << "Gyorsulás: " << c.accel << endl;
      cout << "Tüzel-e: " << (c.didFire?"igen":"nem") << endl;
      (c.didFire ? (cout << "Cél: " << c.aim << endl) : (cout));
      cout << "Sensor energia: " <<c. sensorEnergy; //no endl
      return os;
    }
  };
  
  struct SensorDataElement
  {
    enum Type {Active, Passive};
    Type detectType;
    int player;
    Point place;
    Point velocity;
    Point nextPlace;
    bool sureFire;
    friend ostream& operator<<(ostream& os, const SensorDataElement& sde)
    {
      cout << ((sde.detectType == Active)?"Aktív":"Passzív") << " szenzor jelzett!" << endl;
      cout << "Játékos: " << sde.player << endl;
      cout << "Hely: " << sde.place << endl;
      cout << "Sebesség: " << sde.velocity << endl;
      cout << "Jósolt hely: " << sde.nextPlace << endl;
      cout << "Biztos-e a találat: " << (sde.sureFire?"igen":"nem"); // no endl
      return os;
    }
  };

  typedef vector<SensorDataElement> SensorData;

  struct Round
  {
    vector<Command> commands;
    vector<SensorData> datas;
    Round(unsigned int numOfShips) : commands(numOfShips), datas(numOfShips) {}
  };

  unsigned int numOfShips;
  vector<Ship> ships;
  int roundNumber = 0;
  vector<Round> rounds;
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
    //time
    cout << roundNumber << ". kör" << endl << endl;
    //own ship data
    cout << ships[currentPlayer] << endl << endl;
    //other ship data
    //with SensorDataElement class
    if (roundNumber > 0 && not ships[currentPlayer].isDestroyed)
    {
      for (SensorDataElement sde : rounds[roundNumber-1].datas[currentPlayer])
      {
        cout << sde << endl << endl;
      }
    }
    cout << "----------------------" << endl;
  }

  void getCommand(int currentPlayer)
  //with Command class
  {
    cout << "Adj meg parancsokat!" << endl;
    Command& currentPlayerCommand = rounds[roundNumber].commands[currentPlayer];
    string input;
    do
    {
      getline(cin, input);
      if (input == "" or input == "\n")
      {
        continue;
      }
      vector<string> cmd = tokenize(input);
      //parse (move x y z, aim x, sensor energy, data) 
      //todo:rewrite
      if (cmd[0] == "move")
      {
        if (cmd.size() != 4)
        {
          cout << "Rossz számú szó!" << endl;
        } else
        {
          currentPlayerCommand.accel = {strTo<float>(cmd[1]), strTo<float>(cmd[2]), strTo<float>(cmd[3])};
          //check max acc
          if (currentPlayerCommand.accel.length() > ships[currentPlayer].maxAcceleration)
          {
            cout << "Túl nagy gyorsulást adtál meg!" << endl;
            cout << "A hajód " << ships[currentPlayer].maxAcceleration << " gyorsulásra képes!" << endl;
            currentPlayerCommand.accel = {0, 0, 0};
          } 
        }
      } else if (cmd[0] == "aim")
      {
        if (cmd.size() != 2)
        {
          cout << "Rossz számú szó!" << endl;
        } else
        {
          if (cmd[1] == "off")
          {
            currentPlayerCommand.didFire = false;
          } else
          {
            int target = strTo<float>(cmd[1]);
            //Is the aiming sure?
            bool valid = false;
            for (auto sde : rounds[(roundNumber>0?(roundNumber-1):roundNumber)].datas[currentPlayer]) //ugly
            {
              if (sde.player == target && sde.sureFire)
              {
                valid = true;
              }
            }
            if (valid)
            {
              currentPlayerCommand.didFire = true;
              currentPlayerCommand.aim = strTo<float>(cmd[1]);
            } else
            {
              cout << "Nem biztos a lövés!" << endl;
            }
          }
        }
      } else if(cmd[0] == "sensor")
      {
        if (cmd.size() == 2)
        {
          currentPlayerCommand.sensorEnergy = strTo<float>(cmd[1]);
          //check max sensor energy
          if (strTo<float>(cmd[1]) > ships[currentPlayer].maxSensorEnergy)
          {
            cout << "Túl nagy szenzor energiát adtál meg!" << endl;
            cout << "A hajód " << ships[currentPlayer].maxSensorEnergy << "-re képes!" << endl;
            currentPlayerCommand.sensorEnergy = 0;
          }
        } else
        {
          cout << "Rossz számú szó!" << endl;
        }
      } else if (cmd[0] == "data")
      {
        cout << currentPlayerCommand << endl;
      } else
      {
        cout << "Ismeretlen parancs!" << endl;
      }
    } while (input != "over");
  }
  
  void manageBubbles()
  {
    //create bubbles
    for (int player=0; player<numOfShips; player++)
    {
      Ship ship = ships[player];
      Command c = rounds[roundNumber].commands[player];
      if (not ship.isDestroyed)
      {
        //passive bubble
        int vis = floor(c.accel.length())+(c.didFire?10:0)+(c.sensorEnergy); //placeholder
        bubbles.push_back(Bubble
        {
          ship.place,      //place 
          player,          //emitter
          ship.velocity,   //emitterVelocity
          vis,             //visibility 
          0,               //age 
          Bubble::Passive, //btype
          0                //receiver
        });
        
        //active bubble
        if (c.sensorEnergy > 0)
        {
          bubbles.push_back(Bubble
          {
            ship.place,
            player,
            ship.velocity,
            c.sensorEnergy,   //visibility
            0,                //age
            Bubble::Active,
            0                 //receiver
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
      Command c = rounds[roundNumber].commands[player];
      if(c.didFire)
      {
        int oldT = projectiles[c.aim];
        int newT = floor( distance(ships[player].place, ships[c.aim].place) / SOL );
        if (oldT == -1)
        {
          projectiles[c.aim] = newT;
        } else
        {
          projectiles[c.aim] = min(oldT, newT);
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
          ships[player].isDestroyed = true;
          winManager.lose(player);
        }
        projectiles[player]--;
      }
    }
  }

  pair<Point, Point> moveObject(Point place, Point velocity, Point accel, float time, float maxVelocity)
  {
    Point endVelocity;
    Point endPlace;
    float solut[2];
    int numOfSolut;
    float a = pow(accel.length(), 2);
    float b = 2*(velocity.x*accel.x + velocity.y*accel.y + velocity.z*accel.z);
    float c = pow(velocity.length(), 2) - pow(maxVelocity, 2);
    if (a == 0)
    {
      endVelocity = velocity;
      endPlace = place + velocity*time;
    } else
    {
      solve2(a, b, c, solut, numOfSolut);
      if (numOfSolut != 2)
      {
        throw 1;
      }
      float reachMaxT = max(solut[0], solut[1]);
      if (reachMaxT < 0)
      {
        throw 1;
      }
      if (reachMaxT > time)
      {
        endVelocity = velocity + accel*time;
        endPlace = place + velocity * time + accel*(pow(time, 2)/2);
      } else
      {
        endVelocity = velocity + accel*reachMaxT;
        endPlace = place + endVelocity*(time-reachMaxT) + accel*(pow(reachMaxT, 2)/2);
      }
    }
    return make_pair(endPlace, endVelocity);
  }
  
  void moveShips()
  {
    for (int player = 0; player<numOfShips; player++)
    {
      const Command& c = rounds[roundNumber].commands[player];
      Ship& s = ships[player];
      if (not s.isDestroyed)
      {
        pair<Point, Point> res = moveObject(s.place, s.velocity, c.accel, ROUND_TIME, s.maxVelocity);
        s.place = res.first;
        //reaching origo == win
        if (s.place.length() <= s.hullRadius) //can go over, if the ship is fast
        {
          winManager.win(player);
        }
        s.velocity = res.second;
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
      for (Bubble bubble : bubbles)
      {
        //I dont want self-detections.
        if (bubble.emitter == player)
        {
          continue;
        }
        int dist = distance(bubble.origin, ships[player].place);
        if (dist < bubble.age*SOL*ROUND_TIME && 
            dist > (bubble.age-1)*SOL*ROUND_TIME) // collision was in this round
        {
          SensorData& sd = rounds[roundNumber].datas[player];
          //passive sensor
          if (bubble.btype == Bubble::Active || bubble.btype == Bubble::Passive)
          {
            if (didDetectPassively(bubble, player))
            {
              sd.push_back(SensorDataElement
              {
                SensorDataElement::Passive, //detectType
                bubble.emitter,         //player
                bubble.origin,          //place
                bubble.emitterVelocity, //velocity
                moveObject(bubble.origin, bubble.emitterVelocity, {0, 0, 0}, ROUND_TIME, 0).first, //nextPlace
                isSureFire(bubble, player) //sureFire?
              });
            }
          }

          //active sensor
          if (bubble.btype == Bubble::Reflection && bubble.receiver == player)
          {
            if (didDetectActively(bubble, player))
            {
              sd.push_back(SensorDataElement
              {
                SensorDataElement::Active,
                bubble.emitter,
                bubble.origin,
                bubble.emitterVelocity,
                moveObject(bubble.origin, bubble.emitterVelocity, {0, 0, 0}, ROUND_TIME, 0).first,
                isSureFire(bubble, player)
              });
            }
          }

          //create reflection
          if (bubble.btype == Bubble::Active)
          {
            bubbles.push_back(Bubble
            {
              ships[player].place,      //place 
              player,                   //emitter
              ships[player].velocity,   //emitterVelocity
              bubble.visibility,        //visibility 
              0,                        //age 
              Bubble::Reflection,       //btype
              bubble.emitter            //receiver
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
      rounds.push_back(Round(numOfShips));
      adminPhase(); // ONLY FOR TEST
      waitForEnter();
      for(int i=0; i<numOfShips; i++)
      {
        askToContinue(i);
        giveInformation(i);
        if (ships[i].isDestroyed)
        {
          waitForEnter();
        } else
        {
          getCommand(i);
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
