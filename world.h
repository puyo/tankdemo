// 3D Tank Game 1.0
// Gregory McIntyre 1998
// struct/class definitions - world


#include "allegro.h"
#include "consts.h"


#ifndef WORLD_H
#define WORLD_H

class WORLD
{
 public:
  WORLD();
  ~WORLD();
  void reset();
  void draw();

  MATRIX trans, cam;

 protected:
  V3D start[PLAYERS];
};

#endif
