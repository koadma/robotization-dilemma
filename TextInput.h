#pragma once

#include "GUIElement.h"

class TextInput : public GUIElement {
public:
  string text;
  TextValidatorFunc validator;
  TextInputFunc input;
  TextInput(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, TextInputFunc linput, TextValidatorFunc lvalidator = *textValidator) :
    GUIElement(lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    text = ltext;
    input = linput;
    validator = lvalidator;
  }
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseClicked(int mx, int my);
  void render();
};
