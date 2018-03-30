#pragma once

#include "GUIElement.h"
#include "LabelBind.h"

class TableRow : public GUIElement {
public:
  list<GUIElement*> data;
  TableRow(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, 0, 0) {
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  void getRect();
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);

  GUIElement* getElementById(string id);
  ~TableRow();
};

class Table : public GUIElement {
public:
  list<TableRow*> data;
  //float chy;
  int sba, sbb; //scroll bar top - bottom
  int height;
  float scroll;
  Table(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg, colorargb lactive) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, 0) {
  }
  void render();
  void getRect(int winWidth, int winHeight, int offsetX, int offsetY);
  void getRect();
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);

  GUIElement* getElementById(string id);
  ~Table();
};
