#include "TextInput.h"

int TextInput::keyPressed(unsigned char key, int mx, int my) {
  if (active) {
    if(key == '\b' && text.length()) {
      text.pop_back();
      return 1;
    }
    if (key == '\n' || key == '\r') {
      input(text);
      return 3;
    }
    if (validator(key)) {
      text+=key;
      return 1;
    }
  }
  return 0;
}

int TextInput::specialPressed(int key, int mx, int my) {
  return 0;
}

int TextInput::mouseClicked(int mx, int my) {
  bool oactive = active;
  active = isIn(mx, my);
  return oactive xor active;
}

void TextInput::render() {
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

  renderBitmapString((cax+cbx) / 2.0f, (cay + cby) / 2.0f, text, textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

