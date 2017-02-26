#pragma once

#include "Button.h"
#include "TextInput.h"
#include "Label.h"

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

  struct WindowData {
    WindowManagers windowManagers;
    int parent;
    list<int> children;
    map<int, GUIElement*> elements;
    int maxbutton = 0;
  };

  extern WindowManagers defaultWindowManagers;
  extern map<int, WindowData> windows;

  int CreateMainWindow(int x = 40, int y = 40, int width = 640, int height = 480, string caption = "", WindowManagers managers = defaultWindowManagers);

  int CreateSubWindow(int parent, int x = 0, int y = 0, int width = 100, int height = 100, WindowManagers managers = defaultWindowManagers);

  void SetUpWindow(int id, int parent, WindowManagers manager);

  int DestroyWindow(int id);

  bool elementMouseEnterManager(int id, int mstate);

  bool elementMouseMoveManager(int id, int x, int y);

  bool elementMouseClickManager(int id, int x, int y);

  bool elementMouseWheelManager(int id, int a, int b, int x, int y);

  bool elementKeyPressManager(int id, unsigned char kay, int x, int y);

  bool elementSpecialPressManager(int id, int key, int x, int y);

  void elementResizeManager(int id, int width, int height);

  void elementRenderManager(int id);

  int createButton(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, ClickCallback clickCallback);

  int createLabel(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, int center);

  int createTextInput(int id, Coordiante mincorner, Coordiante maxcorner, colorargb bg, colorargb active, colorargb textColor, string text, TextInputFunc inputCallback, TextValidatorFunc validator = *textValidator);
  
  void deleteElement(int winId, int elemId);

  void deleteElements(int winId);
}