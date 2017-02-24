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

  glClearColor(1, 1, 1, 1);

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

  buttonRenderManager(glutGetWindow());
  
  glutSwapBuffers();
}
void Graphics::defaultResizeManager(int x, int y) {
  glViewport(0, 0, x, y);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void Graphics::defaultKeyManager(unsigned char key, int x, int y) {}
void Graphics::defaultSpecialKeyManager(int key, int x, int y) {}
void Graphics::defaultMouseEntryManager(int state) {
  buttonMouseEnterManager(glutGetWindow(), state);
}
void Graphics::defaultMouseMoveManager(int x, int y) {
  if (buttonMouseMoveManager(glutGetWindow(), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseClickManager(int idk, int key, int x, int y) {
  buttonMouseClickManager(glutGetWindow(), x, glutGet(GLUT_WINDOW_HEIGHT) - y);
  glutPostRedisplay();
}
void Graphics::defaultMouseWheelManager(int idk, int key, int x, int y) {}

Graphics::WindowManagers Graphics::defaultWindowManagers;
map<int, Graphics::WindowData> Graphics::windows;


bool Graphics::buttonMouseEnterManager(int id, int mstate) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.buttons.begin();

  bool state = false;

  while (it != itid->second.buttons.end()) {
    state |= it->second->mouseEnter(mstate);
    ++it;
  }
  return state;
}

bool Graphics::buttonMouseMoveManager(int id, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.buttons.begin();

  bool state = false;

  while (it != itid->second.buttons.end()) {
    state |= it->second->mouseMoved(x, y);
    ++it;
  }
  return state;
}

void Graphics::buttonMouseClickManager(int id, int x, int y) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.buttons.begin();

  while (it != itid->second.buttons.end()) {
    it->second->mouseClicked(x, y);
    ++it;
  }
}

void Graphics::buttonRenderManager(int id) {
  auto itid = Graphics::windows.find(id);
  auto it = itid->second.buttons.begin();

  while (it != itid->second.buttons.end()) {
    it->second->render();
    ++it;
  }
}

int Graphics::createButton(int id, int x, int y, int width, int height, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  windows[id].buttons.insert({ windows[id].maxbutton,new Button(x, y, width, height, bg, active, textColor, text, clickCallback) });
  windows[id].maxbutton++;
  return windows[id].maxbutton - 1;
}

void Graphics::deleteButton(int id, int button) {
  delete windows[id].buttons[button];
  windows[id].buttons.erase(button);
}