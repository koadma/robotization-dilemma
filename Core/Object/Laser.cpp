#include "../Ship.h"
#ifdef M_SERVER
#include "../../Server/Game.h"
#endif

void Laser::getVStatus(DataElement* data) {
  DataElement* shote = new DataElement();
  for (auto&& it : _shots) {
    DataElement* ne = new DataElement();

    DataElement* timee = new DataElement();
    vGFunc<time_type_s>(get<0>(it), timee);
    ne->addChild(timee);

    DataElement* enee = new DataElement();
    vGFunc<energy_type_J>(get<1>(it), enee);
    ne->addChild(enee);

    DataElement* dire = new DataElement();
    get<2>(it).get(dire);
    ne->addChild(dire);

    DataElement* usede = new DataElement();
    vGFunc<bool>(get<3>(it), usede);
    ne->addChild(usede);

    shote->addChild(ne);
  }
  data->addChild(shote);

  DataElement* mstoe = new DataElement();
  _maxStorage.get(mstoe);
  data->addChild(mstoe);
}
void Laser::setVStatus(DataElement* data) {
  _shots.clear();

  for (auto&& it : data->_children[0]->_children) {

    tuple<time_type_s, energy_type_J, sVec3, bool> nVal;

    vSFunc<time_type_s>(get<0>(nVal), it->_children[0]);
    vSFunc<energy_type_J>(get<1>(nVal), it->_children[1]);
    get<2>(nVal).set(it->_children[2]);
    vSFunc<bool>(get<3>(nVal), it->_children[3]);

    _shots.push_back(nVal);
  }

  _maxStorage.set(data->_children[1]);

}
void Laser::collectEvents(list<StateChange*> &addTo, time_type_s time) {
  for (auto&& it : _shots) {
    if (time <= get<0>(it) && get<3>(it) == false) {
      LaserShot* ev = new LaserShot();
      ev->_dir = get<2>(it);
      ev->_energy = get<1>(it);
      ev->_o = this;
      ev->_time = get<0>(it);
      addTo.push_back(ev);
    }
    get<3>(it) = true;
  }
}

#ifdef M_SERVER
void Laser::energyCallbackV(time_type_s time, Game* g) {
  //It is not necessary to stop the battery charge, thats automatic.
}
#endif

#ifdef M_CLIENT


void Laser::setSidebar() {
  setSidebarElement("html/laser_settings.xml");

  reinterpret_cast<Graphics::LabelBindHwnd>(Graphics::getElementById("objectLaserSidebarSurefireLabel"))->text =
    new TextBind<
    TextBindFunc<string>
    >("%",
      TextBindFunc<string>(isSurefire)
      );
}
void Laser::drawObjectVirt(float camcx, float camcy, float camcz, float d, time_type_s time, bool full) {
  if (full) {
    for (auto&& it : _shots) {
      if (time - ROUND_TIME <= get<0>(it) && get<0>(it) <= time + ROUND_TIME) {
        if (time < get<0>(it)) {
          glColor3f(0.6f, 0.0f, 0.0f);
        }
        else {
          glColor3f(1.0f, 0.0f, 0.0f);
        }
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        mVec3 p = getMovement(time).pos;
        glVertex3d(
        to_doubleT<double, distance_type_m>(p.x),
        to_doubleT<double, distance_type_m>(p.y),
        to_doubleT<double, distance_type_m>(p.z)
        );
        sVec3 dvec = get<2>(it);
        dvec.norm();
        dvec *= (get<1>(it) / 100.0f + _radius);
        glVertex3d(
        to_doubleT<double, distance_type_m>(p.x) + to_doubleT<double, distance_type_m>(dvec.x),
        to_doubleT<double, distance_type_m>(p.y) + to_doubleT<double, distance_type_m>(dvec.y),
        to_doubleT<double, distance_type_m>(p.z) + to_doubleT<double, distance_type_m>(dvec.z));
        glEnd();
      }
    }
  }
}
#endif