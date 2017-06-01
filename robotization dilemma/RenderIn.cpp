#include "RenderIn.h"

void ingameEngineSidebarAccInputX(string s) {
  reinterpret_cast<Engine*>(selected)->setComponent(0,strTo<float>(s));
}

void ingameEngineSidebarAccInputY(string s) {
  reinterpret_cast<Engine*>(selected)->setComponent(1, strTo<float>(s));
}

void ingameEngineSidebarAccInputZ(string s) {
  reinterpret_cast<Engine*>(selected)->setComponent(2, strTo<float>(s));
}

void ingameSensorSidebarEnergyInput(string s) {
  reinterpret_cast<Sensor*>(selected)->setEnergy(strTo<float>(s));
}

void ingameMenuCommitButton() {
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectMainGameTurnLabel"))->text = "Not your turn";
  ship->commit();
}

void ingameMenuExitButton() {
  Graphics::deleteElements(objectGameSubWindow);
  Graphics::deleteElements(objectMenuSubWindow);

  createMainMenu();
}

void joinMenuInput(string inp) {

}

void joinMenuInputButton() {

  string ip = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuIpInput"))->text;
  string port = reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuPortInput"))->text;

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
    unsigned char* c;
    int l;
    vector<pair<unsigned char*,int>> d;
    
    serialize(VersionA, &c, l);
    d.push_back({c, l});
    serialize(VersionB, &c, l);
    d.push_back({ c, l });
    serialize(VersionC, &c, l);
    d.push_back({ c, l });
    concat(d, &c, l);
    Connection->SendData(c, PacketLogin, l);
  }
}

void gameMenuJoinButton() {
  Graphics::setElements(objectMenuSubWindow, "html/join_menu.html");
}

void mainMenuPlayButton() {
  Graphics::setElements(objectMenuSubWindow, "html/game_menu.html");
}

void mainMenuExitButton() {
  glutExit();
}

int InitWindow() {
  Graphics::setName("mainMenuPlayButton", mainMenuPlayButton);
  Graphics::setName("mainMenuExitButton", mainMenuExitButton);
  Graphics::setName("gameMenuJoinButton", gameMenuJoinButton);
  Graphics::setName("joinMenuInputButton", joinMenuInputButton);
  Graphics::setName("ingameMenuExitButton", ingameMenuExitButton);
  Graphics::setName("joinMenuInput", joinMenuInput);
  Graphics::setName("ingameMenuCommitButton", ingameMenuCommitButton);
  Graphics::setName("ingameSensorSidebarEnergyInput", ingameSensorSidebarEnergyInput);
  Graphics::setName("ingameEngineSidebarAccInputX", ingameEngineSidebarAccInputX);
  Graphics::setName("ingameEngineSidebarAccInputY", ingameEngineSidebarAccInputY);
  Graphics::setName("ingameEngineSidebarAccInputZ", ingameEngineSidebarAccInputZ);


  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);
  Graphics::setName("floatValidator", floatValidator);

  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, "objectMenuSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  objectGameSubWindow = Graphics::createPanel(objectMainWindow, "objectGameSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  createMainMenu();
  return 0;
}