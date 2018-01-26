#include "Image.h"

void Image::render() {
  if (texture == 0) {
    int w, h;
    texture = png_texture_load(filename.c_str(), w, h);
  }
  setColor(0xffffffff);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 1);
  glVertex2d(cax, cay);
  glTexCoord2f(1, 1);
  glVertex2d(cbx, cay);
  glTexCoord2f(1, 0);
  glVertex2d(cbx, cby);
  glTexCoord2f(0, 0);
  glVertex2d(cax, cby);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

