#pragma once

#include "GUIElement.h"
#include "LabelBind.h"

class TableRow : public GUIElement {
public:
  list<LabelBindCore*> data;
  int align;
  TableRow(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, int lalign) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    align = lalign;
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
};

class Table : public GUIElement {
public:
  list<TableRow*> data;
  int align;
  Coordiante height;
  //float chy;
  float scroll;
  Table(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, int lalign, Coordiante lheight) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    align = lalign;
    height = lheight;
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
};
