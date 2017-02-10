#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>

struct Point
{
  float x, y, z;

  Point() {}
  Point(float x, float y, float z) : x(x), y(y), z(z) {}
  Point& randomize();
  float length() const;
  float& operator[](int n);
  float operator[](int n) const;
  Point operator+(const Point& other) const;
  Point operator*(float scalar) const;
  Point operator*(const Point& other) const;
  Point operator/(const Point& other) const;
  Point operator-(const Point& other) const;
  friend float distance(const Point& p1, const Point& p2);
  friend std::ostream& operator<<(std::ostream& os, const Point& p);
  friend Point operator*(float scalar, Point p);

  enum Exception {WRONG_INDEX};
};

#endif
