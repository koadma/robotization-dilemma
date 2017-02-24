#include "Button.h"


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

void setColor(colorargb v) {
  //cout << ((v & 0x00FF0000) >> 16) / 255.0 << " " << ((v & 0x0000FF00) >> 8) / 255.0 << " " << ((v & 0x000000FF) >> 0) / 255.0 << " " << ((v & 0xFF000000) >> 24) / 255.0 << endl;
  glColor3f(((v & 0x00FF0000) >> 16)/255.0f, ((v & 0x0000FF00) >> 8) / 255.0f, ((v & 0x000000FF) >> 0) / 255.0f);
}

void renderBitmapString(float x, float y, string text, colorargb color, bool center) {
  setColor(color);
  int length = text.length();
  if (center) {
    glRasterPos2f(x - (9*length) / 2, y - 5);
  }
  else {
    glRasterPos2f(x, y);
  }
  glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(text.c_str()));
}


Button::Button(int lx, int ly, int lwidth, int lheight, colorargb lbg, colorargb lactive, colorargb ltextColor, string ltext, ClickCallback lclickCallback) {
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
}
bool Button::mouseEnter(int state) {
  bool oactive = active;
  active &= state; //if mouse leaves, deactivate.
  return oactive xor active; //rerender if changed
}
bool Button::mouseMoved(int mx, int my) {
  bool oactive = active;
  active = (x <= mx) && (mx <= x + width) && (y <= my) && (my <= y + height);
  return oactive xor active; //if state changed
}

void Button::mouseClicked(int mx, int my) {
  if((x <= mx) && (mx <= x + width) && (y <= my) && (my <= y + height)) {
    clickCallback();
  }
}
void Button::render() {

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

