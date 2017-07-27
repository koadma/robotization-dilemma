#include "Bubble.h"

using namespace std;

Movement Movement::getAt(time_type_s gTime, vel_type_mpers maxVelocity) const  {
  Movement m = *this;
  m.pos = pos + vel*(gTime-gTimeStamp) + acc*pow(gTime-gTimeStamp, 2) / 2.0f;
  m.vel = vel + acc*(gTime - gTimeStamp);
  m.gTimeStamp = gTime;
  return m;
}
Movement Movement::getAt(time_type_s gTime) const {
  return getAt(gTime, SOL);
}
void Movement::get(DataElement* data) {
  DataElement* timee = new DataElement();
  timee->_core->fromType<float>(gTimeStamp);
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
  typee->_core->fromType<int>(type());
  data->addChild(typee);

  DataElement* pathe = new DataElement();
  pathe->_core->fromType<string>(pathData);
  data->addChild(pathe);

  DataElement* rade = new DataElement();
  rade->_core->fromType<double>(radius);
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

  //type
  //type = data->_children[0]->_core->toType<int>();

  //pathData
  pathData = data->_children[0]->_core->toType<string>();

  //radius
  radius = data->_children[0]->_core->toType<double>();
}
Movement::~Movement() {

}
