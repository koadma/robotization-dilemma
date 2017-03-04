#pragma once

#include "Button.h"
#include "TextInput.h"
#include "Label.h"
#include "Panel.h"

using namespace std;

namespace Graphics {
  typedef void(*RenderManager)();
  typedef void(*ResizeManager)(int x, int y);
  typedef void(*KeyManager)(unsigned char key, int x, int y);
  typedef void(*SpecialKeyManager)(int key, int x, int y);
  typedef void(*MouseEntryManager)(int state);
  typedef void(*MouseMoveManager)(int x, int y);
  typedef void(*MouseClickManager)(int idk, int key, int x, int y);
  typedef void(*MouseWheelManager)(int idk, int key, int x, int y);
  
  void defaultRenderManager();
  void defaultResizeManager(int x, int y);
  void defaultKeyManager(unsigned char key, int x, int y);
  void defaultSpecialKeyManager(int key, int x, int y);
  void defaultMouseEntryManager(int state);
  void defaultMouseMoveManager(int x, int y);
  void defaultMouseClickManager(int idk, int key, int x, int y);
  void defaultMouseWheelManager(int idk, int key, int x, int y);

  struct WindowManagers {
    RenderManager renderManager             = defaultRenderManager;
    ResizeManager resizeManager             = defaultResizeManager;
    KeyManager keyManager                   = defaultKeyManager;
    SpecialKeyManager specialKeyManager     = defaultSpecialKeyManager;
    MouseEntryManager mouseEntryManager     = defaultMouseEntryManager;
    MouseMoveManager mouseMoveManager       = defaultMouseMoveManager;
    MouseClickManager mouseClickManager     = defaultMouseClickManager;
    MouseWheelManager mouseWheelManager     = defaultMouseWheelManager;
  };
  
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

  typedef GUIElement* ElemHwnd;
  typedef Panel* PanelHwnd;
  typedef Button* ButtonHwnd;
  typedef TextInput* TextInputHwnd;
  typedef Label* LabelHwnd;
  typedef Graphics::GWindow* WinHwnd;

  WinHwnd CreateMainWindow(int x = 40, int y = 40, int width = 640, int height = 480, string caption = "", WindowManagers managers = defaultWindowManagers);

  WinHwnd SetUpWindow(int id, int parent, Coordiante x, Coordiante y, WindowManagers manager);

  int DestroyWindow(WinHwnd id);

  WinHwnd GetWinHwnd(int id);

  bool elementMouseEnterManager(WinHwnd id, int mstate);

  bool elementMouseMoveManager(WinHwnd id, int x, int y);

  bool elementMouseClickManager(WinHwnd id, int x, int y);

  bool elementMouseWheelManager(WinHwnd id, int a, int b, int x, int y);

  bool elementKeyPressManager(WinHwnd id, unsigned char kay, int x, int y);

  bool elementSpecialPressManager(WinHwnd id, int key, int x, int y);

  void elementResizeManager(WinHwnd id, int width, int height);

  void elementResizeManager(PanelHwnd id, int width, int height);

  void elementRenderManager(WinHwnd id);

  ButtonHwnd createButton(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback);
  ButtonHwnd createButton(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback);

  LabelHwnd createLabel(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);
  LabelHwnd createLabel(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);

  TextInputHwnd createTextInput(WinHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  TextInputHwnd createTextInput(PanelHwnd id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  
  ElemHwnd createElement(PanelHwnd id, ElemHwnd elem);

  void deleteElement(ElemHwnd elemId);

  void deleteElements(PanelHwnd id);

  void deleteElements(WinHwnd winId);
}