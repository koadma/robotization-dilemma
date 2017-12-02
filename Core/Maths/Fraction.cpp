#include "Fraction.h"

Fraction::Fraction() {
	a = 0;
	b = 1;
}

Fraction::Fraction(int a) {
	this->a = a;
	this->b = 1;
}

Fraction::Fraction(int a, int b) {
	this->a = a;
	if (!b) {

		b = 1;
		return;
	}
	this->b = b;
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
		a = atoi(s.c_str());
		b = 1;
	}

	this->simplify();
}

Fraction operator+(Fraction x, Fraction y) {
	return Fraction(x.a*y.b + x.b*y.a, x.b*y.b).simplify();
}

Fraction operator + (Fraction x, int y) {
	return Fraction(x.a + y*x.b, x.b).simplify();
}

Fraction & Fraction::operator +=(Fraction rhs) {
  long long na = a*rhs.b + b*rhs.a;
  long long nb = b*rhs.b;
  a = na;
  b = nb;
  simplify();
  return *this;
}

Fraction operator - (Fraction x, Fraction y) {
	return Fraction(x.a*y.b - x.b*y.a, x.b*y.b).simplify();
}

Fraction operator - (Fraction x, int y) {
	return Fraction(x.a - y*x.b, x.b).simplify();
}

Fraction operator - (Fraction x) {
  return Fraction(-x.a, x.b).simplify();
}

Fraction & Fraction::operator -=(Fraction rhs) {
  long long na = a*rhs.b - b*rhs.a;
  long long nb = b*rhs.b;
  a = na;
  b = nb;
  simplify();
  return *this;
}

Fraction operator*(Fraction x, Fraction y) {
	return Fraction(x.a*y.a, x.b*y.b).simplify();
}

Fraction operator*(Fraction x, int y) {
	return Fraction(x.a*y, x.b).simplify();
}

Fraction operator / (Fraction x, Fraction y) {
	return Fraction(x.a*y.b, x.b*y.a).simplify();
}

Fraction operator / (Fraction x, int y) {
	return Fraction(x.a, x.b*y).simplify();
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

ostream& operator<<(ostream& o, Fraction f) {
	if (f.a == 0)
		return o;

	if (f.a * f.b < 0) {
		o << '-';
	}
	else if (f.a * f.b > 0) {
		o << '+';
	}

	if (f.b == 1 || f.b == -1) {
		o << abs(f.a);
	}
	else {
		o << abs(f.a) << '/' << abs(f.b);
	}

	return o;
}

Fraction Fraction::simplify() {
	int d = gcd(a, b);

	a /= d;
	b /= d;

  if (b < 0) {
    a *= -1;
    b *= -1;
  }

	return Fraction(a, b);
}

int Fraction::gcd(int x, int y) {
	if (y == 0)
		return x;
	else
		return (gcd(y, x%y));
}

Fraction::operator double() const {
  return double(a)/double(b);
}