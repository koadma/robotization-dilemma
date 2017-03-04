#pragma once

#include "GUIElement.h"

class Canvas : public GUIElement {
public:
  IWindowManagers managers;
  Canvas(Coordiante lmincorner, Coordiante lmaxcorner, IWindowManagers lmanagers) :
    GUIElement(lmincorner, lmaxcorner, 0, 0, 0) {
    managers = lmanagers;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int mx, int my);
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseWheel(int a, int b, int mx, int my);
  void render();
};
