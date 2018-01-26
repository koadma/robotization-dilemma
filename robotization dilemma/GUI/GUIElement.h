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
  virtual int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  virtual void render();
  virtual int getWidth();
  virtual int getWidth(int winWidth);
  virtual int getHeight();
  virtual int getHeight(int winHeight);
  virtual GUIElement* getElementById(string id);
};
