#pragma once

#include "GUIElement.h"

class Button : public GUIElement{
public:
  ClickCallback clickCallback;
  Button(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) :
  GUIElement(lx, ly, lwidth, lheight, lbg, lactive, ltextColor, ltext) {
    clickCallback = lclickCallback;
  }
  bool mouseEnter(int state);
  bool mouseMoved(int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();
};
