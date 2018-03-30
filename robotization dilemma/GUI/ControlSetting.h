#pragma once

#include "GUIElement.h"

class ControlSetting : public GUIElement {
public:
  key selected;
  int id;
  ControlInputFunc input;
  ControlSetting(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, key lselected, int lid, ControlInputFunc linput) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    selected = lselected;
    input = linput;
    id = lid;
  }
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  void render();
  ~ControlSetting();
};
