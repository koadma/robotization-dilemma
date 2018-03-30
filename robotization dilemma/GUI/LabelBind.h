#pragma once

#include "GUIElement.h"

class LabelBind : public GUIElement {
public:
  TextBindCore* text;
  int align;
  ClickCallback clickCallback;
  LabelBind(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltext, int lalign) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltext) {
    align = lalign;
  }
  void render();
  ~LabelBind();
};
