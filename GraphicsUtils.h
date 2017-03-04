#pragma once

#include "Point.h"
#include "Ship.h"
#include "Game.h"

using namespace std;

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
typedef int(*IKeyManager)(unsigned char key, int x, int y);
typedef int(*ISpecialKeyManager)(int key, int x, int y);
typedef int(*IMouseEntryManager)(int state);
typedef int(*IMouseMoveManager)(int x, int y);
typedef int(*IMouseClickManager)(int idk, int key, int x, int y);
typedef int(*IMouseWheelManager)(int idk, int key, int x, int y);

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
  IRenderManager renderManager;
  IResizeManager resizeManager;
  IKeyManager keyManager;
  ISpecialKeyManager specialKeyManager;
  IMouseEntryManager mouseEntryManager;
  IMouseMoveManager mouseMoveManager;
  IMouseClickManager mouseClickManager;
  IMouseWheelManager mouseWheelManager;
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