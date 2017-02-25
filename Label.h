#pragma once

#include "GUIElement.h"

class Label : public GUIElement {
public:
  int align;
  ClickCallback clickCallback;
  Label(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, int lalign) :
    GUIElement(lx, ly, lwidth, lheight, lbg, lactive, ltextColor, ltext) {
    align = lalign;
  }
  void render();
};
