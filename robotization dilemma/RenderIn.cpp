#include "RenderIn.h"

void ingameEngineSidebarAccInputX(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 0,strTo<acc_type_mperss>(s));
}

void ingameEngineSidebarAccInputY(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 1, strTo<acc_type_mperss>(s));
}

void ingameEngineSidebarAccInputZ(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 2, strTo<acc_type_mperss>(s));
}

void ingameLaserSidebarDirInputX(string s) {
  reinterpret_cast<Laser*>(selectedo)->setComponent(0, strTo<float>(s));
}

void ingameLaserSidebarDirInputY(string s) {
  reinterpret_cast<Laser*>(selectedo)->setComponent(1, strTo<float>(s));
}

void ingameLaserSidebarDirInputZ(string s) {
  reinterpret_cast<Laser*>(selectedo)->setComponent(2, strTo<float>(s));
}

void ingameLaserSidebarEnergyInput(string s) {
  reinterpret_cast<Laser*>(selectedo)->setEnergy(strTo<float>(s));
}

void ingameSensorSidebarPowerInput(string s) {
  reinterpret_cast<Sensor*>(selectedo)->setTargetPower(timeNow, strTo<power_type_W>(s));
}

void ingameSensorSidebarEnergyInput(string s) {
  reinterpret_cast<Sensor*>(selectedo)->setTargetEnergy(strTo<energy_type_J>(s));
}

void ingameSensorSidebarPing() {
  reinterpret_cast<Sensor*>(selectedo)->ping(timeNow);
}

void ingameSensorSidebarAutofire(bool checked) {
  reinterpret_cast<Sensor*>(selectedo)->setAutofire(checked);
}

void ingameLaserSidebarShoot() {
  reinterpret_cast<Laser*>(selectedo)->shoot(timeNow);
}

void ingameMenuCommitButton() {
  reinterpret_cast<Graphics::ButtonHwnd>(Graphics::getElementById("objectMainGameCommitButton"))->text = "Not your turn";
  ship->commit();
}

void ingameMenuExitButton() {
  Graphics::deleteElements(objectGameSubWindow);
  Graphics::deleteElements(objectMenuSubWindow);

  createMainMenu();
}

void joinMenuInput(string inp) {

}

void connectServer(string ip, string port, string code) {
  Graphics::deleteElements(objectMenuSubWindow);

  Connection = new NetworkC(
    ip,
    port,
    recivePacket
    );

  if (Connection->error != 0) {
    delete Connection;
    createMainMenu();
  }
  else {
    DataElement* data = new DataElement();

    DataElement* ae = new DataElement();
    ae->_core->fromType<int>(VersionA);
    data->addChild(ae);

    DataElement* be = new DataElement();
    be->_core->fromType<int>(VersionB);
    data->addChild(be);

    DataElement* ce = new DataElement();
    ce->_core->fromType<int>(VersionC);
    data->addChild(ce);

    DataElement* codee = new DataElement();
    codee->_core->fromType<string>(code);
    data->addChild(codee);

    Connection->SendData(data, PacketLogin);
  }
}

void joinMenuInputButton() {

  string ip = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuIpInput"))->text;
  string port = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuPortInput"))->text;

  connectServer(ip, port, "");
}

void gameMenuJoinButton() {
  Graphics::setElements(objectMenuSubWindow, "html/join_menu.xml");
}

void gameMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.xml");
}

void mainMenuStoryModePlayButton() {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.xml");
}

void mainMenuPlayButton() {
  Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.xml");
}

void mainMenuExitButton() {
  glutExit();
}

void storyMenuJoinButton() {
	Graphics::setElements(objectMenuSubWindow, "html/newsingle_menu.xml");
}

void storyMenuLoadButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void storyMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void ffaMenuNewGameButton() {
	Graphics::setElements(objectMenuSubWindow, "html/game_menu.xml");
}

void ffaMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void ffaMenuLoadGameButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void newSingleMenu3PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void newSingleMenu4PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void newSingleMenu5PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
}

void newSingleMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.xml");
}

void joinMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/game_menu.xml");
}

void ingameTimeSliderInput(float val) {
  timeNow = val;
}

void ingameMenuEnergyButton() {
  Graphics::WinHwnd win = Graphics::CreateMainWindow(200, 200, 800, 600, "Energy info: " + selectedo->name());
  Graphics::PlotHwnd plt = Graphics::createPlot(win, "plot", Coordiante{ 0, 0 }, Coordiante{ 1, 1 }, hexToInt("ff000000"), hexToInt("ffffffff"), hexToInt("ff00ff00"));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_maxStorage), hexToInt("ffff0000")));
  plt->plotData.push_back(new PlotLineVUT<linear<energy_type_J, Fraction, time_type_s>, energy_type_J, time_type_s>(&(selectedo->_energySystem->_val), hexToInt("ffffff00")));
}

void ingameMenuPowerButton() {
  Graphics::WinHwnd win = Graphics::CreateMainWindow(200, 200, 800, 600, "Power info: " + selectedo->name());
  Graphics::PlotHwnd plt = Graphics::createPlot(win, "plot", Coordiante{ 0, 0 }, Coordiante{ 1, 1 }, hexToInt("ff000000"), hexToInt("ffffffff"), hexToInt("ff00ff00"));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_maxGeneratedPower), hexToInt("ffff0000")));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_generatedPower), hexToInt("ffffff00")));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_maxUseablePower), hexToInt("ff0000ff")));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_requestedPower), hexToInt("ffff00ff")));
  plt->plotData.push_back(new PlotLineVUT<value<power_type_W>>(&(selectedo->_usedPower), hexToInt("ff00ffff")));
}

string getCurrentName() {
  if (selectedo != NULL) {
    return selectedo->name();
  }
  if (ship != NULL) {
    return "Ship";
  }
  return "";
}

int getCurrentHealth() {
  if (selectedo != NULL) {
    return selectedo->getHealth(timeNow);
  }
  if (ship != NULL) {
    return ship->getHealth(timeNow);
  }
  return 0;
}

int getCurrentMaxHealth() {
  if (selectedo != NULL) {
    return selectedo->getMaxHealth(timeNow);
  }
  if (ship != NULL) {
    return ship->getMaxHealth(timeNow);
  }
  return 0;
}

power_type_W getCurrentMaxGeneratedPower() {
  if (selectedo != NULL) {
    return selectedo->getMaxGeneratedPower(timeNow);
  }
  if (ship != NULL) {
    return ship->getMaxGeneratedPower(timeNow);
  }
  return 0;
}
power_type_W getCurrentGeneratedPower() {
  if (selectedo != NULL) {
    return selectedo->getGeneratedPower(timeNow);
  }
  if (ship != NULL) {
    return ship->getGeneratedPower(timeNow);
  }
  return 0;
}
power_type_W getCurrentMaxUseablePower() {
  if (selectedo != NULL) {
    return selectedo->getMaxUseablePower(timeNow);
  }
  if (ship != NULL) {
    return ship->getMaxUseablePower(timeNow);
  }
  return 0;
}
power_type_W getCurrentRequestedPower() {
  if (selectedo != NULL) {
    return selectedo->getRequestedPower(timeNow);
  }
  if (ship != NULL) {
    return ship->getRequestedPower(timeNow);
  }
  return 0;
}
power_type_W getCurrentUsedPower() {
  if (selectedo != NULL) {
    return selectedo->getUsedPower(timeNow);
  }
  if (ship != NULL) {
    return ship->getUsedPower(timeNow);
  }
  return 0;
}
energy_type_J getCurrentMaxEnergy() {
  if (selectedo != NULL) {
    return selectedo->getMaxEnergy(timeNow);
  }
  if (ship != NULL) {
    return ship->getMaxEnergy(timeNow);
  }
  return 0;
}
energy_type_J getCurrentStoredEnergy() {
  if (selectedo != NULL) {
    return selectedo->getStoredEnergy(timeNow);
  }
  if (ship != NULL) {
    return ship->getStoredEnergy(timeNow);
  }
  return 0;
}

string isSurefire() {
  if (selecteds != NULL && selectedo != NULL && selectedo->type() == Object::Laser) {
    sVec3 sdir;
    bool b = surefire(ship->mov, selecteds->keyframes, timeNow, sdir);
    if (b) {
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputX"))->text = to_string(sdir.x);
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputY"))->text = to_string(sdir.y);
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputZ"))->text = to_string(sdir.z);
      reinterpret_cast<Laser*>(selectedo)->setDir(sdir);
      return "YES";
    }
    return "NO";
  }
  else {
    return "No sighting selected";
  }
}

void bindLabels() {
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectSidebarName"))->text =
    new TextBind<
    TextBindFunc<string>
    >("%",
      TextBindFunc<string>(getCurrentName)
      );

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectSidebarHealth"))->text =
    new TextBind<
    TextBindFunc<int>,
    TextBindFunc<int>
    >("% / %",
      TextBindFunc<int>(getCurrentHealth),
      TextBindFunc<int>(getCurrentMaxHealth)
      );
}

int InitGraphics() {
  Graphics::setName("newSingleMenuBackButton", newSingleMenuBackButton);
  Graphics::setName("joinMenuBackButton", joinMenuBackButton);
  Graphics::setName("newSingleMenu3PlayerButton", newSingleMenu3PlayerButton);
  Graphics::setName("newSingleMenu4PlayerButton", newSingleMenu4PlayerButton);
  Graphics::setName("newSingleMenu5PlayerButton", newSingleMenu5PlayerButton);
  Graphics::setName("ffaMenuNewGameButton", ffaMenuNewGameButton);
  Graphics::setName("ffaMenuBackButton", ffaMenuBackButton);
  Graphics::setName("ffaMenuLoadGameButton", ffaMenuLoadGameButton);
  Graphics::setName("storyMenuJoinButton", storyMenuJoinButton);
  Graphics::setName("storyMenuLoadButton", storyMenuLoadButton);
  Graphics::setName("storyMenuBackButton", storyMenuBackButton);
  Graphics::setName("mainMenuStoryModePlayButton", mainMenuStoryModePlayButton);
  Graphics::setName("mainMenuPlayButton", mainMenuPlayButton);
  Graphics::setName("mainMenuExitButton", mainMenuExitButton);
  Graphics::setName("gameMenuJoinButton", gameMenuJoinButton);
  Graphics::setName("gameMenuBackButton", gameMenuBackButton);
  Graphics::setName("joinMenuInputButton", joinMenuInputButton);
  Graphics::setName("joinMenuInput", joinMenuInput);
  Graphics::setName("ingameMenuExitButton", ingameMenuExitButton);
  Graphics::setName("ingameMenuEnergyButton", ingameMenuEnergyButton);
  Graphics::setName("ingameMenuPowerButton", ingameMenuPowerButton);
  Graphics::setName("ingameMenuCommitButton", ingameMenuCommitButton);
  Graphics::setName("ingameSensorSidebarPowerInput", ingameSensorSidebarPowerInput);
  Graphics::setName("ingameSensorSidebarEnergyInput", ingameSensorSidebarEnergyInput);
  Graphics::setName("ingameSensorSidebarPing", ingameSensorSidebarPing);
  Graphics::setName("ingameSensorSidebarAutofire", ingameSensorSidebarAutofire);
  Graphics::setName("ingameEngineSidebarAccInputX", ingameEngineSidebarAccInputX);
  Graphics::setName("ingameEngineSidebarAccInputY", ingameEngineSidebarAccInputY);
  Graphics::setName("ingameEngineSidebarAccInputZ", ingameEngineSidebarAccInputZ);
  Graphics::setName("ingameLaserSidebarEnergyInput", ingameLaserSidebarEnergyInput);
  Graphics::setName("ingameLaserSidebarDirInputX", ingameLaserSidebarDirInputX);
  Graphics::setName("ingameLaserSidebarDirInputY", ingameLaserSidebarDirInputY);
  Graphics::setName("ingameLaserSidebarDirInputZ", ingameLaserSidebarDirInputZ);
  Graphics::setName("ingameLaserSidebarShoot", ingameLaserSidebarShoot);
  Graphics::setName("ingameTimeSliderInput", ingameTimeSliderInput);

  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);
  Graphics::setName("floatValidator", floatValidator);

  int argc = 0;
  char **argv = new char*[0];
  glutInit(&argc, argv);

  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, "objectMenuSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  objectGameSubWindow = Graphics::createPanel(objectMainWindow, "objectGameSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  return 0;
}