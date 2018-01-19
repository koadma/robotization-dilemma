#include "Graphics.h"


Graphics::WinHwnd Graphics::CreateMainWindow(int x, int y, int width, int height, string caption, WindowManagers managers) {
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);

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
    glutKeyboardUpFunc(manager.keyUpManager);
    glutSpecialUpFunc(manager.specialUpKeyManager);
    glutEntryFunc(manager.mouseEntryManager);
    glutMotionFunc(manager.mouseMoveManager);
    glutPassiveMotionFunc(manager.mouseMoveManager);
    glutMouseFunc(manager.mouseClickManager);
    glutMouseWheelFunc(manager.mouseWheelManager);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glClearColor(((ElementBackColor && 0xff) >> 0)/255.0, ((ElementBackColor && 0xff00) >> 8) / 255.0, ((ElementBackColor && 0xff0000) >> 16) / 255.0, 1);
   }

  GWindow* data = new GWindow;
  
  PanelHwnd panel = new Panel("", Coordiante{ 0,0,0,0 }, Coordiante{ 1,1,0,0 }, ElementBackColor);

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
  netlock.lock();
  
  glutSetWindow(glutGetWindow());

  glClear(GL_COLOR_BUFFER_BIT);
  resetViewport();

  elementRenderManager(GetWinHwnd(glutGetWindow()));
  
  glutSwapBuffers();

  netlock.unlock();
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
void Graphics::defaultKeyUpManager(unsigned char key, int x, int y) {
  if (1 & elementKeyUpManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
    glutPostRedisplay();
  }
}
void Graphics::defaultSpecialKeyUpManager(int key, int x, int y) {
  if (1 & elementSpecialUpManager(GetWinHwnd(glutGetWindow()), key, x, glutGet(GLUT_WINDOW_HEIGHT) - y)) {
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
  Graphics::defaultKeyUpManager,
  Graphics::defaultSpecialKeyUpManager,
  Graphics::defaultMouseEntryManager,
  Graphics::defaultMouseMoveManager,
  Graphics::defaultMouseClickManager,
  Graphics::defaultMouseWheelManager,
};
map<int, Graphics::GWindow*> Graphics::windows;
map<string, void(*)()> Graphics::funcs;

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

int Graphics::elementKeyUpManager(WinHwnd id, unsigned char key, int x, int y) {
  return id->myPanel->keyUp(key, x, y);
}

int Graphics::elementSpecialUpManager(WinHwnd id, int key, int x, int y) {
  return id->myPanel->specialUp(key, x, y);
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

Graphics::ButtonHwnd Graphics::createButton(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  return new Button(lname, mincorner, maxcorner, bg, active, textColor, text, clickCallback);
}
Graphics::ButtonHwnd Graphics::createButton(xml_node<> *me) {
  return createButton(
    me->first_attribute("id")->value(),
    Coordiante {
      strTo<float>(me->first_attribute("minrelx")->value()),
      strTo<float>(me->first_attribute("minrely")->value()),
      strTo<float>(me->first_attribute("minabsx")->value()),
      strTo<float>(me->first_attribute("minabsy")->value()),
    }, Coordiante {
      strTo<float>(me->first_attribute("maxrelx")->value()),
      strTo<float>(me->first_attribute("maxrely")->value()),
      strTo<float>(me->first_attribute("maxabsx")->value()),
      strTo<float>(me->first_attribute("maxabsy")->value()),
    },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    me->value(),
    reinterpret_cast<ClickCallback>(funcs[me->first_attribute("callback")->value()]));
}

Graphics::CheckboxHwnd Graphics::createCheckbox(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, bool checked, CheckCallback checkCallback) {
  return new Checkbox(lname, mincorner, maxcorner, bg, active, textColor, checked, checkCallback);
}
Graphics::CheckboxHwnd Graphics::createCheckbox(xml_node<> *me) {
  return createCheckbox(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    strTo<bool>(me->value()),
    reinterpret_cast<CheckCallback>(funcs[me->first_attribute("callback")->value()]));
}

Graphics::LabelHwnd Graphics::createLabel(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  return new Label(lname, mincorner, maxcorner, bg, active, textColor, text, align);
}
Graphics::LabelHwnd Graphics::createLabel(xml_node<> *me) {
  return createLabel(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    me->value(),
    strTo<int>(me->first_attribute("align")->value()));
}

Graphics::TextInputHwnd Graphics::createTextInput(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  return new TextInput(lname, mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
}
Graphics::TextInputHwnd Graphics::createTextInput(xml_node<> *me) {
  return createTextInput(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    me->value(),
    reinterpret_cast<TextInputFunc>(funcs[me->first_attribute("inputfunc")->value()]),
    reinterpret_cast<TextValidatorFunc>(funcs[me->first_attribute("validatorfunc")->value()]));
}

Graphics::ControlHwnd Graphics::createControl(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, key selected, int id, ControlInputFunc inputCallback) {
  return new ControlSetting(lname, mincorner, maxcorner, bg, active, textColor, selected, id, inputCallback);
}
Graphics::ControlHwnd Graphics::createControl(xml_node<> *me) {
  return createControl(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    key{strTo<int>(me->value())},
    strTo<int>(me->first_attribute("id")->value()),
    reinterpret_cast<ControlInputFunc>(funcs[me->first_attribute("inputfunc")->value()]));
}

Graphics::CanvasHwnd Graphics::createCanvas(  string lname, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers) {
  return new Canvas(lname, mincorner, maxcorner, managers);
}

Graphics::PlotHwnd Graphics::createPlot(  string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor) {
  return new Plot(lname, mincorner, maxcorner, bg, active, textColor);
}

Graphics::PanelHwnd Graphics::createPanel(  string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return new Panel(lname, mincorner, maxcorner, bg);
}
Graphics::PanelHwnd Graphics::createPanel(  xml_node<> *me) {
  PanelHwnd p = createPanel(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()));

  setElements(p, me);

  return p;
}

Graphics::TableHwnd Graphics::createTable(  string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return new Table(lname, mincorner, maxcorner, bg);
}
Graphics::TableHwnd Graphics::createTable(  xml_node<> *me) {
  TableHwnd p = createTable(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()));

  setElements(p, me);

  return p;
}

Graphics::TablerowHwnd Graphics::createTableRow(string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return new TableRow(lname, mincorner, maxcorner, bg);
}
Graphics::TablerowHwnd Graphics::createTableRow(  xml_node<> *me) {
  TablerowHwnd p = createTableRow(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()));

  setElements(p, me);

  return p;
}

Graphics::ContainerHwnd Graphics::createContainer(  string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return new Container(lname, mincorner, maxcorner, bg);
}
Graphics::ContainerHwnd Graphics::createContainer(  xml_node<> *me) {
  ContainerHwnd p = createContainer(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()));

  return p;
}

Graphics::SliderHwnd Graphics::createSlider(  string name, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback) {
return new Slider(name, mincorner, maxcorner, bg, active, textColor, pulledcolor, min, max, value, quanta, clickCallback);
}
Graphics::SliderHwnd Graphics::createSlider(  xml_node<> *me) {
  SliderHwnd p = createSlider(
    me->first_attribute("id")->value(),
    Coordiante{
      strTo<float>(me->first_attribute("minrelx")->value()),
      strTo<float>(me->first_attribute("minrely")->value()),
      strTo<float>(me->first_attribute("minabsx")->value()),
      strTo<float>(me->first_attribute("minabsy")->value()),
    },
    Coordiante{
      strTo<float>(me->first_attribute("maxrelx")->value()),
      strTo<float>(me->first_attribute("maxrely")->value()),
      strTo<float>(me->first_attribute("maxabsx")->value()),
      strTo<float>(me->first_attribute("maxabsy")->value()),
    },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    hexToInt(me->first_attribute("pulledcolor")->value()),
    strTo<float>(me->first_attribute("minvalue")->value()),
    strTo<float>(me->first_attribute("maxvalue")->value()),
    strTo<float>(me->first_attribute("startvalue")->value()),
    strTo<float>(me->first_attribute("quanta")->value()),
    reinterpret_cast<SliderInputFunc>(funcs[me->first_attribute("callback")->value()])
  );
  return p;
}

Graphics::LabelBindHwnd Graphics::createLabelBind(  string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, int center) {
  return new LabelBind(lname, mincorner, maxcorner, bg, active, textColor, center);
}
Graphics::LabelBindHwnd Graphics::createLabelBind(  xml_node<> *me) {
  return createLabelBind(
    me->first_attribute("id")->value(),
    Coordiante{
    strTo<float>(me->first_attribute("minrelx")->value()),
    strTo<float>(me->first_attribute("minrely")->value()),
    strTo<float>(me->first_attribute("minabsx")->value()),
    strTo<float>(me->first_attribute("minabsy")->value()),
  }, Coordiante{
    strTo<float>(me->first_attribute("maxrelx")->value()),
    strTo<float>(me->first_attribute("maxrely")->value()),
    strTo<float>(me->first_attribute("maxabsx")->value()),
    strTo<float>(me->first_attribute("maxabsy")->value()),
  },
    hexToInt(me->first_attribute("bgcolor")->value()),
    hexToInt(me->first_attribute("activecolor")->value()),
    hexToInt(me->first_attribute("textcolor")->value()),
    strTo<int>(me->first_attribute("align")->value()));
}

void Graphics::deleteElements(WinHwnd id) {
  deleteElements(id->myPanel);
}

Graphics::ElemHwnd Graphics::addElement(WinHwnd id, ElemHwnd elem) {
  return addElement(id->myPanel, elem);
}
Graphics::ElemHwnd Graphics::addElement(PanelHwnd id, ElemHwnd elem) {
  id->elements.push_back(elem);
  id->getRect();
  return elem;
}
Graphics::ElemHwnd Graphics::addElement(TableHwnd id, TablerowHwnd elem) {
  id->data.push_back(elem);
  id->getRect();
  return elem;
}
Graphics::ElemHwnd Graphics::addElement(TablerowHwnd id, ElemHwnd elem) {
  id->data.push_back(elem);
  id->getRect();
  return elem;
}

Graphics::ElemHwnd Graphics::createElement(xml_node<> *me) {
  string name = me->name();
  if (name == "button") {
    return createButton(me);
  }
  else if (name == "checkbox") {
    return createCheckbox(me);
  }
  else if (name == "label" || name == "text") {
    return createLabel(me);
  }
  else if (name == "labelbind" || name == "textbind") {
    return createLabelBind(me);
  }
  else if (name == "container") {
    return createContainer(me);
  }
  else if (name == "textinput" || name == "input") {
    return createTextInput(me);
  }
  else if (name == "panel" || name == "div") {
    return createPanel(me);
  }
  else if (name == "slider") {
    return createSlider(me);
  }
  else if (name == "config") {
    return createControl(me);
  }
  else if (name == "table") {
    return createTable(me);
  }
  else {
    throw 1;
    return NULL;
  }
}

void Graphics::deleteElement(ElemHwnd elem) {
  elem->toDelete = true;
}

void Graphics::deleteElements(PanelHwnd id) {
  id->deleteElements(false);
}

void Graphics::setElements(PanelHwnd id, xml_node<> *data) {
  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling())
  {
    addElement(id, createElement(pElem));
  }
  return;
}

void Graphics::setElements(PanelHwnd id, string filename) {
  xml_document<> doc;

  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  deleteElements(id);

  setElements(id, doc.first_node("body"));

  defaultResizeManager(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  glutPostRedisplay();
}

void Graphics::setElements(TableHwnd id, xml_node<> *data) {
  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling())
  {
    string name = pElem->name();
    if (name == "tr") {
      addElement(id, createTableRow(pElem));
    }
  }
}
void Graphics::setElements(TablerowHwnd id, xml_node<> *data) {
  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling())
  {
    addElement(id, createElement(pElem));
  }
}

Graphics::ElemHwnd Graphics::getElementById(Graphics::PanelHwnd pId, string id) {
  return pId->getElementById(id);
}

Graphics::ElemHwnd Graphics::getElementById(Graphics::WinHwnd winId, string id) {
  return getElementById(winId->myPanel, id);
}

Graphics::ElemHwnd Graphics::getElementById(string id) {
  auto it = windows.begin();

  ElemHwnd res = NULL;

  while (it != windows.end() && res == NULL) {

    ElemHwnd e = getElementById(it->second, id);

    if (e != NULL) {
      res = e;
    }

    ++it;
  }

  return res;
}