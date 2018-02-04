#pragma once

#include "GUIElement.h"

class Container : public GUIElement {
public:
  GUIElement* element; 
  Container(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void transformCoordiantes(int &mx, int &my);
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  int mouseWheel(int a, int b, int mx, int my);
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  void render();

  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);

  void deleteElements(bool hard);

  GUIElement* getElementById(string id);
  /*bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();*/
};
