#pragma once

#include "GUIElement.h"

class ControlSetting : public GUIElement {
public:
  key selected;
  int id;
  ControlInputFunc input;
  ControlSetting(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, key lselected, int lid, ControlInputFunc linput) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    selected = lselected;
    input = linput;
    id = lid;
  }
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  void render();
};
