#include <cstdlib>
#include <iostream>
#include <cmath>
#include "Point.h"
#include "constants.h"

using namespace std;

Point& Point::randomize()
{
  //I am not sure if this is random
  /*int distFromO = ((float) rand() / RAND_MAX  - 0.5) * MAP_SIZE;
  this->x = (float) rand() / RAND_MAX - 0.5;
  float angle = (float) rand() / (RAND_MAX/(2*PI));
  this->y = cos(angle);
  this->z = sin(angle);
  (*this) = (*this)*distFromO;*/
  
  //this certainly is random, but slow
  bool notEnd;
  do
  {
    for (int i=0; i<=2; i++)
    {
      (*this)[i] = ((float) rand() / RAND_MAX - 0.5) * MAP_SIZE;
    }
    notEnd = this->length() > MAP_SIZE/2;
  } while (notEnd);
  return *this;
}

float& Point::operator[](int n)
{
  switch (n) 
  {
    case 0:
    return x;
    break;

    case 1:
    return y;
    break;

    case 2:
    return z;
    break;

    default:
    throw WRONG_INDEX;
  }
} 

float Point::operator[](int n) const
{
  switch (n) 
  {
    case 0:
    return x;
    break;

    case 1:
    return y;
    break;

    case 2:
    return z;
    break;

    default:
    throw WRONG_INDEX;
  }
}

float Point::length() const
{
  const Point& p = *this;
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

Point Point::operator+(const Point& other) const
{
  Point res;
  res[0] = (*this)[0]+other[0];
  res[1] = (*this)[1]+other[1];
  res[2] = (*this)[2]+other[2];
  return res;
}

Point Point::operator*(float scalar) const 
{
  Point res;
  res[0] = (*this)[0]*scalar;
  res[1] = (*this)[1]*scalar;
  res[2] = (*this)[2]*scalar;
  return res;
}

Point Point::operator*(const Point& other) const
{
  Point res;
  res.x = this->x * other.x;
  res.y = this->y * other.y;
  res.z = this->z * other.z;
  return res;
}

Point Point::operator/(const Point& other) const
{
  Point res;
  res.x = this->x / other.x;
  res.y = this->y / other.y;
  res.z = this->z / other.z;
  return res;
}

Point Point::operator-(const Point& other) const
{
  return (*this) + (other*(-1));
}

float distance(const Point& p1, const Point& p2)
{
  return (p1-p2).length();
}

ostream& operator<<(ostream& os, const Point& p)
{
  cout << '(' << p.x << ", " << p.y << ", " << p.z << ')'; //no endl
  return os;
}

Point operator*(float scalar, Point p)
{
  return p*scalar;
}
