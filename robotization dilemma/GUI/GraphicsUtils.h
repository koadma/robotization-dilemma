#pragma once

#include "../GL/freeglut.h"
#include "../ClientCore.h"


using namespace std;

typedef uint32_t colorargb;

struct key {
  int keycode;
  bool isNormal() {
    return keycode < 256;
  }
  char toNormal() {
    assert(isNormal());
    return keycode;
  }
  int toSpecial() {
    return keycode >> 8;
  }
  void fromNormal(unsigned char key) {
    keycode = key;
  }
  void fromSpecial(int key) {
    keycode = key << 8;
  }
  string toName() {
    if (33 <= keycode && keycode <= 96) { //puncutation, uppercase
      return string(1, char(keycode));
    }
    if (97 <= keycode && keycode <= 122) { //lowercase prints as upper
      return string(1, char(keycode - 'a' + 'A'));
    }
    if (123 <= keycode && keycode <= 126) { //special
      return string(1, char(keycode));
    }
    switch (keycode) {
    case ' ':
      return "SPACE";
      break;
    case 9:
      return "TAB";
      break;
    case 8:
      return "BACKSPACE";
      break;
    case '\n':
    case '\r':
      return "ENTER";
      break;
    case 127:
      return "DELETE";
      break;
    case 27:
      return "ESC";
      break;
    }
    switch (toSpecial()) {
      //Function keys
    case GLUT_KEY_SHIFT_L:
      return "LSHIFT";
      break;
    case GLUT_KEY_SHIFT_R:
      return "RSHIFT";
      break;
    case GLUT_KEY_ALT_L:
      return "LALT";
      break;
    case GLUT_KEY_ALT_R:
      return "RALT";
      break;
    case GLUT_KEY_CTRL_L:
      return "LCTRL";
      break;
    case GLUT_KEY_CTRL_R:
      return "RCTRL";
      break;

      //Toggle keys
    case GLUT_KEY_NUM_LOCK:
      return "NUMLOCK";
      break;

    case GLUT_KEY_BEGIN:
      return "BEGIN";
      break;
    case GLUT_KEY_HOME:
      return "HOME";
      break;
    case GLUT_KEY_END:
      return "END";
      break;
    case GLUT_KEY_INSERT:
      return "INSERT";
      break;
    case GLUT_KEY_DELETE:
      return "DELETE";
      break;

    case GLUT_KEY_PAGE_DOWN:
      return "PGDOWN";
      break;
    case GLUT_KEY_PAGE_UP:
      return "PGUP";
      break;
      ///Repeat

      //Arrow
    case GLUT_KEY_LEFT:
      return "LEFT";
      break;
    case GLUT_KEY_RIGHT:
      return "RIGHT";
      break;
    case GLUT_KEY_UP:
      return "UP";
      break;
    case GLUT_KEY_DOWN:
      return "DOWN";
      break;

      //F
    case GLUT_KEY_F1:
      return "F1";
      break;
    case GLUT_KEY_F2:
      return "F2";
      break;
    case GLUT_KEY_F3:
      return "F3";
      break;
    case GLUT_KEY_F4:
      return "F4";
      break;
    case GLUT_KEY_F5:
      return "F5";
      break;
    case GLUT_KEY_F6:
      return "F6";
      break;
    case GLUT_KEY_F7:
      return "F7";
      break;
    case GLUT_KEY_F8:
      return "F8";
      break;
    case GLUT_KEY_F9:
      return "F9";
      break;
    case GLUT_KEY_F10:
      return "F10";
      break;
    case GLUT_KEY_F11:
      return "F11";
      break;
    case GLUT_KEY_F12:
      return "F12";
      break;
    }
    return "ERROR" + to_string(keycode);
  }
};

typedef void(*ClickCallback)();
typedef void(*CheckCallback)(bool);
typedef void(*TextInputFunc)(string);
typedef void(*ControlInputFunc)(key, int);
typedef void(*SliderInputFunc)(float);
typedef bool(*TextValidatorFunc)(string, int, unsigned char);

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
  IKeyManager keyManager;// = defaultIKeyManager;
  ISpecialKeyManager specialKeyManager;//= defaultISpecialKeyManager;
  IKeyManager keyUpManager;// = defaultIKeyManager;
  ISpecialKeyManager specialUpKeyManager;//= defaultISpecialKeyManager;
  IMouseEntryManager mouseEntryManager;//= defaultIMouseEntryManager;
  IMouseMoveManager mouseMoveManager;//= defaultIMouseMoveManager;
  IMouseClickManager mouseClickManager;//= defaultIMouseClickManager;
  IMouseWheelManager mouseWheelManager;//= defaultIMouseWheelManager;
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

bool numericalValidator(string s, int cursor, unsigned char c);
bool floatValidator(string s, int cursor, unsigned char c);
bool textValidator(string s, int cursor, unsigned char c);