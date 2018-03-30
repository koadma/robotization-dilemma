#pragma once

#include "../../Core/Scripts/Scripts.h"
#include "Button.h"
#include "Checkbox.h"
#include "TextInput.h"
#include "Label.h"
#include "LabelBind.h"
#include "Panel.h"
#include "Canvas.h"
#include "Plot.h"
#include "Slider.h"
#include "Container.h"
#include "Table.h"
#include "Image.h"
#include "ControlSetting.h"
#include "Popup.h"

namespace Graphics {
  
  void defaultRenderManager();
  void defaultResizeManager(int x, int y);
  void defaultKeyManager(unsigned char key, int x, int y);
  void defaultSpecialKeyManager(int key, int x, int y);
  void defaultKeyUpManager(unsigned char key, int x, int y);
  void defaultSpecialKeyUpManager(int key, int x, int y);
  void defaultMouseEntryManager(int state);
  void defaultMouseMoveManager(int x, int y);
  void defaultMouseClickManager(int idk, int key, int x, int y);
  void defaultMouseWheelManager(int idk, int key, int x, int y);

  class GWindow {
  public:
    Coordinate minc, maxc;
    float ax, ay, bx, by;
    int id;
    WindowManagers windowManagers;
    int parent;
    Panel* myPanel;
    void getWin(float pax, float pay, float pbx, float pby);
  };

  typedef GUIElement* ElemHwnd;
  typedef Panel* PanelHwnd;
  typedef Container* ContainerHwnd;
  typedef Button* ButtonHwnd;
  typedef TextInput* TextInputHwnd;
  typedef Label* LabelHwnd;
  typedef LabelBind* LabelBindHwnd;
  typedef Canvas* CanvasHwnd;
  typedef Plot* PlotHwnd;
  typedef Slider* SliderHwnd;
  typedef Checkbox* CheckboxHwnd;
  typedef ControlSetting* ControlHwnd;
  typedef Table* TableHwnd;
  typedef TableRow* TablerowHwnd;
  typedef Image* ImageHwnd;
  typedef GWindow* WinHwnd;

  extern WindowManagers defaultWindowManagers;
  extern map<int, WinHwnd> windows;

  class GLocation {
    int x;
    int y;
    WinHwnd window;
  };

  extern map<string, void(*)()> funcs;
  extern set<key_location> keysdown;

  WinHwnd CreateMainWindow(int x = 40, int y = 40, int width = 640, int height = 480, string caption = "", WindowManagers managers = defaultWindowManagers);

  WinHwnd SetUpWindow(int id, int parent, Coordinate x, Coordinate y, WindowManagers manager);

  int DestroyWindow(WinHwnd id);

  WinHwnd GetWinHwnd(int id);

  int elementMouseEnterManager(WinHwnd id, int mstate);

  int elementMouseMoveManager(WinHwnd id, int x, int y);

  int elementGUIEventManager(WinHwnd id, gui_event evt, int mx, int my, set<key_location>& down);

  void elementResizeManager(WinHwnd id, int width, int height);

  void elementResizeManager(PanelHwnd id, int width, int height);

  void elementRenderManager(WinHwnd id);

  template <typename T> void setName(string name, T func) {
    funcs[name] = reinterpret_cast<void(*)()>(func);
  }

  ButtonHwnd createButton(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, key trigger, ClickCallback clickCallback);
  ButtonHwnd createButton(xml_node<> *me);

  CheckboxHwnd createCheckbox(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, bool checked, CheckCallback checkCallback);
  CheckboxHwnd createCheckbox(xml_node<> *me);
  
  LabelHwnd createLabel(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);
  LabelHwnd createLabel(xml_node<> *me);

  ImageHwnd createImage(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int align);
  ImageHwnd createImage(xml_node<> *me);

  TextInputHwnd createTextInput(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  TextInputHwnd createTextInput(xml_node<> *me);

  ControlHwnd createControl(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, key selected, int id, ControlInputFunc inputCallback);
  ControlHwnd createControl(xml_node<> *me);

  CanvasHwnd createCanvas(string lname, Coordinate mincorner, Coordinate maxcorner, IWindowManagers managers);

  PlotHwnd createPlot(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor);

  PanelHwnd createPanel(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg);
  PanelHwnd createPanel(xml_node<> *me);

  ContainerHwnd createContainer(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg);
  ContainerHwnd createContainer(xml_node<> *me);

  SliderHwnd createSlider(string name, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, colorargb pulledcolor, float min, float max, float value, float quanta, SliderInputFunc clickCallback);
  SliderHwnd createSlider(xml_node<> *me);

  LabelBindHwnd createLabelBind(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active, colorargb textColor, int center);
  LabelBindHwnd createLabelBind(xml_node<> *me);

  TableHwnd createTable(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg, colorargb active);
  TableHwnd createTable( xml_node<> *me);

  TablerowHwnd createTableRow(string lname, Coordinate mincorner, Coordinate maxcorner, colorargb bg);
  TablerowHwnd createTableRow(xml_node<> *me);

  ElemHwnd addElement(WinHwnd id, ElemHwnd elem);
  ElemHwnd addElement(PanelHwnd id, ElemHwnd elem);
  ElemHwnd addElement(TableHwnd id, TablerowHwnd elem);
  ElemHwnd addElement(TablerowHwnd id, ElemHwnd elem);

  ElemHwnd createElement(xml_node<> *me);

  void deleteElement(ElemHwnd elemId);

  void deleteElements(PanelHwnd id);
  void deleteElements(TableHwnd id);
  void deleteElements(TablerowHwnd id);
  
  void deleteElements(WinHwnd winId);

  void setElements(PanelHwnd id, xml_node<> *data);
  void setElements(TableHwnd id, xml_node<> *data);
  void setElements(TablerowHwnd id, xml_node<> *data);

  void setElements(PanelHwnd id, string filename);

  ElemHwnd getElementById(PanelHwnd pId, string id);

  ElemHwnd getElementById(WinHwnd winId, string id);

  ElemHwnd getElementById(string id);
}