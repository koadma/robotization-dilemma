#include "GameCanvas.h"

void mainMenuStoryModePlayButton(string sender) {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.xml");
}
void mainMenuFFAPlayButton(string sender) {
  Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.xml");
}
void mainMenuSettingsButton(string sender) {
  Graphics::setElements(objectMenuSubWindow, "html/game_settings.xml");
  Graphics::TableHwnd table = (Graphics::TableHwnd)Graphics::getElementById("objectSettingsMenuTable");
  createSettings(table);
}
void mainMenuExitButton(string sender) {
  glutExit();
}

//Main / Story
void storyMenuNewButton(string sender) {
	Graphics::setElements(objectMenuSubWindow, "html/newsingle_menu.xml");
}
void storyMenuLoadButton(string sender) {
  createMainMenu();
}
void storyMenuBackButton(string sender) {
  createMainMenu();
}

//Main / Story / New
void newSingleMenu3PlayerButton(string sender) {
  createMainMenu();
}
void newSingleMenu4PlayerButton(string sender) {
  createMainMenu();
}
void newSingleMenu5PlayerButton(string sender) {
  createMainMenu();
}
void newSingleMenuBackButton(string sender) {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.xml");
}

//Main / FFA
void ffaMenuNewGameButton(string sender) {
  Graphics::setElements(objectMenuSubWindow, "html/new_menu.xml");
}
void ffaMenuJoinGameButton(string sender) {
  Graphics::setElements(objectMenuSubWindow, "html/join_menu.xml");
}
void ffaMenuBackButton(string sender) {
  createMainMenu();
}

//Main / FFA / Join
void joinMenuInput(string inp) {

}
void joinMenuInputButton(string sender) {

  string ip = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuIpInput"))->text;
  string port = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuPortInput"))->text;

  connectServer(ip, port, "");
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

    if(code != "") {
      DataElement* codee = new DataElement();
      codee->_core->fromType<string>(code);
      data->addChild(codee);
    }

    Connection->SendData(data, PacketLogin);
  }
}
void joinMenuBackButton(string sender) {
	Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.xml");
}

//Main / FFA / New
void newMenuPlayerSliderInput(float val) {

}
void newMenuInputButton(string sender) {
  int players = (int)round(reinterpret_cast<Graphics::SliderHwnd>(Graphics::getElementById("objectNewMenuPlayerCount"))->val);

  startServer(players);
}
void startServer(int players) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));


  if (!CreateProcess(TEXT("./Server.exe"),   // No module name (use command line)
    (LPWSTR)(("-n " + to_string(players)).c_str()),        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi)           // Pointer to PROCESS_INFORMATION structure
  ) {
    printf("Creating integrated server failed with code %d\n", GetLastError());
    return;
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}
void newMenuBackButton(string sender) {
  Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.xml");
}

//Main / Settings
void settingsMenuInput(key nkey, int id) {
  keybindReply(nkey, id);
}
void settingsMenuBackButton(string sender) {
  createMainMenu();
  saveKeybinds();
}

//Ingame / GUI
void ingameMenuCommitButton(string sender) {
  reinterpret_cast<Graphics::ButtonHwnd>(Graphics::getElementById("objectMainGameCommitButton"))->text = "Not your turn";
  ship->commit();
}
void ingameTimeSliderInput(float val) {
  timeNow = val;
}
void ingameMenuEnergyButton(string sender) {
  if (selectedo != NULL) {
    Graphics::WinHwnd win = Graphics::CreateMainWindow("Energy info: " + selectedo->name());
    Graphics::PlotHwnd plt = Graphics::createPlot("plot", fullContainer, hexToInt("ff000000"), hexToInt("ffffffff"), hexToInt("ff00ff00"));
    Graphics::addElement(win, plt);
    plt->plotData.push_back(new PlotLineVUT<value<energy_type_J>, energy_type_J>(&(selectedo->_maxStorage), hexToInt("ffff0000"), "max storage"));
    plt->plotData.push_back(new PlotLineVUT<linear<energy_type_J, power_type_W, time_type_s>, energy_type_J, time_type_s>(&(selectedo->_energySystem->_val), hexToInt("ffffff00"), "storage"));
  }
}

//Ingame / Ship
void ingameMenuPowerButton(string sender) {
  if (selectedo != NULL) {
    Graphics::WinHwnd win = Graphics::CreateMainWindow("Power info: " + selectedo->name());
    Graphics::PlotHwnd plt = Graphics::createPlot("plot", fullContainer, hexToInt("ff000000"), hexToInt("ffffffff"), hexToInt("ff00ff00"));
    Graphics::addElement(win, plt);
    plt->plotData.push_back(new PlotLineVUT<value<power_type_W>, power_type_W>(&(selectedo->_maxGeneratedPower), hexToInt("ffff0000"), "max generated power"));
    plt->plotData.push_back(new PlotLineVUT<value<power_type_W>, power_type_W>(&(selectedo->_generatedPower), hexToInt("ffffff00"), "generated power"));
    plt->plotData.push_back(new PlotLineVUT<value<power_type_W>, power_type_W>(&(selectedo->_maxUseablePower), hexToInt("ff0000ff"), "max useable power"));
    plt->plotData.push_back(new PlotLineVUT<value<power_type_W>, power_type_W>(&(selectedo->_requestedPower), hexToInt("ffff00ff"), "requested power"));
    plt->plotData.push_back(new PlotLineVUT<value<power_type_W>, power_type_W>(&(selectedo->_usedPower), hexToInt("ff00ffff"), "used power"));
  }
}
void ingameMenuExitButton(string sender) {
  Graphics::deleteElements(objectGameSubWindow);
  Graphics::deleteElements(objectMenuSubWindow);

  createMainMenu();
}

//Ingame / Sensor
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
  reinterpret_cast<Sensor*>(selectedo)->setAutofire(checked, timeNow);
}

//Ingame / Engine
void ingameEngineSidebarAccInputX(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 0, strTo<acc_type_mperss>(s));
}
void ingameEngineSidebarAccInputY(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 1, strTo<acc_type_mperss>(s));
}
void ingameEngineSidebarAccInputZ(string s) {
  reinterpret_cast<Engine*>(selectedo)->setComponent(timeNow, 2, strTo<acc_type_mperss>(s));
}

//Ingame / Laser
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
void ingameLaserSidebarShoot() {
  reinterpret_cast<Laser*>(selectedo)->shoot(timeNow);
}

float getCurrentZoom() {
  return getValPerTick(MainGameCanvas::d, 0.1, 0.2);
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

float getCurrent(bool vel, int index) {
  if(ship->mov.size()) {
    if (vel) {
      return to_doubleT<float, vel_type_mpers>(ship->mov.getAt(timeNow).vel[index]);
    }
    else {
      return to_doubleT<float, distance_type_m>(ship->mov.getAt(timeNow).pos[index]);
    }
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
    bool b = surefire(ship->mov, selecteds->_keyframes, timeNow, ((Laser*)selectedo)->_relativePos, sdir);
    //if (b) {
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputX"))->text = to_string(sdir.x);
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputY"))->text = to_string(sdir.y);
      reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectLaserSidebarDirInputZ"))->text = to_string(sdir.z);
      reinterpret_cast<Laser*>(selectedo)->setDir(sdir);
      return "Sure!";
    //}
    return "Uncertain";
  }
  else {
    return "No sighting selected";
  }
}

void bindLabels() {
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectMainGameCanvasScale"))->text =
    new TextBind<
    TextBindFunc<float>
    >("1 : %m",
      TextBindFunc<float>(getCurrentZoom)
      );
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectMainGameShipLocation"))->text =
    new TextBind<
    TextBindFunc<float, bool, int>,
    TextBindFunc<float, bool, int>,
    TextBindFunc<float, bool, int>
    >("Pos X: %, Y: %, Z: %",
      TextBindFunc<float, bool, int>(getCurrent, false, 0),
      TextBindFunc<float, bool, int>(getCurrent, false, 1),
      TextBindFunc<float, bool, int>(getCurrent, false, 2)
      );
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectMainGameShipVelocity"))->text =
    new TextBind<
    TextBindFunc<float, bool, int>,
    TextBindFunc<float, bool, int>,
    TextBindFunc<float, bool, int>
    >("Vel X: %, Y: %, Z: %",
      TextBindFunc<float, bool, int>(getCurrent, true, 0),
      TextBindFunc<float, bool, int>(getCurrent, true, 1),
      TextBindFunc<float, bool, int>(getCurrent, true, 2)
      );
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
  //Main
  Graphics::setName("mainMenuStoryModePlayButton", mainMenuStoryModePlayButton);
  Graphics::setName("mainMenuFFAPlayButton", mainMenuFFAPlayButton);
  Graphics::setName("mainMenuSettingsButton", mainMenuSettingsButton);
  Graphics::setName("mainMenuExitButton", mainMenuExitButton);

  //Main / Story
  Graphics::setName("storyMenuNewButton", storyMenuNewButton);
  Graphics::setName("storyMenuLoadButton", storyMenuLoadButton);
  Graphics::setName("storyMenuBackButton", storyMenuBackButton);
  
  //Main / Story / New
  Graphics::setName("newSingleMenu3PlayerButton", newSingleMenu3PlayerButton);
  Graphics::setName("newSingleMenu4PlayerButton", newSingleMenu4PlayerButton);
  Graphics::setName("newSingleMenu5PlayerButton", newSingleMenu5PlayerButton);
  Graphics::setName("newSingleMenuBackButton", newSingleMenuBackButton);

  //Main / Ffa
  Graphics::setName("ffaMenuNewGameButton", ffaMenuNewGameButton);
  Graphics::setName("ffaMenuJoinGameButton", ffaMenuJoinGameButton);
  Graphics::setName("ffaMenuBackButton", ffaMenuBackButton);

  //Main / Ffa / New
  Graphics::setName("newMenuPlayerSliderInput", newMenuPlayerSliderInput);
  Graphics::setName("newMenuInputButton", newMenuInputButton);
  Graphics::setName("newMenuBackButton", newMenuBackButton);

  //Main / Ffa / Join
  Graphics::setName("joinMenuInputButton", joinMenuInputButton);
  Graphics::setName("joinMenuInput", joinMenuInput);
  Graphics::setName("joinMenuBackButton", joinMenuBackButton);
  
  //Main / Settings
  Graphics::setName("settingsMenuBackButton", settingsMenuBackButton);

  //Ingame / GUI
  Graphics::setName("ingameMenuExitButton", ingameMenuExitButton);
  Graphics::setName("ingameMenuCommitButton", ingameMenuCommitButton);
  Graphics::setName("ingameTimeSliderInput", ingameTimeSliderInput);

  //Ingame / Ship
  Graphics::setName("ingameMenuEnergyButton", ingameMenuEnergyButton);
  Graphics::setName("ingameMenuPowerButton", ingameMenuPowerButton);

  //Ingame / Sensor
  Graphics::setName("ingameSensorSidebarPowerInput", ingameSensorSidebarPowerInput);
  Graphics::setName("ingameSensorSidebarEnergyInput", ingameSensorSidebarEnergyInput);
  Graphics::setName("ingameSensorSidebarPing", ingameSensorSidebarPing);
  Graphics::setName("ingameSensorSidebarAutofire", ingameSensorSidebarAutofire);

  //Ingame / Engine
  Graphics::setName("ingameEngineSidebarAccInputX", ingameEngineSidebarAccInputX);
  Graphics::setName("ingameEngineSidebarAccInputY", ingameEngineSidebarAccInputY);
  Graphics::setName("ingameEngineSidebarAccInputZ", ingameEngineSidebarAccInputZ);

  //Ingame / Laser
  Graphics::setName("ingameLaserSidebarEnergyInput", ingameLaserSidebarEnergyInput);
  Graphics::setName("ingameLaserSidebarDirInputX", ingameLaserSidebarDirInputX);
  Graphics::setName("ingameLaserSidebarDirInputY", ingameLaserSidebarDirInputY);
  Graphics::setName("ingameLaserSidebarDirInputZ", ingameLaserSidebarDirInputZ);
  Graphics::setName("ingameLaserSidebarShoot", ingameLaserSidebarShoot);
  
  //Validators
  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);
  Graphics::setName("floatValidator", floatValidator);

  int argc = 0;
  char **argv = new char*[0];
  glutInit(&argc, argv);

  //Subwindows
  objectMainWindow = Graphics::CreateMainWindow("Game", Graphics::defaultWindowManagers, 640, 480, true);
  objectMenuSubWindow = Graphics::createPanel("objectMenuSubWindow", fullContainer, getColor("div", "bgcolor"));
  Graphics::addElement(objectMainWindow, objectMenuSubWindow);
  objectGameSubWindow = Graphics::createPanel("objectGameSubWindow", fullContainer, getColor("div", "bgcolor"));
  Graphics::addElement(objectMainWindow, objectGameSubWindow);
  return 0;
}