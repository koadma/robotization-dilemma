#pragma once

#include "GUIElement.h"

class Canvas : public GUIElement {
public:
  IWindowManagers managers;
  Canvas(string lname, Coordiante lmincorner, Coordiante lmaxcorner, IWindowManagers lmanagers) :
    GUIElement(lname, lmincorner, lmaxcorner, 0, 0, 0) {
    managers = lmanagers;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key>& down);
  void render();
};
