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
int Container::mouseClicked(int button, int state, int mx, int my) {
  int bstate = 0;

  if (element != NULL) {
    bstate = element->mouseClicked(button, state, mx, my);
  }

  return bstate;
}
int Container::mouseWheel(int delta, int state, int mx, int my) {
  //cout << delta << "\t" << state << endl;
  int bstate = 0;

  if (element != NULL) {
    bstate = element->mouseWheel(delta, state, mx, my);
  }
  
  return bstate;
}
int Container::keyPressed(unsigned char key, int mx, int my) {
  //cout << key;
  int bstate = 0;

  if (element != NULL) {
    bstate = element->keyPressed(key, mx, my);
  }

  return bstate;
}
int Container::specialPressed(int key, int mx, int my) {
  //cout << key << endl;

  int bstate = 0;

  if (element != NULL) {
    bstate = element->specialPressed(key, mx, my);
    
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