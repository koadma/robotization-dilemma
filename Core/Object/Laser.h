#pragma once

#include "Object.h"

class Laser : public Object {       //Order of serialisation
private:
  list<tuple<time_type_s, energy_type_J, sVec3, bool>> _shots; //time, energy, direction, eventCrated

  energy_type_J _tempE;
  sVec3 _tempD;
public:
  Laser(Drone* parentShip, uint64_t ID) :
    Object(parentShip, ID)
  {}

  int type() { return Type::Laser; }

  /*energy_type_J getMaxEnergy(time_type_s time) { return 0; }
  energy_type_J getStoredEnergy(time_type_s time) { return 0; }
  energy_type_J useEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() - amount;
    energy_type_J remain = max(0.0, -nenergy);
    _energyStored.addFrame(time, max(0.0, nenergy)); //cant go below 0 energy
    return max(0.0, remain);
  }
  energy_type_J chargeEnergy(time_type_s time, energy_type_J amount) {
    energy_type_J nenergy = _energyStored.getAt(time)() + amount;
    energy_type_J extra = nenergy - _maxStorage.getAt(time)();
    _energyStored.addFrame(time, min(_maxStorage.getAt(time)(), _energyStored.getAt(time)())); //cant go below 0 energy
    return max(0.0, extra);
  }*/

  energy_type_J getDisplayMaxEnergy(time_type_s time) { ///Max stored energy, used in an operation, whatever applicable
    return _maxStorage.getAt(time)();
}
  energy_type_J getDisplayEnergy(time_type_s time) {
    return _energySystem->_val.getAt(time);
  }

#ifdef M_SERVER
  void energyCallbackV(time_type_s t, Game* g);
#endif

  void setComponent(int index, scalar_type comp) {
    _tempD[index] = comp;
  }
  void setDir(sVec3 dir) {
    _tempD = dir;
  }
  void setEnergy(energy_type_J val) {
    _tempE = val;
  }

  void shoot(time_type_s time) {
    _shots.push_back(make_tuple(time, _tempE, _tempD.norm() * SOL, false));
    #ifdef M_CLIENT
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarEnergyInput"))->text = "0";
    setEnergy(0);
    #endif
  }

#ifdef M_CLIENT
  void setSidebar();
  void drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time, bool full);
#endif

  void collectEvents(list<StateChange*> &addTo, time_type_s time);

  void getVStatus(DataElement* data);
  void setVStatus(DataElement* data);
};