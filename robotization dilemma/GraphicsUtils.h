#pragma once

#include "GL\freeglut.h"
#include "ClientCore.h"
#include "..\rapidxml\rapidxml.hpp"


using namespace std;

using namespace rapidxml;

typedef uint32_t colorargb;

typedef void(*ClickCallback)();
typedef void(*TextInputFunc)(string);
typedef bool(*TextValidatorFunc)(unsigned char);

typedef void(*RenderManager)();
typedef void(*ResizeManager)(int x, int y);
typedef void(*KeyManager)(unsigned char key, int x, int y);
typedef void(*SpecialKeyManager)(int key, int x, int y);
typedef void(*MouseEntryManager)(int state);
typedef void(*MouseMoveManager)(int x, int y);
typedef void(*MouseClickManager)(int idk, int key, int x, int y);
typedef void(*MouseWheelManager)(int idk, int key, int x, int y);

typedef int(*IRenderManager)(int ax, int ay, int bx, int by);
typedef int(*IResizeManager)(int x, int y);
typedef int(*IKeyManager)(unsigned char key, int x, int y, bool in);
typedef int(*ISpecialKeyManager)(int key, int x, int y, bool in);
typedef int(*IMouseEntryManager)(int state);
typedef int(*IMouseMoveManager)(int x, int y);
typedef int(*IMouseClickManager)(int idk, int key, int x, int y, bool in);
typedef int(*IMouseWheelManager)(int idk, int key, int x, int y, bool in);

int defaultIRenderManager(int ax, int ay, int bx, int by);
int defaultIResizeManager(int x, int y);
int defaultIKeyManager(unsigned char key, int x, int y, bool in);
int defaultISpecialKeyManager(int key, int x, int y, bool in);
int defaultIMouseEntryManager(int state);
int defaultIMouseMoveManager(int x, int y);
int defaultIMouseClickManager(int idk, int key, int x, int y, bool in);
int defaultIMouseWheelManager(int idk, int key, int x, int y, bool in);

struct WindowManagers {
  RenderManager renderManager;
  ResizeManager resizeManager;
  KeyManager keyManager;
  SpecialKeyManager specialKeyManager;
  MouseEntryManager mouseEntryManager;
  MouseMoveManager mouseMoveManager;
  MouseClickManager mouseClickManager;
  MouseWheelManager mouseWheelManager;
};

struct IWindowManagers {
  IRenderManager renderManager         ;//= defaultIRenderManager;
  IResizeManager resizeManager;//= defaultIResizeManager;
  IKeyManager keyManager;// = defaultIKeyManager;
  ISpecialKeyManager specialKeyManager;//= defaultISpecialKeyManager;
  IMouseEntryManager mouseEntryManager;//= defaultIMouseEntryManager;
  IMouseMoveManager mouseMoveManager;//= defaultIMouseMoveManager;
  IMouseClickManager mouseClickManager;//= defaultIMouseClickManager;
  IMouseWheelManager mouseWheelManager;//= defaultIMouseWheelManager;
};

//extern map<int, Graphics::WindowData> Graphics::windows;

class Coordiante {
public:
  float x;
  float y;
  float relx;
  float rely;
  Coordiante();
  Coordiante(float relx, float rely);
  Coordiante(float relx, float rely, float absx, float absy);
  float GetX(float WindowWidht);
  float GetY(float WindowHeight);
};

static void shapesPrintf(int row, int col, const char *fmt, ...);

void setColor(colorargb v);

void renderBitmapString(float x, float y, string text, colorargb color, bool center);

bool numericalValidator(unsigned char c);
bool textValidator(unsigned char c);