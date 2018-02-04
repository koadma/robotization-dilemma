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

int Button::mouseEnter(int state) {
  bool oactive = active;
  active &= state; //if mouse leaves, deactivate.
  return oactive xor active; //rerender if changed
}
int Button::mouseMoved(int mx, int my) {
  bool oactive = active;
  active = isIn(mx, my);
  return oactive xor active; //if state changed
}

int Button::mouseClicked(int button, int state, int mx, int my) {
  if (isIn(mx, my) && state == 0 && button == 0) { //click, left
    clickCallback();
    return 3;
  }
  return 0;
}
void Button::render() {

  glBegin(GL_QUADS);
  if (active) {
    setColor(activeColor);
  }
  else {
    setColor(bgColor);
  }
  glVertex2d(cax, cay);
  glVertex2d(cbx, cay);
  glVertex2d(cbx, cby);
  glVertex2d(cax, cby);
  glEnd();
  
  renderBitmapString((cax + cbx) / 2.0f, (cay + cby) / 2.0f, text, textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

