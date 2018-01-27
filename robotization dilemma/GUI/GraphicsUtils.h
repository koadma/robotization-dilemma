#pragma once

#include "../GL/freeglut.h"
#include "../ClientCore.h"
#include "../stb_image.h"

using namespace std;

typedef uint32_t colorargb;

extern map<string, map<string, colorargb>> colors;

colorargb getColor(string object, string key);
colorargb getColor(string val);
colorargb getColor(xml_node<>* me, string elem, string key);

void loadColors(string filename = "html/colors.cfg");

class Canvas;

bool operator==(const key& lhs, const key& rhs);

bool operator<(const key& lhs, const key& rhs);

bool operator<=(const key& lhs, const key& rhs);

bool operator>(const key& lhs, const key& rhs);

bool operator>=(const key& lhs, const key& rhs);


class key {
public:
  enum type {
    type_key = 0,
    type_special = 1,
    type_mouse = 2,
    type_wheel = 3
  };
  int _type;
  int _keycode;
  bool isKey();
  void fromKey(unsigned char key);
  void fromSpecial(int key);
  void fromMouse(int button);
  void fromWheel(int delta);
  string toKeyName();
  string toSpecialName();
  string toMouseName();
  string toWheelName();
  string toName();
  key(int keycode, int type);
  key(string keycode);
  key();
};

struct key_location : public key {
public:
  int _mx;
  int _my;
  void setLocation(int mx, int my);
  key_location(int keycode, int type, int mx, int my);
  key_location(key k) {
    _keycode = k._keycode;
    _type = k._type;
  }
  key_location();
};

struct gui_event {
  key_location _key;
  enum type {
    evt_none = 0,
    evt_down = 1,
    evt_pressed = 2,
    evt_up = 3
  };
  type _type;
  string toName();
  gui_event(key_location key, type type);
};

typedef void(*ClickCallback)();
typedef void(*CheckCallback)(bool);
typedef void(*TextInputFunc)(string);
typedef void(*ControlInputFunc)(key, int);
typedef void(*SliderInputFunc)(float);
typedef bool(*TextValidatorFunc)(string, int, unsigned char);

typedef void(*RenderManager)();
typedef void(*ResizeManager)(int x, int y);
typedef void(*GUIEventManager)(gui_event evt, int x, int y, set<key_location>& down);
typedef void(*KeyManager)(unsigned char key, int x, int y);
typedef void(*SpecialKeyManager)(int key, int x, int y);
typedef void(*MouseClickManager)(int idk, int key, int x, int y);
typedef void(*MouseWheelManager)(int idk, int key, int x, int y);
typedef void(*MouseEntryManager)(int state);
typedef void(*MouseMoveManager)(int x, int y);

typedef int(*IRenderManager)(int ax, int ay, int bx, int by);
typedef int(*IResizeManager)(int x, int y);
typedef int(*IGUIEventManager)(gui_event evt, int x, int y, set<key_location>& down, Canvas* me);
typedef int(*IMouseEntryManager)(int state);
typedef int(*IMouseMoveManager)(int x, int y);

int defaultIRenderManager(int ax, int ay, int bx, int by);
int defaultIResizeManager(int x, int y);
int defaultIGUIEventManager(gui_event evt, int x, int y, set<key_location>& down);
int defaultIMouseEntryManager(int state);
int defaultIMouseMoveManager(int x, int y);

struct OpenGLData {
  GLdouble model_view[16];
  GLdouble projection[16];
  GLint viewport[4];
  vec3<double> cameraEye;
};

struct WindowManagers {
  RenderManager renderManager;
  ResizeManager resizeManager;
  KeyManager keyManager;
  SpecialKeyManager specialKeyManager;
  KeyManager keyUpManager;
  SpecialKeyManager specialUpKeyManager;
  MouseEntryManager mouseEntryManager;
  MouseMoveManager mouseMoveManager;
  MouseClickManager mouseClickManager;
  MouseWheelManager mouseWheelManager;
};

struct IWindowManagers {
  IRenderManager renderManager         ;//= defaultIRenderManager;
  IResizeManager resizeManager;//= defaultIResizeManager;
  IGUIEventManager guiEventManager;// = defaultIKeyManager;
  IMouseEntryManager mouseEntryManager;//= defaultIMouseEntryManager;
  IMouseMoveManager mouseMoveManager;//= defaultIMouseMoveManager;
};

//extern map<int, Graphics::WindowData> Graphics::windows;

namespace Graphics {

  void resetViewport();

}

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

void renderBitmapString(float x, float y, string text, colorargb color, bool center, int cursor = -1);

GLuint png_texture_load(string filename, int& w, int& h);

bool numericalValidator(string s, int cursor, unsigned char c);
bool floatValidator(string s, int cursor, unsigned char c);
bool textValidator(string s, int cursor, unsigned char c);