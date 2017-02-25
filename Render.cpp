#include "Render.h"

int objectMainWindow;
int objectMainMenuPlayButton;
int objectPlayMenuNewButton;

void newMenuPlayerCountInput(string inp) {
    Graphics::deleteElements(objectMainWindow);
    Game game(atoi(inp.c_str()));
    createMainMenu();
}

void gameMenuNewButton() {
  Graphics::deleteElements(objectMainWindow);
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  Graphics::createLabel(objectMainWindow, 10, 70, 100, 50, ElementBaseColor, ElementActiveColor, ElementTextColor, "Player Count", 1);
  Graphics::createTextInput(objectMainWindow, 10, 10, 100, 50, ElementBaseColor, ElementActiveColor, ElementTextColor, "0", *newMenuPlayerCountInput, *numericalValidator);
  //runGame();
}

void mainMenuPlayButton() {
  Graphics::deleteElements(objectMainWindow);
  objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, ElementBaseColor, ElementActiveColor, ElementTextColor, "New Game", *gameMenuNewButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void mainMenuExitButton() {
  Graphics::deleteElement(objectMainWindow, objectMainMenuPlayButton);
  glutExit();
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void createMainMenu() {
  objectMainMenuPlayButton = Graphics::createButton(objectMainWindow, 10, 70, 100, 50, ElementBaseColor, ElementActiveColor, ElementTextColor, "Play", *mainMenuPlayButton);
  objectMainMenuPlayButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, ElementBaseColor, ElementActiveColor, ElementTextColor, "Exit", *mainMenuExitButton);

}

int InitWindow() {
  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  createMainMenu();
  //int window3d = Graphics::CreateSubWindow(mainWindow, 0, 0, 800, 600);
  return objectMainWindow;
}