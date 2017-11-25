#pragma once

#include "constants.h"

#include <algorithm>
#include <cmath>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>
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
#include <queue>
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

template <typename T>
class Zero {
private:
  T val;
public:
  Zero();
  T ret();
};

extern Zero<int>        intZero;
extern Zero<float>    floatZero;
extern Zero<double>  doubleZero;
extern Zero<string>  stringZero;

///####################################################///
//         DO NOT TOUCH THE TEXTBIND CLASSES!!!         //
///####################################################///
namespace TextBindHelper
{
  template <int... Is>
  struct index {};

  template <int N, int... Is>
  struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

  template <int... Is>
  struct gen_seq<0, Is...> : index<Is...> {};
}
template<typename T>
class TextBindBase {
public:
  virtual const T get() {
    throw 1;
    return T();
  }
};
template<typename T>
class TextBindVar : public TextBindBase<T> {
public:
  T* _val;
  TextBindVar<T>() {

  }
  TextBindVar<T>(T* val) {
    _val = val;
  }
  const T get() {
    return *_val;
  }
};
template<typename T, typename... Ts>
class TextBindFunc : public TextBindBase<T> {
public:
  std::function<T(Ts...)> _func;
  std::tuple<Ts...> _args;
  template <typename F, typename... Args>
  TextBindFunc<T, Ts...>(F&& func, Args&&... args)
    : _func(std::forward<F>(func)),
    _args(std::forward<Args>(args)...)
  {}
  template <typename... Args, int... Is>
  T func(std::tuple<Args...>& tup, TextBindHelper::index<Is...>)
  {
    return _func(std::get<Is>(tup)...);
  }
  template <typename... Args>
  T func(std::tuple<Args...>& tup)
  {
    return func(tup, TextBindHelper::gen_seq<sizeof...(Args)>{});
  }
  const T get() {
    return func(_args);
  }
};
class TextBindCore {
public:
  virtual string tostr()
  {
    throw 1;
    return "ERROR!";
  }
};
template <typename... Ts>
class TextBind : public TextBindCore {
private:
  std::string str;
  std::tuple<Ts...> args;
public:
  TextBind(string s, Ts&&... arg)
    : args(std::forward<Ts>(arg)...)
  {
    str = s;
  }
  void SetArgs(Ts&&... arg) {
    args(std::forward<Ts>(arg)...);
  }
  void SetString(string s) {
    str = s;
  }
  template<class F, class...Ts, std::size_t...Is>
  void for_each_in_tuple(string& str, int& n, stringstream& ss, std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>) {
    using expander = int[];
    (void)expander {
      0, ((void)func(str, n, ss, std::get<Is>(tuple)), 0)...
    };
  }
  template<class F, class...Ts>
  void for_each_in_tuple(string& str, int& n, stringstream& ss, std::tuple<Ts...> & tuple, F func) {
    for_each_in_tuple(str, n, ss, tuple, func, std::make_index_sequence<sizeof...(Ts)>());
  }
  string tostr()
  {
    stringstream ss;
    int n = 0;
    for_each_in_tuple(str, n, ss, args,
      [](string& str, int& n, stringstream& ss, auto &x) {
      bool b = true;
      while (b) {
        switch (str[n]) {
        case '\\':
          if (n + 1 < str.size()) {
            if (str[n + 1] == '%') { ss << "%"; }
            else if (str[n + 1] == '\\') { ss << "\\"; }
            else { ss << "\\" << str[n + 1]; }
          }
          n += 2; break;
        case '%':
          b = false; n += 1; break;
        default:
          ss << str[n]; n += 1; break;
        }
      }
      ss << x.get();
    });
    while (n < str.size()) {
      switch (str[n]) {
      case '\\':
        if (n + 1 < str.size()) {
          if (str[n + 1] == '%') { ss << "%"; }
          else if (str[n + 1] == '\\') { ss << "\\"; }
          else { ss << "\\" << str[n + 1]; }
        }
        n += 2; break;
      case '%':
        throw 1; n += 1; break;
      default:
        ss << str[n]; n += 1; break;
      }
    }
    return ss.str();
  }
};

///####################################################///
//         DO NOT TOUCH THE TEXTBIND CLASSES!!!         //
///####################################################///
/* Class how to use:
 1) Create a TextBind object specifying the template string, and the parameters
 2) Call the .tostr() function to substitute the current value
Example:
TextBind<
  TextBindVar<float>,                                                             /// Substitute a variable
  TextBindFunc<float, TStruct*, float*>,                                          /// Member function in TStruct class
  TextBindFunc<string, string, int, int>                                          /// Global function
> b("% asd \\% wa \\ nsd % %\\",                                                  /// Template string (% = to substitute, \% = print % character)
  TextBindVar<float>(&n),                                                         /// Substitute variable "n"
  TextBindFunc<float, TStruct*, float*>(&(TStruct::asd), &t, &x),                 /// Substitute return value of "TSruct::asd" function, called for "TStruct" object "t" with argument pointer to "x"
  TextBindFunc<string, string, int, int>(testFunc, "Wubba lubba dub dub", 6, 5)   /// Substitute return value of "testFunc" with arguments ""Wubba lubba dub dub", 6, 5"
);*/
///####################################################///
//         DO NOT TOUCH THE TEXTBIND CLASSES!!!         //
///####################################################///

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

string randomHexString(size_t length);

double ran1(long int nseed = 0);

uint64_t mix(uint64_t a, uint64_t b);
uint32_t low(uint64_t a);
uint32_t high(uint64_t a);

extern mutex netlock; //Locking for the network thread(s)