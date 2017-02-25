#pragma once

#include "GUIElement.h"

class TextInput : public GUIElement {
public:
  TextValidatorFunc validator;
  TextInputFunc input;
  TextInput(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, TextInputFunc linput, TextValidatorFunc lvalidator = *textValidator) :
    GUIElement(lx, ly, lwidth, lheight, lbg, lactive, ltextColor, ltext) {
    input = linput;
    validator = lvalidator;
  }
  bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();
};
