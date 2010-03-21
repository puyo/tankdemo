// 3D Tank Game 1.0
// Gregory McIntyre 1998
// test functions


#include <cstdio>
#include <allegro.h>
#include "colours.h"
#include "tank.h"

#include "tests.h"


///  FUNCTION DEFINITIONS  /////////////////////////////////

// tank test - rotate and view a tank
void tank_test()
{
 PALETTE pal;
 BITMAP *buffer;
 MATRIX trans, cam;
 TANK p;
 int c, vc, fc;
 fixed dist = (128 << 16);

 buffer = create_bitmap(SCREEN_W, SCREEN_H);

 get_camera_matrix(&cam,
                   0, 0,          0,         // eye position
                   0, 0,          itofix(1), // front vector
                   0, itofix(-1), 0,         // up vector
                   itofix(32),               // field of view
                   itofix(1));               // aspect ratio

 for (;;) {
    clear(buffer);

    for (c = 0; c != 256; c++)
       putpixel(buffer, c, 0, c);

    get_transformation_matrix(&trans, 1<<16, p.xr, p.yr, p.zr, p.x, p.y, dist);

    // transform verticies
    for (vc = 0; vc != 3*8; vc++) {
       apply_matrix(&trans, p.o[vc].x,  p.o[vc].y,  p.o[vc].z,
                           &p.v[vc].x, &p.v[vc].y, &p.v[vc].z);
    }

    for (fc = 0; fc != 3*6; fc++) {
      // cull backfaces
      if (polygon_z_normal(&p.v[p.f[fc].v1],
                           &p.v[p.f[fc].v2],
                           &p.v[p.f[fc].v3]) < 0)
        p.f[fc].visible = FALSE;
      else
        p.f[fc].visible = TRUE;
    }

    // 3D space coodinates -> 2D screen coodinates
    for (vc = 0; vc != 3*8; vc++) {
       apply_matrix(&cam, p.v[vc].x,  p.v[vc].y,  p.v[vc].z,
                         &p.v[vc].x, &p.v[vc].y, &p.v[vc].z);
       persp_project(p.v[vc].x,  p.v[vc].y, p.v[vc].z,
                    &p.v[vc].x, &p.v[vc].y);
    }

    qsort(p.f, 3*6, sizeof(QUAD), quad_cmp);

    // draw the tank
    for (fc = 0; fc != 3*6; fc++) {
       if (p.f[fc].visible) {
         quad3d(buffer, POLYTYPE_GCOL, NULL,
               &p.v[p.f[fc].v1], &p.v[p.f[fc].v2],
               &p.v[p.f[fc].v3], &p.v[p.f[fc].v4]);
       }
    }

    // copy the buffer to the screen
    vsync();
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    // handle keys
    // tank rotation
    if (key[KEY_UP])
      p.xr += (1 << 16);
    if (key[KEY_DOWN])
      p.xr -= (1 << 16);
    if (key[KEY_LEFT])
      p.yr += (1 << 16);
    if (key[KEY_RIGHT])
      p.yr -= (1 << 16);

    // tank movement
    if (key[KEY_COMMA])
      p.x -= (1 << 16);
    if (key[KEY_STOP])
      p.x += (1 << 16);

    // zoom
    if (key[KEY_EQUALS])
      dist -= (5 << 16);
    if (key[KEY_MINUS])
      dist += (5 << 16);

    if (key[KEY_ESC])
      goto getout;

    if (key[KEY_F12]) {
      // capture the screen to a pcx file
      get_palette(pal);
      save_bitmap("tankscrn.pcx", buffer, pal);   
    }
  }
  getout:
  destroy_bitmap(buffer);
}



// callback for qsort()
int quad_cmp(const void *e1, const void *e2)
{
 QUAD *q1 = (QUAD *)e1;
 QUAD *q2 = (QUAD *)e2;

 fixed d1 = q1->vlist[q1->v1].z + q1->vlist[q1->v2].z +
            q1->vlist[q1->v3].z + q1->vlist[q1->v4].z;

 fixed d2 = q2->vlist[q2->v1].z + q2->vlist[q2->v2].z +
            q2->vlist[q2->v3].z + q2->vlist[q2->v4].z;

 return d2 - d1;
}

