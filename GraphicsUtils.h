#pragma once

#include "Point.h"
#include "Ship.h"
#include "Game.h"

using namespace std;

typedef uint32_t colorargb;

typedef void(*ClickCallback)();
typedef void(*TextInputFunc)(string);
typedef bool(*TextValidatorFunc)(unsigned char);
//extern map<int, Graphics::WindowData> Graphics::windows;

class Coordiante {
public:
  float x;
  float y;
  float relx;
  float rely;
  Coordiante();
  Coordiante(float relx, float rely);
  Coordiante(float relx, float rely, float absx, float absy);
  float GetX(float WindowWidht);
  float GetY(float WindowHeight);
};

static void shapesPrintf(int row, int col, const char *fmt, ...);

void setColor(colorargb v);

void renderBitmapString(float x, float y, string text, colorargb color, bool center);

bool numericalValidator(unsigned char c);
bool textValidator(unsigned char c);