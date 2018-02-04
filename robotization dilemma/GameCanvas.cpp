#include "GameCanvas.h"


float MainGameCanvas::camcx = 0, MainGameCanvas::camcy = 0, MainGameCanvas::camcz = 0; //World coordiante center
float MainGameCanvas::camphi = 0, MainGameCanvas::camtheta = 0; //phi: x-z, from x, positive to z. theta: from xz to y.
float MainGameCanvas::d = 100000;
int MainGameCanvas::mxold;
int MainGameCanvas::myold;
int MainGameCanvas::mousebuttons = 0; //left, center, right

OpenGLData MainGameCanvas::view;

void MainGameCanvas::normalizeAngles() {

  if (camtheta > HALF_PI-0.01) {
    camtheta = HALF_PI - 0.01;
  }
  if (camtheta < -HALF_PI + 0.01) {
    camtheta = -HALF_PI + 0.01;
  }
  camphi = wrapAngle(camphi + PI) - PI; //between -180 and 180
                                        //cout << camphi << " " << camtheta << endl;
}
int MainGameCanvas::renderManager(int ax, int ay, int bx, int by) {

  glViewport(ax, ay, bx - ax, by - ay);
  glClear(GL_DEPTH_BUFFER_BIT);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  view.cameraEye = { camcx/d + cos(camphi) * cos(camtheta), camcy / d + sin(camtheta), camcz / d + sin(camphi) * cos(camtheta) };

  gluPerspective(60.0, (bx - ax) / float(by - ay), 0.1, 1000);
  gluLookAt(
    view.cameraEye.x, view.cameraEye.y, view.cameraEye.z,
    camcx/d, camcy/d, camcz/d,
    -cos(camphi) * sin(camtheta), cos(camtheta), -sin(camphi) * sin(camtheta)
    );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

  glGetDoublev(GL_MODELVIEW_MATRIX, view.model_view);
  glGetDoublev(GL_PROJECTION_MATRIX, view.projection);
  glGetIntegerv(GL_VIEWPORT, view.viewport);

  drawCoordinatingSystem(camcx/d, camcy/d, camcz/d, 2);
  drawXZPlane(camcx/d, camcy/d, camcz/d, 0.1, 10);

  //drawPointingVector(camcx/d, camcy / d, camcz / d, 2 / d, 3.4 / d, 5 / d, 5);

  /*drawPointingVector(camcx, camcy, camcz, 5, 2, 1.1, 5);

  drawPointingVector(camcx, camcy, camcz, 4.2, 2.4, 4.2, 4);

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
  glEnd();   // Done drawing the pyramid*/

  ship->drawSightings(camcx, camcy, camcz, d, view);

  ship->drawObjects(camcx, camcy, camcz, d, true);

  //glTranslatef(0.0f, 0.0f, 0.0f); //OpenGL needs this
  glFlush();

  Graphics::resetViewport();

  return 0;
}
int MainGameCanvas::resizeManager(int x, int y) {
  return 0;
}
int MainGameCanvas::keyManager(unsigned char key, int x, int y, bool in) {
  if(in) {
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
    if (key == 'c') {
      mVec3 m = ship->mov.getAt(timeNow).pos;
      camcx = m.x;
      camcy = m.y;
      camcz = m.z;
    }
    if (key == 'o') {
      camcx = 0;
      camcy = 0;
      camcz = 0;
    }
    if (key == 'v' && selecteds) {
      mVec3 m = selecteds->getAt(timeNow).pos;
      camcx = m.x;
      camcy = m.y;
      camcz = m.z;
    }
    //glutPostRedisplay();
    return 3;
  }
  else {
    return 0;
  }
}
int MainGameCanvas::specialKeyManager(int key, int x, int y, bool in) {
  return 0;
}
int MainGameCanvas::keyUpManager(unsigned char key, int x, int y, bool in) {
  return 0;
}
int MainGameCanvas::specialKeyUpManager(int key, int x, int y, bool in) {
  return 0;
}
int MainGameCanvas::mouseEntryManager(int state) {
  mousebuttons = 0;
  return 0;
}
int MainGameCanvas::mouseMoveManager(int x, int y) {
  int dx = x - mxold;
  int dy = y - myold;

  int res = 0;

  if (mousebuttons & 1) { //left, drag
    camcx -= 0.001f * d * (dx * sin(camphi) - dy * sin(camtheta) * cos(camphi));
    camcy -= 0.001f * d * (dy * cos(camtheta));
    camcz -= 0.001f * d * (-dx * cos(camphi) - dy * sin(camtheta) * sin(camphi));
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
int MainGameCanvas::mouseClickManager(int button, int state, int x, int y, bool in) {
  if(in) {
    mousebuttons ^= mousebuttons & (1 << button); //remove bit for button;
    mousebuttons ^= (state ^ 1) << button;
    if (button == 0 && state == 0) { //left down
      GLdouble pos3D_ax = 0, pos3D_ay = 0, pos3D_az = 0;

      // get 3D coordinates based on window coordinates

      gluUnProject(x, y, 0,
        view.model_view, view.projection, view.viewport,
        &pos3D_ax, &pos3D_ay, &pos3D_az);

      vec3<double> rayori = { pos3D_ax, pos3D_ay, pos3D_az };
      vec3<double> raydir = rayori - view.cameraEye;

      ship->selectSighting(rayori, raydir, d);
    }
  }
  else {
    mousebuttons = 0;
  }
  //cout << mousebuttons << endl;
  return 0;
}
int MainGameCanvas::mouseWheelManager(int idk, int key, int x, int y, bool in) {
  if(in) {
    d *= 1.0f - (float(key) / 15.0f);
    d = max(d, 1.0f);
    return 1;
  }
  return 0;
}

float MainGameShipCanvas::camcx = 0, MainGameShipCanvas::camcy = 0, MainGameShipCanvas::camcz = 0;
float MainGameShipCanvas::camphi = HALF_PI, MainGameShipCanvas::camtheta = QUARTER_PI; //phi: x-z, from x, positive to z. theta: from xz to y.
float MainGameShipCanvas::d = 1000;
int MainGameShipCanvas::mxold;
int MainGameShipCanvas::myold;
int MainGameShipCanvas::mousebuttons = 0; //left, center, right

OpenGLData MainGameShipCanvas::view;

void MainGameShipCanvas::normalizeAngles() {

  if (camtheta > HALF_PI) {
    camtheta = HALF_PI;
  }
  if (camtheta < -HALF_PI) {
    camtheta = -HALF_PI;
  }
  camphi = wrapAngle(camphi + PI) - PI; //between -180 and 180
                                        //cout << camphi << " " << camtheta << endl;
}
int MainGameShipCanvas::renderManager(int ax, int ay, int bx, int by) {

  glViewport(ax, ay, bx - ax, by - ay);
  glClear(GL_DEPTH_BUFFER_BIT);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  view.cameraEye = { camcx + d * cos(camphi) * cos(camtheta), camcy + d * sin(camtheta), camcz + d * sin(camphi) * cos(camtheta) };

  gluPerspective(60.0, (bx - ax) / float(by - ay), 1, 20000000);
  gluLookAt(
    view.cameraEye.x, view.cameraEye.y, view.cameraEye.z,
    camcx, camcy, camcz,
    -cos(camphi) * sin(camtheta), cos(camtheta), -sin(camphi) * sin(camtheta)
    );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

  glGetDoublev(GL_MODELVIEW_MATRIX, view.model_view);
  glGetDoublev(GL_PROJECTION_MATRIX, view.projection);
  glGetIntegerv(GL_VIEWPORT, view.viewport);

                                  // Render a color-cube consisting of 6 quads with different colors
  glLoadIdentity();                 // Reset the model-view matrix

  drawCoordinatingSystem(camcx, camcy, camcz, d);

  /*drawPointingVector(camcx, camcy, camcz, 2, 3.4, 5, 4);

  drawPointingVector(camcx, camcy, camcz, 5, 2, 1.1, 5);

  drawPointingVector(camcx, camcy, camcz, 4.2, 2.4, 4.2, 4);

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
  glEnd();   // Done drawing the pyramid*/

  ship->drawObjects(camcx, camcy, camcz, d, false);

  glFlush();

  Graphics::resetViewport();

  return 0;
}
int MainGameShipCanvas::resizeManager(int x, int y) {
  return 0;
}
int MainGameShipCanvas::keyManager(unsigned char key, int x, int y, bool in) {
  if (in) {
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
  else {
    return 0;
  }
}
int MainGameShipCanvas::specialKeyManager(int key, int x, int y, bool in) {
  return 0;
}
int MainGameShipCanvas::keyUpManager(unsigned char key, int x, int y, bool in) {
  return 0;
}
int MainGameShipCanvas::specialKeyUpManager(int key, int x, int y, bool in) {
  return 0;
}
int MainGameShipCanvas::mouseEntryManager(int state) {
  mousebuttons = 0;
  return 0;
}
int MainGameShipCanvas::mouseMoveManager(int x, int y) {
  int dx = x - mxold;
  int dy = y - myold;

  int res = 0;

  if (mousebuttons & 1) { //left, drag
    camcx -= 0.001f * d * (dx * sin(camphi) - dy * sin(camtheta) * cos(camphi));
    camcy -= 0.001f * d * (dy * cos(camtheta));
    camcz -= 0.001f * d * (-dx * cos(camphi) - dy * sin(camtheta) * sin(camphi));
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
int MainGameShipCanvas::mouseClickManager(int button, int state, int x, int y, bool in) {
  if (in) {
    mousebuttons ^= mousebuttons & (1 << button); //remove bit for button;
    mousebuttons ^= (state ^ 1) << button;
    if (button == 0 && state == 0) { //left down
      GLdouble pos3D_ax, pos3D_ay, pos3D_az;

      // get 3D coordinates based on window coordinates

      gluUnProject(x, y, 0.01,
        view.model_view, view.projection, view.viewport,
        &pos3D_ax, &pos3D_ay, &pos3D_az);

      vec3<double> rayori = {pos3D_ax, pos3D_ay, pos3D_az};
      vec3<double> raydir = rayori - view.cameraEye;

      ship->setSidebar(rayori, raydir);
    }
  }
  else {
    mousebuttons = 0;
  }
  return 0;
}
int MainGameShipCanvas::mouseWheelManager(int idk, int key, int x, int y, bool in) {
  if (in) {
    d *= 1.0f - (float(key) / 15.0f);
    d = max(d, 1.0f);
    return 1;
  }
  return 0;
}
