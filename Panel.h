#pragma once

#include "GUIElement.h"

class Panel : public GUIElement {
public:
  list<GUIElement*> elements; 
  Panel(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg) :
    GUIElement(lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void transformCoordiantes(int &mx, int &my);
  bool mouseEnter(int state);
  bool mouseMoved(int mx, int my);
  bool mouseClicked(int mx, int my);
  bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  void render();

  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);

  void deleteElements(bool hard);
  /*bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();*/
};
