#include "Render.h"

Graphics::WinHwnd objectMainWindow;
  Graphics::PanelHwnd objectMenuSubWindow;
    Graphics::ButtonHwnd objectMainMenuPlayButton;
    Graphics::ButtonHwnd objectPlayMenuNewButton;
    Graphics::TextInputHwnd objectGameMenuPlayerCountInput;
  Graphics::PanelHwnd object3DGameWindow;
    Graphics::CanvasHwnd objectMainGameCanvas;
//

Game* game;



namespace MainGameCanvas{
  float camcx = 0, camcy = 0, camcz = 0;
  float camphi = 0, camtheta = 0; //phi: x-z, from x, positive to z. theta: from xz to y.

  inline float wrapAngle(float angle)
  {
    return angle - TWO_PI * floor(angle / TWO_PI);
  }

  void normalizeAngles() {

    if(camtheta > HALF_PI) {
      camtheta = HALF_PI;
    }
    if (camtheta < -HALF_PI) {
      camtheta = -HALF_PI;
    }
    camphi = wrapAngle(camphi + PI) - PI; //between -180 and 180
    //cout << camphi << " " << camtheta << endl;
  }
  float d = 10;
  int mousebuttons = 0; //left, center, right
  int mxold;
  int myold;
  int renderManager(int ax, int ay, int bx, int by) {

    glViewport(ax, ay, bx-ax, by-ay);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (bx - ax) / float(by - ay), 1, 20000000);
    gluLookAt(
      camcx + d * cos(camphi) * cos(camtheta), camcz + d * sin(camtheta), camcy + d * sin(camphi) * cos(camtheta),
      camcx, camcy, camcz,
      -cos(camphi) * sin(camtheta), cos(camtheta), -sin(camphi) * sin(camtheta)
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

                                    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(1.5f, 0.0f, 0.0f);  // Move right and into the screen

    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
                                      // Top face (y = 1.0f)
                                      // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube

              // Render a pyramid consists of 4 triangles
    glLoadIdentity();                  // Reset the model-view matrix
    glTranslatef(-1.5f, 0.0f, 0.0f);  // Move left and into the screen

    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
                                     // Front
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left
    glColor3f(1.0f, 0.0f, 0.0f);       // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);       // Blue
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);       // Green
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid

    glFlush();
    return 0;
  }
  int resizeManager(int x, int y) {
    return 0;
  }
  int keyManager(unsigned char key, int x, int y) {
    if (key == 'w') {
      camtheta += 0.2;
    }
    if (key == 'a') {
      camphi += 0.2;
    }
    if (key == 's') {
      camtheta -= 0.2;
    }
    if (key == 'd') {
      camphi -= 0.2;
    }
    //glutPostRedisplay();
    return 3;
  }
  int specialKeyManager(int key, int x, int y) {
    return 0;
  }
  int mouseEntryManager(int state) {
    mousebuttons = 0;
    return 0;
  }
  int mouseMoveManager(int x, int y) {
    int dx = x - mxold;
    int dy = y - myold;

    int res = 0;

    if (mousebuttons & 1) { //left, drag
      res = 0;
    }
    if (mousebuttons & 4) { //right, rotate
      camphi += dx / 100.0f;
      camtheta -= dy / 100.0f;
      normalizeAngles();
      res = 1;
    }

    mxold = x;
    myold = y;
    return res;
  }
  int mouseClickManager(int button, int state, int x, int y) {
    mousebuttons ^= mousebuttons & (1 << button); //remove bit for button;
    mousebuttons ^= (state ^ 1) << button;
    //cout << mousebuttons << endl;
    return 0;
  }
  int mouseWheelManager(int idk, int key, int x, int y) {
    d *= 1.0f - (float(key)/15.0f);
    d = max(d, 1.0f);
    return 1;
  }
}

void newMenuPlayerCountInput(string inp) {
    Graphics::deleteElements(objectMenuSubWindow);
    game = new Game(atoi(inp.c_str()));
    createMainMenu();
}

void newMenuPlayerCountInputButton() {
  newMenuPlayerCountInput(objectGameMenuPlayerCountInput->text);
}

void gameMenuNewButton() {
  Graphics::deleteElements(objectMenuSubWindow);
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  Graphics::createLabel(objectMenuSubWindow, Coordiante{ 0.3f, 0.5f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.4f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Player Count", 1);
  objectGameMenuPlayerCountInput = Graphics::createTextInput(objectMenuSubWindow, Coordiante{ 0.3f, 0.6f, 5.0f, 5.0f }, Coordiante{ 0.6f, 0.5f, 0.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "0", *newMenuPlayerCountInput, *numericalValidator);
  Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.6f, 0.6f, 0, 5.0f }, Coordiante{ 0.7f, 0.5f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Ok", *newMenuPlayerCountInputButton);
  //runGame();
}

void mainMenuPlayButton() {
  Graphics::deleteElements(objectMenuSubWindow);
  objectPlayMenuNewButton = Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.3f, 0.55f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.45f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "New Game", *gameMenuNewButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void mainMenuExitButton() {
  Graphics::deleteElement(objectMainMenuPlayButton);
  glutExit();
  //objectPlayMenuNewButton = Graphics::createButton(objectMainWindow, 10, 10, 100, 50, 0xffff0000, 0xff00ff00, 0xff0000ff, "New Game", *mainMenuPlayButton);
  //Graphics::createTextInput(objectMainWindow, 10, 70, 100, 50, 0xff00ffff, 0xffff00ff, 0xffffff00, "DemoText", *numericalValidator);
  //runGame();
}

void createMainMenu() {
  objectMainMenuPlayButton = Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.3f, 0.5f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.4f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Play", *mainMenuPlayButton);
  objectMainMenuPlayButton = Graphics::createButton(objectMenuSubWindow, Coordiante{ 0.3f, 0.6f, 5.0f, 5.0f }, Coordiante{ 0.7f, 0.5f, -5.0f, -5.0f }, ElementBaseColor, ElementActiveColor, ElementTextColor, "Exit", *mainMenuExitButton);
  objectMainGameCanvas = Graphics::createCanvas(object3DGameWindow, Coordiante{ 0.0f, 1.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 0.0f, 0.0f, 0.0f }, IWindowManagers{MainGameCanvas::renderManager, MainGameCanvas::resizeManager, MainGameCanvas::keyManager, MainGameCanvas::specialKeyManager, MainGameCanvas::mouseEntryManager, MainGameCanvas::mouseMoveManager, MainGameCanvas::mouseClickManager, MainGameCanvas::mouseWheelManager});
}

int InitWindow() {
  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, Coordiante{ 0.0f, 1.0f, 0.0f, 0.0f }, Coordiante{ 0.5f, 0.0f, 0.0f, 0.0f }, ElementBackColor);
  object3DGameWindow = Graphics::createPanel(objectMainWindow, Coordiante{ 0.5f, 1.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 0.0f, 0.0f, 0.0f }, ElementBackColor);
  createMainMenu();
  return 0;
}