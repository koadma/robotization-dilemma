#pragma once

#include "GUIElement.h"

class Slider : public GUIElement {
private:
  void mouseAt(int x, int y);
  bool mdown = false;
public:
  string text;
  SliderInputFunc clickCallback;
  colorargb pulledcolor;
  float minv;
  float maxv;
  float val;
  float quanta;
  Slider(string lname, Coordinate lmincorner, Coordinate lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, colorargb lpulledcolor, float lmin, float lmax, float lvalue, float lquanta, SliderInputFunc lclickCallback) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    minv = lmin;
    maxv = lmax;
    val = lvalue;
    quanta = lquanta;
    clickCallback = lclickCallback;
    pulledcolor = lpulledcolor;
  }
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int guiEvent(gui_event evt, int mx, int my, set<key_location>& down);
  void render();
  ~Slider();
};
