#include "TextInput.h"

bool TextInput::keyPressed(unsigned char key, int mx, int my) {
  if (active) {
    if(key == '\b' && text.length()) {
      text.pop_back();
      return true;
    }
    if (key == '\n' || key == '\r') {
      input(text);
      return true;
    }
    if (validator(key)) {
      text+=key;
      return true;
    }
  }
  return false;
}

bool TextInput::specialPressed(int key, int mx, int my) {
  return false;
}

bool TextInput::mouseClicked(int mx, int my) {
  bool oactive = active;
  active = ((x <= mx) && (mx <= x + width) && (y <= my) && (my <= y + height));
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
  glVertex2d(x, y);
  glVertex2d(x + width, y);
  glVertex2d(x + width, y + height);
  glVertex2d(x, y + height);
  glEnd();

  renderBitmapString(x + width / 2.0f, y + height / 2.0f, text, textColor, true);
  //shapesPrintf(0, 0, text.c_str());
}

