#pragma once

#include "GUIElement.h"

class Button : public GUIElement{
public:
  string text;
  ClickCallback clickCallback;
  Button(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) :
  GUIElement(lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
  text = ltext;
    clickCallback = lclickCallback;
  }
  bool mouseEnter(int state);
  bool mouseMoved(int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();
};
