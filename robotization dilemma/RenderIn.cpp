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

void ingameLaserSidebarAccInputX(string s) {
  reinterpret_cast<Laser*>(selected)->setComponent(0, strTo<float>(s));
}

void ingameLaserSidebarAccInputY(string s) {
  reinterpret_cast<Laser*>(selected)->setComponent(1, strTo<float>(s));
}

void ingameLaserSidebarAccInputZ(string s) {
  reinterpret_cast<Laser*>(selected)->setComponent(2, strTo<float>(s));
}

void ingameLaserSidebarEnergyInput(string s) {
  reinterpret_cast<Laser*>(selected)->setEnergy(strTo<float>(s));
}

void ingameSensorSidebarEnergyInput(string s) {
  reinterpret_cast<Sensor*>(selected)->setEnergy(strTo<float>(s));
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

    Connection->SendData(data, PacketLogin);
  }
}

void gameMenuJoinButton() {
  Graphics::setElements(objectMenuSubWindow, "html/join_menu.html");
}

void gameMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.html");
}

void mainMenuStoryModePlayButton() {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.html");
}

void mainMenuPlayButton() {
  Graphics::setElements(objectMenuSubWindow, "html/ffa_menu.html");
}

void mainMenuExitButton() {
  glutExit();
}

void storyMenuJoinButton() {
	Graphics::setElements(objectMenuSubWindow, "html/newsingle_menu.html");
}

void storyMenuLoadButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void storyMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void ffaMenuNewGameButton() {
	Graphics::setElements(objectMenuSubWindow, "html/game_menu.html");
}

void ffaMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void ffaMenuLoadGameButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void newSingleMenu3PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void newSingleMenu4PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void newSingleMenu5PlayerButton() {
	Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
}

void newSingleMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/story_menu.html");
}

void joinMenuBackButton() {
	Graphics::setElements(objectMenuSubWindow, "html/game_menu.html");
}

int InitWindow() {
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
  Graphics::setName("ingameMenuExitButton", ingameMenuExitButton);
  Graphics::setName("joinMenuInput", joinMenuInput);
  Graphics::setName("ingameMenuCommitButton", ingameMenuCommitButton);
  Graphics::setName("ingameSensorSidebarEnergyInput", ingameSensorSidebarEnergyInput);
  Graphics::setName("ingameEngineSidebarAccInputX", ingameEngineSidebarAccInputX);
  Graphics::setName("ingameEngineSidebarAccInputY", ingameEngineSidebarAccInputY);
  Graphics::setName("ingameEngineSidebarAccInputZ", ingameEngineSidebarAccInputZ);
  Graphics::setName("ingameLaserSidebarEnergyInput", ingameLaserSidebarEnergyInput);
  Graphics::setName("ingameLaserSidebarAccInputX", ingameLaserSidebarAccInputX);
  Graphics::setName("ingameLaserSidebarAccInputY", ingameLaserSidebarAccInputY);
  Graphics::setName("ingameLaserSidebarAccInputZ", ingameLaserSidebarAccInputZ);

  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);
  Graphics::setName("floatValidator", floatValidator);

  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, "objectMenuSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  objectGameSubWindow = Graphics::createPanel(objectMainWindow, "objectGameSubWindow", Coordiante{ 0.0f, 0.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 1.0f, 0.0f, 0.0f }, ElementBackColor);
  createMainMenu();
  return 0;
}