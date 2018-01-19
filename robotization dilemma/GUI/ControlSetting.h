#pragma once

#include "GUIElement.h"

class ControlSetting : public GUIElement {
public:
  key selected;
  ControlInputFunc input;
  ControlSetting(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, key lselected, ControlInputFunc linput) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    selected = lselected;
    input = linput;
  }
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  void render();
};
