#include "Graphics.h"



Graphics::WinHwnd Graphics::CreateMainWindow(int x, int y, int width, int height, string caption, WindowManagers managers) {
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);

  int argc = 0;
  char **argv = new char*[0];

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  glutCreateWindow(caption.c_str());

  //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  int id = glutGetWindow();

  return SetUpWindow(id, -1, managers);
}

Graphics::WinHwnd Graphics::CreateSubWindow(WinHwnd parent, int x, int y, int width, int height, WindowManagers managers) {
  int id = glutCreateSubWindow(parent->id, x, y, width, height);

  parent->children.push_back(id);

  return SetUpWindow(id, parent->id, managers);
}

Graphics::WinHwnd Graphics::SetUpWindow(int id, int parent, WindowManagers manager) {
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

  WindowData* data = new WindowData;
  data->id = id;
  data->parent = parent;
  data->windowManagers = manager;
  windows[id] = data;

  return data;
}

int Graphics::DestroyWindow(WinHwnd id) {
  auto it = id->children.begin();
  //Delete all children
  while (it != id->children.end()) {
    DestroyWindow(windows[*it]);
    ++it;
  }
  //Remove from list
  windows.erase(id->id);
  //Destroy this window
  glutDestroyWindow(id->id);
  delete id;
  return 0;
}

Graphics::WinHwnd Graphics::GetWinHwnd(int id) {
  return windows[id];
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

  elementRenderManager(GetWinHwnd(glutGetWindow()));
  
  glutSwapBuffers();
}
void Graphics::defaultResizeManager(int x, int y) {
  glViewport(0, 0, x, y);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  elementResizeManager(GetWinHwnd(glutGetWindow()), x, y);
}
void Graphics::defaultKeyManager(unsigned char key, int x, int y) {
  if (elementKeyPressManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultSpecialKeyManager(int key, int x, int y) {
  if (elementSpecialPressManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseEntryManager(int state) {
  if (elementMouseEnterManager(GetWinHwnd(glutGetWindow()), state)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseMoveManager(int x, int y) {
  if (elementMouseMoveManager(GetWinHwnd(glutGetWindow()), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseClickManager(int idk, int key, int x, int y) {
  if(elementMouseClickManager(GetWinHwnd(glutGetWindow()), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseWheelManager(int idk, int key, int x, int y) {
  if (elementMouseWheelManager(GetWinHwnd(glutGetWindow()), idk, key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}

Graphics::WindowManagers Graphics::defaultWindowManagers;
map<int, Graphics::WindowData*> Graphics::windows;

bool Graphics::elementMouseEnterManager(WinHwnd id, int mstate) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end()) {
    state |= it->second->mouseEnter(mstate);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseMoveManager(WinHwnd id, int x, int y) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end() && !state) {
    state |= it->second->mouseMoved(x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseClickManager(WinHwnd id, int x, int y) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end() && !state) {
    state |= it->second->mouseClicked(x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementMouseWheelManager(WinHwnd id, int a, int b, int x, int y) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end() && !state) {
    state |= it->second->mouseWheel(a,b,x,y);
    ++it;
  }
  return state;
}

bool Graphics::elementKeyPressManager(WinHwnd id, unsigned char key, int x, int y) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end() && !state) {
    state |= it->second->keyPressed(key, x, y);
    ++it;
  }
  return state;
}

bool Graphics::elementSpecialPressManager(WinHwnd id, int key, int x, int y) {
  auto it = id->elements.begin();
  bool state = false;

  while (it != id->elements.end() && !state) {
    state |= it->second->specialPressed(key, x, y);
    ++it;
  }
  return state;
}

void Graphics::elementResizeManager(WinHwnd id, int width, int height) {
  auto it = id->elements.begin();

  while (it != id->elements.end()) {
    it->second->getRect(width, height);
    ++it;
  }
}

void Graphics::elementRenderManager(WinHwnd id) {
  auto it = id->elements.begin();

  while (it != id->elements.end()) {
    if (it->second->toDelete) {
      auto itdel = it;
      ++it;
      delete itdel->second;
      id->elements.erase(itdel);
    } else {
      it->second->render();
      ++it;
    }
  }
}

Graphics::ElemHwnd Graphics::createButton(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  ElemHwnd elem = new Button(mincorner, maxcorner, bg, active, textColor, text, clickCallback);
  return createElement(id, elem);
}

Graphics::ElemHwnd Graphics::createLabel(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  ElemHwnd elem = new Label(mincorner, maxcorner, bg, active, textColor, text, align);
  return createElement(id, elem);
}

Graphics::ElemHwnd Graphics::createTextInput(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  ElemHwnd elem = new TextInput(mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
  return createElement(id, elem);
}

Graphics::ElemHwnd Graphics::createElement(WinHwnd id, ElemHwnd elem) {
  id->elements.insert({id->maxbutton, elem});
  id->maxbutton++;
  Graphics::elementResizeManager(id, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  return elem;
}

void Graphics::deleteElement(ElemHwnd elem) {
  elem->toDelete = true;
}

void Graphics::deleteElements(WinHwnd id) {
  auto it = id->elements.begin();
  while(it != id->elements.end()) {
    if(it->second != NULL) {
      it->second->toDelete = true;
    }
    else {
      throw 1;
    }
    ++it;
  }
}