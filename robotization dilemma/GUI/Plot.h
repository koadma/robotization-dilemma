#pragma once

#include "GUIElement.h"

class NoTypeIter {
public:
  virtual double getX() {
    return 0;
  }
  virtual double getY(double time) {
    return 0;
  }
  virtual bool next() {
    return false;
  };
  virtual bool prev() {
    return false;
  };
  virtual NoTypeIter* copy() {
    return NULL;
  }
};

template<typename V, typename U = V, typename T = time_type_s>
class NoTypeIterVUT : public NoTypeIter {
public:
  typename map<T,V>::iterator _iter;
  keyframe<V,U,T>* _data;
  double getX() {
    return _iter->first;
  }
  virtual double getY(double time) {
    return _iter->second.getAt(time);
  }
  bool next() {
    ++_iter;
    if (_iter == _data->_frames.end()) {
      return false;
    }
    return true;
  };
  bool prev() {
    --_iter;
    if (_iter == _data->_frames.begin()) {
      return false;
    }
    return true;
  };
  NoTypeIter* copy() {
    NoTypeIterVUT<V, U, T>* newIt = new NoTypeIterVUT<V,U,T>();
    newIt->_data = _data;
    newIt->_iter = _iter;
    return newIt;
  }
};

class PlotLine {
public:
  colorargb color;
  virtual int size() {
    return 0;
  }
  virtual NoTypeIter* first() {
    return NULL;
  };
  virtual NoTypeIter* last() {
    return NULL;
  };
};

template<typename V, typename U=V, typename T=time_type_s>
class PlotLineVUT : public PlotLine {
public:
  keyframe<V, U, T>* _data;
  PlotLineVUT<V, U, T>(keyframe<V, U, T>* data, colorargb lcolor) {
   _data = data;
   color = lcolor;
  }
  NoTypeIter* first() {
     NoTypeIterVUT<V, U, T>* iter = new NoTypeIterVUT<V,U,T>();
     iter->_iter = _data->_frames.begin();
     iter->_data = _data;
     return iter;
  };
  NoTypeIter* last() {
    NoTypeIterVUT<V, U, T>* iter = new NoTypeIterVUT<V, U, T>();
    iter->_iter = _data->_frames.end();
    iter->_data = _data;
    --(iter->_iter);
    return iter;
  };
};

//keyframe type
class Plot : public GUIElement {
public:
  list<PlotLine*> plotData;
  double ox, oy; //in plot coords, for center of plot
  double sx, sy; //data / plotwidth
  
  Plot(string lname, Coordiante lmincorner, Coordiante lmaxcorner, colorargb lbg, colorargb lactive, colorargb ltextColor) :
    GUIElement(lname, lmincorner, lmaxcorner, lbg, lactive, ltextColor) {
    ox = 0;//(frames->getFirst() + frames->getLast())/2.0;
    oy = 0;//(frames->getDoubleAt(frames->getFirst()) +frames->getDoubleAt(frames->getLast()))/2.0;
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
