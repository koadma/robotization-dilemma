#pragma once

#include "GUIElement.h"

class Checkbox : public GUIElement{
public:
  bool checked;
  CheckCallback checkCallback;
  Checkbox(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, bool lchecked, CheckCallback lcheckCallback) :
  GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
  checked = lchecked;
  checkCallback = lcheckCallback;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  void render();
};