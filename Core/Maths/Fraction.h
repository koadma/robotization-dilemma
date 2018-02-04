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

  void get(DataElement* data) {
    DataElement* ae = new DataElement();
    ae->_core->fromType<long long>(a);
    data->addChild(ae);

    DataElement* be = new DataElement();
    be->_core->fromType<long long>(b);
    data->addChild(be);
  }
  void set(DataElement* data) {
    a = data->_children[0]->_core->toType<long long>();
    b = data->_children[1]->_core->toType<long long>();
  }


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

//ostream& operator<<(ostream& o, Fraction f);