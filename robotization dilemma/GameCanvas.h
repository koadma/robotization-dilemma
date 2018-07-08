#pragma once

#include "RenderIn.h"

namespace MainGameCanvas {
  extern float camcx, camcy, camcz;
  extern float camphi, camtheta; //phi: x-z, from x, positive to z. theta: from xz to y.
  extern float d;
  extern int mousebuttons; //left, center, right

  extern OpenGLData view;

  void normalizeAngles();
  int renderManager(int ax, int ay, int bx, int by, set<key_location>& down);
  int resizeManager(int x, int y);
  int mouseEntryManager(int state);
  int mouseMoveManager(int x, int y, int ox, int oy, set<key_location>& down);
  int guiEventManager(gui_event evt, int mx, int my, set<key_location>& down, Canvas* me);
}

namespace MainGameShipCanvas {
  extern float camcx, camcy, camcz;
  extern float camphi, camtheta; //phi: x-z, from x, positive to z. theta: from xz to y.
  extern float d;
  extern int mousebuttons; //left, center, right

  extern OpenGLData view;

  void normalizeAngles();
  int renderManager(int ax, int ay, int bx, int by, set<key_location>& down);
  int resizeManager(int x, int y);
  int mouseEntryManager(int state);
  int mouseMoveManager(int x, int y, int ox, int oy, set<key_location>& down);
  int guiEventManager(gui_event evt, int mx, int my, set<key_location>& down, Canvas* me);
}