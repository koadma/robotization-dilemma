#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Generator::getVStatus(DataElement* data) {
  DataElement* maxe = new DataElement();
  _maxPower.get(maxe);
  data->addChild(maxe);

  DataElement* store = new DataElement();
  _energyStored.get(store);
  data->addChild(store);

  DataElement* mstoe = new DataElement();
  _maxStorage.get(mstoe);
  data->addChild(mstoe);
}
void Generator::setVStatus(DataElement* data) {
  _maxPower.set(data->_children[0]);
  _energyStored.set(data->_children[1]);
  _maxStorage.set(data->_children[2]);
}
void Generator::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  ThermalRadiation* ev = new ThermalRadiation();
  ev->_o = this;
  addTo.push_back(ev);
}


#ifdef M_CLIENT

void Generator::setSidebar() {
  setSidebarElement("html/generator_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectGeneratorSidebarMaxEnergyLabel"))->text =
    new TextBind<
    TextBindFunc<power_type_W>
    >("Max: %",
      TextBindFunc<power_type_W>(getCurrentMaxPower)
      );
}
#endif

#ifdef M_SERVER

#endif