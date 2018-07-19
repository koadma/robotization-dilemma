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
int SensorDetect::type() {
  return EvTSensorDetect;
}
void StateChange::getV(DataElement* data) {

}
void StateChange::get(DataElement* data) {
  DataElement* ide = new DataElement();
  uint64_t idev = type();
  ide->_core->fromType<uint64_t>(idev);
  data->addChild(ide);

  DataElement* oe = new DataElement();
  uint64_t ov = _o->getId();
  oe->_core->fromType<uint64_t>(ov);
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
  cout << "Ping event sent" << endl;
  DataElement* enee = new DataElement();
  enee->_core->fromType<energy_type_J>(_energy);
  data->addChild(enee);
}
int SensorAutofire::type() {
  return EvTSensorAutofire;
}
void SensorAutofire::getV(DataElement* data) {
  DataElement* afe = new DataElement();
  afe->_core->fromType<bool>(_autofire);
  data->addChild(afe);
}

#ifdef M_SERVER
void Event::apply(Game *g) {
  _isApplying = true;
  applyV(g);
  _isApplying = false;
}
void Event::applyV(Game *g) {
  throw 1;
}
void Collision::applyV(Game *g) {
  if (_p->verify(_time, _o->getMovement(_time).pos)) { //for future non full sphere em waves (e.g. cones)
    LOG INFO GAME "Passed" END;
    _o->getPath(_time, _p, g);
  }
}
void BatteryDrain::applyV(Game *g) {
  _d->energyUpdate(_time, g);
}
void SensorDetect::applyV(Game *g) {
  _o->detectCallback(_time, _closed,_what,  g);
}
void StateChange::applyV(Game *g) {
  applyVV(g);
}
void StateChange::applyVV(Game *g) {
  throw 1;
}
void StateChange::set(DataElement* data, Game* game) {
  _o = game->getObject(data->_children[1]->_core->toType<uint64_t>());
  if (_o == NULL) {
    LOG LERROR GAME "Unknown Object ID " << data->_children[1]->_core->toType<uint64_t>() END;
  }
  _time = data->_children[2]->_core->toType<float>();

  setV(data->_children[3], game);
}
void StateChange::setV(DataElement* data, Game* game) {

}
void EngineAcc::applyVV(Game *g) {
  reinterpret_cast<Engine*>(_o)->setTargetAccel(_time, _acc, g);

  g->recalcIntersects(_o->_parentShip);
}
void EngineAcc::setV(DataElement* data, Game* game) {
  _acc.set(data->_children[0]);
}
void SensorPow::applyVV(Game *g) {
  reinterpret_cast<Sensor*>(_o)->setTargetPower(_time, _power, g);
}
void SensorPow::setV(DataElement* data, Game* game) {
  _power = data->_children[0]->_core->toType<power_type_W>();
}
void LaserShot::applyVV(Game *g) {
  Shot* s = new Shot();
  s->energy = _energy - _o->useEnergy(_time, _energy, g);
  s->origin = _o->getMovement(_time).getAt(_time, SOL).pos;
  s->originID = _o->getId();
  s->gStartTime = _time;
  s->vel = _dir;
  LOG INFO GAME "Laser " << _o->getId() << " shot " << s END;
  g->add(s);
}
void LaserShot::setV(DataElement* data, Game* game) {
  _dir.set(data->_children[0]);

  _energy = data->_children[1]->_core->toType<energy_type_J>();
}
void ThermalRadiation::applyVV(Game *g) {
  Bubble* b = new Bubble();
  if(_o->getAccel(_time).length() > 0) {
    b->setConstrains({ constrain(constrain::include, -_o->getAccel(_time), cos(60.0_deg))}); //Blast in direction
  } else {
    b->setConstrains({ constrain(constrain::include, {1,0,0}, -2)}); //Include all directions
  }
  b->bsource = Bubble_Thermal;
  b->btype = Bubble_Row_Border;
  b->emitter = _o->getMovement(_time);
  b->energy = to_doubleT<energy_type_J>(_o->getUsedPower(_time));
  b->gStartTime = _time;
  b->originID = _o->getId();

  //_o->_parentShip->energyUpdate(_time, game); //recalculate ship energy info

  _o->changeSelfBubble(b, g);
}
void ThermalRadiation::setV(DataElement* data, Game* game) {

}
void SensorPing::applyVV(Game *g) {
  Bubble* b = new Bubble();
  b->setConstrains({constrain(constrain::include, { 1, 0, 0 }, -2)}); //Include all directions
  b->bsource = Bubble_Ping;
  b->emitter = _o->getMovement(_time);
  b->energy = _energy - _o->useEnergy(_time, _energy, g);
  b->gStartTime = _time;
  b->originID = _o->getId();

  _o->_parentShip->energyUpdate(_time, game); //recalculate ship energy info
  LOG INFO GAME "Sensor " << _o->getId() << " pinged " << b END;
  g->add(b);
}
void SensorPing::setV(DataElement* data, Game* game) {
  _energy = data->_children[0]->_core->toType<energy_type_J>();
}
void SensorAutofire::applyVV(Game *g) {
  reinterpret_cast<Sensor*>(_o)->setAutofire(_autofire, _time);
}
void SensorAutofire::setV(DataElement* data, Game* game) {
  _autofire = data->_children[0]->_core->toType<bool>();
}
#endif
