#pragma once

#include "Fraction.h"

template<typename T>
class Polynomial{
public:
	vector<T> Coefficient;
	int degree;

	Polynomial();
	Polynomial(int d);
	Polynomial(vector<T> c);
	Polynomial(T a, int b);
	Polynomial Trim();

  T at(T val);
};

template<typename T> Polynomial<T> operator+ (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> Polynomial<T> operator- (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> Polynomial<T> operator* (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T, typename U> Polynomial<T> operator* (Polynomial<T> lhs, U rhs);
template<typename T, typename U> Polynomial<T> operator* (U lhs, Polynomial<T> rhs);
template<typename T> Polynomial<T> operator/(Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> Polynomial<T> operator%(Polynomial<T> lhs, Polynomial<T> rhs);

template<typename T> pair<Polynomial<T>, Polynomial<T>> PolyDiv(Polynomial<T> lhs, Polynomial<T> rhs);

template<typename T> bool operator> (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> bool operator< (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> bool operator>= (Polynomial<T> lhs, Polynomial<T> rhs);
template<typename T> bool operator<= (Polynomial<T> lhs, Polynomial<T> rhs);

template<typename T> istream& operator>>(istream& i, Polynomial<T> &p);
template<typename T> ostream& operator<<(ostream& o, Polynomial<T>& p);

typedef Polynomial<int> PolynomialI;
typedef Polynomial<float> PolynomialF;
typedef Polynomial<double> PolynomialD;
typedef Polynomial<Fraction> PolynomialR;