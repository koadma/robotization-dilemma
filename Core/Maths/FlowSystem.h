#pragma once

#include "Fraction.h"

//typedef int FlowType;

//Derivative, value, time type
template <typename T, typename V, typename U>
class FlowVertex;

//Derivative, value, time type
template <typename T, typename V, typename U>
class FlowEdge
{
public:
  T _flow, _capacity;
  T _totalFlow;
  T _totalCapacity;

  // _u -> _v
  FlowVertex<T, V, U>* _u;
  FlowVertex<T, V, U>* _v;

  bool _real;

  FlowEdge(T flow, T capacity, FlowVertex<T, V, U>* u, FlowVertex<T, V, U>* v, bool real = false)
  {
    _totalFlow = T(0);
    _flow = flow;
    _capacity = capacity;
    _totalCapacity = _capacity;
    _u = u;
    _v = v;
    _real = real;
  }
};

//Derivative, value, time type
template <typename T, typename V, typename U>
class FlowVertex
{
public:
  int _h;
  T _flow;
  T _delta;
  T _drain;
  list<FlowEdge<T, V, U>*> _edges;

  keyframe<linear<V, T, U>, V, U> _val;
  V _maxVal;

  //Goal (+: out)
  T _goal;
  //Charge max (-)
  T _maxCharge;
  bool _canCharge = true;
  //Drain max (+)
  T _maxDrain;
  bool _canDrain = true;

  int _ID;

  FlowVertex(int h, T flow, T goal, T maxCharge, T maxDrain, V maxVal, V startVal, U startTime)
  {
    _h = h;
    _flow = flow;
    _delta = T(0);
    _drain = T(0);
    _goal = goal;
    _maxCharge = maxCharge;
    _maxDrain = maxDrain;
    _maxVal = maxVal;
    _val.addFrame(startTime, linear<V,T,U>{startVal, startTime});
  }

  ~FlowVertex() {
    while (_edges.size()) {
      delete *(_edges.begin());
    }
  }
};

//Derivative, value, time type
template <typename T, typename V, typename U>
class FlowGraph
{
public:
  list<FlowVertex<T, V, U>*> _ver;

  FlowVertex<T, V, U>* _s;
  FlowVertex<T, V, U>* _t;
  U _lastUpdate;

  FlowGraph();

  void preflow(FlowVertex<T, V, U>* s);

  FlowVertex<T, V, U>* getFirstOverflow();
  
  void updateReverseFlow(FlowEdge<T, V, U>* i, T flow);

  bool push(FlowVertex<T, V, U>* u);

  void relabel(FlowVertex<T, V, U>* u);

  FlowEdge<T, V, U>* addEdge(FlowVertex<T, V, U>* u, FlowVertex<T, V, U>* v,  T w, bool real = true, T flow = T(0));

  pair<FlowEdge<T, V, U>*, FlowEdge<T, V, U>*> addSymmetricEdge(FlowVertex<T, V, U>* u, FlowVertex<T, V, U>* v, T w, T flow = T(0));

  FlowVertex<T, V, U>* addVertex(T goal, T maxCharge, T maxDrain, V maxval, V startval = V(0), V startitme = V(0));

  void addRootEdges(int mode);

  void clean(bool decrease);
 
  void reset();

  void getMaxFlow();
  void getMaxFlow(FlowVertex<T, V, U>* s, FlowVertex<T, V, U>* t);

  vector<U> goTo(U time);

  ~FlowGraph();
};

template <typename T, typename V, typename U>
FlowGraph<T, V, U>::FlowGraph() {
  _s = new FlowVertex<T, V, U>(0, 0, 0, 0, 0, 0, 0, 0);
  _ver.push_back(_s);

  _t = new FlowVertex<T, V, U>(0, 0, 0, 0, 0, 0, 0, 0);
  _ver.push_back(_t);
}

template <typename T, typename V, typename U>
FlowEdge<T, V, U>* FlowGraph<T, V, U>::addEdge(FlowVertex<T, V, U>* u, FlowVertex<T, V, U>* v, T capacity, bool real, T flow)
{
  FlowEdge<T, V, U>* n = new FlowEdge<T, V, U>(flow, capacity, u, v, real);
  u->_edges.push_back(n);
  return n;
}

template <typename T, typename V, typename U>
pair<FlowEdge<T, V, U>*, FlowEdge<T, V, U>*> FlowGraph<T, V, U>::addSymmetricEdge(FlowVertex<T, V, U>* u, FlowVertex<T, V, U>* v, T w, T flow) {
  return{ addEdge(u, v, w, flow),addEdge(v, u, w, flow) };
}

template <typename T, typename V, typename U>
FlowVertex<T, V, U>* FlowGraph<T, V, U>::addVertex(T goal, T maxCharge, T maxDrain, V maxval, V startval, V starttime) {
  FlowVertex<T, V, U>* n = new FlowVertex<T, V, U>(0, 0, goal, maxCharge, maxDrain, maxval, startval, starttime);
  _ver.push_back(n);
  return n;
}

// perform preflow
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::preflow(FlowVertex<T, V, U>* s)
{
  s->_h = _ver.size();

  for (auto&& it : s->_edges)
  {
    it->_flow = it->_capacity;

    it->_v->_flow += it->_flow;

    addEdge(it->_v, s, 0, false, -it->_flow);
    //it->_v->_edges.push_back(new FlowEdge(-it->_flow, 0, it->_v, s));
  }
}

// first overflowing vertex
template <typename T, typename V, typename U>
FlowVertex<T, V, U>* FlowGraph<T, V, U>::getFirstOverflow()
{
  for (auto&& it : _ver) {
    if (it != _s && it != _t) {
      if (it->_flow > T(0))
        return it;
    }
  }
  // NULL if none
  return NULL;
}

// Update reverse flow for flow added on ith Edge
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::updateReverseFlow(FlowEdge<T, V, U>* i, T flow)
{
  FlowVertex<T, V, U>* u = i->_v;
  FlowVertex<T, V, U>* v = i->_u;

  for (auto&& j : u->_edges)
  {
    if (j->_v == v)
    {
      j->_flow -= flow;
      return;
    }
  }

  addEdge(u, v, flow, false);
}

// push from u
template <typename T, typename V, typename U>
bool FlowGraph<T, V, U>::push(FlowVertex<T, V, U>* u)
{
  for (auto&& it : u->_edges)
  {
    if (it->_flow == it->_capacity)
      continue;

    if (u->_h > it->_v->_h)
    {
      T flow = min(it->_capacity - it->_flow,
        u->_flow);

      u->_flow -= flow;

      it->_v->_flow += flow;

      it->_flow += flow;

      updateReverseFlow(it, flow);

      return true;
    }
  }

  return false;
}

// relabel u
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::relabel(FlowVertex<T, V, U>* u)
{
  int mh = INT_MAX;

  for (auto&& it : u->_edges)
  {
    if (it->_flow == it->_capacity)
      continue;

    if (it->_v->_h < mh)
    {
      mh = it->_v->_h;
      u->_h = mh + 1;
    }
  }
}

//connect to supersink, supersource.
//0: goal, 1: charge, 2: drain
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::addRootEdges(int mode) {
  T requested;

  for (auto&& it : _ver) {
    requested = T(0);
    switch (mode) {
    case 0:
      requested = it->_goal;
      break;
    case 1:
      if (it->_canCharge) {
        requested = it->_maxCharge;
      }
      break;
    case 2:
      if (it->_canDrain) {
        requested = it->_maxDrain;
      }
      break;
    }
    if (requested > T(0)) {
      addEdge(it, _t, requested, false);
    }
    if (requested < T(0)) {
      addEdge(_s, it, -requested, false);
    }
  }
}

//clean graph, decrease capacity?
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::clean(bool decrease) {
  for (auto&& it : _ver) {
    auto et = it->_edges.begin();

    while (et != it->_edges.end()) {
      if (!(*et)->_real) {
        auto et2 = et;
        ++et;
        it->_edges.erase(et2);
      }
      else {
        (*et)->_u->_delta -= (*et)->_flow;
        (*et)->_v->_delta += (*et)->_flow;

        (*et)->_totalFlow += (*et)->_flow;
        if (decrease) {
          (*et)->_capacity -= (*et)->_flow;
        }
        (*et)->_flow = T(0);
        ++et;
      }
    }
    it->_flow = T(0);
    it->_h = 0;
  }
}

template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::reset() {
  for (auto&& it : _ver) {
    it->_delta = T(0);
    for (auto&& et : it->_edges) {
      et->_capacity = et->_totalCapacity;
      et->_totalFlow = T(0);
    }
  }
}

// calc max flow
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::getMaxFlow(FlowVertex<T, V, U>* s, FlowVertex<T, V, U>* t)
{
  _s = s;
  _t = t;
  preflow(s);

  while (getFirstOverflow() != NULL)
  {
    FlowVertex<T, V, U>* u = getFirstOverflow();
    if (!push(u))
      relabel(u);
  }
}
template <typename T, typename V, typename U>
void FlowGraph<T, V, U>::getMaxFlow() {
  //Reset the graph
  reset();
  clean(true);
  //Add root edges, goal mode
  addRootEdges(0);
  getMaxFlow(_s, _t);

  //Wipe graph
  clean(true);

  //Add root edges, charge mode
  addRootEdges(1);
  getMaxFlow(_s, _t);

  //Wipe graph
  clean(true);

  //Add root edges, drain mode
  addRootEdges(2);
  getMaxFlow(_s, _t);

  clean(true);
}

template <typename T, typename V, typename U>
vector<U> FlowGraph<T, V, U>::goTo(U time) {
  vector<U> endtimes;
  if (time > _lastUpdate) {
    for (auto&& it : _ver) {
      it->_canCharge = (it->_val.getAt(time) < 0.99 * it->_maxVal);
      it->_canDrain = (0.01 * it->_maxVal < it->_val.getAt(time));
    }
    getMaxFlow();
    for (auto&& it : _ver) {
      if (it->_delta > T(0)) {
        endtimes.push_back((it->_maxVal - it->_val.getAt(time)) / double(it->_delta));
      }
      if (it->_delta < T(0)) {
        endtimes.push_back((V(0) - it->_val.getAt(time)) / double(it->_delta));
      }
      it->_val.addFrame(time, linear<V,T,U>(it->_val.getAt(time), time, it->_delta));
    }
    _lastUpdate = time;
  }
  return endtimes;
}

template <typename T, typename V, typename U>
FlowGraph<T, V, U>::~FlowGraph() {
  while (_ver.size()) {
    delete *(_ver.begin());
  }
}
