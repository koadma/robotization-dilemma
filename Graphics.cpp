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

  return SetUpWindow(id, -1, Coordiante(0,0,x,y), Coordiante(0, 0, x+width, y+height), managers);
}

Graphics::WinHwnd Graphics::SetUpWindow(int id, int parent, Coordiante minc, Coordiante maxc, WindowManagers manager) {
  if(id != -1) {
    glutReshapeFunc(manager.resizeManager);
    glutDisplayFunc(manager.renderManager);
    glutKeyboardFunc(manager.keyManager);
    glutSpecialFunc(manager.specialKeyManager);
    glutEntryFunc(manager.mouseEntryManager);
    glutMotionFunc(manager.mouseMoveManager);
    glutPassiveMotionFunc(manager.mouseMoveManager);
    glutMouseFunc(manager.mouseClickManager);
    glutMouseWheelFunc(manager.mouseWheelManager);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glClearColor(((ElementBackColor && 0xff) >> 0)/255.0, ((ElementBackColor && 0xff00) >> 8) / 255.0, ((ElementBackColor && 0xff0000) >> 16) / 255.0, 1);
   }

  GWindow* data = new GWindow;
  
  PanelHwnd panel = new Panel(Coordiante{ 0,1,0,0 }, Coordiante{ 1,0,0,0 }, ElementBackColor);

  data->id = id;
  data->parent = parent;
  data->windowManagers = manager;
  data->minc = minc;
  data->maxc = maxc;
  data->myPanel = panel;
  if (id != -1) {
    windows[id] = data;
  }
  return data;
}

int Graphics::DestroyWindow(WinHwnd id) {
 
  id->myPanel->deleteElements(true);

  delete id->myPanel;

  windows.erase(id->id);

   glutDestroyWindow(id->id);

  delete id;
  return 0;
}

Graphics::WinHwnd Graphics::GetWinHwnd(int id) {
  return windows[id];
}

void Graphics::GWindow::getWin(float pax, float pay, float pbx, float pby) {
  ax = pax + minc.GetX(pbx - pax);
  ay = pay + minc.GetY(pby - pay);
  bx = pbx + minc.GetX(pbx - pax);
  by = pby + minc.GetY(pby - pay);
}

void Graphics::defaultRenderManager() {
  glutSetWindow(glutGetWindow());
  //int arr[4];
  //glGetIntegerv(GL_VIEWPORT, arr);

  glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  glClear(GL_COLOR_BUFFER_BIT);
  //glGetIntegerv(GL_VIEWPORT, arr);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, glutGet(GLUT_WINDOW_WIDTH),
    0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3ub(0, 255, 0);

  elementRenderManager(GetWinHwnd(glutGetWindow()));
  
  /*
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
  */
  glutSwapBuffers();
}
void Graphics::defaultResizeManager(int x, int y) {
  WinHwnd h = GetWinHwnd(glutGetWindow());
  int width = x;//glutGet(GLUT_WINDOW_WIDTH);
  int height = y;//glutGet(GLUT_WINDOW_HEIGHT);
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  elementResizeManager(h, width, height);
}
void Graphics::defaultKeyManager(unsigned char key, int x, int y) {
  if (1 & elementKeyPressManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultSpecialKeyManager(int key, int x, int y) {
  if (1 & elementSpecialPressManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseEntryManager(int state) {
  if (1 & elementMouseEnterManager(GetWinHwnd(glutGetWindow()), state)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseMoveManager(int x, int y) {
  if (1 & elementMouseMoveManager(GetWinHwnd(glutGetWindow()), x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseClickManager(int button, int state, int x, int y) {
  if (1 & elementMouseClickManager(GetWinHwnd(glutGetWindow()), button, state, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultMouseWheelManager(int idk, int key, int x, int y) {
  if (1 & elementMouseWheelManager(GetWinHwnd(glutGetWindow()), idk, key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}

WindowManagers Graphics::defaultWindowManagers =
WindowManagers {
  Graphics::defaultRenderManager,
  Graphics::defaultResizeManager,
  Graphics::defaultKeyManager,
  Graphics::defaultSpecialKeyManager,
  Graphics::defaultMouseEntryManager,
  Graphics::defaultMouseMoveManager,
  Graphics::defaultMouseClickManager,
  Graphics::defaultMouseWheelManager,
};
map<int, Graphics::GWindow*> Graphics::windows;

int Graphics::elementMouseEnterManager(WinHwnd id, int mstate) {
  return id->myPanel->mouseEnter(mstate);
}

int Graphics::elementMouseMoveManager(WinHwnd id, int x, int y) {
  return id->myPanel->mouseMoved(x, y);
}

int Graphics::elementMouseClickManager(WinHwnd id, int button, int state, int x, int y) {
  //cout << button << " " << state << " " << x << " " << y << endl;
  return id->myPanel->mouseClicked(button, state, x, y);
}

int Graphics::elementMouseWheelManager(WinHwnd id, int a, int b, int x, int y) {
  return id->myPanel->mouseWheel(a, b, x, y);
}

int Graphics::elementKeyPressManager(WinHwnd id, unsigned char key, int x, int y) {
  return id->myPanel->keyPressed(key, x, y);
}

int Graphics::elementSpecialPressManager(WinHwnd id, int key, int x, int y) {
  return id->myPanel->specialPressed(key, x, y);
}

void Graphics::elementResizeManager(WinHwnd id, int width, int height) {
  return elementResizeManager(id->myPanel, width, height);
}

void Graphics::elementResizeManager(PanelHwnd id, int width, int height) {
  return id->getRect(width, height, 0, 0);
}

void Graphics::elementRenderManager(WinHwnd id) {
  id->myPanel->render();
}

Graphics::ButtonHwnd Graphics::createButton(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  return createButton(id->myPanel, mincorner, maxcorner, bg, active, textColor, text, clickCallback);
}
Graphics::ButtonHwnd Graphics::createButton(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  ElemHwnd elem = new Button(mincorner, maxcorner, bg, active, textColor, text, clickCallback);
  return reinterpret_cast<ButtonHwnd>(createElement(id, elem));
}

Graphics::LabelHwnd Graphics::createLabel(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  return createLabel(id->myPanel, mincorner, maxcorner, bg, active, textColor, text, align);
}
Graphics::LabelHwnd Graphics::createLabel(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  ElemHwnd elem = new Label(mincorner, maxcorner, bg, active, textColor, text, align);
  return reinterpret_cast<LabelHwnd>(createElement(id, elem));
}

Graphics::TextInputHwnd Graphics::createTextInput(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  return createTextInput(id->myPanel, mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
}
Graphics::TextInputHwnd Graphics::createTextInput(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  ElemHwnd elem = new TextInput(mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
  return reinterpret_cast<TextInputHwnd>(createElement(id, elem));
}

Graphics::CanvasHwnd Graphics::createCanvas(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers) {
  return createCanvas(id->myPanel, mincorner, maxcorner, managers);
}
Graphics::CanvasHwnd Graphics::createCanvas(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers) {
  ElemHwnd elem = new Canvas(mincorner, maxcorner, managers);
  return reinterpret_cast<CanvasHwnd>(createElement(id, elem));
}

Graphics::PanelHwnd Graphics::createPanel(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return createPanel(id->myPanel, mincorner, maxcorner, bg);
}
Graphics::PanelHwnd Graphics::createPanel(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  ElemHwnd elem = new Panel(mincorner, maxcorner, bg);
  return reinterpret_cast<PanelHwnd>(createElement(id, elem));
}

Graphics::ElemHwnd Graphics::createElement(PanelHwnd id, ElemHwnd elem) {
  id->elements.push_back(elem);
  Graphics::elementResizeManager(id, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  return elem;
}

void Graphics::deleteElement(ElemHwnd elem) {
  elem->toDelete = true;
}

void Graphics::deleteElements(PanelHwnd id) {
  id->deleteElements(false);
}

void Graphics::deleteElements(WinHwnd id) {
  deleteElements(id->myPanel);
}