#include "WinManager.h"
#include <algorithm>

using namespace std;
/*
WinManager::WinManager(unsigned int numOfPlayers) :
  numOfPlayers(numOfPlayers),
  canWin(numOfPlayers, true),
  isGameOver(false),
  hasWin(false)
  {
    if (numOfPlayers < 2)
    {
      throw TOO_FEW_PLAYERS;
    }
  }

void WinManager::lose(unsigned int player)
{
  if (player >= numOfPlayers)
  {
    throw NOT_EXISTING_PLAYER;
  } 
  canWin[player] = false;
  unsigned int numOfPosWinrs = count(canWin.begin(), canWin.end(), true);
  if (numOfPosWinrs == 0)
  {
    hasWin = false;
    isGameOver = true;
  } else if (numOfPosWinrs == 1)
  {
    hasWin = true;
    winner = distance( canWin.begin(), find(canWin.begin(), canWin.end(), true) );
    isGameOver = true;
  }
}

void WinManager::win(unsigned int player)
{
  if (isGameOver)
  {
    throw GAME_ALREADY_OVER;
  }
  if (player >= numOfPlayers)
  {
    throw NOT_EXISTING_PLAYER;
  }
  if (not canWin[player])
  {
    throw ALREADY_LOST;
  }
  isGameOver = true;
  hasWin = true;
  winner = player;
}

bool WinManager::isOver() const
{
  return isGameOver;
}

bool WinManager::hasWinner() const
{
  if (not isGameOver)
  {
    throw GAME_NOT_OVER;
  }
  return hasWin;
}

unsigned int WinManager::getWinner() const
{
  if (not isGameOver)
  {
    throw GAME_NOT_OVER;
  }
  if (not hasWin)
  {
    throw NO_WINNER;
  }
  return winner;
}
*/