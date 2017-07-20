#include "Slider.h"

void Slider::mouseAt(int x, int y) {
  val = minv + (maxv - minv) * float(x - cax) / float(cbx - cax);
  val = val + quanta / 2.0f - fmod(val + quanta/2.0f, quanta);
  val = min(max(val, minv),maxv);
}

int Slider::mouseEnter(int state) {
  return 0;
}
int Slider::mouseMoved(int mx, int my) {
  bool oactive = active;
  if (active && (mousebuttons & 1)) {
    mouseAt(mx, my);
  }
  else {
    active = false;
    clickCallback(val);
  }

  return (mousebuttons & 1) | (oactive xor active); //if state changed
}

int Slider::mouseClicked(int button, int state, int mx, int my) {
  bool oactive = active;

  if (isIn(mx, my)) {
    active = true;
    mousebuttons ^= mousebuttons & (1 << button); //remove bit for button;
    mousebuttons ^= (state ^ 1) << button;
    if (mousebuttons & 1) {
      mouseAt(mx, my);
    }
  }
  else {
    mousebuttons = 0;
    active = false;
    clickCallback(val);
  }

  if (state == 0 && button == 1) {
    mouseAt(mx, my);
    return 1;
  }
  return oactive xor active; //rerender if changed
}

void Slider::render() {

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

  //inner bar
  glBegin(GL_QUADS);
  if (active) {
    setColor(pulledcolor);
  }
  else {
    setColor(activeColor);
  }
  glVertex2d(cax, cay);
  glVertex2d(cax + (cbx-cax)*float(val-minv)/float(maxv-minv), cay);
  glVertex2d(cax + (cbx - cax)*float(val - minv) / float(maxv - minv), cby);
  glVertex2d(cax, cby);
  glEnd();

  renderBitmapString((cax + cbx) / 2.0f, (cay + cby) / 2.0f, to_string(val), textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

