#pragma once

#include "GUIElement.h"

class Label : public GUIElement {
public:
  int align;
  ClickCallback clickCallback;
  Label(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, int lalign) :
    GUIElement(lmincorner, lmaxcorner, lbg, lactive, ltextColor, ltext) {
    align = lalign;
  }
  void render();
};
