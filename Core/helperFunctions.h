#ifndef __HELPERFUNCS_H__
#define __HELPERFUNCS_H__

#include "..\Core\constants.h"

#include <algorithm>
#include <cmath>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <locale>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>




#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif

using namespace std;

/*#if !defined(__cplusplus) || defined(_MSC_EXTENSIONS)
#define and	&&
#define and_eq	&=
#define bitand	&
#define bitor	|
#define compl	~
#define not	!
#define not_eq	!=
#define or		||
#define or_eq	|=
#define xor	^
#define xor_eq	^=
#endif /* !defined(__cplusplus) || defined(_MSC_EXTENSIONS) */

//Function for solving quadratic equations.
void solve2(float a, float b, float c, float sol[2], int& numOfSol);

//Function for clearing the terminal.
void terminalClear();

//Function for waiting for enter.
void waitForEnter();

//Function for tokenizing a string to vector of string.
std::vector<std::string> tokenize(std::string str, char split = ' ');

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
  for (int i = 0; i<vec.size(); i++)
  {
    os << vec[i] << ((i == vec.size() - 1) ? "" : " ");
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

int hexToInt(string& str, int s);

int hexToInt(string str);

vector<double> solveQuadratic(double &a, double &b, double &c);
//----------------------------------------------------------------------------
vector<double> solveCubic(double &a, double &b, double &c, double &d);
//----------------------------------------------------------------------------
vector<double> solveQuartic(double &a, double &b, double &c, double &d, double &e);

#endif
