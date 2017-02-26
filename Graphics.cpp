#include "Graphics.h"



int Graphics::CreateMainWindow(int x, int y, int width, int height, string caption, WindowManagers managers) {
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);

  int argc = 0;
  char **argv = new char*[0];

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  glutCreateWindow(caption.c_str());

  //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  int id = glutGetWindow();

  SetUpWindow(id, -1, managers);

  return id;
}

int Graphics::CreateSubWindow(int parent, int x, int y, int width, int height, WindowManagers managers) {
  int id = glutCreateSubWindow(parent, x, y, width, height);

  windows[parent].children.push_back(id);

  SetUpWindow(id, parent, managers);

  return id;
}

void Graphics::SetUpWindow(int id, int parent, WindowManagers manager) {
  glutReshapeFunc(manager.resizeManager);
  glutDisplayFunc(manager.renderManager);
  glutKeyboardFunc(manager.keyManager);
  glutSpecialFunc(manager.specialKeyManager);
  glutEntryFunc(manager.mouseEntryManager);
  glutPassiveMotionFunc(manager.mouseMoveManager);
  glutMouseFunc(manager.mouseClickManager);
  glutMouseWheelFunc(manager.mouseWheelManager);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glClearColor(((ElementBackColor && 0xff) >> 0)/255.0, ((ElementBackColor && 0xff00) >> 8) / 255.0, ((ElementBackColor && 0xff0000) >> 16) / 255.0, 1);

  WindowData data;
  data.parent = -1;
  data.windowManagers = manager;
  windows[id] = data;
}

int Graphics::DestroyWindow(int id) {
  auto it = windows[id].children.begin();
  //Delete all children
  while (it != windows[id].children.end()) {
    DestroyWindow(*it);
    ++it;
  }
  //Remove from list
  windows.erase(id);
  //Destroy this window
  glutDestroyWindow(id);

  return 0;
}

void Graphics::defaultRenderManager() {
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, glutGet(GLUT_WINDOW_WIDTH),
    0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3ub(0, 1, 0);

  elementRenderManager(glutGetWindow());
  
  glutSwapBuffers();
}
void Graphics::defaultResizeManager(int x, int y) {
  glViewport(0, 0, x, y);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  elementResizeManager(glutGetWindow(), x, y);
}
void Graphics::defaultKeyManager(unsigned char key, int x, int y) {
  if (elementKeyPressManager(glutGetWindow(), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultSpecialKeyManager(int key, int x, int y) {
  if (elementSpecialPressManager(glutGetWindow(), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseEntryManager(int state) {
  if (elementMouseEnterManager(glutGetWindow(), state)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseMoveManager(int x, int y) {
  if (elementMouseMoveManager(glutGetWindow(), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseClickManager(int idk, int key, int x, int y) {
  if(elementMouseClickManager(glutGetWindow(), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseWheelManager(int idk, int key, int x, int y) {
  if (elementMouseWheelManager(glutGetWindow(), idk, key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}

Graphics::WindowManagers Graphics::defaultWindowManagers;
map<int, Graphics::WindowData> Graphics::windows;

bool Graphics::elementMouseEnterManager(int id, int mstate) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->mouseEnter(mstate);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseMoveManager(int id, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->mouseMoved(x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseClickManager(int id, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->mouseClicked(x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseWheelManager(int id, int a, int b, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->mouseWheel(a,b,x,y);
    ++it;
  }
  return state;
}

bool Graphics::elementKeyPressManager(int id, unsigned char key, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->keyPressed(key, x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementSpecialPressManager(int id, int key, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  bool state = false;

  while (it != itid->second.elements.end() && !state) {
    state |= it->second->specialPressed(key, x, y);
    ++it;
  }
  return state;
}

void Graphics::elementResizeManager(int id, int width, int height) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  while (it != itid->second.elements.end()) {
    it->second->getRect(width, height);
    ++it;
  }
}

void Graphics::elementRenderManager(int id) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.elements.begin();

  while (it != itid->second.elements.end()) {
    if (it->second->toDelete) {
      auto itdel = it;
      ++it;
      delete itdel->second;
      itid->second.elements.erase(itdel);
    } else {
      it->second->render();
      ++it;
    }
  }
}



int Graphics::createButton(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  windows[id].elements.insert({ windows[id].maxbutton,new Button(mincorner, maxcorner, bg, active, textColor, text, clickCallback) });
  windows[id].maxbutton++;
  Graphics::elementResizeManager(id, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  return windows[id].maxbutton - 1;
}

int Graphics::createLabel(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  windows[id].elements.insert({ windows[id].maxbutton,new Label(mincorner, maxcorner, bg, active, textColor, text, align) });
  windows[id].maxbutton++;
  Graphics::elementResizeManager(id, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  return windows[id].maxbutton - 1;
}

int Graphics::createTextInput(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  windows[id].elements.insert({ windows[id].maxbutton,new TextInput(mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator) });
  windows[id].maxbutton++;
  Graphics::elementResizeManager(id, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  return windows[id].maxbutton - 1;
}

void Graphics::deleteElement(int id, int elem) {
  windows[id].elements[elem]->toDelete = true;
}

void Graphics::deleteElements(int id) {
  auto it = windows[id].elements.begin();
  while(it != windows[id].elements.end()) {
    if(it->second != NULL) {
      it->second->toDelete = true;
    }
    else {
      throw 1;
    }
    ++it;
  }
}