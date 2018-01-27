#pragma once

#include "GUIElement.h"

class Image : public GUIElement {
public:
  GLuint texture;
  string filename;
  int align;
  int w, h;
  Image(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor, string lfilename, int lalign) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    filename = lfilename;
    texture = 0;
    align = lalign;
  }
  void render();
  ~Image() {
    glDeleteTextures(1, &texture);
  }
};
