#pragma once

#include "../Core/Point.h"

template<typename T> class Polynomial;

typedef Polynomial<int> PolynomialI;
typedef Polynomial<float> PolynomialF;
typedef Polynomial<double> PolynomialD;
//typedef Polynomial<Fraction> PolynomialR;


typedef vec3<PolynomialF>    fpVec3;

template<typename T>
class Polynomial{
public:
	vector<T> Coefficient;
	int degree;

	Polynomial() {
    this->degree = 0;
    this->Coefficient = vector<Fraction>(0);
  }
	Polynomial(int d) {
    this->Coefficient.resize(d + 1);
    this->degree = d;
    for (int i = 0; i <= d; ++i) {
      this->Coefficient[i] = 0;
    }
  }
	Polynomial(vector<T> c) {
    this->degree = c.size() - 1;
    this->Coefficient = c;
  }
	Polynomial(T a, int b) {
    this->Coefficient.resize(b + 1);
    this->degree = b;
    this->Coefficient[this->degree] = a;
  }
	Polynomial Trim() {
    Polynomial p = *this;
    while (p.Coefficient.size() > 0 && p.Coefficient[p.Coefficient.size() - 1] == Fraction(0)) {
      p.degree--;
      p.Coefficient.resize(p.Coefficient.size() - 1);

    }
    return p;
  }

  T at(T val) {
    float val = 0;
    for (int i = 0; i < this->Coefficient.size(); i++) {
      val += this->Coefficient[i] * pow(value, i);
    }
    return val
  }

  template<typename T> Polynomial<T>& operator*= (Polynomial<T>& rhs) {
    return *this * rhs;
  }
  template<typename T> Polynomial<T>& operator+= (Polynomial<T>& rhs) {
    return *this + rhs;
  }
};

template<typename T> Polynomial<T> operator+ (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p3(max(lhs.degree, rhs.degree));
  for (int i = 0; i <= p3.degree; i++) {
    p3.Coefficient[i] = ((lhs.Coefficient.size() > i) ? lhs.Coefficient[i] : 0) + ((rhs.Coefficient.size() > i) ? rhs.Coefficient[i] : 0);
  }
  return (p3).Trim();
}
template<typename T> Polynomial<T> operator- (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p3(max(lhs.degree, rhs.degree));
  for (int i = 0; i <= p3.degree; i++) {
    p3.Coefficient[i] = ((lhs.Coefficient.size() > i) ? lhs.Coefficient[i] : 0) - ((rhs.Coefficient.size() > i) ? rhs.Coefficient[i] : 0);
  }
  return (p3).Trim();
}

template<typename T> Polynomial<T> operator* (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p3(lhs.degree + rhs.degree);

  for (int i = 0; i <= lhs.degree; i++) {
    for (int j = 0; j <= rhs.degree; j++) {
      p3.Coefficient[i + j] = p3.Coefficient[i + j] + lhs.Coefficient[i] * rhs.Coefficient[j];
    }
  }
  return (p3).Trim();
}
template<typename T> vec3<Polynomial<T>> operator* (Polynomial<T> lhs, vec3<T> rhs) {
  vec3<Polynomial<T>> p3(lhs.degree);

  for(int j = 0; j< 3; j++) {
    for (int i = 0; i <= lhs.degree; i++) {
      p3[j].Coefficient[i] = rhs[j] * lhs.Coefficient[i];
    }
    p3[j].Trim();
  }

  return p3;
}
template<typename T> vec3<Polynomial<T>> operator* (vec3<T> lhs, Polynomial<T> rhs) {
  return rhs*lhs;
}
template<typename T> Polynomial<T> operator* (Polynomial<T> lhs, float rhs) {
  Polynomial<T> p3(lhs.degree);

  for (int i = 0; i <= lhs.degree; i++) {
    p3.Coefficient[i] = lhs.Coefficient[i] * rhs;
  }

  return (p3).Trim();
}
template<typename T> Polynomial<T> operator* (float lhs, Polynomial<T> rhs) {
  return rhs * lhs;
}

template<typename T> Polynomial<T> operator/(Polynomial<T> lhs, Polynomial<T> rhs) {
  return PolyDiv(lhs, rhs).first;
}
template<typename T> Polynomial<T> operator%(Polynomial<T> lhs, Polynomial<T> rhs) {
  return PolyDiv(lhs, rhs).second;
}

template<typename T> pair<Polynomial<T>, Polynomial<T>> PolyDiv(Polynomial<T> lhs, Polynomial<T> rhs) {
  pair<Polynomial<T>, Polynomial<T>> p = make_pair(Polynomial<T>(), Polynomial<T>());

  if (lhs.Coefficient.size()<rhs.Coefficient.size()) {
    p = make_pair(Polynomial<T>(), lhs);
  }
  else {
    Polynomial q(Fraction(1), lhs.degree - rhs.degree);
    Polynomial nlhs = (lhs - q*(rhs*(lhs.Coefficient[lhs.degree] / rhs.Coefficient[rhs.degree]))).Trim();
    p = PolyDiv(nlhs, rhs);
    p.first = p.first + Polynomial<T>(lhs.Coefficient[lhs.degree] / rhs.Coefficient[rhs.degree], lhs.degree - rhs.degree);
  }
  return p;
}

template<typename T> bool operator> (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p = (lhs - rhs).Trim();
  if (p.degree == 0) {
    return false;
  }
  if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b > 0) {
    return true;
  }
  return false;
}
template<typename T> bool operator< (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p = (lhs - rhs).Trim();
  if (p.degree == 0) {
    return false;
  }
  if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b < 0) {
    return true;
  }
  return false;
}
template<typename T> bool operator>= (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p = (lhs - rhs).Trim();
  if (p.degree == 0) {
    return true;
  }
  if (p.Coefficient[p.Coefficient.size() - 1].a*p.Coefficient[p.Coefficient.size() - 1].b > 0) {
    return true;
  }
  return false;
}
template<typename T> bool operator<= (Polynomial<T> lhs, Polynomial<T> rhs) {
  Polynomial<T> p = (lhs - rhs).Trim();
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

template<typename T> struct Term {
public:
  T coefficient;
  map<char, int> terms;
  Term();
  Term(string s);
  int getPower(char c) {
    return terms[c];
  }
};

template<typename T> bool operator== (Term<T>& lhs, Term<T>& rhs);

template<typename T> class Equation {
public:
  list<Term<T>> terms;
  Equation();
  Equation(vector<pair<T,string> > newterms);
  void addTerm(Term<T>& t);
  void addTerm(T coefficient, string terms);
  void addTerms(vector<pair<T, string> > newterms);
  void addTerms(Equation& newterms);

  int countTerms(char t);

  typename list<Term<T>>::iterator getFirstTerm(char t);

  void substitute(Equation<T> eq, char t);
};

template<typename T> Equation<T> operator*(Equation<T>& lhs, Term<T>& rhs);
template<typename T> Equation<T> operator+(Equation<T>& lhs, Equation<T>& rhs);
template<typename T> Equation<T> operator*(Equation<T>& lhs, Equation<T>& rhs);
template<typename T> Term<T> operator*(Term<T>& lhs, Term<T>& rhs);
template<typename T> Equation<T> operator/(Equation<T>& lhs, T& rhs);
template<typename T> Equation<T> operator*(Equation<T>& lhs, T& rhs);