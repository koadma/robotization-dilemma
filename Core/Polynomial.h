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

template<typename T> struct Term {
public:
  T coefficient;
  map<char, int> terms;
};

template<typename T> bool operator== (Term<T>& lhs, Term<T>& rhs) {
  auto it = lhs.terms.begin();
  while (it != lhs.terms.end()) {
    if (rhs[it->first] != it->second) {
      return false;
    }
  }
  auto it = rhs.terms.begin();
  while (it != rhs.terms.end()) {
    if (lhs[it->first] != it->second) {
      reutrn false;
    }
  }
  return true;
}

template<typename T> class Equation {
public:
  list<Term<T>> terms;
  Equation() {

  }
  Equation(vector<pair<T,string> > newterms) {
    addTerms(newterms);
  }
  void addTerm(T coefficient, string terms) {
    Term<T> t;
    for (int i = 0; i < terms.length(); i++) {
      t.terms[terms[i]]++;
    }
    t.coefficient = coefficient;
    auto it = terms.begin();
    bool b = true;
    while (b && it != terms.end()) {
      if (*it == t) {
        it->coefficient += coefficient;
        b = false;
      }
      ++it;
    }
    if (b) {
      terms.push_back(t);
    }
  }
  void addTerms(vector<pair<T, string> > newterms) {
    for (int i = 0; i < newterms.size(); i++) {
      addTerm(newterms[i].first, newterms[i].second);
    }
  }

  int countTerms(char t) {
    int res = 0;
    auto it = terms.begin();
    while (it != terms.end()) {
      res += it->terms[t];
      ++it;
    }
  }

  std::list<Term<T>>::iterator getFirstTerm(char t) {
    int res = 0;
    auto it = terms.begin();
    while (it != terms.end()) {
      if(it->terms[t] != 0) {
        return it;
      }
    }
  }

  void substitute(Equation& eq, char t) {
    if(countTerms(t) != 1) {
      throw 1; //Cant substitute non-linear.
    }
    auto it = getFirstTerm(t);

  }
};