#ifndef __POINT_H__
#define __POINT_H__

#include "Fraction.h"


template<typename T> struct vec3;
//typedef vec3<float>           fVec3;
typedef vec3<int>               iVec3;
typedef vec3<bool>              bVec3;
typedef vec3<distance_type_m>   mVec3;
typedef vec3<vel_type_mpers>    mpsVec3;
typedef vec3<acc_type_mperss>   mpssVec3;
typedef vec3<scalar_type>       sVec3;
typedef vec3<double>            fVec3;

template<typename T>
class vec3 {
public:
  enum Exception { WRONG_INDEX };

  T x;
  T y;
  T z;

  vec3() : x(0), y(0), z(0) {}
  vec3(T value) : x(value), y(value), z(value) {};
  vec3(T ax, T ay, T az) : x(ax), y(ay), z(az) {};
  /*vec3(proxy p) : x(*p.px), y(*p.py), z(*p.pz) {};*/
  template<typename U> vec3(vec3<U> c)
    : x((T)c.x), y((T)c.y), z((T)c.z) {};

  vec3<T> & operator+=(vec3<T> rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z;
    return *this;
  }
  vec3<T> & operator-=(vec3<T> rhs) {
    x -= rhs.x; y -= rhs.y; z -= rhs.z;
    return *this;
  }
  vec3<T> & operator*=(vec3<T> rhs) {
    x *= rhs.x; y *= rhs.y; z *= rhs.z;
    return *this;
  }
  vec3<T> & operator*=(T rhs) {
    x *= rhs; y *= rhs; z *= rhs;
    return *this;
  }
  vec3<T> & operator/=(vec3<T> rhs) {
    x /= rhs.x; y /= rhs.y; z /= rhs.z;
    return *this;
  }
  vec3<T> & operator%=(vec3<T> rhs) {
    x %= rhs.x; y %= rhs.y; z %= rhs.z;
    return *this;
  }
  vec3<T> & operator&=(vec3<T> rhs) {
    x &= rhs.x; y &= rhs.y; z &= rhs.z;
    return *this;
  }
  vec3<T> & operator|=(vec3<T> rhs) {
    x |= rhs.x; y |= rhs.y; z |= rhs.z;
    return *this;
  }
  vec3<T> & operator^=(vec3<T> rhs) {
    x ^= rhs.x; y ^= rhs.y; z ^= rhs.z;
    return *this;
  }
  /*vec3<T> & operator<<=(vec3<T> rhs) {
    x ^= rhs.x; y ^= rhs.y; z ^= rhs.z;
    return *this;
  }
  vec3<T> & operator>>=(vec3<T> rhs) {
    x ^= rhs.x; y ^= rhs.y; z ^= rhs.z;
    return *this;
  }*/

  vec3<T> operator+ (vec3<T> rhs) const { return vec3<T>(*this) += rhs; }
  vec3<T> operator- (vec3<T> rhs) const { return vec3<T>(*this) -= rhs; }
  vec3<T> operator* (vec3<T> rhs) const { return vec3<T>(*this) *= rhs; }
  vec3<T> operator* (T rhs) const { return vec3<T>(*this) *= rhs; }
  vec3<T> operator/ (vec3<T> rhs) const { return vec3<T>(*this) /= rhs; }
  vec3<T> operator% (vec3<T> rhs) const { return vec3<T>(*this) %= rhs; }
  vec3<T> operator& (vec3<T> rhs) const { return vec3<T>(*this) &= rhs; }
  vec3<T> operator| (vec3<T> rhs) const { return vec3<T>(*this) |= rhs; }
  vec3<T> operator^ (vec3<T> rhs) const { return vec3<T>(*this) ^= rhs; }
  /*vec3<T> operator<<(vec3<T> rhs) const { return vec3<T>(*this) <<= rhs; }
  vec3<T> operator >> (vec3<T> rhs) const { return vec3<T>(*this) >>= rhs; }*/

  vec3<T> operator+() const { return vec3<T>(+x, +y, +z); }
  vec3<T> operator-() const { return vec3<T>(-x, -y, -z); }
  vec3<T> operator~() const { return vec3<T>(~x, ~y, ~z); }
  vec3<T> operator!() const { return vec3<T>(!x, !y, !z); }

  vec3<T> & operator++() { ++x, ++y, ++z; return *this; }
  vec3<T> & operator--() { --x, --y, --z; return *this; }
  vec3<T> operator++(int) { vec3<T> cp(*this); ++x, ++y, ++z; return cp; }
  vec3<T> operator--(int) { vec3<T> cp(*this); --x, --y, --z; return cp; }

  T& operator[](int n)
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
  /*const T & operator[](int ix) const {
    return ix == 0 ? x : ix == 1 ? y : ix == 2 ? z : 0;
  }*/
  T sqrlen() const {
    return x*x + y*y + z*z;
  }
  double length() const {
    return sqrt(double(this->sqrlen()));
  }
  void norm() {
    *this /= length();
  }

  bVec3 operator==(vec3<T> rhs) const {
    return bvec3(x == rhs.x, y == rhs.y, z == rhs.z);
  }
  bVec3 operator!=(vec3<T> rhs) const {
    return bvec3(x != rhs.x, y != rhs.y, z != rhs.z);
  }
  bVec3 operator<(vec3<T> rhs) const {
    return bvec3(x < rhs.x, y < rhs.y, z < rhs.z);
  }
  bVec3 operator<=(vec3<T> rhs) const {
    return bvec3(x <= rhs.x, y <= rhs.y, z <= rhs.z);
  }
  bVec3 operator>=(vec3<T> rhs) const {
    return bvec3(x >= rhs.x, y >= rhs.y, z >= rhs.z);
  }
  bVec3 operator>(vec3<T> rhs) const {
    return bvec3(x > rhs.x, y > rhs.y, z > rhs.z);
  }

  fVec3 at(float t) { //Only for polynomial vectors
    return {x.at(t), y.at(t), z.at(t)};
  }

  vec3<T> randomize(T r)
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
      for (int i = 0; i <= 2; i++)
      {
        (*this)[i] = ((float)rand() / RAND_MAX - 0.5) * r;
      }
      notEnd = this->length() > r / 2;
    } while (notEnd);
    return *this;
  }

  void get(DataElement* data) {
    DataElement* xx = new DataElement();
    xx->_core->fromType<T>(x);
    data->addChild(xx);

    DataElement* yy = new DataElement();
    yy->_core->fromType<T>(y);
    data->addChild(yy);

    DataElement* zz = new DataElement();
    zz->_core->fromType<T>(z);
    data->addChild(zz);
  }
  void set(DataElement* data) {
    x = data->_children[0]->_core->toType<T>();
    y = data->_children[1]->_core->toType<T>();
    z = data->_children[2]->_core->toType<T>();
  }
};

inline bool band(bVec3 v) { return v.x && v.y && v.z; }
inline bool bor(bVec3 v) { return v.x || v.y || v.z; }
inline bool bxor(bVec3 v) { return v.x != v.y != v.z; }


template<typename T> std::ostream& operator<<(std::ostream& os, const vec3<T>& v)
{
  throw 1;
  os << v.x << " " << v.y << " " << v.z;
  return os;
}
template<typename T> std::istream& operator>> <>(std::istream& is, vec3<T>& v)
{
  throw 1;
  is >> v.x >> v.y >> v.z;
  return is;
}

template<typename T> inline T dot(vec3<T> lhs, vec3<T> rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
template<typename T> inline vec3<T> crs(vec3<T> lhs, vec3<T> rhs) {
  return vec3<T>(lhs.y*rhs.z - lhs.z*rhs.y,
    lhs.z*rhs.x - lhs.x*rhs.z,
    lhs.x*rhs.y - lhs.y*rhs.z);
}

template<typename T> inline float distance(vec3<T> lhs, vec3<T> rhs) {
  return (lhs-rhs).length();
}

#endif
