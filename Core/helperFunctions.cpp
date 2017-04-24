#include "helperFunctions.h"
#include <cmath>
#include <cstdlib>

using namespace std;

void solve2(float a, float b, float c, float sol[2], int& numOfSol)
{
  if (a == 0)
  {
    throw 1;
  }
  float disc = pow(b,2)-4*a*c;
  if (disc > 0)
  {
    numOfSol = 2;
    sol[0] = (-b+sqrt(disc))/(2*a);
    sol[1] = (-b-sqrt(disc))/(2*a);
  } else if (disc = 0)
  {
    numOfSol = 1;
    sol[0] = -b/(2*a);
  } else
  {
    numOfSol = 0;
  }
}

void terminalClear()
{
  /*
  //user can scroll back on some terminal
  for (int i=0; i<50; i++)
  {
    cout << '\n';
  }
  */
  #ifdef _WIN32
    system("cls"); //when i cross-compile and run it in wine, this does not work fsr
  #else
    system("clear"); //(on my terminal, i can scroll back)
  #endif
}

void waitForEnter()
{
  #ifdef _WIN32
    //todo: cin.ignore and cin.peek
    string inp;
    getline(cin, inp); //
    getline(cin, inp); //ugly
  #else
    system("read"); //ugly, inelegant, slow, not portable
  #endif
}

vector<string> tokenize(string str, char split)
{
  vector<string> tokens;
  string::iterator begin = str.begin();
  string::iterator it = str.begin();

  while (it != str.end()) {
    if (*it == split) {
      tokens.push_back(string(begin, it));
      begin = it;
    }
    ++it;
  }
  tokens.push_back(string(begin, it));

  return tokens;
}
