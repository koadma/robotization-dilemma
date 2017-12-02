#pragma once

#include "GUIElement.h"

class Plot : public GUIElement {
public:
  Plot(string lname, Coordiante lmincorner, Coordiante lmaxcorner) :
    GUIElement(lname, lmincorner, lmaxcorner, 0, 0, hexToInt("ff00ff00")) {
  }
  virtual int mouseEnter(int state) {
    return 0;
  }
  virtual int mouseMoved(int mx, int my) {
    return 0;
  }
  virtual int mouseClicked(int button, int state, int mx, int my) {
    return 0;
  }
  virtual int keyPressed(unsigned char key, int mx, int my) {
    return 0;
  }
  virtual int specialPressed(int key, int mx, int my) {
    return 0;
  }
  virtual int mouseWheel(int a, int b, int mx, int my) {
    return 0;
  }
  virtual void render() {
  }
};

//keyframe type
template<typename T, typename V, typename U>
class PlotT : public Plot {
public:
  keyframe<T, V, U>* frames;
  double ox, oy; //in plot coords, for center of plot
  double sx, sy; //data / plotwidth
  
  PlotT(string lname, Coordiante lmincorner, Coordiante lmaxcorner, keyframe<T, V, U>* lframes) :
    Plot(lname, lmincorner, lmaxcorner) {
    frames = lframes;
    ox = (frames->getFirst() + frames->getLast())/2.0;
    oy = (frames->getDoubleAt(frames->getFirst()) +frames->getDoubleAt(frames->getLast()))/2.0;
    sx = 1;//frames->getLast() - frames->getFirst();
    sy = 1;//frames->getDoubleAt(frames->getLast()) - frames->getDoubleAt(frames->getFirst());
  }
  int mousebuttons;
  int mxold, myold;
  int mouseEnter(int state);
  int mouseMoved(int mx, int my);
  int mouseClicked(int button, int state, int mx, int my);
  int keyPressed(unsigned char key, int mx, int my);
  int specialPressed(int key, int mx, int my);
  int mouseWheel(int a, int b, int mx, int my);
  void render();
  int get(double ori, double scale, double v, int h);
};

template<typename T, typename V, typename U>
int PlotT<T, V, U>::mouseEnter(int state) {
  mousebuttons = 0;
  return 0;
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::mouseMoved(int mx, int my) {
  int dx = mx - mxold;
  int dy = my - myold;

  if (mousebuttons & 1) { //left, drag
    ox -= sx * dx;
    oy -= sy * dy;
  }

  /*if (mousebuttons & 4) { //right, scale
    int ccx = (cbx + cax) / 2.0;
    int ccy = (cby + cay) / 2.0;
    sx *= (mxold - ccx) / double(mx - ccx);
    sy *= (myold - ccy) / double(my - ccy);
    sx = max(0.1, sx);
    sy = max(0.1, sy);
  }*/

  mxold = mx;
  myold = my;
  return 1;
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::mouseClicked(int button, int state, int mx, int my) {
  if (isIn(mx, my)) {
    mousebuttons ^= mousebuttons & (1 << button); //remove bit for button;
    mousebuttons ^= (state ^ 1) << button;
  }
  else {
    mousebuttons = 0;
  }
  return 0;
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::keyPressed(unsigned char key, int mx, int my) {
  return 0;
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::specialPressed(int key, int mx, int my) {
  return 0;
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::mouseWheel(int a, int b, int mx, int my) {
  ox += ((cbx + cax) / 2.0 - mx)*(pow(1.1, -b)-1)*sx;
  oy += ((cby + cay) / 2.0 - my)*(pow(1.1, -b)-1)*sy;

  sx *= pow(1.1, -b);
  sy *= pow(1.1, -b);
  return 1;
}

template<typename T, typename V, typename U>
void PlotT<T, V, U>::render() {
  const int xedge = 75;
  const int yedge = 20;

  //Grid

  glLineWidth(1.0f);

  glViewport(cax, cay, cbx - cax, cby - cay);

  double tickx = pow(10, floor(log10(100 * sx)));
  double ticky = pow(10, floor(log10(100 * sx)));

  if (tickx < 40 * sx) {
    tickx *= 2;
  }
  if (tickx < 40 * sx) {
    tickx *= 2;
  }
  if (ticky < 40 * sy) {
    ticky *= 2;
  }
  if (ticky < 40 * sy) {
    ticky *= 2;
  }

  double minx = ox - (cbx - cax) / 2.0*sx;
  double maxx = ox + (cbx - cax) / 2.0*sx;
  double miny = oy - (cby - cay) / 2.0*sy;
  double maxy = oy + (cby - cay) / 2.0*sy;

  minx -= fmodf(minx, tickx);
  miny -= fmodf(miny, ticky);
  maxx += tickx - fmodf(maxx, tickx);
  maxy += ticky - fmodf(maxy, ticky);

  glBegin(GL_LINES);
  glColor3f(0.0f, 1.0f, 0.0f);
  for (double d = minx; d < maxx; d += tickx) {
    if(get(ox, sx, d, cbx - cax) > xedge) {
      glVertex2f(get(ox , sx, d, cbx - cax) - 0.5, yedge - 5.5);
      glVertex2f(get(ox , sx, d, cbx - cax) - 0.5, cby - cay);
    }
  }
  glEnd();

  glBegin(GL_LINES);
  glColor3f(0.0f, 1.0f, 0.0f);
  for (double d = miny; d < maxy; d += ticky) {
    if (get(oy, sy, d, cby - cay) > yedge) {
      glVertex2f(xedge - 5.5, get(oy, sy, d, cby - cay) - 0.5);
      glVertex2f(cbx - cax, get(oy, sy, d, cby - cay) - 0.5);
    }
  }
  glEnd();

  //Frame

  glLineWidth(3.0f);

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(cax + xedge, cay + yedge);
  glVertex2f(cax + xedge, cby);
  glVertex2f(cax + xedge, cay + yedge);
  glVertex2f(cbx, cay + yedge);
  glEnd();

  //Data

  glColor3f(1.0f, 0.0f, 0.0f);
  auto eit = frames->_frames.end();
  --eit;
  for (auto it = frames->_frames.begin(); it != eit; ++it) {
    auto nit = it;
    ++nit;
    glBegin(GL_LINES);

    glVertex2f(get(ox, sx, it->first, cbx - cax), get(oy , sy, it->second.getAt(it->first), cby - cay));
    glVertex2f(get(ox, sx, nit->first, cbx - cax ), get(oy , sy, it->second.getAt(nit->first), cby - cay));
    glEnd();
  }

  //Axis labels

  tickx = pow(10, floor(log10(100 * sx)));
  ticky = pow(10, floor(log10(100 * sx)));

  if (tickx < 80 * sx) {
    tickx *= 2;
  }
  if (tickx < 80 * sx) {
    tickx *= 2;
  }
  if (tickx < 80 * sx) {
    tickx *= 2;
  }
  if (ticky < 40 * sy) {
    ticky *= 2;
  }
  if (ticky < 40 * sy) {
    ticky *= 2;
  }

  minx = ox - (cbx - cax) / 2.0*sx;
  maxx = ox + (cbx - cax) / 2.0*sx;
  miny = oy - (cby - cay) / 2.0*sy;
  maxy = oy + (cby - cay) / 2.0*sy;

  minx -= fmodf(minx, tickx);
  miny -= fmodf(miny, ticky);
  maxx += tickx - fmodf(maxx, tickx);
  maxy += ticky - fmodf(maxy, ticky);

  for (double d = minx; d < maxx; d += tickx) {
    renderBitmapString(get(ox, sx, d, cbx - cax), 5, to_string(d, 2), textColor, 1);
  }
  for (double d = miny; d < maxy; d += ticky) {
    renderBitmapString(5, get(oy, sy, d, cby - cay) - 5, to_string(d, 2), textColor, 0);
  }

  Graphics::resetViewport();
}

template<typename T, typename V, typename U>
int PlotT<T, V, U>::get(double ori, double scale, double v, int h) {
  return h / 2.0 + (v - ori) / scale;
}
