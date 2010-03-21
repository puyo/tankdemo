// 3D Tank Game 1.0
// Gregory McIntyre 1998
// polygon function definitions

#include "allegro.h"
#include "structs.h"


POLYGON::POLYGON()
{
 x = y = z = 0;
}



POLY_8::POLY_8()
{
}



void POLY_8::common_rect_setup(fixed x1, fixed y1, fixed z1, fixed x2, fixed y2, fixed z2, char col)
{
 v[0].x = x1;
 v[0].y = y1;
 v[0].z = z1;
 v[0].c = col+0;
 v[1].x = x2;
 v[1].y = y1;
 v[1].z = z1;
 v[1].c = col+2;
 v[2].x = x2;
 v[2].y = y1;
 v[2].z = z1;
 v[2].c = col+4;
 v[3].x = x2;
 v[3].y = y2;
 v[3].z = z1;
 v[3].c = col+6;
 v[4].x = x1;
 v[4].y = y1;
 v[4].z = z2;
 v[4].c = col+8;
 v[5].x = x1;
 v[5].y = y2;
 v[5].z = z2;
 v[5].c = col+10;
 v[6].x = x2;
 v[6].y = y1;
 v[6].z = z2;
 v[6].c = col+12;
 v[7].x = x2;
 v[7].y = y2;
 v[7].z = z2;
 v[7].c = col+14;
}
