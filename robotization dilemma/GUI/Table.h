#pragma once

#include "GUIElement.h"
#include "LabelBind.h"

class TableRow : public GUIElement {
public:
  list<GUIElement*> data;
  TableRow(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  void getRect();
};

class Table : public GUIElement {
public:
  list<TableRow*> data;
  //float chy;
  float scroll;
  Table(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  void getRect();
};
