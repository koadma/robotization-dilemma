#include "GraphicsUtils.h"


static void shapesPrintf(int row, int col, const char *fmt, ...)
{
  static char buf[256];
  int viewport[4];
  void *font = GLUT_BITMAP_9_BY_15;
  va_list args;

  va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
  (void)_vsnprintf_s(buf, sizeof(buf), fmt, args);
#else
  (void)vsnprintf_s(buf, sizeof(buf), fmt, args);
#endif
  va_end(args);

  glGetIntegerv(GL_VIEWPORT, viewport);

  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glOrtho(0, viewport[2], 0, viewport[3], -1, 1);

  glRasterPos2i
    (
      glutBitmapWidth(font, ' ') * col,
      -glutBitmapHeight(font) * (row + 2) + viewport[3]
      );
  glutBitmapString(font, (unsigned char*)buf);

  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

Coordiante::Coordiante() {
  x = 0;
  y = 0;
  relx = 0;
  rely = 0;
}
Coordiante::Coordiante(float lrelx, float lrely) {
  x = 0;
  y = 0;
  relx = lrelx;
  rely = 1-lrely;
}
Coordiante::Coordiante(float lrelx, float lrely, float labsx, float labsy) {
  x = labsx;
  y = labsy;
  relx = lrelx;
  rely = 1-lrely;
}
float Coordiante::GetX(float WindowWidht) {
  return WindowWidht*relx + x;
}
float Coordiante::GetY(float WindowHeight) {
  return WindowHeight*rely + y;
}

void setColor(colorargb v) {
  //cout << ((v & 0x00FF0000) >> 16) / 255.0 << " " << ((v & 0x0000FF00) >> 8) / 255.0 << " " << ((v & 0x000000FF) >> 0) / 255.0 << " " << ((v & 0xFF000000) >> 24) / 255.0 << endl;
  glColor3f(((v & 0x00FF0000) >> 16) / 255.0f, ((v & 0x0000FF00) >> 8) / 255.0f, ((v & 0x000000FF) >> 0) / 255.0f);
}

void renderBitmapString(float x, float y, string text, colorargb color, bool center) {
  setColor(color);
  int length = text.length();
  if (center) {
    glRasterPos2f(x - (9 * length) / 2, y - 5);
  }
  else {
    glRasterPos2f(x, y);
  }
  glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(text.c_str()));
}

bool numericalValidator(unsigned char c) {
  return ('0' <= c && c <= '9');
}

bool textValidator(unsigned char c) {
  return (32 <= c);
}