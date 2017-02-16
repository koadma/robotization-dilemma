#include "Bubble.h"

using namespace std;

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
            Point(),//moveObject(bubble.origin, bubble.emitterVelocity, {0, 0, 0}, ROUND_TIME, 0).first, 
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
            Point(),//placeHolder
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
