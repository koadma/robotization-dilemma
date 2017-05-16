#pragma once

#include "GUIElement.h"

class Label : public GUIElement {
public:
  string text;
  int align;
  ClickCallback clickCallback;
  Label(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, int lalign) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    text = ltext;
    align = lalign;
  }
  void render();
};