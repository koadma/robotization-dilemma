#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Bubble.h"

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
};

#endif
