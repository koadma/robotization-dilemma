#pragma once

#include "GraphicsUtils.h"

using namespace std;

class GUIElement {
public:
  string name;
  float cax, cay, cbx, cby; //Not for long term storage.
  Coordiante mincorner;
  Coordiante maxcorner;
  colorargb bgColor; //#aarrggbb
  colorargb activeColor; //#aarrggbb
  colorargb textColor;
  bool active;
  bool toDelete;

  GUIElement(string name, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor);
  virtual bool isIn(int mx, int my);
  virtual void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  virtual void getRect();
  virtual int mouseEnter(int state);
  virtual int mouseMoved(int mx, int my);
  virtual int mouseClicked(int button, int state, int mx, int my);
  virtual int keyPressed(unsigned char key, int mx, int my);
  virtual int specialPressed(int key, int mx, int my);
  virtual int keyUp(unsigned char key, int mx, int my);
  virtual int specialUp(int key, int mx, int my);
  virtual int mouseWheel(int a, int b, int mx, int my);
  virtual void render();
  virtual int getWidth();
  virtual int getWidth(int winWidth);
  virtual int getHeight();
  virtual int getHeight(int winHeight);
  virtual GUIElement* getElementById(string id);
};
