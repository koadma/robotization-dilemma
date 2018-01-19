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
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  int mouseWheel(int a, int b, int mx, int my);
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int keyUp(unsigned char key, int mx, int my);
  int specialUp(int key, int mx, int my);

  void deleteElements(bool hard);

  GUIElement* getElementById(string id);
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
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  int mouseWheel(int a, int b, int mx, int my);
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int keyUp(unsigned char key, int mx, int my);
  int specialUp(int key, int mx, int my);

  void deleteElements(bool hard);

  GUIElement* getElementById(string id);
};
