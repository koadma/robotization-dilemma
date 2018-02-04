#pragma once

#include "Object.h"

class Sensor : public Object {       //Order of serialisation
private:
  energy_type_J _tempE;
  list<tuple<time_type_s, energy_type_J, bool>> _pings; //time, energy, eventCrate
  ScriptInstruction* _sensitivity;
  keyframe<value<bool> > _autofire;
public:
  Sensor(Drone* parentShip, uint64_t ID) :
    Object(parentShip, ID)
  {}
  Sensor(Drone* parentShip, uint64_t ID, mVec3 relativePos, int maxHealth, distance_type_m radius, int health, power_type_W maxPower) :
  Object(parentShip, ID, relativePos, maxHealth, radius, health) {
    maxUseablePowerChange(-1, maxPower);
    requestedPowerChange(-1, 0);
    maxStorageChange(-1, 10000);
    _name = "Sensor";
    /*ScriptIConstant* sens = new ScriptIConstant();
    sens->_val = new ScriptData();
    sens->_val->type = ScriptData::TNUMERIC;
    sens->_val->_data.fromType<float>(1);*/
    ifstream sensfile("html/sensor_detect.xml");
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
    _autofire.addFrame(0, 0);
  }

  int type() { return Type::Sensor; }

  /*power_type_W getUsedPower(time_type_s time) { return _power.getAt(time)(); }
  power_type_W getMaxPower(time_type_s time) { return _maxPower; }*/

  virtual power_type_W getDisplayMaxPower(time_type_s time) { ///Max generatable power, max usable, whatever applicable
    return 0;
  }
  virtual power_type_W getDisplayPower(time_type_s time) {
    return 0;
  }
  virtual energy_type_J getDisplayMaxEnergy(time_type_s time) { ///Max stored energy, used in an operation, whatever applicable
    return 0;
  }
  virtual energy_type_J getDisplayEnergy(time_type_s time) {
    return 0;
  }

  void setTargetPower(time_type_s time, power_type_W val) {
    requestedPowerChange(time, val);
  }
  void setTargetEnergy(energy_type_J val) {
    _tempE = val;
  }
  void setAutofire(bool val, time_type_s time) {
    _autofire.addFrame(time, val);
  }
  void ping(time_type_s time) {
    _pings.push_back(make_tuple(time, _tempE, false));
#ifdef M_CLIENT
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectSensorSidebarEnergyInput"))->text = "0";
    setTargetEnergy(0);
#endif
  }


#ifdef M_SERVER
  void setTargetPower(time_type_s time, power_type_W val, Game* g);

  void energyCallbackV(time_type_s t, Game* g);

  void getPathVirt(time_type_s time, Path* p, Game* g);

  bool loadV(xml_node<>* data) {
    xml_node<>* elem;

    elem = data->first_node("max_power");
    if (!elem) {
      return false;
    }

    maxUseablePowerChange(0, strTo<power_type_W>(elem->value()));

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