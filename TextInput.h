#pragma once

#include "GUIElement.h"

class TextInput : public GUIElement {
public:
  TextValidatorFunc validator;
  TextInputFunc input;
  TextInput(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, TextInputFunc linput, TextValidatorFunc lvalidator = *textValidator) :
    GUIElement(lmincorner, lmaxcorner, lbg, lactive, ltextColor, ltext) {
    input = linput;
    validator = lvalidator;
  }
  bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();
};
