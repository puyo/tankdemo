// 3D Tank Game 1.0
// Gregory McIntyre 1998
// main function

#include <stdio.h>
#include "allegro.h"
#include "consts.h"
#include "world.h"
#include "tank.h"

#include "tests.h"


///  FUNCTION PROTOTYPES  //////////////////////////////////

int init();
void finish();



///  GLOBAL DATA  //////////////////////////////////////////

BITMAP *buffer;
PALETTE pal;

WORLD world;
TANK p[PLAYERS];



///  MAIN FUNCTION  ////////////////////////////////////////

int main(void)
{
 int error;

 error = init();
 if (error)
   return error;

 tank_test();

 finish();
 return 0;
}



int init()
{
 int c, w, h;

 allegro_init();
 install_timer();
 install_keyboard();

 for (c = 0; c != 32; c++)
    pal[c].r = pal[c].g = pal[c].b = c * 63/32;

 for (c = 0; c != 16; c++) {
    pal[32 + c].r = 8 + c*2;
    pal[32 + c].g = pal[32 + c].b = 0;
 }
 for (c = 0; c != 16; c++) {
    pal[48 + c].g = 8 + c*2;
    pal[48 + c].r = pal[48 + c].b = 0;
 }
 for (c = 0; c != 16; c++) {
    pal[64 + c].b = 8 + c*2;
    pal[64 + c].r = pal[64 + c].g = 0;
 }
 for (c = 0; c != 16; c++) {
    pal[80 + c].r = pal[80 + c].g = 8 + c*2;
    pal[80 + c].b = 0;
 }
 for (c = 0; c != 16; c++) {
    pal[96 + c].r = pal[96 + c].b = 8 + c*2;
    pal[96 + c].g = 0;
 }
 for (c = 0; c != 16; c++) {
    pal[112 + c].g = pal[112 + c].b = 8 + c*2;
    pal[112 + c].r = 0;
 }
 pal[255].r = pal[255].g = pal[255].b = 63;

 set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);
 set_palette(pal);

 /*
 if (!gfx_mode_select(&c, &w, &h)) {
   allegro_exit();
   return 1;
 }

 if (set_gfx_mode(c, w, h, 0, 0) != 0) {
   allegro_exit();
   printf("Error setting graphics mode\n%s\n\n", allegro_error);
   return 1;
 }
 */

 buffer = create_bitmap(SCREEN_W, SCREEN_H);

 set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

 get_camera_matrix(&world.cam,
                   0, 0, 0,             // eye position
                   0, 0, itofix(1),     // front vector
                   0, itofix(-1), 0,    // up vector
                   itofix(32),          // field of view
                   itofix(1));          // aspect ratio
 return 0;
}
END_OF_MAIN();



void finish()
{
 destroy_bitmap(buffer);
 allegro_exit();
}
