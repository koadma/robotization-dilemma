#include "Fraction.h"

Fraction::Fraction() {
	a = 0;
	b = 1;
}

Fraction::Fraction(long long a) {
	this->a = a;
	this->b = 1;
}

Fraction::Fraction(long long a, long long b) {
	this->a = a;
	if (!b) {

		b = 1;
		return;
	}
	this->b = b;
}

/*Fraction::Fraction(double d) {
  
}*/

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
/*
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
}*/

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

string Fraction::toStr(int prec) {
  long long na = a;
  long long nb = b;

  stringstream ss;
  ss << a/b << ".";
  na = na % nb;
  
  do {
    a*=10;
    ss << a/b;
    a-=(a/b)*b;
    --prec;
  } while(prec>0);

  return ss.str();
}

//b rounded to length of a is a
bool approxEqual(Fraction a, Fraction b, int prec) {
  Fraction pm(5,10); //0.5

  if (prec < 0) {
    pm.b *= pow(10, -prec);
  }
  if (prec > 0) {
    pm.a *= pow(10, prec);
  }

  return (a-pm <= b && b<a+pm);
}

Fraction approx(string s) {
  string integer, fractional;
  size_t dot = s.find('.');
  if (dot == s.npos) {
    s += ".0";
    dot = s.find('.');
  }
  integer = s.substr(0, dot);
  fractional = s.substr(dot+1, s.length() - dot - 1);
  Fraction mine(0, 1);
  Fraction p(1, 2); //p=a+c q=b+d
  Fraction maxe(1, 1);
  Fraction fract;
  fract.a = strTo<long long>(fractional);
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
  while (!fe && !approxEqual(fract, p, -int(fractional.length()))) { //megn�zz�k, hogy az aktu�lis p/q t�rt j�l becs�li-e a szamot ha igen akkor abbahagyjuk a keres�st
    if (p>fract) { //az aktu�lis p/q t�rt nagyobb mint a k�zel�tend� �rt�k akkor balra megy�nk
      maxe = p;
      p.a += mine.a;
      p.b += mine.b;
    }
    else { //egy�bk�nt jobbra
      mine = p;
      p.a += maxe.a;
      p.b += maxe.b;
    }
  }
  long long integ = strTo<long long>(integer);
  if (integer.length() && integer[0] == '-') {
    p = p*-1;
  };
  p += integ;
  return p;
}

/*

int main() {
	//beolvas�s
	string szam;
	int precision;
	cin>>szam>>precision;
	
	string egyik, masik; //eg�szr�sz, t�rtr�sz
	
	string* curr=&egyik;
	for(auto i:szam) {
		if(i!='.') curr->push_back(i);
		else curr=&masik;
	}
	
	//ha eg�sz a sz�m ki�rjuk alapb�l
	bool egesz=1;
	for(auto i:masik) egesz&=i=='0';
	
	if(egesz) {
		cout<<egyik<<" 1\n";
		return 0;
	}

	//Stern-Brocot f�ban az aktu�lis 3 t�rt
	
	ll a=0, b=1;
	ll p=1, q=2; //p=a+c q=b+d
	ll c=1, d=1;
	string x=masik;
	

	
	string akt;
	while(!ok(x,akt=div(p,q,x.size()+1))) { //megn�zz�k, hogy az aktu�lis p/q t�rt j�l becs�li-e a szamot ha igen akkor abbahagyjuk a keres�st
		string xv=x; //xv = x + egy 0-�s jegy, hogy x.size()+1 jegy� legyen
		xv.push_back('0');
		
		if(akt>xv) { //az aktu�lis p/q t�rt nagyobb mint a k�zel�tend� �rt�k akkor balra megy�nk
			c=p;
			d=q;
			p+=a;
			q+=b;
		}else { //egy�bk�nt jobbra
			a=p;
			b=q;
			p+=c;
			q+=d;
		}
	}
	
	ll egyikint=0;
	stringstream ss(egyik); //az eg�szr�szt sz�mm� konvert�ljuk majd ki�rjuk az eredm�nyt
	ss>>egyikint;
	
	cout<<(p+q*egyikint)<<" "<<q<<"\n";
	return 0;
}
*/