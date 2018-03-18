#pragma once

#include "Fraction.h"

typedef Fraction longDouble;

longDouble sqrt(longDouble _val, unsigned int _prec = 64);
longDouble pow(longDouble _val, int n);
longDouble minv(const longDouble _a, const longDouble _b);
longDouble maxv(const longDouble _a, const longDouble _b);