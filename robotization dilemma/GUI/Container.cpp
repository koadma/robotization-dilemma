#include "Container.h"

void Container::transformCoordiantes(int &mx, int &my) {
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

  if (element != NULL) {
    bstate = element->mouseMoved(mx, my);
  }

  return bstate;
}
int Container::guiEvent(gui_event evt, int mx, int my, set<key_location>& down) {
  //cout << key << endl;

  int bstate = 0;

  if (element != NULL) {
    bstate = element->guiEvent(evt, mx, my, down);
  }
  return bstate;
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
    element->getRect(cbx - cax, cby - cay, cax, cay);
  }
}

void Container::deleteElements(bool hard) {
  if (element != NULL) {
    element->toDelete = true;
    if (hard) {
      delete element;
      element = NULL;
    }
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