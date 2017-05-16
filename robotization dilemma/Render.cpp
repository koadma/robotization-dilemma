#include "Render.h"

Graphics::WinHwnd objectMainWindow;
  Graphics::PanelHwnd objectGameSubWindow;
    Graphics::CanvasHwnd objectMainGameCanvas;
  Graphics::PanelHwnd objectMenuSubWindow;

NetworkC *Connection;
Ship* ship;

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
  void drawPointingVector(float x, float y, float z, int l) {
    glLineWidth((1 + (l & 1)));

    glTranslatef(camcx, camcy, camcz);
    x -= camcx;
    y -= camcy;
    z -= camcz;

    glBegin(GL_LINES);

    float m = (1 + (l & 1))/2.0f;
    if(l & 16) { //16: Draw vector
      glLineWidth(2.0f);
      glBegin(GL_LINES);
      glColor3f(1.0f*m, 1.0f*m, 1.0f*m);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(x, y, z);
      glEnd();
    }
    if (l & 10) { //2,8: Draw components from origin
      glLineWidth(2.0f);
      glBegin(GL_LINES);
      glColor3f(1.0f*m, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(x, 0.0f, 0.0f);
      glColor3f(0.0f, 1.0f*m, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, y, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f*m);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, z);
      glEnd();
    }
    if (l & 12) { //4,8: Draw components from point
      glLineWidth(2.0f);
      glBegin(GL_LINES);
      glColor3f(1.0f*m, 0.0f, 0.0f);
      glVertex3f(0.0f, y, z);
      glVertex3f(x, y, z);
      glColor3f(0.0f, 1.0f*m, 0.0f);
      glVertex3f(x, 0.0f, z);
      glVertex3f(x, y, z);
      glColor3f(0.0f, 0.0f, 1.0f*m);
      glVertex3f(x, y, 0.0f);
      glVertex3f(x, y, z);
      glEnd();
    }
    if (l & 8) { //8: Finish cube
      glLineWidth(2.0f);
      glBegin(GL_LINES);
      glColor3f(1.0f*m, 0.0f, 0.0f);
      glVertex3f(0.0f, y, 0.0f);
      glVertex3f(x, y, 0.0f);
      glVertex3f(0.0f, 0.0f, z);
      glVertex3f(x, 0.0f, z);
      glColor3f(0.0f, 1.0f*m, 0.0f);
      glVertex3f(x, 0.0f, 0.0f);
      glVertex3f(x, y, 0.0f);
      glVertex3f(0.0f, 0.0f, z);
      glVertex3f(0.0f, y, z);
      glColor3f(0.0f, 0.0f, 1.0f*m);
      glVertex3f(x, 0.0f, 0.0f);
      glVertex3f(x, 0.0f, z);
      glVertex3f(0.0f, y, 0.0f);
      glVertex3f(0.0f, y, z);
      glEnd();
    }

    glTranslatef(-camcx, -camcy, -camcz);
  }
  void drawCoordinatingSystem() {
    glTranslatef(camcx, camcy, camcz);

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(d/2, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, d/2, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, d/2);

    glColor3f(0.3f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-d / 2, 0.0f, 0.0f);
    glColor3f(0.0f, 0.3f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -d / 2, 0.0f);
    glColor3f(0.0f, 0.0f, 0.3f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -d / 2);
    glEnd();

    glTranslatef(-camcx, -camcy, -camcz);
  }
  int renderManager(int ax, int ay, int bx, int by) {

    glViewport(ax, ay, bx-ax, by-ay);
    glClear(GL_DEPTH_BUFFER_BIT);
       

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (bx - ax) / float(by - ay), 1, 20000000);
    gluLookAt(
      camcx + d * cos(camphi) * cos(camtheta), camcy + d * sin(camtheta), camcz + d * sin(camphi) * cos(camtheta),
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

    drawCoordinatingSystem();
    
    drawPointingVector(2, 3.4, 5, 4);

    drawPointingVector(5, 2, 1.1, 5);

    drawPointingVector(4.2, 2.4, 4.2, 4);

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

    Graphics::resetViewport();

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
      camcx -= 0.001f * d * (dx * sin(camphi) - dy * sin(camtheta) * cos(camphi));
      camcy -= 0.001f * d * (dy * cos(camtheta));
      camcz -= 0.001f * d * (- dx * cos(camphi) - dy * sin(camtheta) * sin(camphi));
      res = 1;
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

void newRound(int id) {

}

void recivePacket(unsigned char* data, int id, int dataLan, NetworkC* client, Ship* ship) {
  switch (id) {
    case PacketLogin:
      if (strTo<int>(string(reinterpret_cast<char*>(data))) == LoginErrorOk) {
        Connection->ConnectedShip = ship = new Ship();
        objectMainGameCanvas = Graphics::createCanvas(
          objectGameSubWindow,
          "objectMainGameCanvas",
          Coordiante{ 0.0f, 1.0f, 0.0f, 0.0f },
          Coordiante{ 1.0f, 0.0f, 0.0f, 0.0f },
          IWindowManagers{
            MainGameCanvas::renderManager,
            MainGameCanvas::resizeManager,
            MainGameCanvas::keyManager,
            MainGameCanvas::specialKeyManager,
            MainGameCanvas::mouseEntryManager,
            MainGameCanvas::mouseMoveManager, 
            MainGameCanvas::mouseClickManager,
            MainGameCanvas::mouseWheelManager
          }
        );
      }
      else {
        createMainMenu();
      }
    break;
    case PacketNewRound:
      newRound(strTo<int>(string(reinterpret_cast<char*>(data))));
    case PacketGameOver:
      createMainMenu();
    break;
    case PacketCommand:
      //createMainMenu();
    break;
    case PacketSensor:
      //createMainMenu();
    break;
    case PacketShipData:
      //createMainMenu();
    break;
  }
}

void ingameMenuExitButton() {
  Graphics::deleteElements(objectGameSubWindow);
  Graphics::deleteElements(objectMenuSubWindow);

  createMainMenu();
}

void joinMenuInput(string inp) {

}

void joinMenuInputButton() {
  Graphics::deleteElements(objectMenuSubWindow);

  Connection = new NetworkC(
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuIpInput"))->text,
    reinterpret_cast<Graphics::TextInputHwnd>(Graphics::getElementById("objectJoinMenuPortInput"))->text,
    recivePacket
  );


}

void gameMenuJoinButton() {
  Graphics::setElements(objectMenuSubWindow, "html/join_menu.html");
}

void mainMenuPlayButton() {
  Graphics::setElements(objectMenuSubWindow, "html/game_menu.html");
}

void mainMenuExitButton() {
  glutExit();
}

void createMainMenu() {
  Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
  Graphics::deleteElements(objectGameSubWindow);
}

int InitWindow() {
  Graphics::setName("mainMenuPlayButton", mainMenuPlayButton);
  Graphics::setName("mainMenuExitButton", mainMenuExitButton);
  Graphics::setName("gameMenuJoinButton", gameMenuJoinButton);
  Graphics::setName("joinMenuInputButton", joinMenuInputButton);
  Graphics::setName("ingameMenuExitButton", ingameMenuExitButton);
  Graphics::setName("joinMenuInput", joinMenuInput);

  Graphics::setName("textValidator", textValidator);
  Graphics::setName("numericalValidator", numericalValidator);

  objectMainWindow = Graphics::CreateMainWindow(200, 200, 800, 600, "Game");
  objectMenuSubWindow = Graphics::createPanel(objectMainWindow, "objectMenuSubWindow", Coordiante{ 0.0f, 1.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 0.0f, 0.0f, 0.0f }, ElementBackColor);
  objectGameSubWindow = Graphics::createPanel(objectMainWindow, "objectGameSubWindow", Coordiante{ 0.0f, 1.0f, 0.0f, 0.0f }, Coordiante{ 1.0f, 0.0f, 0.0f, 0.0f }, ElementBackColor);
  createMainMenu();
  return 0;
}