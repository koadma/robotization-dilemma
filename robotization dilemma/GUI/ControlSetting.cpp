#include "ControlSetting.h"

int ControlSetting::keyPressed(unsigned char key, int mx, int my) {
  if (active) {
    selected.fromNormal(key);
    input(selected, id);
    return 1;
  }
  return 0;
}

int ControlSetting::specialPressed(int key, int mx, int my) {
  if (active) {
    selected.fromSpecial(key);
    input(selected, id);
    return 1;
  }
  return 0;
}

int ControlSetting::mouseClicked(int button, int state, int mx, int my) {
  bool oactive = active;
  if(state == 0 && button == 0) { //click, left
    active = isIn(mx, my);
  }
  return oactive xor active;
}

void ControlSetting::render() {
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

  renderBitmapString((cax+cbx) / 2.0f, (cay + cby) / 2.0f, selected.toName(), textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

