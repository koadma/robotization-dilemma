#ifndef __WINMANAGER_H__
#define __WINMANAGER_H__

#include <vector>

/*
Class for managing win conditions for the game.
There can be 0 winners, so before you call getWinner(),
you should call hasWinner() first.
Of course, before you call hasWinner(), you should call isOver() to know
whether the game has ended.
*/

class WinManager
{
public:
  WinManager(unsigned int numOfPlayers);
  void lose(unsigned int player);
  void win(unsigned int player);
  bool isOver() const;
  bool hasWinner() const;
  unsigned int getWinner() const;

private:
  unsigned int numOfPlayers;
  std::vector<bool> canWin;
  bool isGameOver;
  bool hasWin;
  unsigned int winner;

  enum Exception {GAME_ALREADY_OVER, GAME_NOT_OVER, TOO_FEW_PLAYERS, NOT_EXISTING_PLAYER, NO_WINNER, ALREADY_LOST};
};

#endif
