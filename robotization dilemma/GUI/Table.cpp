#include "Table.h"

void TableRow::render() {
  auto it = data.begin();

  while (it != data.end()) {
    if ((*it)->toDelete) {
      delete *it;
      auto it2 = it;
      ++it;
      data.erase(it2);
    }
    else {
      //glPushMatrix();
      //glViewport(cax, cay, cbx - cax, cby - cay);
      //glScissor((*it)->cax, (*it)->cay, (*it)->cbx - (*it)->cax, (*it)->cby - (*it)->cay);
      (*it)->render();
      //glPopMatrix();
      ++it;
    }

  }
}

void TableRow::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  getRect();
}

void TableRow::getRect() {
  int chx = cax;

  auto it = data.begin();

  while (it != data.end()) {
    (*it)->getRect(cbx - cax, cby - cay, chx, cay);
    //chx = (*it)->cbx;
    ++it;
  }
}

int TableRow::mouseEnter(int state) {
  auto it = data.end();

  int bstate = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseEnter(state);
    }
  }
  return bstate;
}
int TableRow::mouseMoved(int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->mouseMoved(mx, my);
    }
  }
  return state;
}
int TableRow::mouseClicked(int button, int state, int mx, int my) {
  auto it = data.end();

  int bstate = 0;

  while (it != data.begin() && !(bstate & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseClicked(button, state, mx, my);
    }
  }
  return bstate;
}
int TableRow::mouseWheel(int state, int delta, int mx, int my) {
  return 0;
}
int TableRow::keyPressed(unsigned char key, int mx, int my) {

  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyPressed(key, mx, my);
    }
  }
  return state;
}
int TableRow::specialPressed(int key, int mx, int my) {

  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialPressed(key, mx, my);
    }
  }
  return state;
}
int TableRow::keyUp(unsigned char key, int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyUp(key, mx, my);
    }
  }
  return state;
}
int TableRow::specialUp(int key, int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialUp(key, mx, my);
    }
  }
  return state;
}

void TableRow::deleteElements(bool hard) {
  auto it = data.begin();
  while (it != data.end()) {
    if ((*it) != NULL) {
      (*it)->toDelete = true;
      if (hard) {
        delete *it;
        data.erase(it);
      }
    }
    else {
      data.erase(it);
      throw 1;
    }
    ++it;
  }
}

GUIElement* TableRow::getElementById(string id) {
  if (name == id) {
    return this;
  }
  else {
    GUIElement* res = NULL;

    auto it = data.begin();

    while (it != data.end() && res == NULL) {
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

void Table::render() {
  glEnable(GL_SCISSOR_TEST);
  glScissor(cax, cay, cbx - cax, cby - cay);

  auto it = data.begin();

  while (it != data.end()) {
    if ((*it)->toDelete) {
      delete *it;
      auto it2 = it;
      ++it;
      data.erase(it2);
    }
    else {
      //glPushMatrix();
      //glViewport(cax, cay, cbx - cax, cby - cay);
      //glScissor((*it)->cax, (*it)->cay, (*it)->cbx - (*it)->cax, (*it)->cby - (*it)->cay);
      (*it)->render();
      //glPopMatrix();
      ++it;
    }

  }

  glBegin(GL_QUADS);
  setColor(activeColor);
  glVertex2d(cbx - 15, sba);
  glVertex2d(cbx - 5, sba);
  glVertex2d(cbx - 5, sbb);
  glVertex2d(cbx - 15, sbb);
  glEnd();

  Graphics::resetViewport();
}

void Table::getRect(int winWidth, int winHeight, int offsetX, int offsetY) {
  cax = offsetX + mincorner.GetX(winWidth);
  cay = offsetY + mincorner.GetY(winHeight);
  cbx = offsetX + maxcorner.GetX(winWidth);
  cby = offsetY + maxcorner.GetY(winHeight);

  getRect();
}

void Table::getRect() {
  if(data.size()) {
    

    height = 0;

    for (auto&& it : data) {
      height += it->getHeight(cby - cay);
    }
    int minscroll = cby - cay - height; //lower most
    if (scroll < minscroll) {
      scroll = minscroll;
    }
    int maxscroll = 0; //upper most
    if (scroll > maxscroll) {
      scroll = maxscroll;
    }
    
    int chy = cby - scroll;

    for (auto&& it : data) {
      it->getRect(cbx - cax - 20, cby - cay, cax, chy - it->getHeight(cby - cay));
      chy = it->cay;
    }

    if(maxscroll != minscroll) {
      sbb = cby - 5 + ((cby - cay - 10) * scroll)/(height);
      sba = cby - 5 + ((cby - cay - 10) * (cay - cby + scroll)) / height;
    }
    else {
      sba = (cay + cby) / 2;
      sbb = (cay + cby) / 2;
    }

    if (sbb - sba < 10) {
      int sbc = (sba + sbb) / 2;
      sba = sbc - 5;
      sbb = sbc + 5;
    }
  }
}

void Table::deleteElements(bool hard) {
  auto it = data.begin();
  while (it != data.end()) {
    if ((*it) != NULL) {
      (*it)->toDelete = true;
      if (hard) {
        delete *it;
        data.erase(it);
      }
    }
    else {
      data.erase(it);
      throw 1;
    }
    ++it;
  }
}

int Table::mouseEnter(int state) {
  auto it = data.end();
  
  int bstate = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseEnter(state);
    }
  }
  return bstate;
}
int Table::mouseMoved(int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->mouseMoved(mx, my);
    }
  }
  return state;
}
int Table::mouseClicked(int button, int state, int mx, int my) {
  auto it = data.end();

  int bstate = 0;

  while (it != data.begin() && !(bstate & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseClicked(button, state, mx, my);
    }
  }
  return bstate;
}
int Table::mouseWheel(int state, int delta, int mx, int my) {
  if (isIn(mx, my)) {
    scroll += 30 * delta;
    getRect();
    return 1;
  }
  return 0;
  /*auto it = data.end();

  int bstate = 0;

  while (it != data.begin() && !(bstate & 2)) {
    --it;
    if (!(*it)->toDelete) {
      bstate |= (*it)->mouseWheel(delta, state, mx, my);
    }
  }
  return bstate;*/
}
int Table::keyPressed(unsigned char key, int mx, int my) {

  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyPressed(key, mx, my);
    }
  }
  return state;
}
int Table::specialPressed(int key, int mx, int my) {

  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialPressed(key, mx, my);
    }
  }
  return state;
}
int Table::keyUp(unsigned char key, int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->keyUp(key, mx, my);
    }
  }
  return state;
}
int Table::specialUp(int key, int mx, int my) {
  auto it = data.end();

  int state = 0;

  while (it != data.begin() && !(state & 2)) {
    --it;
    if (!(*it)->toDelete) {
      state |= (*it)->specialUp(key, mx, my);
    }
  }
  return state;
}

GUIElement* Table::getElementById(string id) {
  if (name == id) {
    return this;
  }
  else {
    GUIElement* res = NULL;

    auto it = data.begin();

    while (it != data.end() && res == NULL) {
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