#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Sensor::getVStatus(DataElement* data) {
  DataElement* enee = new DataElement();
  _power.get(enee);
  data->addChild(enee);

  DataElement* maxe = new DataElement();
  maxe->_core->fromType<power_type_W>(_maxPower);
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
}
void Sensor::setVStatus(DataElement* data) {
  _power.set(data->_children[0]);
  _maxPower = data->_children[1]->_core->toType<float>();

  _pings.clear();

  for (auto&& it : data->_children[2]->_children) {

    tuple<time_type_s, energy_type_J, bool> nVal;

    vSFunc<time_type_s>(get<0>(nVal), it->_children[0]);
    vSFunc<energy_type_J>(get<1>(nVal), it->_children[1]);
    vSFunc<bool>(get<2>(nVal), it->_children[2]);

    _pings.push_back(nVal);
  }
}
void Sensor::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _power._frames) {
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
}

#ifdef M_CLIENT
void Sensor::setSidebar() {
  setSidebarElement("html/sensor_settings.xml");

  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectSensorSidebarPowerLabel"))->text = " / " + to_string(_maxPower, 0);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectSensorSidebarPowerInput"))->text = to_string(_power.getAt(timeNow)(), 2);
  reinterpret_cast<Graphics::CheckboxHwnd>(Graphics::getElementById("objectSensorAutoFireCheckbox"))->checked = _autofire;
}
#endif

#ifdef M_SERVER
void Sensor::getPathVirt(time_type_s time, Path* p) {
  if (p->type() == Path::PathTypeBubble) {
    /*float h = _health.getAt(time)() / float(_maxHealth);
    cout << "SENSOR " << _ID << " EN " << reinterpret_cast<Bubble*>(p)->energy << " FROM " << p->originID << endl;
    if (ran1() < 2 * (1 - 1 / (1 + h))) {
      float e = reinterpret_cast<Bubble*>(p)->energy * _power.getAt(time)();
      if (ran1() < 1 / (1 + pow(2, (1 / e - e) * 3))) { //detect
        cout << "Detected" << endl;
        Sighting* s = new Sighting();
        Movement m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
        parentShip->sightMovement(m, time);
      }
    }*/
    ScriptData* d = new ScriptData();
    ScriptData* hph = new ScriptData();
    hph->type = ScriptData::TNUMERIC;
    hph->_data.fromType<double>(_health.getAt(time)() / double(_maxHealth));
    ScriptData* epp = new ScriptData();
    epp->type = ScriptData::TNUMERIC;
    epp->_data.fromType<double>(reinterpret_cast<Bubble*>(p)->energy * _power.getAt(time)());
    ScriptData* meta = new ScriptData();
    meta->type = ScriptData::TSTRING;
    meta->_data.fromType<string>(reinterpret_cast<Bubble*>(p)->data);
    d->_elems.insert({ "relHealth", hph });
    d->_elems.insert({ "relEnergy", epp });
    d->_elems.insert({ "metaData", meta });
    ScriptData* res = _sensitivity->run(*d);
    if (ran1() < res->_data.toType<float>()) {
      cout << "Detected" << endl;
      Sighting* s = new Sighting();
      Movement m = reinterpret_cast<Bubble*>(p)->emitter; ///TODO Memory safe??
      parentShip->sightMovement(m, time);
    }
    delete res;
    delete d;
  }
}
#endif