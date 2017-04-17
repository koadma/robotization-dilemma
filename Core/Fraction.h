#pragma once

#include "..\Core\helperFunctions.h"

class Fraction {
public:
	long long a, b;

	Fraction();
	Fraction(int a);
	Fraction(int a, int b);
	Fraction(string s);

	Fraction simplify();
private:
	int gcd(int x, int y);
};

Fraction operator+(Fraction x, Fraction y);

Fraction operator + (Fraction x, int y);

Fraction operator - (Fraction x, Fraction y);

Fraction operator - (Fraction x, int y);

Fraction operator*(Fraction x, Fraction y);

Fraction operator*(Fraction x, int y);

Fraction operator / (Fraction x, Fraction y);

Fraction operator / (Fraction x, int y);

bool operator== (Fraction lhs, Fraction rhs);

ostream& operator<<(ostream& o, Fraction f);