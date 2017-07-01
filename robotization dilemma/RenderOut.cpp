#include "RenderOut.h"

Graphics::WinHwnd objectMainWindow;
Graphics::PanelHwnd objectGameSubWindow;
Graphics::CanvasHwnd objectMainGameCanvas;
Graphics::CanvasHwnd objectMainGameShipCanvas;
Graphics::PanelHwnd objectMenuSubWindow;

NetworkC *Connection;

float wrapAngle(float angle)
{
  return angle - TWO_PI * floor(angle / TWO_PI);
}
void drawPointingVector(float camcx, float camcy, float camcz, float x, float y, float z, int l) {
  glLineWidth((1 + (l & 1)));

  glTranslatef(camcx, camcy, camcz);
  x -= camcx;
  y -= camcy;
  z -= camcz;

  glBegin(GL_LINES);

  float m = (1 + (l & 1)) / 2.0f;
  if (l & 16) { //16: Draw vector
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
void drawCoordinatingSystem(float camcx, float camcy, float camcz, float d) {
  glTranslatef(camcx, camcy, camcz);

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(d / 2, 0.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, d / 2, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, d / 2);

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

void renderNewRound(int id) {
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectMainGameCommitButton"))->text = "Turn " + to_string(id);
}

void createMainMenu() {
  Graphics::setElements(objectMenuSubWindow, "html/main_menu.html");
  Graphics::deleteElements(objectGameSubWindow);
}