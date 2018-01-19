#include "Panel.h"

void Panel::transformCoordiantes(int &mx, int &my) {
  /*mx -= cax;
  my -= cay;*/
}
int Panel::mouseEnter(int state) {
  auto it = elements.end();

  int bstate = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseEnter(state);
    }
  }
  return bstate;
}
int Panel::mouseMoved(int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int state = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->mouseMoved(mx, my);
    }
  }
  return state;
}
int Panel::mouseClicked(int button, int state, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int bstate = 0;

  while (it != elements.begin() && !(bstate & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseClicked(button, state, mx, my);
    }
  }
  return bstate;
}
int Panel::mouseWheel(int delta, int state, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int bstate = 0;

  while (it != elements.begin() && !(bstate & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseWheel(delta, state, mx, my);
    }
  }
  return bstate;
}
int Panel::keyPressed(unsigned char key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int state = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyPressed(key, mx, my);
    }
  }
  return state;
}
int Panel::specialPressed(int key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int state = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialPressed(key, mx, my);
    }
  }
  return state;
}
int Panel::keyUp(unsigned char key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int state = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyUp(key, mx, my);
    }
  }
  return state;
}
int Panel::specialUp(int key, int mx, int my) {
  transformCoordiantes(mx, my);

  auto it = elements.end();

  int state = 0;

  while (it != elements.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialUp(key, mx, my);
    }
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
      //glPushMatrix();
      //glViewport(cax, cay, cbx - cax, cby - cay);
      //glScissor((*it)->cax, (*it)->cay, (*it)->cbx - (*it)->cax, (*it)->cby - (*it)->cay);
      (*it)->render();
      //glPopMatrix();
      ++it;
    }
    
  }
}

void Panel::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  getRect();
}

void Panel::getRect() {
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

GUIElement* Panel::getElementById(string id) {
  if (name == id) {
    return this;
  }
  else {
    GUIElement* res = NULL;

    auto it = elements.begin();

    while (it != elements.end() && res == NULL) {
      if (!(*it)->toDelete) {
        GUIElement* e = (*it)->getElementById(id);

        if (e != NULL) {
          res = e;
        }
      }

      ++it;
    }
    return res;
  }
}