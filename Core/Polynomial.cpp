#pragma once

#include "Polynomial.h"


template<typename T> Polynomial<T>::Polynomial() {
	this->degree = 0;
	this->Coefficient = vector<Fraction>(0);
}

template<typename T> Polynomial<T>::Polynomial(int d) {
	this->Coefficient.resize(d + 1);
	this->degree = d;
	for (int i = 0; i <= d; ++i) {
		this->Coefficient[i] = 0;
	}
}

template<typename T> Polynomial<T>::Polynomial(vector<T> c) {
	this->degree = c.size() - 1;
	this->Coefficient = c;
}

template<typename T> Polynomial<T>::Polynomial(T a, int b) {
	this->Coefficient.resize(b + 1);
	this->degree = b;
	this->Coefficient[this->degree] = a;
}

template<typename T> Polynomial<T> operator+ (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p3(max(lhs.degree, rhs.degree));
	for (int i = 0; i <= p3.degree; i++) {
		p3.Coefficient[i] = ((lhs.Coefficient.size() > i) ? lhs.Coefficient[i] : 0) + ((rhs.Coefficient.size() > i) ? rhs.Coefficient[i] : 0);
	}
	return (p3).Trim();
}
template<typename T> Polynomial<T> operator- (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p3(max(lhs.degree, rhs.degree));
	for (int i = 0; i <= p3.degree; i++) {
		p3.Coefficient[i] = ((lhs.Coefficient.size() > i) ? lhs.Coefficient[i] : 0) - ((rhs.Coefficient.size() > i) ? rhs.Coefficient[i] : 0);
	}
	return (p3).Trim();
}
template<typename T> Polynomial<T> operator* (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p3(lhs.degree + rhs.degree);

	for (int i = 0; i <= lhs.degree; i++) {
		for (int j = 0; j <= rhs.degree; j++) {
			p3.Coefficient[i + j] = p3.Coefficient[i + j] + lhs.Coefficient[i] * rhs.Coefficient[j];
		}
	}
	return (p3).Trim();
}

template<typename T, typename U> Polynomial<T> operator* (Polynomial<T> lhs, U rhs) {
	Polynomial p3(lhs.degree);

	for (int i = 0; i <= lhs.degree; i++) {
		p3.Coefficient[i] = lhs.Coefficient[i] * rhs;
	}

	return (p3).Trim();
}
template<typename T, typename U> Polynomial<T> operator* (U lhs, Polynomial<T> rhs) {
  Polynomial p3(rhs.degree);

  for (int i = 0; i <= rhs.degree; i++) {
    p3.Coefficient[i] = rhs.Coefficient[i] * lhs;
  }

  return (p3).Trim();
}

template<typename T> Polynomial<T> operator/(Polynomial<T> lhs, Polynomial<T> rhs) {
	return PolyDiv(lhs, rhs).first;
}
template<typename T> Polynomial<T> operator%(Polynomial<T> lhs, Polynomial<T> rhs) {
	return PolyDiv(lhs, rhs).second;
}

template<typename T> bool operator> (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p = (lhs - rhs).Trim();
	if (p.degree == 0) {
		return false;
	}
	if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b > 0) {
		return true;
	}
	return false;
}
template<typename T> bool operator< (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p = (lhs - rhs).Trim();
	if (p.degree == 0) {
		return false;
	}
	if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b < 0) {
		return true;
	}
	return false;
}
template<typename T> bool operator>= (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p = (lhs - rhs).Trim();
	if (p.degree == 0) {
		return true;
	}
	if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b > 0) {
		return true;
	}
	return false;
}
template<typename T> bool operator<= (Polynomial<T> lhs, Polynomial<T> rhs) {
	Polynomial p = (lhs - rhs).Trim();
	if (p.degree == 0) {
		return true;
	}
	if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b < 0) {
		return true;
	}
	return false;
}


template<typename T> istream& operator>>(istream& i, Polynomial<T> &p) {
	stack<Fraction> s;
	string h;
	char c = ' ';
	while (c != '\n') {

		i.get(c);
		if (c == ' ' || c == '\n') {
			s.push(Fraction(h));
			h.clear();
		}
		else {
			h.push_back(c);
		}
	}
	while (s.size()) {
		p.Coefficient.push_back(s.top());
		s.pop();
	}
	p.degree = p.Coefficient.size() - 1;
	return i;
}

template<typename T> ostream& operator<<(ostream& o, Polynomial<T>& p) {
	for (int i = p.degree; i > 0; --i) {
		if (p.Coefficient[i].a * p.Coefficient[i].b != 0) {
			o << p.Coefficient[i] << "x";

			if (i > 1) {
				o << '^' << i;
			}
		}
	}
	if (p.Coefficient.size()) {
		o << p.Coefficient[0] << endl;
	}
	return o;
}

template<typename T> pair<Polynomial<T>, Polynomial<T>> PolyDiv(Polynomial<T> lhs, Polynomial<T> rhs) {
	pair<Polynomial, Polynomial> p = make_pair(Polynomial(), Polynomial());

	if (lhs.Coefficient.size()<rhs.Coefficient.size()){
		p = make_pair(Polynomial(), lhs);
	}
	else {
		Polynomial q(Fraction(1), lhs.degree - rhs.degree);
		Polynomial nlhs = (lhs - q*(rhs*(lhs.Coefficient[lhs.degree] / rhs.Coefficient[rhs.degree]))).Trim();
		p = PolyDiv(nlhs, rhs);
		p.first = p.first + Polynomial(lhs.Coefficient[lhs.degree] / rhs.Coefficient[rhs.degree], lhs.degree - rhs.degree);
	}
	return p;
}

template<typename T> Polynomial<T> Polynomial<T>::Trim() {
	Polynomial p = *this;
	while (p.Coefficient.size() > 0 && p.Coefficient[p.Coefficient.size() - 1] == Fraction(0)) {
		p.degree--;
		p.Coefficient.resize(p.Coefficient.size() - 1);

	}
	return p;
}

template<typename T> T Polynomial<T>::at(T value) {
  float val = 0;
  for (int i = 0; i < this->Coefficient.size(); i++) {
    val += this->Coefficient[i] * pow(value, i);
  }
  return val
}