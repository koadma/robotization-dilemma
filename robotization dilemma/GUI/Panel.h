#pragma once

#include "GUIElement.h"

class Panel : public GUIElement {
public:
  list<GUIElement*> elements; 
  Panel(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void transformCoordinates(int &mx, int &my);
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  void render();

  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  void getRect();

  GUIElement* getElementById(string id);
  /*bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();*/
  ~Panel();
};
