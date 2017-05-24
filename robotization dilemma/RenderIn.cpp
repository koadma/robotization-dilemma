#include "RenderIn.h"

void ingameMenuSensorInput(string s) {
  ship->setSensorEnergy(strTo<int>(s));
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
  Graphics::deleteElements(objectMenuSubWindow);

  Connection = new NetworkC(
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuIpInput"))->text,
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuPortInput"))->text,
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
  Graphics::setName("ingameMenuSensorInput", ingameMenuSensorInput);

  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);

  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, "objectMenuSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  objectGameSubWindow = Graphics::createPanel(objectMainWindow, "objectGameSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  createMainMenu();
  return 0;
}