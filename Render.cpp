#include "Render.h"


void playButton() {
  runGame();
}

void runGame() {
  string inp;
  do
  {
    unsigned int numOfShips;
    cout << "Hajok szama: ";
    cin >> numOfShips;
    Game game(numOfShips);
    cout << "Akarsz uj jatekot kezdeni? (y - igen, barmi mas - nem)" << endl;
    cin >> inp;
  } while (inp == "y");
}


int InitWindow() {
  int mainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  Graphics::createButton(mainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "Play", *playButton);
  //int window3d = Graphics::CreateSubWindow(mainWindow, 0, 0, 800, 600);
  return mainWindow;
}