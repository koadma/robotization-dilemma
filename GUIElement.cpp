#include "GUIElement.h"

GUIElement::GUIElement(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext) {
  x = lx;
  y = ly;
  width = lwidth;
  height = lheight;
  bgColor = lbg;
  activeColor = lactive;
  textColor = ltextColor;
  text = ltext;
  active = false;
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

