#pragma once

#include "GUIElement.h"

class LabelBindCore : public GUIElement {
public:
  LabelBindCore(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
  }
  void render() {
    throw 1;
  }
};


template <typename ...Ts>
class LabelBind : public LabelBindCore {
public:
  TextBind<Ts...> text;
  int align;
  ClickCallback clickCallback;
  LabelBind(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, int lalign) :
    LabelBindCore(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    text.SetString(ltext);
    align = lalign;
  }
  void render();
};
