#include "helperFunctions.h"
#include <cmath>
#include <cstdlib>

using namespace std;

template <typename T>
Zero<T>::Zero() { val = T(); }
template <typename T>
T Zero<T>::ret() {
  return val;
}

Zero<int>        intZero;
Zero<float>    floatZero;
Zero<double>  doubleZero;
Zero<string>  stringZero;

void solve2(float a, float b, float c, float sol[2], int& numOfSol)
{
  if (a == 0)
  {
    if (b == 0) {
      numOfSol = 0;
    }
    else {
      sol[0] = -c/b;
    }
  }
  float disc = pow(b,2)-4*a*c;
  if (disc > 0)
  {
    numOfSol = 2;
    sol[0] = (-b+sqrt(disc))/(2*a);
    sol[1] = (-b-sqrt(disc))/(2*a);
  } else if (disc == 0)
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

int hexToInt(string& str, int s) {
  if (s >= str.size()) {
    return 0;
  }
  else {
    int cval = 0;
    if ('0' <= str[str.size() - s - 1] && str[str.size() - s - 1] <= '9') {
      cval = str[str.size() - s - 1] - '0';
    }
    if ('A' <= str[str.size() - s - 1] && str[str.size() - s - 1] <= 'F') {
      cval = str[str.size() - s - 1] - 'A' + 10;
    }
    if ('a' <= str[str.size() - s - 1] && str[str.size() - s - 1] <= 'f') {
      cval = str[str.size() - s - 1] - 'a' + 10;
    }
    return 16 * hexToInt(str, s + 1) + cval;
  }
}

int hexToInt(string str) {
  return hexToInt(str, 0);
}

vector<double> solveQuadratic(double &a, double &b, double &c)
{
  if (a == 0.0 || abs(a / b) < 1.0e-6)
  {
    if (abs(b) < 1.0e-4)
      return vector<double>{};
    else
    {
      return vector<double>{-c / b};
    }
  }

  double discriminant = b * b - 4.0 * a * c;
  if (discriminant >= 0.0)
  {
    discriminant = sqrt(discriminant);
    return vector<double>{(b + discriminant) * -0.5 / a, (b - discriminant) * -0.5 / a};
  }

  return vector<double>{};
}
//----------------------------------------------------------------------------
vector<double> solveCubic(double &a, double &b, double &c, double &d)
{
  if (a == 0.0/* || abs(a / b) < 1.0e-6*/)
    return solveQuadratic(b, c, d);

  double B = b / a, C = c / a, D = d / a;

  double Q = (B*B - C*3.0) / 9.0, QQQ = Q*Q*Q;
  double R = (2.0*B*B*B - 9.0*B*C + 27.0*D) / 54.0, RR = R*R;

  // 3 real roots
  if (RR<QQQ)
  {
    /* This sqrt and division is safe, since RR >= 0, so QQQ > RR,    */
    /* so QQQ > 0.  The acos is also safe, since RR/QQQ < 1, and      */
    /* thus R/sqrt(QQQ) < 1.                                     */
    double theta = acos(R / sqrt(QQQ));
    /* This sqrt is safe, since QQQ >= 0, and thus Q >= 0             */
    double r1, r2, r3;
    r1 = r2 = r3 = -2.0*sqrt(Q);
    r1 *= cos(theta / 3.0);
    r2 *= cos((theta + 2 * PI) / 3.0);
    r3 *= cos((theta - 2 * PI) / 3.0);

    r1 -= B / 3.0;
    r2 -= B / 3.0;
    r3 -= B / 3.0;

    return vector<double>{r1, r2, r3};
  }
  // 1 real root
  else
  {
    double A2 = -pow(fabs(R) + sqrt(RR - QQQ), 1.0 / 3.0);

    float r = 0;
    if (A2 != 0.0) {
      if (R<0.0) A2 = -A2;
      r = A2 + Q / A2;
    }
    r -= B / 3.0;
    return vector<double>{r};
  }
}
//----------------------------------------------------------------------------
vector<double> solveQuartic(double &a, double &b, double &c, double &d, double &e)
{
  // When a or (a and b) are magnitudes of order smaller than C,D,E
  // just ignore them entirely. 
  if (a == 0 || abs(a / b) < 1.0e-6 || abs(a / c) < 1.0e-6)
    return solveCubic(b, c, d, e);

  // Uses Ferrari's Method
  double aa = a*a, aaa = aa*a, bb = b*b, bbb = bb*b;
  double p = -3.0*bb / (8.0*aa) + c / a, pp = p * p;
  double q = bbb / (8.0*aaa) - b*c / (2.0*aa) + d / a;
  double r = -3.0*bbb*b / (256.0*aaa*a) + c*bb / (16.0*aaa) - b*d / (4.0*aa) + e / a;

  if (q == 0.0)
  {
    vector<double> res;
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
    vector<double> res;

    //solve 0 = beta^2 - 8z(z^2+alpha z+alpha^2/4-gamma)
    //0 = -beta^2/8 + z^3 + alpha z^2 + z alpha^2/4-z gamma

    double ca = 8;
    double cb = 8 * p;
    double cc = 2 * pp - 8 * r;
    double cd = -q*q;

    vector<double> cres = solveCubic(ca, cb, cc, cd);

    double m = cres[0];
    for (int i = 1; i < cres.size(); i++) {
      m = max(m, cres[i]);
    }

    if (m >= 0) {
      vector<double> tempres;

      double ta = 1;
      double tb = sqrt(2 * m);
      double tc = p / 2.0 + m - q / (2 * sqrt(2 * m));

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
    vector<double> corres;
    for (int i = 0; i < res.size(); i++) {
      double vali = 1;
      double deri = 1;
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

void serialize(string s, unsigned char** data, int& dataLen) {
  *data = new unsigned char[s.size()];
  for (int i = 0; i < s.size(); i++) {
    (*data)[i] = s[i];
  }
  dataLen = s.size();
}

string deserializes(unsigned char* data, int dataLen) {
  string s(dataLen, ' ');
  for (int i = 0; i < s.size(); i++) {
    s[i] = data[i];
  }
  //delete data;
  return s;
}
int    deserializei(unsigned char* data, int dataLen) {
  return strTo<int>(deserializes(data, dataLen));
}
float  deserializef(unsigned char* data, int dataLen) {
  return strTo<float>(deserializes(data, dataLen));
}
double deserialized(unsigned char* data, int dataLen) {
  return strTo<double>(deserializes(data, dataLen));
}

double ran1(long int nseed) {
  //random -seed expected
  int j;
  long k;
  static long iy = 0;
  static long iv[NTAB];
  static long int idum = 0;
  /*long iy = 0;
  long iv[NTAB];*/
  double temp;
  if (nseed != 0) {
    idum = nseed;
  }
  if (idum <= 0 || !iy) {
    if (-idum < 1) {
      idum = 1;
    }
    else {
      idum = -idum;
    }
    for (j = NTAB + 7; j >= 0; j--) {
      k = idum / IQ;
      idum = IA * (idum - k * IQ) - IR * k;
      if (idum < 0) {
        idum += IM;
      }
      if (j < NTAB) {
        iv[j] = idum;
      }
    }
    iy = iv[0];
  }
  k = idum / IQ;
  idum = IA * (idum - k * IQ) - IR * k;
  if (idum < 0) {
    idum += IM;
  }
  j = iy / NDIV;
  iy = iv[j];
  iv[j] = idum;
  if ((temp = AM * iy) > RNMX) {
    return RNMX;
  }
  else {
    return temp;
  }
}

uint64_t mix(uint64_t a, uint64_t b) {
  return (a << 32) + b;
}

uint32_t low(uint64_t a) {
  return a & 0xffffffff;
}

uint32_t high(uint64_t a) {
  return a >> 32;
}

string randomHexString(size_t length) {
  string s(length, '-');
  for (size_t i = 0; i < length; i++) {
    int r = 16 * ran1();
    if (r < 10) {
      s[i] = '0'+r;
    }
    else {
      s[i] = 'A'+r-10;
    }
  }
  return s;
}

mutex netlock; //Locking for the network thread(s)
