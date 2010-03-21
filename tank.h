// 3D Tank Game 1.0
// Gregory McIntyre 1998
// struct/class definitions - tank


#include "allegro.h"
#include "structs.h"


#ifndef TANK_H
#define TANK_H

class TANK : public POLYGON
{
 public:
  TANK();
  void set_col(char col);
  void draw(BITMAP *bitmap);

  V3D v[3*8];  // current verticies
  V3D o[3*8];  // original verticies (defining object)
  QUAD f[3*6]; // faces
  fixed xr, yr, zr;
};

#endif
