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

static void shapesPrintf(int row, int col, const char *fmt, ...);

void setColor(colorargb v);

void renderBitmapString(float x, float y, string text, colorargb color, bool center);

bool numericalValidator(unsigned char c);
bool textValidator(unsigned char c);