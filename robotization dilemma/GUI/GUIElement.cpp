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

int GUIElement::mouseClicked(int button, int state, int mx, int my) {
  return 0;
}

int GUIElement::keyPressed(unsigned char key, int mx, int my) {
  return 0;
}

int GUIElement::specialPressed(int key, int mx, int my) {
  return 0;
}

int GUIElement::keyUp(unsigned char key, int mx, int my) {
  return 0;
}

int GUIElement::specialUp(int key, int mx, int my) {
  return 0;
}

int GUIElement::mouseWheel(int a, int b, int mx, int my) {
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

