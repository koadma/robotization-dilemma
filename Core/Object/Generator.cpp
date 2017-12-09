#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

Generator::Generator(uint64_t ID) : Object(ID) {}
Generator::Generator(mVec3 relativePos, int maxHealth, float radius, int health, float maxPower, uint64_t ID) : Object(relativePos, maxHealth, radius, health, ID) {
  _maxPower.addFrame(0, maxPower);
  _name = "Generator";
  _energyStored.addFrame(0, 0);
  _maxStorage.addFrame(0, 0);
}
int Generator::type() { return Type::Generator; }

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