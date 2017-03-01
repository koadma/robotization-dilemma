#include "Render.h"

Graphics::WinHwnd objectMainWindow;
Graphics::WinHwnd objectMenuSubWindow;
Graphics::ElemHwnd objectMainMenuPlayButton;
Graphics::ElemHwnd objectPlayMenuNewButton;
Graphics::ElemHwnd objectGameMenuPlayerCountInput;
Graphics::WinHwnd object3DGameWindow;

void newMenuPlayerCountInput(string inp) {
    Graphics::deleteElements(objectMenuSubWindow);
    Game game(atoi(inp.c_str()));
    createMainMenu();
}

void newMenuPlayerCountInputButton() {
  newMenuPlayerCountInput(objectGameMenuPlayerCountInput->text);
}

void gameMenuNewButton() {
  Graphics::deleteElements(objectMenuSubWindow);
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  Graphics::createLabel(objectMenuSubWindow, Coordiante{ 0.3f, 0.5f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.4f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Player Count", 1);
  objectGameMenuPlayerCountInput = Graphics::createTextInput(objectMenuSubWindow, Coordiante{ 0.3f, 0.6f, 5.0f, 5.0f }, Coordiante{ 0.6f, 0.5f, 0.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "0", *newMenuPlayerCountInput, *numericalValidator);
  Graphics::createButton(objectMainWindow, Coordiante{ 0.6f, 0.6f, 0, 5.0f }, Coordiante{ 0.7f, 0.5f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Ok", *newMenuPlayerCountInputButton);
  //runGame();
}

void mainMenuPlayButton() {
  Graphics::deleteElements(objectMainWindow);
  objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, Coordiante{ 0.3f, 0.55f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.45f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "New Game", *gameMenuNewButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void mainMenuExitButton() {
  Graphics::deleteElement(objectMainMenuPlayButton);
  glutExit();
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void createMainMenu() {
  objectMainMenuPlayButton = Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.3f, 0.5f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.4f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Play", *mainMenuPlayButton);
  objectMainMenuPlayButton = Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.3f, 0.6f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.5f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Exit", *mainMenuExitButton);
}

int InitWindow() {
  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = objectMainWindow;//Graphics::CreateSubWindow(objectMainWindow,0,0,800,800);
  createMainMenu();
  return 0;
}