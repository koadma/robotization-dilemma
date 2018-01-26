#pragma once

#include "GUIElement.h"

class Image : public GUIElement {
public:
  GLuint texture;
  string filename;
  Image(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string lfilename) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    filename = lfilename;
    texture = 0;
  }
  void render();
  ~Image() {
    glDeleteTextures(1, &texture);
  }
};
