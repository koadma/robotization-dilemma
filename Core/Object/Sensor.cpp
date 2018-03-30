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
double Sensor::getSensorChance(Bubble* what, time_type_s when) {
  ScriptData* d = new ScriptData();
  ScriptData* hph = new ScriptData();
  hph->_data = new ScriptDataNumber(_health.getAt(when) / double(_maxHealth.getAt(when)));
  ScriptData* epp = new ScriptData();
  epp->_data = new ScriptDataNumber((what->getFlux(when, getMovement(when).pos) * _radius * _radius * PI * _selfUsedPower.getAt(when)).toDouble());
  ScriptData* meta = new ScriptData();
  meta->_data = new ScriptDataString(what->data);
  d->_elems.insert({ "relHealth", hph });
  d->_elems.insert({ "relEnergy", epp });
  d->_elems.insert({ "metaData", meta });
  ScriptData* res = _sensitivity->run(*d);
  double resd = ((ScriptDataNumber*)res->_data)->_num;
  DeletePtr(res);
  DeletePtr(d);
  return resd;
}

void Sensor::getPathVirt(time_type_s time, Path* p, Game* g) {
  if (p->type() == Path::PathTypeBubble) {
    Bubble* b = (Bubble*)p;
    if (high(b->originID) == high(this->_ID)) {
      return;
    }
    LOG INFO GAME "Pow: " << _selfUsedPower.getAt(time) END;
    LOG INFO GAME "Src: " << reinterpret_cast<Bubble*>(p)->emitter.pos END;
    LOG INFO GAME "Emt: " << reinterpret_cast<Bubble*>(p)->gEmissionTime END;
    LOG INFO GAME "OID: " << reinterpret_cast<Bubble*>(p)->originID END;
    LOG INFO GAME "SID: " << _ID END;
    LOG INFO GAME "En : " << b->getFlux(time, getMovement(time).pos) * _radius * _radius * PI END;

    if (b->btype == BubbleType::Bubble_Pulse) {
      float detectCh = getSensorChance(b, time);
      LOG INFO GAME "Ch: " << detectCh END;
      if (ran1() < detectCh) {
        LOG INFO GAME "Detected" END;
        detectCallback(time, true, p, game);
      }
    }
    if (b->btype == BubbleType::Bubble_Row_Border) {
      energy_type_J en_before = 0;
      double endtime_inv = 0;
      double begtime_inv = 0;
      if (b->before != NULL) {
        double endval = getSensorChance(b->before, time);
        double endprob = endval / (1 - endval); //probability of detection, 1/s
        endtime_inv = -endprob / (log(ran1())-5);
      }
      double begval = getSensorChance(b, time);
      double begprob = begval / (1- begval); //probability of detection, 1/s
      begtime_inv = -begprob / (log(ran1()) - 5);
      if (endtime_inv + begtime_inv > EPS) {
        SensorDetect* nd;
        if (b->before != NULL) {
          double detecttime = 1 / (begtime_inv + endtime_inv);
          LOG INFO GAME "End detected in " << detecttime END;
          nd = new SensorDetect();
          nd->_o = this;
          nd->_time = time + detecttime;
          nd->_what = b->before;
          nd->_closed = true;
          g->add(nd);
        }

        double detecttime = 1 / (begtime_inv);
        LOG INFO GAME "New detected in " << detecttime END;
        nd = new SensorDetect();
        nd->_o = this;
        nd->_time = time + detecttime;
        nd->_what = p;
        nd->_closed = false;
        g->add(nd);
      }
      else {
        LOG INFO GAME "Never detected" END;
      }
    }
  }
}
void Sensor::detectCallback(time_type_s time, bool closed, Path* p, Game* g) {
  if (p->type() == Path::PathTypeBubble) {
    Bubble* b = (Bubble*)p;
    if(b->btype == Bubble_Row_Border) {
      _parentShip->sightPath(b, time, g, closed, _autofire.getAt(time)());
    }
    else {
      _parentShip->sightPath(b, time, g, false, _autofire.getAt(time)());
    }
  }
}
#endif