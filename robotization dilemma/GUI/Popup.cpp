#include "Popup.h"

Coordinate to_mincorner(Coordinate topleft, Coordinate dimension) {
  return Coordinate(topleft.relx, topleft.rely - dimension.rely, topleft.x, topleft.y - dimension.y);
}
Coordinate to_maxcorner(Coordinate topleft, Coordinate dimension) {
  return Coordinate(topleft.relx + dimension.relx, topleft.rely, topleft.x + dimension.x, topleft.y);
}

void Popup::transformCoordinates(int &mx, int &my) {
}
void Popup::render() {
  if (element->toDelete) {
    delete element;
  } else {
    element->render();
  }
}

void Popup::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  if (element != NULL) {
    element->getRect(cbx - cax, cby - cay, cax, cay);
  }
}

void Popup::deleteElements(bool hard) {
  if (element != NULL) {
    element->toDelete = true;
    if (hard) {
      delete element;
    }
  }
}

GUIElement* Popup::getElementById(string id) {
  if (name == id) {
    return this;
  } else {
    GUIElement* res = NULL;
    if (element != NULL) {
      GUIElement* e = element->getElementById(id);
    }
    return res;
  }
}

Popup::~Popup() {
  if (element != NULL) {
    delete element;
  }
}