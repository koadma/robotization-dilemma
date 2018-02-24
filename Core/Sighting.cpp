#include "Sighting.h"


/*
int Sighting::getLastSmaller(time_type_s t)
{
  int first = 0, last = int(keyframes.size()) - 1;
  while (first <= last)
  {
    int mid = (first + last) / 2;
    if (keyframes[mid]->gTimeStamp >= t)
      last = mid - 1;
    else
      first = mid + 1;
  }
  return first - 1 < 0 ? -1 : first - 1;
}
Movement Sighting::estimatePos(time_type_s t, vel_type_mpers maxVelocity) {
  int id = getLastSmaller(t);
  if (id == -1) {
    id = 0;
  }
  if (id < keyframes.size()) {
    return keyframes[id]->getAt(t, maxVelocity);
  }
  else {
    throw 1;
    return Movement();
  }
}
#ifdef M_CLIENT
void Sighting::drawSighting(float camcx, float camcy, float camcz, float d, vel_type_mpers maxVel) {
  glLineWidth(2.0f);

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (keyframes.size()) {
    time_type_s t = keyframes[0]->gTimeStamp;

    while (t < keyframes[keyframes.size() - 1]->gTimeStamp + ROUND_TIME) {
      Movement estpos = estimatePos(t, maxVel);
      glVertex3d(estpos.pos.x, estpos.pos.y, estpos.pos.z);
      t += ROUND_TIME * 0.1f;
    }
  }

  glEnd();

  //drawPointingVector(camcx, camcy, camcz, d);
}
#endif
void Sighting::getSighting(DataElement* data) {
  keyframes.get(data);
}
void Sighting::setSighting(DataElement* data) {
  keyframes.set(data);
}
Sighting::~Sighting() {

}
*/

#ifdef M_CLIENT
mVec3 Sighting::reCalcPosition(mVec3 position, mVec3 viewCenter, distance_type_m maxDist) {
  return viewCenter + ((position - viewCenter) / maxDist)*5000;
}

void Sighting::drawSighting(mVec3 viewCenter, float d, vel_type_mpers maxVel, time_type_s time, /*distance_type_m maxDist,*/ OpenGLData data, bool selected) {
  /*//Close

  if (selected) {
    glLineWidth(3.0f);
  }
  else {
    glLineWidth(1.0f);
  }

  glBegin(GL_LINES);
  if (keyframes.size()) {
    time_type_s t = keyframes.getFirst();

    while (t < keyframes.getLast() + ROUND_TIME) {
      Movement estpos = getAt(t);
      if (t < keyframes.getLast()) {
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      else {
        glColor3f(0.2f, 1.0f, 0.2f);
      }
      glVertex3d(estpos.pos.x, estpos.pos.y, estpos.pos.z);
      t += ROUND_TIME * 0.1f;
    }
  }
  
  glEnd();

  if (keyframes.size() && keyframes.getFirst() < time && (time < keyframes.getLast() + ROUND_TIME || selected)) {
    Movement now = keyframes.getAt(time);

    glTranslated(now.pos.x, now.pos.y, now.pos.z);
    if(selected) {
      glColor3f(0.0f, 1.0f, 1.0f);
    } else {
      glColor3f(0.0f, 0.7f, 1.0f);
    }
    glutSolidSphere(now.radius, 20, 20);
    glTranslated(-now.pos.x, -now.pos.y, -now.pos.z);
  }

  //Far

  if(keyframes.size() && keyframes.getFirst() <= time) {
    if (selected) {
      glColor3f(1.0f, 0.0f, 0.0f);
    }
    else {
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    glBegin(GL_LINES);
    glVertex3d(viewCenter.x, viewCenter.y, viewCenter.z);
    mVec3 position = keyframes.getAt(time).pos;
    mVec3 transVec = reCalcPosition(position, viewCenter, maxDist);
    glVertex3d(transVec.x, transVec.y, transVec.z);
  

    //GLdouble scx, scy, scz;
    //gluProject(transVec.x, transVec.y, transVec.z, data.model_view, data.projection, data.viewport, &scx, &scy, &scz);

    mVec3 deltaVector = keyframes.getAt(time).vel;
    glVertex3d(transVec.x, transVec.y, transVec.z);
    glVertex3d(transVec.x + deltaVector.x, transVec.y + deltaVector.y, transVec.z + deltaVector.z);
    glEnd();
  }*/

  if (_keyframes.size() && _keyframes.getFirst() < time && ((time < _keyframes.getLast() + ROUND_TIME && _closed) || selected || (_closed && time < _closetime))) {
    Movement now = _keyframes.getAt(time);
    glBegin(GL_LINE_STRIP);
    if (selected) {
      setColor(0xffff0000);
      glLineWidth(3.0);
    }
    else {
      setColor(0xffff8080);
      glLineWidth(1.0);
    }
    glVertex3d(viewCenter.x/d, viewCenter.y/d, viewCenter.z/d);
    glVertex3d(now.pos.x/d, viewCenter.y/d, now.pos.z/d);
    glVertex3d(now.pos.x/d, now.pos.y/d, now.pos.z/d);
    glEnd();
    glTranslated(now.pos.x / d, now.pos.y / d, now.pos.z / d);
    glutSolidSphere(SightingSize, 20, 20);
    glTranslated(-now.pos.x / d, -now.pos.y / d, -now.pos.z / d);
  }
}

#endif

//Surefire maths

result coordinates(mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, acc_type_mperss amax, distance_type_m d, time_type_s tpassed)
{
  //az ellenseg es a sajat hajo PILLANATNYI tavolsaga skalarisan es vektorialisan
  distance_type_m s = (starttarget - startship).length();
  mVec3 svec = startship - starttarget;
  //az ellenseg utolso ismert sebessegenek abszolut erteke
  distance_type_m v = vtarget.length();
  //az utolso ismert sebesseg es az s vektor altal bezart szog
  distance_type_m vcosgamma = dot(svec, vtarget) / (s);
  //az utolso ismert sebesseg alapjan mikorra erne be a hajot egy lezerlovedek
  double t3 = s*(-vcosgamma + sqrt(v*v - vcosgamma*vcosgamma + SOL*SOL)) / (SOL*SOL - v*v);
  result direction(starttarget + vtarget*t3 - startship, true);
  //idoben beerne meg?
  if ((sqrt(d / amax) - tpassed) < t3)
  {
    direction.answ = false;
  }
  return direction;
}

//automata visszaloveshez a koordianatak, ez meg az eszleles pillanataban megtortenik
result surefire1(time_type_s obsTime, mpsVec3 vTarget, mVec3 sTarget, time_type_s now, mVec3 sShip, acc_type_mperss amax, distance_type_m d)
{
  //vec3d nvship = vship; //a hajo sebesseg MOST
  mpsVec3 nvtarget = vTarget; //az ellenseg utolso ismert sebessege
  mVec3 nstarget = sTarget; //ez alapjan hol lehetett az ellenseg, amikor visszalottunk?
  mVec3 nsship = sShip; //mi hol voltunk amikor visszalottunk?
                            //a visszaverodes es eszleles kozott eltelt ido
  //double actpass = (starttarget - startship).length() / SOL;
  double actpass = now - obsTime;

  //amikor eszleljuk a visszaverodest addigra az ellenseg mar mozgott, az utolso ismert pozicio alapjan itt lehetett amikor visszalottunk ra
  nstarget = nstarget + nvtarget*actpass;

  //hova lo a hajo,  ha az automata loves van engedelyezve
  return coordinates(nvtarget, nstarget, nsship, amax, d, actpass);
}
/*
//azt szamoljuk ki hogy az elozo korben tortent eszlelesre a mostani mozgassorok közben meddig biztos a talalat
vector<result> surefire2(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, vector<vec4d> movements, double utime, double round)
{
  mpsVec3 nvship = vship; //a hajo sebesseg MOST
  mpsVec3 nvtarget = vtarget; //az ellenseg utolso ismert sebessege
  mVec3 nstarget = starttarget; //ez alapjan hol lehet most az ellenseg?
  mVec3 nsship = nowship; //mi most hol vagyunk a kor elejen?
                          //a visszaverodes es a kor kezdete kozott eltelt ido, ami a visszaverodes es eszleles+ eszleles es kor kezdete idok osszege
  double actpass = sqrt((starttarget.x - startship.x)*(starttarget.x - startship.x) + (starttarget.y - startship.y)*(starttarget.y - startship.y) + (starttarget.z - startship.z)*(starttarget.z - startship.z)) / SOL + (round - 1) * 20 - utime;
  //double itsum = 0; //a mozgasparancsok idoosszege

  //amikor eszleljuk a visszaverodest addigra az ellenseg mar mozgott, az utolso ismert pozicio alapjan itt lehet most
  nstarget = nstarget + nvtarget*actpass;

  vector<result> results;
  //a kor elejen ha lovunk mi tortenne
  results.push_back(coordinates(nvtarget, nstarget, nsship, amax, d, actpass));

  //a kor mozgasai, hiszen az eszleles amire most reagalni tudunk meg az elozo korben tortent
  for (auto i = movements.begin(); i < movements.end(); i++)
  {
    //egyszerubb ez a nev egy picit, meg bennemaradt a nagy osszeolvasztott fgv bol esnem akartam szetszedni mar
    double mtime = i->t;

    //a celpont feltetelezett mozgasa
    nstarget = nstarget + nvtarget*(mtime);

    //a mi mozgasunk
    nsship.x = nsship.x + nvship.x*(mtime)+(i->x)*(mtime)*(mtime);
    nsship.y = nsship.y + nvship.y*(mtime)+(i->y)*(mtime)*(mtime);
    nsship.z = nsship.z + nvship.z*(mtime)+(i->z)*(mtime)*(mtime);

    //igy valtozik a sebessegunk
    nvship.x = nvship.x + (i->x)*(mtime);
    nvship.y = nvship.y + (i->y)*(mtime);
    nvship.z = nvship.z + (i->z)*(mtime);

    //igy az eltelt ido
    actpass = actpass + mtime;

    //a mozgasreszlet utan hova lovunk?
    results.push_back(coordinates(nvtarget, nstarget, nsship, amax, d, actpass));

  }

  return results;

}

result surefire3(mpsVec3 vship, mpsVec3 vtarget, mVec3 starttarget, mVec3 startship, mVec3 nowship, double amax, double d, keyframe<Movement>& movements, time_type_s detectTime, double queryTime, double round)
{
  mpsVec3 nvship = vship; //a hajo sebesseg MOST
  mpsVec3 nvtarget = vtarget; //az ellenseg utolso ismert sebessege
  mVec3 nstarget = starttarget; //ez alapjan hol lehet most az ellenseg?
  mVec3 nsship = nowship; //mi most hol vagyunk a kor elejen?
                          //a visszaverodes es a kor kezdete kozott eltelt ido, ami a visszaverodes es eszleles+ eszleles es kor kezdete idok osszege
  double actpass = sqrt((starttarget.x - startship.x)*(starttarget.x - startship.x) + (starttarget.y - startship.y)*(starttarget.y - startship.y) + (starttarget.z - startship.z)*(starttarget.z - startship.z)) / SOL + (round - 1) * 20 - detectTime;
  double itsum = 0; //a mozgasparancsok idoosszege

                    //amikor eszleljuk a visszaverodest addigra az ellenseg mar mozgott, az utolso ismert pozicio alapjan itt lehet most
  nstarget = nstarget + nvtarget*actpass;

  //a kor mozgasai, hiszen az eszleles amire most reagalni tudunk meg az elozo korben tortent, valahol itt lesz a kerdezett ido is
  for (auto i = movements._frames.begin(); i != movements._frames.end(); i++)
  {
    time_type_s mtime = i->first;

    //a kovetkezo mozgasreszlet vegere tulleptuk-e mar a kerdezett idot?
    if (queryTime <= (round - 1) * 20 + itsum + i->first)
    {
      //ha igen akkor enek a mozgaselemnek a hatasat csak a kerdezett idoig szimulaljuk
      mtime = itsum + i->first - qtime;

      nstarget = nstarget + nvtarget*(mtime);

      nsship = nsship + nvship*(mtime)+(i->x)*(mtime)*(mtime);

      nvship.x = nvship.x + (i->x)*(mtime);

      actpass = actpass + mtime;

      //szoval hova is kell loni a kerdezett idoben es biztos-e meg a talalat?
      result cord = coordinates(nvtarget, nstarget, nsship, amax, d, actpass);
      cout << cord.x << " " << cord.y << " " << cord.z << " " << cord.answ << endl;

      return cord;
    }

    //a kerdezett idoig elvegezzuk a ket hajo mozgatasat
    nstarget = nstarget + nvtarget*(mtime);

    nsship = nsship + nvship*(mtime)+(i->x)*(mtime)*(mtime);

    nvship = nvship + (i->x)*(mtime);

    actpass = actpass + mtime;
    itsum = itsum + i->first;
  }
}*/
