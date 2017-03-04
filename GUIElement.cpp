#include "GUIElement.h"

GUIElement::GUIElement(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor) {
  mincorner = lmincorner;
  maxcorner = lmaxcorner;
  bgColor = lbg;
  activeColor = lactive;
  textColor = ltextColor;
  active = false;
}

void GUIElement::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);
}

bool GUIElement::isIn(int mx, int my) {
  //getRect(winWidth, winHeight);
  if ((cax <= mx) && (mx <= cbx) && (cay <= my) && (my <= cby)) {
    return true;
  }
  return false;
}

bool GUIElement::mouseEnter(int state) {
  return false;
}

bool GUIElement::mouseMoved(int mx, int my) {
  return false;
}

bool GUIElement::mouseClicked(int mx, int my) {
  return false;
}

bool GUIElement::keyPressed(unsigned char key, int mx, int my) {
  return false;
}

bool GUIElement::specialPressed(int key, int mx, int my) {
  return false;
}

bool GUIElement::mouseWheel(int a, int b, int mx, int my) {
  return false;
}

void GUIElement::render() {

}

