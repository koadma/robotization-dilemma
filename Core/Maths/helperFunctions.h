#pragma once

#include "constants.h"

#include <algorithm>
#include <cmath>
#include <ciso646> //defines and, or, not for visual studio, does nothing elsewhere.
#include "BigNumberLibrary/Cbignums.h"
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
  const T* _val;
  TextBindVar<T>() {

  }
  TextBindVar<T>(T* val) {
    _val = val;
  }
  TextBindVar<T>(const T* val) {
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

//String replace
void replaceChar(string& str, char ch1, char ch2);

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
  if (_Dig == 0) {
    ss << _Val;
  }
  else {
    ios::fmtflags f(ss.flags());
    ss << fixed << std::setprecision(_Dig) << std::fixed << _Val << scientific;
    ss.flags(f);
  }
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

template <typename T>
vector<T> solveQuadratic(T &a, T &b, T &c)
{
  if (a == 0.0 || abs(a / b) < 1.0e-6)
  {
    if (abs(b) < 1.0e-4)
      return vector<T>{};
    else
    {
      return vector<T>{-c / b};
    }
  }

  double discriminant = b * b - 4.0 * a * c;
  if (discriminant >= 0.0)
  {
    discriminant = sqrt(discriminant);
    return vector<T>{(b + discriminant) * -0.5 / a, (b - discriminant) * -0.5 / a};
  }

  return vector<T>{};
}
//----------------------------------------------------------------------------
template <typename T>
vector<T> solveCubic(T &a, T &b, T &c, T &d)
{
  if (a == 0.0/* || abs(a / b) < 1.0e-6*/)
    return solveQuadratic(b, c, d);

  double B = b / a, C = c / a, D = d / a;

  T Q = (B*B - C*3.0) / 9.0, QQQ = Q*Q*Q;
  T R = (2.0*B*B*B - 9.0*B*C + 27.0*D) / 54.0, RR = R*R;

  // 3 real roots
  if (RR<QQQ)
  {
    /* This sqrt and division is safe, since RR >= 0, so QQQ > RR,    */
    /* so QQQ > 0.  The acos is also safe, since RR/QQQ < 1, and      */
    /* thus R/sqrt(QQQ) < 1.                                     */
    T theta = acos(R / sqrt(QQQ));
    /* This sqrt is safe, since QQQ >= 0, and thus Q >= 0             */
    T r1, r2, r3;
    r1 = r2 = r3 = -2.0*sqrt(Q);
    r1 *= cos(theta / 3.0);
    r2 *= cos((theta + 2 * PI) / 3.0);
    r3 *= cos((theta - 2 * PI) / 3.0);

    r1 -= B / 3.0;
    r2 -= B / 3.0;
    r3 -= B / 3.0;

    return vector<T>{r1, r2, r3};
  }
  // 1 real root
  else
  {
    T A2 = -pow(fabs(R) + sqrt(RR - QQQ), 1.0 / 3.0);

    float r = 0;
    if (A2 != 0.0) {
      if (R<0.0) A2 = -A2;
      r = A2 + Q / A2;
    }
    r -= B / 3.0;
    return vector<T>{r};
  }
}
//----------------------------------------------------------------------------
template <typename T>
vector<T> solveQuartic(T &a, T &b, T &c, T &d, T &e)
{
  // When a or (a and b) are magnitudes of order smaller than C,D,E
  // just ignore them entirely. 
  if (a == 0 || abs(a / b) < 1.0e-6 || abs(a / c) < 1.0e-6)
    return solveCubic(b, c, d, e);

  // Uses Ferrari's Method
  T aa = a*a, aaa = aa*a, bb = b*b, bbb = bb*b;
  T p = -3.0*bb / (8.0*aa) + c / a, pp = p * p;
  T q = bbb / (8.0*aaa) - b*c / (2.0*aa) + d / a;
  T r = -3.0*bbb*b / (256.0*aaa*a) + c*bb / (16.0*aaa) - b*d / (4.0*aa) + e / a;

  if (q == 0.0)
  {
    vector<T> res;
    if (pp - 4.0*r >= 0) {
      if ((-p + sqrt(pp - 4.0*r)) >= 0) {
        res.push_back(b / (-4.0*a) + sqrt(0.5 * (-p + sqrt(pp - 4.0*r))));
        res.push_back(b / (-4.0*a) - sqrt(0.5 * (-p + sqrt(pp - 4.0*r))));
      }
      if ((-p - sqrt(pp - 4.0*r)) >= 0) {
        res.push_back(b / (-4.0*a) + sqrt(0.5 * (-p - sqrt(pp - 4.0*r))));
        res.push_back(b / (-4.0*a) - sqrt(0.5 * (-p - sqrt(pp - 4.0*r))));
      }
    }
    return res;
  }
  else
  {
    vector<T> res;

    //solve 0 = beta^2 - 8z(z^2+alpha z+alpha^2/4-gamma)
    //0 = -beta^2/8 + z^3 + alpha z^2 + z alpha^2/4-z gamma

    T ca = 8;
    T cb = 8 * p;
    T cc = 2 * pp - 8 * r;
    T cd = -q*q;

    vector<T> cres = solveCubic(ca, cb, cc, cd);

    T m = cres[0];
    for (int i = 1; i < cres.size(); i++) {
      m = max(m, cres[i]);
    }

    if (m >= 0) {
      vector<T> tempres;

      T ta = 1;
      T tb = sqrt(2 * m);
      T tc = p / 2.0 + m - q / (2 * sqrt(2 * m));

      tempres = solveQuadratic(ta, tb, tc);

      for (int i = 0; i < tempres.size(); i++) {
        res.push_back(tempres[i] - b / (4.0*a));
      }

      ta = 1;
      tb = -sqrt(2 * m);
      tc = p / 2.0 + m + q / (2 * sqrt(2 * m));

      tempres = solveQuadratic(ta, tb, tc);

      for (int i = 0; i < tempres.size(); i++) {
        res.push_back(tempres[i] - b / (4.0*a));
      }
    }
    else {
      cout << "ERROR" << endl;
    }
    //fine derivative correction
    vector<T> corres;
    for (int i = 0; i < res.size(); i++) {
      T vali = 1;
      T deri = 1;
      int corr = 0;
      do {
        vali = a*res[i] * res[i] * res[i] * res[i] + b*res[i] * res[i] * res[i] + c*res[i] * res[i] + d*res[i] + e;
        deri = 4 * a* res[i] * res[i] * res[i] + 3 * b* res[i] * res[i] + 2 * c* res[i] + d;
        if (fabs(deri) > 1e-3) {
          res[i] -= vali / deri;
        }
        ++corr;
      } while (corr < 50 && fabs(vali / deri) > 0.1);
      if (corr < 50) {
        corres.push_back(res[i]);
      }
      else {
        cout << "ROOT REMOVED";
      }
    }
    return corres;
  }
}

void serialize(string s, unsigned char** data, int& dataLen);
template <typename T>
void serialize(T v, unsigned char** data, int& dataLen, int prec = 3) {
  serialize(to_string(v,prec), data, dataLen);
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

template <typename T>
T modulo(T& lhs, T& rhs) {
  return fmod((fmod(lhs,rhs)+rhs),rhs);
}

uint64_t mix(uint64_t a, uint64_t b);
uint32_t low(uint64_t a);
uint32_t high(uint64_t a);

extern mutex netlock; //Locking for the network thread(s)