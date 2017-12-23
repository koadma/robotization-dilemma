#pragma once

#include "Keyframe.h"

class Fraction {
public:
	long long a, b;

	Fraction();
	Fraction(long long a);
	Fraction(long long a, long long b);
	Fraction(string s);

  Fraction & operator +=(Fraction rhs);
  Fraction & operator -=(Fraction rhs);

	Fraction simplify();

  operator double() const;
private:
	int gcd(int x, int y);
};

Fraction operator+(Fraction x, Fraction y);

Fraction operator + (Fraction x, int y);

Fraction operator - (Fraction x, Fraction y);

Fraction operator - (Fraction x, int y);

Fraction operator - (Fraction x);

Fraction operator*(Fraction x, Fraction y);

Fraction operator*(Fraction x, int y);

Fraction operator / (Fraction x, Fraction y);

Fraction operator / (Fraction x, int y);

bool operator== (Fraction lhs, Fraction rhs);

bool operator < (Fraction lhs, Fraction rhs);

bool operator > (Fraction lhs, Fraction rhs);

bool operator <= (Fraction lhs, Fraction rhs);

bool operator >= (Fraction lhs, Fraction rhs);

ostream& operator<<(ostream& o, Fraction f);