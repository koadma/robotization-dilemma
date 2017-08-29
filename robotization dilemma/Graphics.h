#pragma once

#include "Button.h"
#include "TextInput.h"
#include "Label.h"
#include "LabelBind.h"
#include "Panel.h"
#include "Canvas.h"
#include "Slider.h"
#include "Container.h"

using namespace std;

namespace Graphics {
  
  void defaultRenderManager();
  void defaultResizeManager(int x, int y);
  void defaultKeyManager(unsigned char key, int x, int y);
  void defaultSpecialKeyManager(int key, int x, int y);
  void defaultMouseEntryManager(int state);
  void defaultMouseMoveManager(int x, int y);
  void defaultMouseClickManager(int idk, int key, int x, int y);
  void defaultMouseWheelManager(int idk, int key, int x, int y);

  /*struct WindowManagers {
    RenderManager renderManager = defaultRenderManager;
    ResizeManager resizeManager = defaultResizeManager;
    KeyManager keyManager = defaultKeyManager;
    SpecialKeyManager specialKeyManager = defaultSpecialKeyManager;
    MouseEntryManager mouseEntryManager = defaultMouseEntryManager;
    MouseMoveManager mouseMoveManager = defaultMouseMoveManager;
    MouseClickManager mouseClickManager = defaultMouseClickManager;
    MouseWheelManager mouseWheelManager = defaultMouseWheelManager;
  };*/

  class GWindow {
  public:
    Coordiante minc, maxc;
    float ax, ay, bx, by;
    int id;
    WindowManagers windowManagers;
    int parent;
    Panel* myPanel;
    void getWin(float pax, float pay, float pbx, float pby);
  };

  extern WindowManagers defaultWindowManagers;
  extern map<int, GWindow*> windows;

  extern map<string, void(*)()> funcs;


  typedef GUIElement* ElemHwnd;
  typedef Panel* PanelHwnd;
  typedef Container* ContainerHwnd;
  typedef Button* ButtonHwnd;
  typedef TextInput* TextInputHwnd;
  typedef Label* LabelHwnd;
  typedef LabelBind* LabelBindHwnd;
  typedef Canvas* CanvasHwnd;
  typedef Slider* SliderHwnd;
  typedef Graphics::GWindow* WinHwnd;

  WinHwnd CreateMainWindow(int x = 40, int y = 40, int width = 640, int height = 480, string caption = "", WindowManagers managers = defaultWindowManagers);

  WinHwnd SetUpWindow(int id, int parent, Coordiante x, Coordiante y, WindowManagers manager);

  int DestroyWindow(WinHwnd id);

  WinHwnd GetWinHwnd(int id);

  int elementMouseEnterManager(WinHwnd id, int mstate);

  int elementMouseMoveManager(WinHwnd id, int x, int y);

  int elementMouseClickManager(WinHwnd id, int button, int state, int x, int y);

  int elementMouseWheelManager(WinHwnd id, int a, int b, int x, int y);

  int elementKeyPressManager(WinHwnd id, unsigned char kay, int x, int y);

  int elementSpecialPressManager(WinHwnd id, int key, int x, int y);

  void elementResizeManager(WinHwnd id, int width, int height);

  void elementResizeManager(PanelHwnd id, int width, int height);

  void elementRenderManager(WinHwnd id);

  template <typename T> void setName(string name, T func) {
    funcs[name] = reinterpret_cast<void(*)()>(func);
  }

  ButtonHwnd createButton(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback);
  ButtonHwnd createButton(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback);
  ButtonHwnd createButton(PanelHwnd id, xml_node<> *me);
  
  LabelHwnd createLabel(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);
  LabelHwnd createLabel(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);
  LabelHwnd createLabel(PanelHwnd id, xml_node<> *me);

  TextInputHwnd createTextInput(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  TextInputHwnd createTextInput(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  TextInputHwnd createTextInput(PanelHwnd id, xml_node<> *me);

  CanvasHwnd createCanvas(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers);
  CanvasHwnd createCanvas(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, IWindowManagers managers);

  PanelHwnd createPanel(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg);
  PanelHwnd createPanel(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg);
  PanelHwnd createPanel(PanelHwnd id, xml_node<> *me);

  ContainerHwnd createContainer(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg);
  ContainerHwnd createContainer(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg);
  ContainerHwnd createContainer(PanelHwnd id, xml_node<> *me);

  SliderHwnd createSlider(WinHwnd id, string name, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback);
  SliderHwnd createSlider(PanelHwnd id, string name, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback);
  SliderHwnd createSlider(PanelHwnd id, xml_node<> *me);

  LabelBindHwnd createLabelBind(WinHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, int center);
  LabelBindHwnd createLabelBind(PanelHwnd id, string lname, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, int center);
  LabelBindHwnd createLabelBind(PanelHwnd id, xml_node<> *me);

  ElemHwnd createElement(PanelHwnd id, ElemHwnd elem);

  void deleteElement(ElemHwnd elemId);

  void deleteElements(PanelHwnd id);

  void deleteElements(WinHwnd winId);

  void setElements(PanelHwnd id, xml_node<> *data);

  void setElements(PanelHwnd id, string filename);

  ElemHwnd getElementById(PanelHwnd pId, string id);

  ElemHwnd getElementById(WinHwnd winId, string id);

  ElemHwnd getElementById(string id);
}