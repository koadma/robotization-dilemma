#pragma once

#include "Keyframe.h"

class Fraction {
public:
	cBigNumber a, b;

  Fraction();

  Fraction(cBigNumber a);
  
  Fraction(int a);
  
  Fraction(cBigNumber a, cBigNumber b);

  Fraction(double d);

  Fraction(string s);

  Fraction& operator+=(const Fraction& rhs) {
    cBigNumber na = a*rhs.b + b*rhs.a;
    cBigNumber nb = b*rhs.b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator-=(const Fraction& rhs) {
    cBigNumber na = a*rhs.b - b*rhs.a;
    cBigNumber nb = b*rhs.b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator*=(const Fraction& rhs) {
    cBigNumber na = a*rhs.a;
    cBigNumber nb = b*rhs.b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator/=(const Fraction& rhs) {
    cBigNumber na = a*rhs.b;
    cBigNumber nb = b*rhs.a;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator+=(const cBigNumber& rhs) {
    cBigNumber na = a + b*rhs;
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator-=(const cBigNumber& rhs) {
    cBigNumber na = a - b*rhs;
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator*=(const cBigNumber& rhs) {
    cBigNumber na = a*rhs;
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator/=(const cBigNumber& rhs) {
    cBigNumber na = a;
    cBigNumber nb = b*rhs;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator+=(const int& rhs) {
    cBigNumber na = a + b*cBigNumber(rhs);
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator-=(const int& rhs) {
    cBigNumber na = a - b*cBigNumber(rhs);
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator*=(const int& rhs) {
    cBigNumber na = a*cBigNumber(rhs);
    cBigNumber nb = b;
    a = na;
    b = nb;
    simplify();
    return *this;
  }
  Fraction& operator/=(const int& rhs) {
    cBigNumber na = a;
    cBigNumber nb = b*cBigNumber(rhs);
    a = na;
    b = nb;
    simplify();
    return *this;
  }

  cBigNumber gcd(cBigNumber x, cBigNumber y);

  double operator()() const;
  operator double() const;

  Fraction& abs();

  string toStr(int prec);

  Fraction& simplify();

  void get(DataElement* data);
  void set(DataElement* data);
};

 //b rounded to length of a is a
bool approxEqual(Fraction a, Fraction b, int prec);
Fraction approx(string s);
Fraction approx(double d, int fprec);

Fraction operator+ (Fraction x, Fraction y);
Fraction operator+ (Fraction x, cBigNumber y);
Fraction operator+ (Fraction x, int y);

Fraction operator- (Fraction x, Fraction y);
Fraction operator- (Fraction x, cBigNumber y);
Fraction operator- (Fraction x, int y);

Fraction operator- (Fraction x);

Fraction operator*(Fraction x, Fraction y);
Fraction operator*(Fraction x, cBigNumber y);
Fraction operator*(Fraction x, int y);

Fraction operator/ (Fraction x, Fraction y);
Fraction operator/ (Fraction x, cBigNumber y);
Fraction operator/ (Fraction x, int y);

bool operator== (Fraction lhs, Fraction rhs);

bool operator< (Fraction lhs, Fraction rhs);

bool operator> (Fraction lhs, Fraction rhs);

bool operator<= (Fraction lhs, Fraction rhs);

bool operator>= (Fraction lhs, Fraction rhs);
