#pragma once

#include "RenderIn.h"

namespace MainGameCanvas {
  extern float camcx, camcy, camcz;
  extern float camphi, camtheta; //phi: x-z, from x, positive to z. theta: from xz to y.
  extern float d;
  extern int mxold;
  extern int myold;
  extern int mousebuttons; //left, center, right

  void normalizeAngles();
  int renderManager(int ax, int ay, int bx, int by);
  int resizeManager(int x, int y);
  int keyManager(unsigned char key, int x, int y, bool in);
  int specialKeyManager(int key, int x, int y, bool in);
  int mouseEntryManager(int state);
  int mouseMoveManager(int x, int y);
  int mouseClickManager(int button, int state, int x, int y, bool in);
  int mouseWheelManager(int idk, int key, int x, int y, bool in);
}

namespace MainGameShipCanvas {
  extern float camcx, camcy, camcz;
  extern float camphi, camtheta; //phi: x-z, from x, positive to z. theta: from xz to y.
  extern float d;
  extern int mxold;
  extern int myold;
  extern int mousebuttons; //left, center, right

  extern GLdouble model_view[16];
  extern GLdouble projection[16];
  extern GLint viewport[4];
  extern vec3<double> cameraEye;

  void normalizeAngles();
  int renderManager(int ax, int ay, int bx, int by);
  int resizeManager(int x, int y);
  int keyManager(unsigned char key, int x, int y, bool in);
  int specialKeyManager(int key, int x, int y, bool in);
  int mouseEntryManager(int state);
  int mouseMoveManager(int x, int y);
  int mouseClickManager(int button, int state, int x, int y, bool in);
  int mouseWheelManager(int idk, int key, int x, int y, bool in);
}