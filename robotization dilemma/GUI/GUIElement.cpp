#include "GUIElement.h"

GUIElement::GUIElement(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor) {
  name = lname;
  mincorner = lmincorner;
  maxcorner = lmaxcorner;
  bgColor = lbg;
  activeColor = lactive;
  textColor = ltextColor;
  active = false;
  toDelete = false;
}

void GUIElement::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  getRect();
}

void GUIElement::getRect() {

}

bool GUIElement::isIn(int mx, int my) {
  //getRect(winWidth, winHeight);
  if ((cax  <= mx) && (mx <= cbx ) && (cay <= my) && (my <= cby)) {
    return true;
  }
  return false;
}

int GUIElement::mouseEnter(int state) {
  return 0;
}

int GUIElement::mouseMoved(int mx, int my) {
  return 0;
}

int GUIElement::guiEvent(gui_event evt, int mx, int my, set<key_location>& down) {
  return 0;
}

void GUIElement::render() {

}

GUIElement* GUIElement::getElementById(string id) {
  if (name == id) {
    return this;
  }
  else {
    return NULL;
  }
}

int GUIElement::getWidth() {
  return cbx - cax;
}
int GUIElement::getWidth(int winWidth) {
  return maxcorner.GetX(winWidth) - mincorner.GetX(winWidth);
}
int GUIElement::getHeight() {
  return cby - cay;
}
int GUIElement::getHeight(int winHeight) {
  return maxcorner.GetY(winHeight) - mincorner.GetY(winHeight);
}