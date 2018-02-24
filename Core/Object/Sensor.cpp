#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Sensor::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  _requestedPower.get(enee);
  data->addChild(enee);

  DataElement* maxe = new DataElement();
  _maxUseablePower.get(maxe);
  data->addChild(maxe);

  DataElement* pinge = new DataElement();
  for (auto&& it : _pings) {
    DataElement* ne = new DataElement();

    DataElement* timee = new DataElement();
    vGFunc<time_type_s>(get<0>(it), timee);
    ne->addChild(timee);

    DataElement* enee = new DataElement();
    vGFunc<energy_type_J>(get<1>(it), enee);
    ne->addChild(enee);

    DataElement* usede = new DataElement();
    vGFunc<bool>(get<2>(it), usede);
    ne->addChild(usede);

    pinge->addChild(ne);
  }

  data->addChild(pinge);

  DataElement* afe = new DataElement();
  _autofire.get(afe);
  data->addChild(afe);
}
void Sensor::setVStatus(DataElement* data) {
  _requestedPower.set(data->_children[0]);
  _maxUseablePower.set(data->_children[1]);

  _pings.clear();

  for (auto&& it : data->_children[2]->_children) {

    tuple<time_type_s, energy_type_J, bool> nVal;

    vSFunc<time_type_s>(get<0>(nVal), it->_children[0]);
    vSFunc<energy_type_J>(get<1>(nVal), it->_children[1]);
    vSFunc<bool>(get<2>(nVal), it->_children[2]);

    _pings.push_back(nVal);
  }

  _autofire.set(data->_children[3]);
}
void Sensor::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _requestedPower._frames) {
    SensorPow* ev = new SensorPow();
    ev->_power = it.second();
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
  }

  for (auto&& it : _pings) {
    if (time <= get<0>(it) && get<2>(it) == false) {
      SensorPing* ev = new SensorPing();
      ev->_energy = get<1>(it);
      ev->_o = this;
      ev->_time = get<0>(it);
      addTo.push_back(ev);
    }
    get<2>(it) = true;
  }

  for (auto&& it : _autofire._frames) {
    SensorAutofire* ev = new SensorAutofire();
    ev->_autofire = it.second();
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
  }
}

#ifdef M_CLIENT
void Sensor::setSidebar() {
  setSidebarElement("html/sensor_settings.xml");

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarPowerLabel"))->text = " / " + to_string(getMaxUseablePower(timeNow), 0);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectSensorSidebarPowerInput"))->text = to_string(getRequestedPower(timeNow), 2);
  reinterpret_cast<Graphics::CheckboxHwnd>(Graphics::getElementById("objectSensorAutoFireCheckbox"))->checked = _autofire.getAt(timeNow)();
}
#endif

#ifdef M_SERVER
void Sensor::setTargetPower(time_type_s time, power_type_W val, Game* g) {
  setTargetPower(time, val);
  _parentShip->energyUpdate(time, g);
}
void Sensor::energyCallbackV(time_type_s time, Game* g) {
  
}
void Sensor::getPathVirt(time_type_s time, Path* p, Game* g) {
  if (p->type() == Path::PathTypeBubble) {
    /*float h = _health.getAt(time)() / float(_maxHealth);
    cout << "SENSOR " << _ID << " EN " << reinterpret_cast<Bubble*>(p)->energy << " FROM " << p->originID << endl;
    if (ran1() < 2 * (1 - 1 / (1 + h))) {
      float e = reinterpret_cast<Bubble*>(p)->energy * _power.getAt(time)();
      if (ran1() < 1 / (1 + pow(2, (1 / e - e) * 3))) { //detect
        cout << "Detected" << endl;
        Sighting* s = new Sighting();
        Movement m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
        _parentShip->sightMovement(m, time);
      }
    }*/
    cout << "Pow: " << _selfUsedPower.getAt(time)();
    cout << "Src: " << reinterpret_cast<Bubble*>(p)->emitter.pos;
    cout << "Emt: " << reinterpret_cast<Bubble*>(p)->gEmissionTime;
    cout << "OID: " << reinterpret_cast<Bubble*>(p)->originID;
    cout << "SID: " << _ID;
    cout << "En : " << reinterpret_cast<Bubble*>(p)->getFlux(time, getMovement(time).pos);
    ScriptData* d = new ScriptData();
    ScriptData* hph = new ScriptData();
    hph->_data = new ScriptDataNumber(_health.getAt(time)() / double(_maxHealth.getAt(time)()));
    ScriptData* epp = new ScriptData();
    epp->_data = new ScriptDataNumber(reinterpret_cast<Bubble*>(p)->getFlux(time, getMovement(time).pos) * _selfUsedPower.getAt(time)() * PI * _radius * _radius);
    ScriptData* meta = new ScriptData();
    meta->_data = new ScriptDataString(reinterpret_cast<Bubble*>(p)->data);
    d->_elems.insert({ "relHealth", hph });
    d->_elems.insert({ "relEnergy", epp });
    d->_elems.insert({ "metaData", meta });
    ScriptData* res = _sensitivity->run(*d);
    float resf = ((ScriptDataNumber*)res->_data)->_num;
    cout << "Ch: " << resf << endl;
    if (((Bubble*)p)->btype == BubbleType::Bubble_Pulse) {
      if (ran1() < resf) {
        cout << "Detected" << endl;
        detectCallback(time, p, game);
      }
    }
    if (((Bubble*)p)->btype == BubbleType::Bubble_Start || ((Bubble*)p)->btype == BubbleType::Bubble_End) {
      float prob = resf / (1-resf);
      float detecttime = -log(1-ran1()) / prob;
      cout << "Detected in " << detecttime << endl;
      SensorDetect* nd = new SensorDetect();
      nd->_o = this;
      nd->_time = time + detecttime;
      nd->_what = p;
      g->add(nd);
    }
    DeletePtr(res);
    DeletePtr(d);
  }
}
void Sensor::detectCallback(time_type_s time, Path* p, Game* g) {
  if (p->type() == Path::PathTypeBubble) {
    Bubble* b = (Bubble*)p;
    if(b->btype == Bubble_Start) {
      if(b->other == NULL || b->other->gEmissionTime > time) {
        Movement m = b->emitter;
        _parentShip->sightMovement(m, time, g, Bubble_Start, _autofire.getAt(time)());
      }
    }
    if (b->btype == Bubble_End) {
      if (true) {
        Movement m = b->emitter;
        _parentShip->sightMovement(m, time, g, Bubble_End, _autofire.getAt(time)());
      }
    }
  }
}
#endif