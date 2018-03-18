#include "BigFloat.h"

longDouble sqrt(longDouble _val, unsigned int _prec) { //precision in 1/2^_prec
  if (_val < 0) {
    throw 1;
    return 0;
  }
  longDouble _lower, _upper, _mid;
  _mid.a = _val.a;
  _mid.a.sqrt();
  _lower.a = _mid.a;
  _upper.a = _mid.a + 1;

  _mid.b = _val.b;
  _mid.b.sqrt();
  _upper.b = _mid.b;
  _lower.b = _mid.b + 1;

  longDouble _diff = _mid*_mid - _val;

  longDouble precSqr;
  precSqr.a = 1;
  precSqr.b = 2;
  precSqr.b.pow(_prec);
  while (abs(_diff) > precSqr) {
    if (_diff > 0) {
      _upper = _mid;
    }
    if (_diff < 0) {
      _lower = _mid;
    }
    _mid = (_lower + _upper) / 2;
    _diff = _mid*_mid - _val;
  }
  return _mid;
}

longDouble pow(longDouble _val, int n) {
  longDouble res;
  if (n >= 0) {
    res.a = _val.a;
    res.a.pow(n);
    res.b = _val.b;
    res.b.pow(n);
  } else {
    res.a = _val.b;
    res.a.pow(-n);
    res.b = _val.a;
    res.b.pow(-n);
  }
  return res;
}

longDouble minv(const longDouble _a, const longDouble _b) {
  if(_a < _b) return _a;
  return _b;
}

longDouble maxv(const longDouble _a, const longDouble _b) {
  if (_a > _b) return _a;
  return _b;
}