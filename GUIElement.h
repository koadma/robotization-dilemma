#pragma once

#include "GraphicsUtils.h"

using namespace std;

class GUIElement {
public:
  float cax, cay, cbx, cby; //Not for long term storage.
  Coordiante mincorner;
  Coordiante maxcorner;
  string text;
  colorargb bgColor; //#aarrggbb
  colorargb activeColor; //#aarrggbb
  colorargb textColor;
  bool active;
  bool toDelete;

  GUIElement(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext);
  virtual void getRect(int winWidth, int winHeight);
  virtual bool mouseEnter(int state);
  virtual bool mouseMoved(int mx, int my);
  virtual bool mouseClicked(int mx, int my);
  virtual bool keyPressed(unsigned char key, int mx, int my);
  virtual bool specialPressed(int key, int mx, int my);
  virtual bool mouseWheel(int a, int b, int mx, int my);
  virtual void render();
};
