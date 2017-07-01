#pragma once

#include "GUIElement.h"

class Button : public GUIElement{
public:
  string text;
  ClickCallback clickCallback;
  Button(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) :
  GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
  text = ltext;
    clickCallback = lclickCallback;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  void render();
};
