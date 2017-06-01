#ifndef __SHIP_H__
#define __SHIP_H__

#include "Sighting.h"

extern class Object;
extern Object* selected;

class Drone;

class Object {       //Order of serialisation
protected:
  fVec3 _relativePos; //0
  int _maxHealth;     //2
  int _health;        //3
  float _radius;      //4
public:

  Object(fVec3 relativePos, int maxHealth, float radius, int health) {
    _relativePos = relativePos;
    _maxHealth = maxHealth;
    _health = health;
    _radius = radius;
  }

  Drone* parentShip;

  static enum Type {
    Ship = 1,
    Shield = 2,
    Sensor = 3,
    Computer = 4,
    Generator = 5,
    Storage = 6,
    Engine = 7
  };
  virtual int type() {throw 1; return 0;}         //1
  virtual float usedEnergy() {throw 1; return 0;}
  virtual float prodEnergy() {return 0;}

  Movement getMovement();

#ifdef M_CLIENT
  list< pair<double, pair<Object*, Path*>>> getIntersect(vec3<double> ori, vec3<double> dir);
  virtual void setSidebarElement();
  virtual void setSidebar();
  void drawObject(float camcx, float camcy, float camcz, float d);
#endif

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    vector<double> times = intersectPaths(&getMovement(), p);
    for (int i = 0; i < times.size(); i++) {
      res.push_back({ times[i],{ this, p } });
    }
    return res;
  }

  ~Object();
};

class Sensor : public Object {       //Order of serialisation
private:
  float _energy;
  float _maxEnergy;
public:
  Sensor(fVec3 relativePos, int maxHealth, float radius, int health, float maxEnergy) : Object(relativePos, maxHealth, radius, health) {
    _energy = 0;
    _maxEnergy = maxEnergy;
  }

  int type() {return Type::Sensor;}

  float usedEnergy() { return _energy; }

  void setEnergy(float val) {
    _energy = max(min(val, _maxEnergy), 0.0f);
  }

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);
};

class Engine : public Object {       //Order of serialisation
private:
  float _maxEnergy;
  fVec3 _accel;
public:
  Engine(fVec3 relativePos, int maxHealth, float radius, int health, float maxEnergy, fVec3 accel) : Object(relativePos, maxHealth, radius, health) {
    _accel = accel;
    _maxEnergy = maxEnergy;
  }

  int type() { return Type::Engine; }
  float usedEnergy() { return _accel.sqrlen(); }

  void setComponent(int c, float val) {
    _accel[c] = val;
  }

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);
};

class Generator : public Object {       //Order of serialisation
private:
  float _maxEnergy;
public:
  Generator(fVec3 relativePos, int maxHealth, float radius, int health, float maxEnergy) : Object(relativePos, maxHealth, radius, health) {
    _maxEnergy = maxEnergy;
  }
  int type() { return Type::Generator; }
  float usedEnergy() { return 0; }
  float prodEnergy() { return _maxEnergy; }

#ifdef M_CLIENT
  void setSidebarElement();
  void setSidebar();
#endif

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);
};

class Drone {
public:
  Movement mov;
};

class Ship : public Drone {
private:
  bool canMove = false; //is the player open to moving / are we waiting for a move.
public:
  Ship() {
    Object* no = new ::Generator({ 100,0,0 }, 1000, 100, 800, 100000);
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Sensor({ -100,0,0 }, 1000, 100, 600, 100000);
    no->parentShip = this;

    objects.push_back(no);

    no = new ::Engine({ 0,173.2f ,0 }, 1000, 100, 900, 100000, {0, 0, 0});
    no->parentShip = this;

    objects.push_back(no);
  }

  list< pair<double, pair<Object*, Path*>>> intersect(Path* p) {
    list< pair<double, pair<Object*, Path*>>> res;
    auto it = objects.begin();
    while (it != objects.end()) {
      res.splice(res.end(), (*it)->intersect(p));
      ++it;
    }
    return res;
  }

  list<Object*> objects;
  list<Sighting*> sightings;
#ifdef M_SERVER
  NetworkS* connectedClient;

  int makeMove(vector<string> & data);
  void newTurn(int id);
  bool packetRecv(unsigned char *Data, int Id, int DataLen, NetworkS* thisptr);
#endif
#ifdef M_CLIENT
  NetworkC* connectedServer;

  void commit();

  void newTurn(int id);

  void setSidebarElement();
  void setSidebar();

  void setSidebar(vec3<double> ori, vec3<double> dir) {
    list< pair<double, pair<Object*, Path*>>> inters;

    auto it = objects.begin();

    while (it != objects.end()) {
      list< pair<double, pair<Object*, Path*>>> temp = (*it)->getIntersect(ori, dir);
      inters.splice(inters.end(), temp);
      ++it;
     }

     inters.sort();

     if(inters.size()) {
       inters.begin()->second.first->setSidebar();
     } else {
       setSidebar();
     }
  }

  void drawSightings(float camcx, float camcy, float camcz, float d);
  void drawObjects(float camcx, float camcy, float camcz, float d);

  bool packetRecv(unsigned char *Data, int Id, int DataLen, NetworkC* thisptr);
#endif
  /*Object* getObject(int type);
  Object* getGenerator();
  Object* getSensor();
  void  setSensorEnergy(float energy);
  float getSensorEnergy();
  float getMaximumSensorEnergy();*/

  float getTotalShipEnergy();
  float getRemainingShipEnergy();
  float getSpentShipEnergy();

  void getStatus(unsigned char** data, int &DataLen);
  void setStatus(unsigned char* data, int DataLen);

  void getSightings(unsigned char** data, int &DataLen);
  void setSightings(unsigned char* data, int DataLen);

  void clearObjects();
  void clearSightings();

  ~Ship();
};

#ifdef M_CLIENT
//extern lis<Command> commands
extern Ship* ship;
#endif

#endif
