#pragma once

#include "GUIElement.h"

class Container : public GUIElement {
public:
  GUIElement* element;
  int ox;
  int oy;
  int mxo;
  int myo;
  bool mid = false;
  Container(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void transformCoordiantes(int &mx, int &my);
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  void render();

  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);

  void deleteElements(bool hard);

  GUIElement* getElementById(string id);
  /*bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();*/
};
