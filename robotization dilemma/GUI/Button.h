#pragma once

#include "GUIElement.h"

class Button : public GUIElement{
public:
  key triggerkey;
  string text;
  ClickCallback clickCallback;
  Button(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, key ltriggerkey, ClickCallback lclickCallback) :
  GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    text = ltext;
    clickCallback = lclickCallback;
    triggerkey = ltriggerkey;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  void render();
  ~Button();
};
