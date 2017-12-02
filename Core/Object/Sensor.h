#pragma once

#include "Object.h"

class Sensor : public Object {       //Order of serialisation
private:
  keyframe<value<power_type_W>> _power;
  power_type_W _maxPower;
  energy_type_J _tempE;
  list<tuple<time_type_s, energy_type_J, bool>> _pings; //time, energy, eventCrate
  ScriptInstruction* _sensitivity;
  bool _autofire;
public:
  Sensor(uint64_t ID) : Object(ID) {}
  Sensor(mVec3 relativePos, int maxHealth, distance_type_m radius, int health, power_type_W maxPower, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
    _power.addFrame(0, 0);
    _maxPower = maxPower;
    _name = "Sensor";
    ScriptIConstant* sens = new ScriptIConstant();
    sens->_val = new ScriptData();
    sens->_val->type = ScriptData::TNUMERIC;
    sens->_val->_data.fromType<float>(1);
    _sensitivity = sens;
    _autofire = false;
  }

  int type() { return Type::Sensor; }

  power_type_W getUsedPower(time_type_s time) { return _power.getAt(time)(); }
  power_type_W getMaxPower(time_type_s time) { return _maxPower; }

  void setPower(time_type_s time, power_type_W val) {
    _power.addFrame(time, max(min(val, _maxPower), 0.0));
  }
  void setEnergy(energy_type_J val) {
    _tempE = val;
  }
  void setAutofire(bool val) {
    _autofire = val;
  }
  void ping(time_type_s time) {
    _pings.push_back(make_tuple(time, _tempE, false));
  }


#ifdef M_SERVER
  void getPathVirt(time_type_s time, Path* p);

  bool loadV(xml_node<>* data) {
    xml_node<>* elem;

    elem = data->first_node("max_power");
    if (!elem) {
      return false;
    }

    _maxPower = strTo<power_type_W>(elem->value());

    elem = data->first_node("sensitivity");
    if (!elem) {
      return false;
    }

    string filename = elem->value();

    ifstream sensfile(filename);

    std::stringstream buffer;
    buffer << sensfile.rdbuf();
    sensfile.close();

    xml_document<> doc;
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    if (_sensitivity) {
      delete _sensitivity;
    }

    _sensitivity = new ScriptIBlock();

    _sensitivity->load(doc.first_node("root"));
  }
#endif

#ifdef M_CLIENT
  void setSidebar();
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};