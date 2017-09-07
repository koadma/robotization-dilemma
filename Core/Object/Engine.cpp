#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Engine::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  maxe->_core->fromType<power_type_W>(_maxPower);
  data->addChild(maxe);

  DataElement* acce = new DataElement();
  _accel.get(acce);
  data->addChild(acce);
}
void Engine::setVStatus(DataElement* data) {
  _maxPower = data->_children[0]->_core->toType<float>();
  _accel.set(data->_children[1]);
}

void Engine::setAccel(time_type_s time, mpssVec3 acc) {
  Movement m = parentShip->mov.getAt(time);
  m.acc = parentShip->getAccel(time);

  _accel.addFrame(time, acc);

  m.acc += acc;
  parentShip->mov.addFrame(time, m);

  parentShip->refreshEnergy(time); //recalculate ship energy info
}
void Engine::setComponent(time_type_s time, int c, acc_type_mperss val) {
  mpssVec3 nval = _accel.getAt(time)();
  nval[c] = val;
  setAccel(time, nval);
}

void Engine::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _accel._frames) {
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
    TextBindFunc<power_type_W>
    >("% / %",
      TextBindFunc<power_type_W>(getCurrentUsedPower),
      TextBindFunc<power_type_W>(getCurrentMaxPower)
      );
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputX"))->text = to_string(_accel.getAt(timeNow)().x, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputY"))->text = to_string(_accel.getAt(timeNow)().y, 2);
  reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectEngineSidebarAccInputZ"))->text = to_string(_accel.getAt(timeNow)().z, 2);
}
#endif

#ifdef M_SERVER

#endif