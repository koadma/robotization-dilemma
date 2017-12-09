#include "Ship.h"
#ifdef M_SERVER
#include "../Server/Game.h"
#endif

int Event::type() {
  return EvTEvent;
}
int Collision::type() {
  return EvTCollision;
}
int BatteryDrain::type() {
  return EvTBatteryDrain;
}
int StateChange::type() {
  return EvTStateChange;
}
void StateChange::getV(DataElement* data) {

}
void StateChange::get(DataElement* data) {
  DataElement* ide = new DataElement();
  ide->_core->fromType<uint64_t>(type());
  data->addChild(ide);

  DataElement* oe = new DataElement();
  oe->_core->fromType<uint64_t>(_o->getId());
  data->addChild(oe);

  DataElement* timee = new DataElement();
  timee->_core->fromType<time_type_s>(_time);
  data->addChild(timee);

  DataElement* virte = new DataElement();
  getV(virte);
  data->addChild(virte);
}
int EngineAcc::type() {
  return EvTEngineAcc;
}
void EngineAcc::getV(DataElement* data) {
  DataElement* acce = new DataElement();
  _acc.get(acce);
  data->addChild(acce);
}
int SensorPow::type() {
  return EvTSensorPow;
}
void SensorPow::getV(DataElement* data) {
  DataElement* powe = new DataElement();
  powe->_core->fromType<power_type_W>(_power);
  data->addChild(powe);
}
int LaserShot::type() {
  return EvTLaserShot;
}
void LaserShot::getV(DataElement* data) {
  DataElement* dire = new DataElement();
  _dir.get(dire);
  data->addChild(dire);

  DataElement* enee = new DataElement();
  enee->_core->fromType<energy_type_J>(_energy);
  data->addChild(enee);
}
int ThermalRadiation::type() {
  return EvTThermalRadiation;
}
void ThermalRadiation::getV(DataElement* data) {

}
int SensorPing::type() {
  return EvTSensorPing;
}
void SensorPing::getV(DataElement* data) {
  DataElement* enee = new DataElement();
  enee->_core->fromType<energy_type_J>(_energy);
  data->addChild(enee);
}


#ifdef M_SERVER
void Event::apply(Game *g) {
  throw 1;
}
void Collision::apply(Game *g) {
  _o->getPath(_time, _p, g);
}
void BatteryDrain::apply(Game *g) {
  cout << __FILE__ << ":" << __LINE__ << " Ship ran out of energy!" << endl;
}
void StateChange::apply(Game *g) {
  throw 1;
}
void StateChange::set(DataElement* data, Game* game) {
  _o = game->getObject(data->_children[1]->_core->toType<uint64_t>());
  if (_o == NULL) {
    cout << __FILE__ << ":" << __LINE__ << " Unknown Object ID " << data->_children[1]->_core->toType<uint64_t>() << endl;
    throw 1;
  }
  _time = data->_children[2]->_core->toType<float>();

  setV(data->_children[3], game);
}
void StateChange::setV(DataElement* data, Game* game) {

}
void EngineAcc::apply(Game *g) {
  reinterpret_cast<Engine*>(_o)->setTargetAccel(_time, _acc);

  _o->parentShip->energyUpdate(_time, g); //recalculate ship energy info

  g->removeIntersect(_o->parentShip);
  g->calcIntersect(_o->parentShip); //recalculate ship related future intersections

  ThermalRadiation ev;
  ev._o = _o;
  ev._time = _time;
  ev.apply(game);
}
void EngineAcc::setV(DataElement* data, Game* game) {
  _acc.set(data->_children[0]);
}
void SensorPow::apply(Game *g) {
  reinterpret_cast<Sensor*>(_o)->setTargetPower(_time, _power);
  _o->parentShip->energyUpdate(_time, game); //recalculate ship energy info
                                        //g.removeIntersect(_o->parentShip);
                                        //g.calcIntersect(_o->parentShip); //recalculate ship related future intersections
}
void SensorPow::setV(DataElement* data, Game* game) {
  _power = data->_children[0]->_core->toType<power_type_W>();
}
void LaserShot::apply(Game *g) {
  Shot* s = new Shot();
  s->energy = _energy/* - _o->useEnergy(_time, _energy)*/;///TODO When energy system is stabilized, enable
  s->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  s->originID = _o->getId();
  s->origintime = _time;
  s->vel = _dir;

  _o->parentShip->energyUpdate(_time, game); //recalculate ship energy info

  g->paths.push_back(s);
  g->calcIntersect(s);
}
void LaserShot::setV(DataElement* data, Game* game) {
  _dir.set(data->_children[0]);

  _energy = data->_children[1]->_core->toType<energy_type_J>();
}
void ThermalRadiation::apply(Game *g) {
  Bubble* b = new Bubble();
  b->btype = Bubble::Thermal;
  b->emitter = _o->getMovement(_time);
  b->energy = _o->getUsedPower(_time);
  b->gEmissionTime = _time;
  b->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  b->originID = _o->getId();

  _o->parentShip->energyUpdate(_time, game); //recalculate ship energy info

  g->paths.push_back(b);
  g->calcIntersect(b);
}
void ThermalRadiation::setV(DataElement* data, Game* game) {

}
void SensorPing::apply(Game *g) {
  Bubble* b = new Bubble();
  b->btype = Bubble::Ping;
  b->emitter = _o->getMovement(_time);
  b->energy = _energy;
  b->gEmissionTime = _time;
  b->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  b->originID = _o->getId();

  _o->parentShip->energyUpdate(_time, game); //recalculate ship energy info

  g->paths.push_back(b);
  g->calcIntersect(b);
}
void SensorPing::setV(DataElement* data, Game* game) {
  _energy = data->_children[0]->_core->toType<energy_type_J>();
}
#endif
