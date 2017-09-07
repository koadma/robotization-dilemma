#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

Movement Object::getMovement(time_type_s time) {
  Movement m = parentShip->mov.getAt(time);
  m.pos = m.pos + _relativePos;
  m.radius = _radius;
  return m;
}
list< pair<double, pair<Object*, Path*>>> Object::intersect(Path* p) {
  list< pair<double, pair<Object*, Path*>>> res;
  auto it = parentShip->mov._frames.begin();
  while (it != parentShip->mov._frames.end()) {
    Movement m = it->second;
    m.pos += _relativePos;
    m.radius = _radius;
    vector<double> times = intersectPaths(p, &m);
    for (auto&& itt : times) {
      auto nit = it;
      ++nit;
      if (it->first <= itt && (nit == parentShip->mov._frames.end() || itt < nit->first)) {
        res.push_back({ itt,{ this, p } });
      }
    }
    ++it;
  }
  return res;
}
void Object::getStatus(DataElement* data) {
  DataElement* typee = new DataElement();
  typee->_core->fromType<int>(type());
  data->addChild(typee);

  DataElement* rele = new DataElement();
  _relativePos.get(rele);
  data->addChild(rele);

  DataElement* maxe = new DataElement();
  maxe->_core->fromType<int>(_maxHealth);
  data->addChild(maxe);

  DataElement* hele = new DataElement();
  _health.get(hele);
  data->addChild(hele);

  DataElement* rade = new DataElement();
  rade->_core->fromType<distance_type_m>(_radius);
  data->addChild(rade);

  DataElement* ide = new DataElement();
  ide->_core->fromType<uint64_t>(_ID);
  data->addChild(ide);

  DataElement* name = new DataElement();
  name->_core->fromType<string>(_name);
  data->addChild(name);

  DataElement* vire = new DataElement();
  getVStatus(vire);
  data->addChild(vire);

}
void Object::setStatus(DataElement* data) {
  _relativePos.set(data->_children[1]);

  _maxHealth = data->_children[2]->_core->toType<int>();

  _health.set(data->_children[3]);

  _radius = data->_children[4]->_core->toType<float>();

  _ID = data->_children[5]->_core->toType<uint64_t>();

  _name = data->_children[6]->_core->toType<string>();

  setVStatus(data->_children[7]);
}

void Object::getVStatus(DataElement* data) {

}
void Object::setVStatus(DataElement* data) {

}


Object::~Object() {

}


void Object::collectEvents(list<StateChange*> &addTo, time_type_s time) {

}


#ifdef M_CLIENT
void Object::setSidebarElement(string filename) {
  bool reset = true;
  if (selectedo == NULL || selectedo->type() != type()) {
    reset = true;
  }
  selectedo = this;
  if (reset) {
    Graphics::setElements(reinterpret_cast<Graphics::PanelHwnd>(Graphics::getElementById("objectIngameMenuSidebar")), filename);

  }
}
void Object::setSidebar() {
  cout << "Unimplemented type " << type() << endl;
}
list< pair<double, pair<Object*, Path*>>> Object::getIntersect(vec3<double> ori, vec3<double> dir) {
  Shot p;
  p.origin = ori;
  p.origintime = 0;
  p.vel = dir;
  Movement m;
  m.pos = _relativePos /*+ parentShip->mov.pos*/;
  m.vel = 0;
  m.acc = 0;
  m.gTimeStamp = 0;
  m.radius = _radius;
  list< pair<double, pair<Object*, Path*>>> res;
  vector<double> times = intersectPaths(&m, &p);
  for (auto&& it : times) {
    res.push_back({ it,{ this, &p } });
  }
  return res;
}
void Object::drawObject(float camcx, float camcy, float camcz, float d, time_type_s time) {
  glTranslated(_relativePos.x, _relativePos.y, _relativePos.z);
  setColor(0xffdf0000 + int(0xdf * _health.getAt(time)() / float(_maxHealth)) + 0x100 * int(0xdf * _health.getAt(time)() / float(_maxHealth)));
  glutSolidSphere(_radius, 20, 20);
  glTranslated(-_relativePos.x, -_relativePos.y, -_relativePos.z);
  drawObjectVirt(camcx, camcy, camcz, d, time);
}
#endif

#ifdef M_SERVER
void Object::getPath(time_type_s time, Path* p, Game* g) {
  if ((p->type() == Path::PathTypeShot) && (p->originID != _ID)) {
    _health.addFrame(time,
      value<int>(max(
        0,
        _health.getAt(time)() - int(((Shot*)p)->energy / 50)
        ))); //TODO BETTER
  }
  if ((p->type() == Path::PathTypeBubble) && (high(p->originID) != high(_ID)) && ((Bubble*)p)->getFlux(time) > BUBBLE_REMOVE && ((Bubble*)p)->btype != Bubble::Chat) { //Thermal and ping are reflected
    Bubble* b = new Bubble();
    b->btype = ((Bubble*)p)->btype;
    b->emitter = getMovement(time);
    b->energy = _radius*_radius*PI*((Bubble*)p)->getFlux(time);
    b->gEmissionTime = time;
    b->origin = getMovement(time).pos;
    b->originID = _ID;

    g->calcIntersect(b);
    g->paths.push_back(b);
  }
  getPathVirt(time, p);
}
#endif