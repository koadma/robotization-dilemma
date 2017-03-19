#pragma once

#include "GraphicsUtils.h"

using namespace std;

class GUIElement {
public:
  float cax, cay, cbx, cby; //Not for long term storage.
  Coordiante mincorner;
  Coordiante maxcorner;
  colorargb bgColor; //#aarrggbb
  colorargb activeColor; //#aarrggbb
  colorargb textColor;
  bool active;
  bool toDelete;

  GUIElement(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor);
  virtual bool isIn(int mx, int my);
  virtual void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  virtual int mouseEnter(int state);
  virtual int mouseMoved(int mx, int my);
  virtual int mouseClicked(int button, int state, int mx, int my);
  virtual int keyPressed(unsigned char key, int mx, int my);
  virtual int specialPressed(int key, int mx, int my);
  virtual int mouseWheel(int a, int b, int mx, int my);
  virtual void render();
};
