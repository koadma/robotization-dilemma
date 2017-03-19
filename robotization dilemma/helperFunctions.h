#ifndef __HELPERFUNCS_H__
#define __HELPERFUNCS_H__

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <list>
#include <locale>
#include <map>

#include "GL\freeglut.h"
#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif


//Function for solving quadratic equations.
void solve2(float a, float b, float c, float sol[2], int& numOfSol);

//Function for clearing the terminal.
void terminalClear();

//Function for waiting for enter.
void waitForEnter();

//Function for tokenizing a string to vector of string.
std::vector<std::string> tokenize(std::string str);

//Function for getting the signum of a type.
template <typename T>
int sgn(T val)
{
  return (T(0) < val) - (val < T(0));
}

//Function for writing out a vector for debug purposes.
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
  for(int i=0; i<vec.size(); i++)
  {
    os << vec[i] << ((i==vec.size()-1)?"":" ");
  }
  return os;
}

//Function for converting strings to types.
template <typename T>
T strTo(std::string str)
{
  std::stringstream ss(str);
  T n;
  ss >> n;
  return n;
}

#endif
