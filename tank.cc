// 3D Tank Game 1.0
// Gregory McIntyre 1998
// tank function definitions

#include "tank.h"
#include "colours.h"


TANK::TANK()
{
 char c;

 // chassis
 o[0].x = itofix(-10);
 o[0].y = itofix( -2);
 o[0].z = itofix(-16);
 o[1].x = itofix(-10);
 o[1].y = itofix(  6);
 o[1].z = itofix(-12);
 o[2].x = itofix( 10);
 o[2].y = itofix( -2);
 o[2].z = itofix(-16);
 o[3].x = itofix( 10);
 o[3].y = itofix(  6);
 o[3].z = itofix(-12);
 o[4].x = itofix(-10);
 o[4].y = itofix( -2);
 o[4].z = itofix( 16);
 o[5].x = itofix(-10);
 o[5].y = itofix(  6);
 o[5].z = itofix( 12);
 o[6].x = itofix( 10);
 o[6].y = itofix( -2);
 o[6].z = itofix( 16);
 o[7].x = itofix( 10);
 o[7].y = itofix(  6);
 o[7].z = itofix( 12);

 // turret
 o[8+0].x = itofix( -4);
 o[8+0].y = itofix( -6);
 o[8+0].z = itofix( -4);
 o[8+1].x = itofix( -6);
 o[8+1].y = itofix( -2);
 o[8+1].z = itofix( -6);
 o[8+2].x = itofix(  4);
 o[8+2].y = itofix( -6);
 o[8+2].z = itofix( -4);
 o[8+3].x = itofix(  6);
 o[8+3].y = itofix( -2);
 o[8+3].z = itofix( -6);
 o[8+4].x = itofix( -4);
 o[8+4].y = itofix( -6);
 o[8+4].z = itofix(  4);
 o[8+5].x = itofix( -6);
 o[8+5].y = itofix( -2);
 o[8+5].z = itofix(  6);
 o[8+6].x = itofix(  4);
 o[8+6].y = itofix( -6);
 o[8+6].z = itofix(  4);
 o[8+7].x = itofix(  6);
 o[8+7].y = itofix( -2);
 o[8+7].z = itofix(  6);

 // cannony bit :)
 o[16+0].x = itofix( -1);
 o[16+0].y = itofix( -5);
 o[16+0].z = itofix(  4);
 o[16+1].x = itofix( -1);
 o[16+1].y = itofix( -3);
 o[16+1].z = itofix(  6);
 o[16+2].x = itofix(  1);
 o[16+2].y = itofix( -5);
 o[16+2].z = itofix(  4);
 o[16+3].x = itofix(  1);
 o[16+3].y = itofix( -3);
 o[16+3].z = itofix(  6);
 o[16+4].x = itofix( -1);
 o[16+4].y = itofix( -5);
 o[16+4].z = itofix( 24);
 o[16+5].x = itofix( -1);
 o[16+5].y = itofix( -3);
 o[16+5].z = itofix( 24);
 o[16+6].x = itofix(  1);
 o[16+6].y = itofix( -5);
 o[16+6].z = itofix( 24);
 o[16+7].x = itofix(  1);
 o[16+7].y = itofix( -3);
 o[16+7].z = itofix( 24);

 set_col(C1);

 for (c = 0; c != 3; c++) {   
    f[6*c + 0].v1 = 8*c + 0;
    f[6*c + 0].v2 = 8*c + 2;
    f[6*c + 0].v3 = 8*c + 3;
    f[6*c + 0].v4 = 8*c + 1;
    f[6*c + 0].visible = 1;
    f[6*c + 0].vlist = v;
    f[6*c + 1].v1 = 8*c + 2;
    f[6*c + 1].v2 = 8*c + 6;
    f[6*c + 1].v3 = 8*c + 7;
    f[6*c + 1].v4 = 8*c + 3;
    f[6*c + 1].visible = 1;
    f[6*c + 1].vlist = v;
    f[6*c + 2].v1 = 8*c + 6;
    f[6*c + 2].v2 = 8*c + 4;
    f[6*c + 2].v3 = 8*c + 5;
    f[6*c + 2].v4 = 8*c + 7;
    f[6*c + 2].visible = 1;
    f[6*c + 2].vlist = v;
    f[6*c + 3].v1 = 8*c + 4;
    f[6*c + 3].v2 = 8*c + 0;
    f[6*c + 3].v3 = 8*c + 1;
    f[6*c + 3].v4 = 8*c + 5;
    f[6*c + 3].visible = 1;
    f[6*c + 3].vlist = v;
    f[6*c + 4].v1 = 8*c + 4;
    f[6*c + 4].v2 = 8*c + 6;
    f[6*c + 4].v3 = 8*c + 2;
    f[6*c + 4].v4 = 8*c + 0;
    f[6*c + 4].visible = 1;
    f[6*c + 4].vlist = v;
    f[6*c + 5].v1 = 8*c + 1;
    f[6*c + 5].v2 = 8*c + 3;
    f[6*c + 5].v3 = 8*c + 7;
    f[6*c + 5].v4 = 8*c + 5;
    f[6*c + 5].visible = 1;
    f[6*c + 5].vlist = v;
 }

 x = y = z = xr = yr = zr = 0;
}


void TANK::set_col(char col)
{
 char i;

 for (i = 0; i != 8; i++) {
    o[i].c = v[i].c = col + i*2;
    o[8 + i].c = v[8 + i].c = col + i*2;
    o[16 + i].c = v[16 + i].c = BLACK + i*2;
 }
}
