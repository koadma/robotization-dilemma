#pragma once

#ifdef M_SERVER
#include "../Server/Bubble.h"
#endif
#ifdef M_CLIENT
#include "../robotization dilemma/RenderOut.h"
#endif


vector<double> intersectPaths(Path* lhs, Path* rhs);


class Sighting {
public:
  vector<Movement*> keyframes; //time - keyframe. Sorted.
  int getLastSmaller(float t);
  Movement estimatePos(float t, float maxVelocity);

#ifdef M_CLIENT
  void drawSighting(float camcx, float camcy, float camcz, float d, float maxVel);
#endif

  void getSighting(DataElement* data);
  void setSighting(DataElement* data);

  void clearKeyframes();
  ~Sighting();
};
