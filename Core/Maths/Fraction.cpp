#include "Fraction.h"

Fraction::Fraction() {
  a = 0;
  b = 1;
}

Fraction::Fraction(cBigNumber a) {
  this->a = a;
  this->b = 1;
}

Fraction::Fraction(int a) {
  this->a = a;
  this->b = 1;
}

Fraction::Fraction(cBigNumber a, cBigNumber b) {
  this->a = a;
  if (!b) {

    b = 1;
    return;
  }
  this->b = b;
}

Fraction::Fraction(double d) {
  *this = approx(d, 10);
}

Fraction::Fraction(string s) {
  string spl[2];
  int p = s.find('/');
  if (0 <= p && p < s.size() - 1) {
    spl[0] = s.substr(0, p);
    spl[1] = s.substr(p + 1, s.size() - p + 1);
    a = atoi(spl[0].c_str());
    b = atoi(spl[1].c_str());
    if (!b) {
      b = 1;
      return;
    }
  }
  else {
    *this = approx(s);
  }

  this->simplify();
}

cBigNumber Fraction::gcd(cBigNumber x, cBigNumber y) {
  if (y == 0)
    return x;
  else
    return (gcd(y, x%y));
}

Fraction& Fraction::simplify() {
  cBigNumber d = gcd(a, b);

  a /= d;
  b /= d;

  if (b < 0) {
    a *= -1;
    b *= -1;
  }

  return Fraction(a, b);
}

double Fraction::operator()() const {
  cBigNumber na = a;
  cBigNumber nb = b;
  na.abs();
  nb.abs();
  double d = 0;

  int prec = 44; //floating point precision (base 2)
  d = (na / nb).tolong();
  na = na % nb;
  double dfc = 1;
  do {
    dfc /= 2;
    na *= 2;
    d += dfc * (na / nb).tolong();
    na = na % nb;
    --prec;
  } while (prec>0);

  if (a*b < 0) {
    d *= -1;
  }
  return d;
}

Fraction::operator double() const {
  cBigNumber na = a;
  cBigNumber nb = b;
  na.abs();
  nb.abs();
  double d = 0;

  int prec = 44; //floating point precision (base 2)
  d = (na / nb).tolong();
  na = na % nb;
  double dfc = 1;
  do {
    dfc /= 2;
    na *= 2;
    d += dfc * (na / nb).tolong();
    na = na % nb;
    --prec;
  } while (prec>0);

  if (a*b < 0) {
    d *= -1;
  }
  return d;
}

string Fraction::toStr(int prec) {
  cBigNumber na = a;
  cBigNumber nb = b;

  stringstream ss;
  ss << a / b << ".";
  na = na % nb;

  do {
    na *= 10;
    ss << na / nb;
    na -= (na / nb)*nb;
    --prec;
  } while (prec>0);

  return ss.str();
}

//b rounded to length of a is a
bool approxEqual(Fraction a, Fraction b, int prec) {
  Fraction pm(5, 10); //0.5

  if (prec < 0) {
    pm.b *= pow(10, -prec);
  }
  if (prec > 0) {
    pm.a *= pow(10, prec);
  }

  return (a - pm <= b && b<a + pm);
}

Fraction approx(string s) {
  string integer, fractional;
  size_t dot = s.find('.');
  if (dot == s.npos) {
    s += ".0";
    dot = s.find('.');
  }
  integer = s.substr(0, dot);
  fractional = s.substr(dot + 1, s.length() - dot - 1);
  Fraction mine(0, 1);
  Fraction p(1, 2); //p=a+c q=b+d
  Fraction maxe(1, 1);
  Fraction fract;
  fract.a = strTo<cBigNumber>(fractional);
  fract.b = pow(10l, fractional.length());
  bool fe = false;
  if (approxEqual(fract, mine, -int(fractional.length()))) {
    fe = true;
    p = mine;
  }
  if (approxEqual(fract, maxe, -int(fractional.length()))) {
    fe = true;
    p = maxe;
  }
  while (!fe && !approxEqual(fract, p, -int(fractional.length()))) {
    if (p>fract) {
      maxe = p;
      p.a += mine.a;
      p.b += mine.b;
    }
    else {
      mine = p;
      p.a += maxe.a;
      p.b += maxe.b;
    }
  }
  cBigNumber integ = strTo<cBigNumber>(integer);
  if (integer.length() && integer[0] == '-') {
    p = p*-1;
  };
  p += integ;
  return p;
}
Fraction approx(double d, int fprec) {
  cBigNumber integ = floor(d);
  d -= floor(d);
  Fraction mine(0, 1);
  Fraction p(1, 2); //a+c / b+d
  Fraction fract;
  Fraction maxe(1, 1);
  fract.b = pow(10, fprec);
  fract.a = d*fract.b;
  bool fe = false;
  if (approxEqual(fract, mine, -fprec)) {
    fe = true;
    p = mine;
  }
  if (approxEqual(fract, maxe, -fprec)) {
    fe = true;
    p = maxe;
  }
  while (!fe && !approxEqual(fract, p, -fprec)) {
    if (p>fract) {
      maxe = p;
      p.a += mine.a;
      p.b += mine.b;
    }
    else {
      mine = p;
      p.a += maxe.a;
      p.b += maxe.b;
    }
  }
  p += integ;
  return p;
}

void Fraction::get(DataElement* data) {
  DataElement* ae = new DataElement();
  ae->_core->fromType(a);
  data->addChild(ae);

  DataElement* be = new DataElement();
  be->_core->fromType(b);
  data->addChild(be);
}
void Fraction::set(DataElement* data) {
  a = data->_children[0]->_core->toType<cBigNumber>();
  b = data->_children[1]->_core->toType<cBigNumber>();
}

Fraction operator+ (Fraction x, Fraction y) {
  return x += y;
}
Fraction operator+ (Fraction x, cBigNumber y) {
  return x += y;
}
Fraction operator+ (Fraction x, int y) {
  return x += y;
}

Fraction operator- (Fraction x, Fraction y) {
  return x -= y;
}
Fraction operator- (Fraction x, cBigNumber y) {
  return x -= y;
}
Fraction operator- (Fraction x, int y) {
  return x -= y;
}

Fraction operator- (Fraction x) {
  return x *= -1;
}

Fraction operator*(Fraction x, Fraction y) {
  return x *= y;
}
Fraction operator*(Fraction x, cBigNumber y) {
  return x *= y;
}
Fraction operator*(Fraction x, int y) {
  return x *= y;
}

Fraction operator/ (Fraction x, Fraction y) {
  return x /= y;
}
Fraction operator/ (Fraction x, cBigNumber y) {
  return x /= y;
}
Fraction operator/ (Fraction x, int y) {
  return x /= y;
}
bool operator== (Fraction lhs, Fraction rhs) {
  lhs.simplify();
  rhs.simplify();
  return (lhs.a*rhs.b == lhs.b*rhs.a);
}

bool operator< (Fraction lhs, Fraction rhs) {
  lhs.simplify();
  rhs.simplify();
  return (lhs.a*rhs.b < lhs.b*rhs.a);
}

bool operator> (Fraction lhs, Fraction rhs) {
  lhs.simplify();
  rhs.simplify();
  return (lhs.a*rhs.b > lhs.b*rhs.a);
}

bool operator<= (Fraction lhs, Fraction rhs) {
  lhs.simplify();
  rhs.simplify();
  return (lhs.a*rhs.b <= lhs.b*rhs.a);
}

bool operator>= (Fraction lhs, Fraction rhs) {
  lhs.simplify();
  rhs.simplify();
  return (lhs.a*rhs.b >= lhs.b*rhs.a);
}
