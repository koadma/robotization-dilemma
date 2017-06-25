#include "Bubble.h"

using namespace std;
/*
void BubbleManager::out() const
{
  for (auto bubble : bubbles)
  {
    cout << bubble << endl << endl;
  }
}

//Did the ship detect the Bubble(active or passive)?
bool BubbleManager::didDetectPassively(Bubble bubble) const
{
  //todo: insert radar equation
  return bubble.visibility >= PASSIVE_SENSOR_MINVIS; //placeholder
}

//Did the ship detect the reflection bubble?
bool BubbleManager::didDetectActively(Bubble bubble) const
{
  //todo
  return bubble.visibility >= ACTIVE_SENSOR_MINVIS; //placeholder
}

//Can the ship hit the target?
bool BubbleManager::isSureFire(Bubble bubble) const
{
  //todo: insert equation
  return true; //placeholder
}
void BubbleManager::move()
{
  for (Bubble& bubble : bubbles)
  {
    bubble.age++;
    bubble.visibility--;
  }
}

void BubbleManager::add(Bubble bubble)
{
  bubbles.push_back(bubble);
}

void BubbleManager::interactWithShip(Ship& ship)
{
  for (Bubble& bubble : bubbles)
  {
    //I dont want self-detections.
    if (bubble.emitter == ship.getOwner())
    {
      continue;
    }
    int dist = distance(bubble.origin, ship.getPlace());
    if (dist < bubble.age*SOL*ROUND_TIME && 
        dist > (bubble.age-1)*SOL*ROUND_TIME) // collision was in this round
    {
      //passive sensor
      if (bubble.btype == Bubble::Active || bubble.btype == Bubble::Passive)
      {
        if (didDetectPassively(bubble))
        {
          ship.sense(SensorDataElement
          {
            SensorDataElement::Passive, //detectType
            bubble.emitter,         //player
            bubble.origin,          //place
            bubble.emitterVelocity, //velocity
            bubble.origin + bubble.emitterVelocity * ROUND_TIME * bubble.age, //nextPlace
            isSureFire(bubble) //sureFire?
          });
        }
      }

      //active sensor
      if (bubble.btype == Bubble::Reflection && bubble.receiver == ship.getOwner())
      {
        if (didDetectActively(bubble))
        {
          ship.sense(SensorDataElement
          {
            SensorDataElement::Active,
            bubble.emitter,
            bubble.origin,
            bubble.emitterVelocity,
            bubble.origin + bubble.emitterVelocity * ROUND_TIME * bubble.age, //nextPlace
            isSureFire(bubble)
          });
        }
      }

      //create reflection
      if (bubble.btype == Bubble::Active)
      {
        bubbles.push_back(Bubble
        {
          ship.getPlace(),                //place 
          ship.getOwner(),                //emitter
          ship.getVelocity(),             //emitterVelocity
          bubble.visibility,              //visibility 
          0,                              //age 
          Bubble::Reflection,             //btype
          bubble.emitter                  //receiver
        }); 
      }
    }
  }
}

ostream& operator<<(ostream& os, const Bubble& b) // only for test
{
  switch (b.btype)
  {
    case Bubble::Active:
    cout << "Aktiv buborek!" << endl;
    break;

    case Bubble::Passive:
    cout << "Passziv buborek!" << endl;
    break;

    case Bubble::Reflection:
    cout << "Visszaverodo buborek!" << endl;
    break;
  }
  if (b.btype == Bubble::Reflection)
  {
    cout << "Tudja fogadni: " << b.receiver << ". jatekos." << endl;
  }
  cout << "Kozeppont: " << b.origin << endl;
  cout << "Kelto jatekos: " << b.emitter << endl;
  cout << "Kelto jatekos sebssege kelteskor: " << b.emitterVelocity << endl;
  cout << "Lathatosag: " << b.visibility << endl;
  cout << "Kora: " << b.age << " kor."; // no endl
  return os;
}
*/


Movement Movement::goTo(float gTime, float maxVelocity) {
  /*if (time < timestamp) {
  return goBackTo(time, maxVelocity);
  }
  else {
  return goForwardTo(time, maxVelocity);
  }*/
  Movement m = *this;
  m.pos = pos + vel*(gTime-gTimeStamp) + acc*pow(gTime-gTimeStamp, 2) / 2.0f;
  m.vel = vel + acc*(gTime - gTimeStamp);
  return m;
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
