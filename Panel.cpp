#include "Panel.h"

void Panel::transformCoordiantes(int &mx, int &my) {
  mx -= cax;
  my -= cay;
}
bool Panel::mouseEnter(int state) {
  auto it = elements.begin();

  bool bstate = false;

  while (it != elements.end()) {
    bstate |= (*it)->mouseEnter(state);
    ++it;
  }
  return bstate;
}
bool Panel::mouseMoved(int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.begin();

  bool state = false;

  while (it != elements.end()) {
    state |= (*it)->mouseMoved(mx, my);
    ++it;
  }
  return state;
}
bool Panel::mouseClicked(int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.begin();

  bool state = false;

  while (it != elements.end()) {
    state |= (*it)->mouseClicked(mx, my);
    ++it;
  }
  return state;
}
bool Panel::keyPressed(unsigned char key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.begin();

  bool state = false;

  while (it != elements.end()) {
    state |= (*it)->keyPressed(key, mx, my);
    ++it;
  }
  return state;
}
bool Panel::specialPressed(int key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.begin();

  bool state = false;

  while (it != elements.end()) {
    state |= (*it)->specialPressed(key, mx, my);
    ++it;
  }
  return state;
}
void Panel::render() {
  auto it = elements.begin();

  while (it != elements.end()) {
    if ((*it)->toDelete) {
      delete *it;
      auto it2 = it;
      ++it;
      elements.erase(it2);
    } else {
      (*it)->render();
      ++it;
    }
    
  }
}

void Panel::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  auto it = elements.begin();

  while (it != elements.end()) {
    (*it)->getRect(cbx - cax, cby - cay, cax, cay);
    ++it;
  }
}

void Panel::deleteElements(bool hard) {
  auto it = elements.begin();
  while (it != elements.end()) {
    if ((*it) != NULL) {
      (*it)->toDelete = true;
      if (hard) {
        delete *it;
        elements.erase(it);
      }
    }
    else {
      elements.erase(it);
      throw 1;
    }
    ++it;
  }
}

