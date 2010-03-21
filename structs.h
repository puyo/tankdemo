// 3D Tank Game 1.0
// Gregory McIntyre 1998
// struct/class definitions


#include "allegro.h"
#include "consts.h"
#include "tests.h"


#define POLY(x)  POLY_##x

#ifndef STRUCT_H
#define STRUCT_H


typedef struct VERTEX
{
 fixed x, y, z;
 char c;
} VERTEX;


typedef struct QUAD
{
 int v1, v2, v3, v4;
 char visible;
 V3D *vlist;
} QUAD;


class POLYGON
{
 public:
  POLYGON();
  fixed x, y, z;
};


class POLY_8 : public POLYGON   // polygon with 8 points
{
 public:
  POLY_8();
  void common_rect_setup(fixed x1, fixed y1, fixed z1,
                         fixed x2, fixed y2, fixed z2, char col);
  void draw(BITMAP *bitmap);

  V3D v[8];  // current
  V3D o[8];  // original
};

#endif
