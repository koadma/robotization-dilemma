#ifndef __HELPERFUNCS_H__
#define __HELPERFUNCS_H__

#include "..\Core\constants.h"

#include <algorithm>
#include <cmath>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
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

//----------------random number generator from numerical recipies ran1()--------
//returns a number flatly distributed between 0 and 1
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif

using namespace std;

///##################################################///
//         DO NOT TOUCH THE TEXTBIND CLASS!!!         //
///##################################################///
template <typename... Ts>
class TextBind
{
private:
  std::string str;
  std::tuple<Ts...> args;
public:
  template <typename... Args>
  TextBind(string s, Args&&... arg)
    : args(std::forward<Args>(arg)...)
  {
    str = s;
  }
  template <typename... Args>
  void SetArgs(Args&&... arg) {
    args(std::forward<Args>(arg)...);
  }
  ///##################################################///
  //         DO NOT TOUCH THE TEXTBIND CLASS!!!         //
  ///##################################################///
  void SetString(string s) {
    str = s;
  }
  template<class F, class...Ts, std::size_t...Is>
  void for_each_in_tuple(string& str, int& n, stringstream& ss, const std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>) {
    using expander = int[];
    (void)expander {
      0, ((void)func(str, n, ss, std::get<Is>(tuple)), 0)...
    };
  }
  template<class F, class...Ts>
  void for_each_in_tuple(string& str, int& n, stringstream& ss, const std::tuple<Ts...> & tuple, F func) {
    for_each_in_tuple(str, n, ss, tuple, func, std::make_index_sequence<sizeof...(Ts)>());
  }
  ///##################################################///
  //         DO NOT TOUCH THE TEXTBIND CLASS!!!         //
  ///##################################################///
  string TextBind<Ts...>::tostr()
  {
    stringstream ss;
    int n = 0;
    for_each_in_tuple(str, n, ss, args,
      [](string& str, int& n, stringstream& ss, const auto &x) {
      bool b = true;
      while (b) {
        switch (str[n]) {
        case '\\':
          if (str[n + 1] == '%') {
            ss << "%";
          }
          else {
            ss << "\\" << str[n + 1];
          }
          n += 2;
          break;
        case '%':
          b = false;
          n += 1;
          break;
        default:
          ss << str[n];
          n += 1;
          break;
        }
      }
      ss << *x;
    }
    );
    ///##################################################///
    //         DO NOT TOUCH THE TEXTBIND CLASS!!!         //
    ///##################################################///
    while (n < str.size()) {
      switch (str[n]) {
      case '\\':
        if (n < str.size() && str[n + 1] == '%') {
          ss << "%";
        }
        else {
          ss << "\\" << str[n + 1];
        }
        n += 2;
        break;
      case '%':
        throw 1;
        n += 1;
        break;
      default:
        ss << str[n];
        n += 1;
        break;
      }
    }

    return ss.str();
  }
};
///##################################################///
//         DO NOT TOUCH THE TEXTBIND CLASS!!!         //
///##################################################///


//Function for solving quadratic equations.
void solve2(float a, float b, float c, float sol[2], int& numOfSol);

//Function for clearing the terminal.
void terminalClear();

//Function for waiting for enter.
void waitForEnter();

//_Val: value, _Dig: Number of digits
template <typename T>
string to_string(T _Val, int _Dig) {
  stringstream ss;
  ss << fixed << std::setprecision(_Dig) << _Val << scientific;
  return ss.str();
}

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

void serialize(string s, unsigned char** data, int& dataLen);
template <typename T>
void serialize(T v, unsigned char** data, int& dataLen, int prec = 3) {
  serialize(to_string(v, prec), data, dataLen);
}

string deserializes(unsigned char* data, int dataLen);
int    deserializei(unsigned char* data, int dataLen);
float  deserializef(unsigned char* data, int dataLen);
double deserialized(unsigned char* data, int dataLen);
template <typename T>
T deserializeT(unsigned char* data, int dataLen) {
  return strTo<T>(deserializes(data, dataLen));
}

double ran1(long int nseed = 0);

uint64_t mix(uint32_t a, uint32_t b);
uint32_t low(uint64_t a);
uint32_t high(uint64_t a);

#endif
