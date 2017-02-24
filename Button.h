#pragma once

#include "Point.h"
#include "Ship.h"
#include "Game.h"

#include "GL\freeglut.h"
#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif

#include <map>

using namespace std;

typedef uint32_t colorargb;

typedef void(*ClickCallback)();
//extern map<int, Graphics::WindowData> Graphics::windows;

static void shapesPrintf(int row, int col, const char *fmt, ...);

void setColor(colorargb v);

void renderBitmapString(float x, float y, string text, colorargb color, bool center);

class Button {
  int x, y;
  int width, height;
  string text;
  colorargb bgColor; //#aarrggbb
  colorargb activeColor; //#aarrggbb
  colorargb textColor;
  bool active;
  ClickCallback clickCallback;
public:
  Button(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback);
  bool mouseEnter(int state);
  bool mouseMoved(int mx, int my);
  void mouseClicked(int mx, int my);
  void render();
};
