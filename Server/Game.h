#ifndef __GAME_H__
#define __GAME_H__

#include "WinManager.h"

class Game {
public:
  bool running;
};

/*
class Game
{
private:
  unsigned int numOfShips;
  std::vector<Ship> ships;
  int roundNumber = 0;
  WinManager winManager;
  BubbleManager bubbles;
  std::vector<int> projectiles; 

  void askToContinue(int nextPlayer) const;
  void giveInformation(int currentPlayer) const;
  void mainGameLoop();
  void playRound();
  void manageBubbles();
  void manageProjectiles();
  void moveShips();
  void manageDetections();
  void giveWinScreen();
public:
  Game(unsigned int numOfShips);
  Game();
};
*/
#endif
