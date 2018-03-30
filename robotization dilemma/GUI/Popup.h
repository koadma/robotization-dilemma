#pragma once
#include "GuiElement.h"

Coordinate to_mincorner(Coordinate topleft, Coordinate dimension);
Coordinate to_maxcorner(Coordinate topleft, Coordinate dimension);

class Popup : public GUIElement {
public:
  GUIElement* element;
  Coordinate topleftc;
  Coordinate sizec;
  Popup(string lname, Coordinate topleft, Coordinate dimension, colorargb lbg) :
    GUIElement(
    lname,
    to_mincorner(topleft, dimension),
    to_maxcorner(topleft, dimension),
    lbg, 0, 0) {
    topleftc = topleft;
    sizec = dimension;
  }
  void transformCoordinates(int &mx, int &my);
  void render();

  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);

  void moveTo(int topleftx, int toplefty);

  void deleteElements(bool hard);

  GUIElement* getElementById(string id);
  /*bool keyPressed(unsigned char key, int mx, int my);
  bool specialPressed(int key, int mx, int my);
  bool mouseClicked(int mx, int my);
  void render();*/
  ~Popup();
};
