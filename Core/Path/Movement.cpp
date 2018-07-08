#include "Movement.h"

Movement Movement::getAt(time_type_s gTime, vel_type_mpers maxVelocity) const {
  Movement m = *this;
  m.pos = pos + vel*(gTime - gTimeStamp) + acc*pow(gTime - gTimeStamp, 2) / 2.0;
  m.vel = vel + acc*(gTime - gTimeStamp);
  m.gTimeStamp = gTime;
  return m;
}
Movement Movement::getAt(time_type_s gTime) const {
  return getAt(gTime, SOL);
}
void Movement::get(DataElement* data) {
  DataElement* timee = new DataElement();
  timee->_core->fromType<time_type_s>(gTimeStamp);
  data->addChild(timee);

  DataElement* pose = new DataElement();
  pos.get(pose);
  data->addChild(pose);

  DataElement* vele = new DataElement();
  vel.get(vele);
  data->addChild(vele);

  DataElement* acce = new DataElement();
  acc.get(acce);
  data->addChild(acce);

  DataElement* typee = new DataElement();
  int typev = type();
  vGFunc(typev, typee);
  data->addChild(typee);

  DataElement* pathe = new DataElement();
  pathe->_core->fromType<string>(pathData);
  data->addChild(pathe);

  DataElement* rade = new DataElement();
  rade->_core->fromType<distance_type_m>(radius);
  data->addChild(rade);
}
void Movement::set(DataElement* data) {
  //gTimeStamp
  gTimeStamp = data->_children[0]->_core->toType<float>();

  //pos
  pos.set(data->_children[1]);

  //vel
  vel.set(data->_children[2]);

  //acc
  acc.set(data->_children[3]);

  //pathData
  pathData = data->_children[5]->_core->toType<string>();

  //radius
  radius = data->_children[6]->_core->toType<double>();
}
Movement::~Movement() {

}
