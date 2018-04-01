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

  glLineWidth(3.0f);
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
void drawXZPlane(float camcx, float camcy, float camcz, float d, int n) {
  glLineWidth(1.0f);
  setColor(0xff808080);

  for (int i = 1; i <= n; i++) {
    if(i > 1) {
      int ticks = 12*(1 << int(log2(i-2)));
      glBegin(GL_LINES);
      for(int j = 0; j<ticks; j++) {
        float angle = j*TWO_PI/ticks;
        glVertex3f(camcx + (i-1)*d*sin(angle), camcy, camcz + (i-1)*d*cos(angle));
        glVertex3f(camcx + i*d*sin(angle), camcy, camcz + i*d*cos(angle));
      }
      glEnd();
    }
    glBegin(GL_LINE_LOOP);
    for(int j = 0; j<48; j++) {
      float angle = j*TWO_PI/48;
      glVertex3f(camcx + i*d*sin(angle), camcy, camcz + i*d*cos(angle));
    }
    glEnd();
  }
}

void renderNewRound(int id) {
  reinterpret_cast<Graphics::LabelHwnd>(Graphics::getElementById("objectMainGameCommitButton"))->text = "Turn " + to_string(id);
  reinterpret_cast<Graphics::SliderHwnd>(Graphics::getElementById("objectMainGameTimeSlider"))->maxv = to_doubleT<double, time_type_s>(ROUND_TIME)*id;
  reinterpret_cast<Graphics::SliderHwnd>(Graphics::getElementById("objectMainGameTimeSlider"))->val = to_doubleT<double, time_type_s>(ROUND_TIME)*(id-1);
}

void createMainMenu() {
  Graphics::setElements(objectMenuSubWindow, "html/main_menu.xml");
  
  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectMainMenuVersion"))->text =
    new TextBind<
    TextBindVar<int>,
    TextBindVar<int>,
    TextBindVar<int>
    >("V%.%.%",
      TextBindVar<int>(&VersionA),
      TextBindVar<int>(&VersionB),
      TextBindVar<int>(&VersionC)
      );

  Graphics::deleteElements(objectGameSubWindow);
}

void createReconnectScreen() {
  Graphics::setElements(objectMenuSubWindow, "html/reconnect_screen.xml");
}

void createSettings(Graphics::TableHwnd& table) {
  for (auto&& it : table->data) {
    it->toDelete = true;
  }
  int i = 0;
  for (auto&& it : keybinds) {
    Graphics::TablerowHwnd row  = Graphics::createTableRow("objectKeybindRow"   + to_string(i), LocationData(LinearScale(0, 0), LinearScale(0,30), LinearScale(0,0), LinearScale(1,0)), getColor("tablerow", "bgcolor"));
    Graphics::LabelHwnd    name = Graphics::createLabel   ("objectKeybindLabel" + to_string(i), LocationData(LinearScale(0, 5), LinearScale(0, 25), LinearScale(0, 0), LinearScale(0.9, -50)), getColor("label", "bgcolor"), getColor("label", "activecolor"), getColor("label", "textcolor"), it.second, 1);
    Graphics::ControlHwnd  ctrl = Graphics::createControl ("objectKeybindInput" + to_string(i), LocationData(LinearScale(0, 5), LinearScale(0, 25), LinearScale(0.9, -45), LinearScale(1, -5)), getColor("control", "bgcolor"), getColor("control", "activecolor"), getColor("control", "textcolor"), it.first, i, keybindReply);
    Graphics::addElement(row, name);
    Graphics::addElement(row, ctrl);
    Graphics::addElement(table, row);
    ++i;
  }
}