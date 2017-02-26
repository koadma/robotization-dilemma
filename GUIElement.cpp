#include "GUIElement.h"

GUIElement::GUIElement(Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext) {
  mincorner = lmincorner;
  maxcorner = lmaxcorner;
  bgColor = lbg;
  activeColor = lactive;
  textColor = ltextColor;
  text = ltext;
  active = false;
}

void GUIElement::getRect(int winWidth, int winHeight) {
  cax = mincorner.GetX(winWidth);
  cay = mincorner.GetY(winHeight);
  cbx = maxcorner.GetX(winWidth);
  cby = maxcorner.GetY(winHeight);
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

