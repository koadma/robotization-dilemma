#include "Ship.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int Ship::getSpentEnergy() const
{
  return command.accel.length()+(command.didFire?100:0)+command.sensorEnergy;
}

ostream& operator<<(ostream& os, const Command& c)
{
  cout << "Gyorsulás: " << c.accel << endl;
  cout << "Tüzel-e: " << (c.didFire?"igen":"nem") << endl;
  if (c.didFire)
  {
    cout << "Cél: " << c.aim << endl;
  }
  cout << "Sensor energia: " <<c. sensorEnergy; //no endl
  return os;
}

ostream& operator<<(ostream& os, const SensorDataElement& sde)
{
  cout << ((sde.detectType == SensorDataElement::Active)?"Aktív":"Passzív") << " szenzor jelzett!" << endl;
  cout << "Játékos: " << sde.player << endl;
  cout << "Hely: " << sde.place << endl;
  cout << "Sebesség: " << sde.velocity << endl;
  cout << "Jósolt hely: " << sde.nextPlace << endl;
  cout << "Biztos-e a találat: " << (sde.sureFire?"igen":"nem"); // no endl
  return os;
}

void Object::move(Point accel, float time)
{
  float solut[2];
  int numOfSolut;
  float a = pow(accel.length(), 2);
  float b = 2*(velocity.x*accel.x + velocity.y*accel.y + velocity.z*accel.z);
  float c = pow(velocity.length(), 2) - pow(maxVelocity, 2);
  if (a == 0)
  {
    velocity = velocity;
    place = place + velocity*time;
  } else
  {
    solve2(a, b, c, solut, numOfSolut);
    float reachMaxT = max(solut[0], solut[1]);
    if (reachMaxT < 0 || numOfSolut != 2)
    {
      throw 1;
    }
    if (reachMaxT > time)
    {
      place = place + velocity * time + accel*(pow(time, 2)/2);
      velocity = velocity + accel*time;
    } else
    {
      Point startVelocity = velocity;
      velocity = velocity + accel*reachMaxT;
      place = place + startVelocity*time + velocity*(time-reachMaxT) + accel*(pow(reachMaxT, 2)/2);
    }
  }
}

void Ship::flushSensorData() 
{
  sensorData = vector<SensorDataElement>();
}

void Ship::giveSensorData() const
{
  for (auto sde : sensorData)
  {
    cout << sde << endl << endl;
  }
}

void Ship::getCommand()
//with Command class
{
  cout << "Adj meg parancsokat!" << endl;
  bool gettingInput = true;
  while (gettingInput)
  {
    string input;
    getline(cin, input);
    if (input == "" or input == "\n")
    {
      continue;
    }
    vector<string> cmd = tokenize(input);
    //parse (move x y z, aim x, sensor energy, data) 
    if (cmd[0] == "move")
    {
      if (cmd.size() != 4)
      {
        cout << "Rossz számú szó!" << endl;
      } else
      {
        command.accel = {strTo<float>(cmd[1]), strTo<float>(cmd[2]), strTo<float>(cmd[3])};
        if (command.accel.length() > maxAcceleration)
        {
          cout << "Túl nagy gyorsulást adtál meg!" << endl;
          cout << "A hajód " << maxAcceleration << " gyorsulásra képes!" << endl;
          command.accel = {0, 0, 0};
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
          command.didFire = false;
        } else
        {
          int target = strTo<float>(cmd[1]);
          //Is the aiming sure?
          bool valid = false;
          for (auto sde : sensorData)
          {
            if (sde.player == target && sde.sureFire)
            {
              valid = true;
            }
          }
          if (valid)
          {
            command.didFire = true;
            command.aim = strTo<float>(cmd[1]);
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
        command.sensorEnergy = strTo<float>(cmd[1]);
        if (strTo<float>(cmd[1]) > maxSensorEnergy)
        {
          cout << "Túl nagy szenzor energiát adtál meg!" << endl;
          cout << "A hajód " << maxSensorEnergy << "-re képes!" << endl;
          command.sensorEnergy = 0;
        }
      } else
      {
        cout << "Rossz számú szó!" << endl;
      }
    } else if (cmd[0] == "data")
    {
      cout << command << endl;
    } else if (input == "over")
    {
      if (getSpentEnergy() <= maxGeneratorEnergy)
      {
        gettingInput = false;
      } else
      {
        cout << getSpentEnergy() << " energiát szeretnél felhasználni." << endl;
        cout << "A hajód generátora maximum " << maxGeneratorEnergy << "-re képes." << endl;
      }
    } else
    {
      cout << "Ismeretlen parancs!" << endl;
    }
  }
}

bool Ship::didFire() const
{
  return command.didFire;
}

int Ship::getAim() const
{
  return command.aim;
}

void Ship::sense(SensorDataElement sde)
{
  sensorData.push_back(sde);
}

int Ship::getVisibility() const
{
  return floor(command.accel.length())+(command.didFire?10:0)+(command.sensorEnergy); // placeholder
}

int Ship::getSensorRadiation() const
{
  return command.sensorEnergy; //placeholder
}

void Ship::move(float time)
{
  this->Object::move(command.accel, time);
}

bool Ship::isDestroyed() const
{
  return destroyed;
}

void Ship::destroy() 
{
  destroyed = true;
}

Point Ship::getPlace() const
{
  return place;
}

Point Ship::getVelocity() const
{
  return velocity;
}

int Ship::getHullRadius() const
{
  return hullRadius;
}


unsigned int Ship::getOwner() const
{
  return owner;
}

ostream& operator<<(ostream& os, const Ship& s)
{
  if (s.destroyed)
  {
    os << "A hajó megsemmisült!"; // no endl
  } else
  {
    os << "Helyvektor: " << s.place << endl;
    os << "Sebességvektor: " << s.velocity; // no endl
  }
  return os;
}
