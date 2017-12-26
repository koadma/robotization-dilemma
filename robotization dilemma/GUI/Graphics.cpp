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

Graphics::ButtonHwnd Graphics::createButton(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  return createButton(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor, text, clickCallback);
}
Graphics::ButtonHwnd Graphics::createButton(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback) {
  ElemHwnd elem = new Button(lname, mincorner, maxcorner, bg, active, textColor, text, clickCallback);
  return reinterpret_cast<ButtonHwnd>(createElement(id, elem));
}
Graphics::ButtonHwnd Graphics::createButton(PanelHwnd id, xml_node<> *me) {
  return createButton(
    id,
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

Graphics::CheckboxHwnd Graphics::createCheckbox(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, bool checked, CheckCallback checkCallback) {
  return createCheckbox(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor, checked, checkCallback);
}
Graphics::CheckboxHwnd Graphics::createCheckbox(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, bool checked, CheckCallback checkCallback) {
  ElemHwnd elem = new Checkbox(lname, mincorner, maxcorner, bg, active, textColor, checked, checkCallback);
  return reinterpret_cast<CheckboxHwnd>(createElement(id, elem));
}
Graphics::CheckboxHwnd Graphics::createCheckbox(PanelHwnd id, xml_node<> *me) {
  return createCheckbox(
    id,
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

Graphics::LabelHwnd Graphics::createLabel(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  return createLabel(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor, text, align);
}
Graphics::LabelHwnd Graphics::createLabel(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align) {
  ElemHwnd elem = new Label(lname, mincorner, maxcorner, bg, active, textColor, text, align);
  return reinterpret_cast<LabelHwnd>(createElement(id, elem));
}
Graphics::LabelHwnd Graphics::createLabel(PanelHwnd id, xml_node<> *me) {
  return createLabel(
    id,
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

Graphics::TextInputHwnd Graphics::createTextInput(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  return createTextInput(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
}
Graphics::TextInputHwnd Graphics::createTextInput(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator) {
  ElemHwnd elem = new TextInput(lname, mincorner, maxcorner, bg, active, textColor, text, inputCallback, validator);
  return reinterpret_cast<TextInputHwnd>(createElement(id, elem));
}
Graphics::TextInputHwnd Graphics::createTextInput(PanelHwnd id, xml_node<> *me) {
  return createTextInput(
    id,
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

Graphics::CanvasHwnd Graphics::createCanvas(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers) {
  return createCanvas(id->myPanel, lname, mincorner, maxcorner, managers);
}
Graphics::CanvasHwnd Graphics::createCanvas(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers) {
  ElemHwnd elem = new Canvas(lname, mincorner, maxcorner, managers);
  return reinterpret_cast<CanvasHwnd>(createElement(id, elem));
}

Graphics::PlotHwnd Graphics::createPlot(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor) {
  return createPlot(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor);
}
Graphics::PlotHwnd Graphics::createPlot(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor) {
  ElemHwnd elem = new Plot(lname, mincorner, maxcorner, bg, active, textColor);
  return reinterpret_cast<PlotHwnd>(createElement(id, elem));
}

Graphics::PanelHwnd Graphics::createPanel(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return createPanel(id->myPanel, lname, mincorner, maxcorner, bg);
}
Graphics::PanelHwnd Graphics::createPanel(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  ElemHwnd elem = new Panel(lname, mincorner, maxcorner, bg);
  return reinterpret_cast<PanelHwnd>(createElement(id, elem));
}
Graphics::PanelHwnd Graphics::createPanel(PanelHwnd id, xml_node<> *me) {
  PanelHwnd p = createPanel(
    id,
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

Graphics::ContainerHwnd Graphics::createContainer(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  return createContainer(id->myPanel, lname, mincorner, maxcorner, bg);
}
Graphics::ContainerHwnd Graphics::createContainer(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg) {
  ElemHwnd elem = new Container(lname, mincorner, maxcorner, bg);
  return reinterpret_cast<ContainerHwnd>(createElement(id, elem));
}
Graphics::ContainerHwnd Graphics::createContainer(PanelHwnd id, xml_node<> *me) {
  ContainerHwnd p = createContainer(
    id,
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

Graphics::SliderHwnd Graphics::createSlider(WinHwnd id, string name, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback) {
return createSlider(id->myPanel, name, mincorner, maxcorner, bg, active, textColor, pulledcolor, min, max, value, quanta, clickCallback);
}
Graphics::SliderHwnd Graphics::createSlider(PanelHwnd id, string name, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback) {
ElemHwnd elem = new Slider(name, mincorner, maxcorner, bg, active, textColor, pulledcolor, min, max, value, quanta, clickCallback);
  return reinterpret_cast<SliderHwnd>(createElement(id, elem));
}
Graphics::SliderHwnd Graphics::createSlider(PanelHwnd id, xml_node<> *me) {
  SliderHwnd p = createSlider(
    id,
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

Graphics::LabelBindHwnd Graphics::createLabelBind(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, int center) {
  return createLabelBind(id->myPanel, lname, mincorner, maxcorner, bg, active, textColor, center);
}
Graphics::LabelBindHwnd Graphics::createLabelBind(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, int center) {
  ElemHwnd elem = new LabelBind(lname, mincorner, maxcorner, bg, active, textColor, center);
  return reinterpret_cast<LabelBindHwnd>(createElement(id, elem));
}
Graphics::LabelBindHwnd Graphics::createLabelBind(PanelHwnd id, xml_node<> *me) {
  return createLabelBind(
    id,
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

void Graphics::setElements(PanelHwnd id, xml_node<> *data) {
  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling())
  {
    string name = pElem->name();
    if (name == "button") {
      createButton(id, pElem);
    }
    else if (name == "checkbox") {
      createCheckbox(id, pElem);
    }
    else if (name == "label" || name == "text") {
      createLabel(id, pElem);
    }
    else if (name == "labelbind" || name == "textbind") {
      createLabelBind(id, pElem);
    }
    else if (name == "container") {
      createContainer(id, pElem);
    }
    else if (name == "textinput" || name == "input") {
      createTextInput(id, pElem);
    }
    else if (name == "panel" || name == "div") {
      createPanel(id, pElem);
    }
    else if (name == "slider") {
      createSlider(id, pElem);
    }
    else {
      throw 1;
      return;
    }
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