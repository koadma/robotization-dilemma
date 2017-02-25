#include "Button.h"

/*Button::Button(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) {
  x = lx;
  y = ly;
  width = lwidth;
  height = lheight;
  bgColor = lbg;
  activeColor = lactive;
  textColor = ltextColor;
  text = ltext;
  clickCallback = lclickCallback;
  active = false;
}*/

bool Button::mouseEnter(int state) {
  bool oactive = active;
  active &= state; //if mouse leaves, deactivate.
  return oactive xor active; //rerender if changed
}
bool Button::mouseMoved(int mx, int my) {
  bool oactive = active;
  active = (x <= mx) && (mx <= x + width) && (y <= my) && (my <= y + height);
  return oactive xor active; //if state changed
}

bool Button::mouseClicked(int mx, int my) {
  if((x <= mx) && (mx <= x + width) && (y <= my) && (my <= y + height)) {
    clickCallback();
    return true;
  }
  return false;
}
void Button::render() {

  glBegin(GL_QUADS);
  if (active) {
    setColor(activeColor);
  }
  else {
    setColor(bgColor);
  }
  glVertex2d(x, y);
  glVertex2d(x + width, y);
  glVertex2d(x + width, y + height);
  glVertex2d(x, y + height);
  glEnd();
  
  renderBitmapString(x + width / 2.0f, y + height / 2.0f, text, textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

