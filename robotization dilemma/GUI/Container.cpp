#include "Container.h"

void Container::transformCoordinates(int &mx, int &my) {
  /*mx -= cax;
  my -= cay;*/
}
int Container::mouseEnter(int state) {
  int bstate = 0;

  if (element != NULL) {
    bstate = element->mouseEnter(state);
  }
  return bstate;
}
int Container::mouseMoved(int mx, int my) {

  int bstate = 0;
  if(mid) {
    ox -= mx - mxo;
    oy -= my - myo;
    if (element != NULL) {
      element->getRect(cbx - cax, cby - cay, cax -ox, cay - oy);
    }
    bstate = 1;
  } else {
    if (element != NULL) {
      bstate = element->mouseMoved(mx, my);
    }
  }
  mxo = mx;
  myo = my;

  return bstate;
}
int Container::guiEvent(gui_event evt, int mx, int my, set<key_location>& down) {
  int bstate = 0;
  if (evt._key._type == evt._key.type_mouse && evt._key._keycode == GLUT_RIGHT_BUTTON) {
    if (evt._type == evt.evt_down) {
      mid = true;
    }
    if (evt._type == evt.evt_up) {
      mid = false;
    }
  }
  else {
    if (element != NULL) {
      bstate = element->guiEvent(evt, mx, my, down);
    }
    return bstate;
  }
}
void Container::render() {
  if(element != NULL) {
    if (element->toDelete) {
      delete element;
      element = NULL;
    } else {
      element->render();
    }
  }
}

void Container::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  if (element != NULL) {
    element->getRect(cbx - cax - ox, cby - cay - oy, cax, cay);
  }
}

GUIElement* Container::getElementById(string id) {
  if (name == id) {
    return this;
  }
  else {
    GUIElement* res = NULL;
    if(element != NULL) {
      GUIElement* e = element->getElementById(id);
    }
    return res;
  }
}

Container::~Container() {
  if (element != NULL) {
    delete element;
  }
}