#include "TextInput.h"

int TextInput::keyPressed(unsigned char key, int mx, int my) {
  if (active) {
    if(key == '\b' && text.length() && cursor > 0) {
      text.erase(cursor - 1, 1);
      cursor--;
      return 1;
    }
    if (key == 127 && text.length() && cursor < text.size()) {
      text.erase(cursor, 1);
      return 1;
    }
    if (key == '\n' || key == '\r') {
      input(text);
      return 3;
    }
    if (validator(text, cursor, key)) {
      text.insert(cursor, 1, key);
      cursor++;
      return 1;
    }
  }
  return 0;
}

int TextInput::specialPressed(int key, int mx, int my) {
  if (active) {
    if (key == 100) {
      cursor = max(0, cursor-1);
      return 1;
    }
    if (key == 102) {
      cursor = min(int(text.size()), cursor + 1);
      return 1;
    }
  }
  return 0;
}

int TextInput::mouseClicked(int button, int state, int mx, int my) {
  bool oactive = active;
  if(state == 0 && button == 0) { //click, left
    active = isIn(mx, my);
  }
  if(oactive xor active) {
    if (active) {
      cursor = text.size();
    }
    if (!active) {
      cursor = -1;
      input(text);
    }
  }
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

  renderBitmapString((cax+cbx) / 2.0f, (cay + cby) / 2.0f, text, textColor, true, cursor);
  //shapesPrintf(0, 0, text.c_str());
}

