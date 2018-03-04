#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Engine::getVStatus(DataElement* data) {
  DataElement* acre = new DataElement();
  _accelRequest.get(acre);
  data->addChild(acre);

  DataElement* acce = new DataElement();
  _accel.get(acce);
  data->addChild(acce);
}
void Engine::setVStatus(DataElement* data) {
  _accelRequest.set(data->_children[0]);
  _accel.set(data->_children[1]);
}

void Engine::setTargetAccel(time_type_s time, mpssVec3 acc) {
  _accelRequest.addFrame(time, acc);
  requestedPowerChange(time, accel2power(acc));
  #ifdef M_CLIENT
  auto mit = _parentShip->mov._frames.upper_bound(time);
  auto aut = _accelRequest.search(time);
  
  _parentShip->mov._frames.erase(mit, _parentShip->mov._frames.end());
  Movement m = _parentShip->mov.getAt(time);
  m.acc = acc;
  _parentShip->mov.addFrame(time, m);

  while (aut != _accelRequest._frames.end()) {
    Movement m = _parentShip->mov.getAt(aut->first);
    m.acc = aut->second();
    _parentShip->mov.addFrame(aut->first, m);
    
    ++aut;
  }

  #endif
}
void Engine::setComponent(time_type_s time, int c, acc_type_mperss val) {
  mpssVec3 nval = _accelRequest.getAt(time)();
  nval[c] = val;
  setTargetAccel(time, nval);
}

#ifdef M_SERVER
void Engine::setTargetAccel(time_type_s time, mpssVec3 acc, Game* g) {
  setTargetAccel(time, acc);
  _parentShip->energyUpdate(time, game);
}
bool Engine::loadV(xml_node<>* data, time_type_s time) {
  setTargetAccel(time, {0,0,0});
  return true;
}
void Engine::energyCallbackV(time_type_s time, Game* g) {
  power_type_W power = _energySystem->_delta;
  
  if(_accelRequest.size() && _accelRequest.getFirst() <= time) {
    mpssVec3 acc = {0,0,0};
    if(_accelRequest.getAt(time)().length() > 0.00001) {
      acc = _accelRequest.getAt(time)().norm() * power2accel(power);
    }
    _accel.addFrame(time, acc);

    Movement m = _parentShip->mov.getAt(time);
    m.acc = _parentShip->getAccel(time);
    _parentShip->mov.addFrame(time, m);

    ThermalRadiation ev;
    ev._o = this;
    ev._time = time;
    ev.apply(g);
  }
}
#endif

void Engine::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _accelRequest._frames) {
    EngineAcc* ev = new EngineAcc();
    ev->_acc = it.second();
    ev->_o = this;
    ev->_time = it.first;
    addTo.push_back(ev);
  }
}

#ifdef M_CLIENT
void Engine::setSidebar() {
  setSidebarElement("html/engine_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectEngineSidebarEnergyLabel"))->text =
    new TextBind<
    TextBindFunc<power_type_W>,
    TextBindFunc<power_type_W>,
    TextBindFunc<power_type_W>
    >("% / % / %",
      TextBindFunc<power_type_W>(getCurrentUsedPower),
      TextBindFunc<power_type_W>(getCurrentRequestedPower),
      TextBindFunc<power_type_W>(getCurrentMaxUseablePower)
      );
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputX"))->text = to_string(_accelRequest.getAt(timeNow)().x, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputY"))->text = to_string(_accelRequest.getAt(timeNow)().y, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputZ"))->text = to_string(_accelRequest.getAt(timeNow)().z, 2);
}
#endif
