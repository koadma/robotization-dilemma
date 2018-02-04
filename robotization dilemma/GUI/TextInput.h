#pragma once

#include "GUIElement.h"

class TextInput : public GUIElement {
public:
  string text;
  int cursor;
  TextValidatorFunc validator;
  TextInputFunc input;
  TextInput(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, TextInputFunc linput, TextValidatorFunc lvalidator = *textValidator) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    text = ltext;
    input = linput;
    validator = lvalidator;
    cursor = -1;
  }
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  void render();
};
