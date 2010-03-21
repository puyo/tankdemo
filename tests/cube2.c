/* 3D test program   */
/* Gregory McIntyre  */
/* 1998              */

#include <stdio.h>
#include <stdlib.h>
#include "allegro.h"

#define NUM_POINTS   (8+6)
#define NUM_FACES    6

#define NUM_MODES    9


/*** STRUCTURE DEFINITIONS ***********************************************/

/* a 3d x,y,z position */
typedef struct POINT
{
 float x, y, z;
} POINT;

/* four vertices plus a normal make up a quad */
typedef struct QUAD 
{
 int v1, v2, v3, v4;
 int normal;
 int visible;
} QUAD;


/*** MAIN FUNCTION *******************************************************/

int main(void) {

 PALLETE mypal;

 COLOR_MAP *trans_map = NULL;
 COLOR_MAP *light_map = NULL;

 /* vertices of the cube */
 static POINT point[NUM_POINTS] =
 {
    /* regular vertices */
    { -32, -32, -32 },
    { -32,  32, -32 },
    {  32,  32, -32 },
    {  32, -32, -32 },
    { -32, -32,  32 },
    { -32,  32,  32 },
    {  32,  32,  32 },
    {  32, -32,  32 },
 
    /* normals */
    { -32, -32, -33 },
    { -32, -32,  33 },
    { -33,  32, -32 },
    {  33,  32, -32 },
    {  32, -33,  32 },
    {  32,  33,  32 }
 };
 

 /* a red gradiented cube (in POLYTYPE_GCOL) */
 static V3D_f vtx[NUM_POINTS] =
 {
    { 0, 0, 0, 0,  0,  32+1  },
    { 0, 0, 0, 0,  31, 32+5  },
    { 0, 0, 0, 31, 31, 32+9  },
    { 0, 0, 0, 31, 0,  32+13 },
    { 0, 0, 0, 31, 0,  32+17 },
    { 0, 0, 0, 31, 31, 32+21 },
    { 0, 0, 0, 0,  31, 32+25 },
    { 0, 0, 0, 0,  0,  32+31 }
 };

 /* a multicoloured cube */
 /*
 static V3D_f vtx[NUM_POINTS] =
 {
    { 0, 0, 0, 0,  0,  48  },
    { 0, 0, 0, 0,  31, 153 },
    { 0, 0, 0, 31, 31, 85  },
    { 0, 0, 0, 31, 0,  221 },
    { 0, 0, 0, 31, 0,  234 },
    { 0, 0, 0, 31, 31, 187 },
    { 0, 0, 0, 0,  31, 119 },
    { 0, 0, 0, 0,  0,  254 }
 };
 */

 /* six faces makes up a cube */
 QUAD face[NUM_FACES] = 
 {
   /* v1 v2 v3 v4 norm */
    { 0, 3, 2, 1, 8  },
    { 4, 5, 6, 7, 9  },
    { 1, 5, 4, 0, 10 },
    { 2, 3, 7, 6, 11 },
    { 7, 3, 0, 4, 12 },
    { 6, 5, 1, 2, 13 }
 };

 int c;
 int mode = POLYTYPE_GCOL;
 int tile = 1;
 
 float xr = -16;
 float yr = 24;
 float zr = 0;
 float dist = 128;
 float vx, vy, vz;
 float nx, ny, nz;
 MATRIX_f transform, camera;
 V3D_f *vertex, *normal;
 BITMAP *buffer;
 BITMAP *texture;

 allegro_init();
 install_timer();
 install_keyboard();

 for (c=0; c<32; c++)
    mypal[c] = desktop_pallete[c];
 for (c=0; c<32; c++) {
    mypal[c+32].r = c*2;
    mypal[c+32].g = mypal[c+32].b = 0;
 }
 for (c=0; c<32; c++) {
    mypal[c+64].g = c*2;
    mypal[c+64].r = mypal[c+64].b = 0;
 }
 for (c=0; c<32; c++) {
    mypal[c+96].b = c*2;
    mypal[c+96].r = mypal[c+96].g = 0;
 }
 for (c=0; c<32; c++) {
    mypal[c+128].r = mypal[c+128].g = c*2;
    mypal[c+128].b = 0;
 }
 for (c=0; c<32; c++) {
    mypal[c+160].r = mypal[c+160].b = c*2;
    mypal[c+160].g = 0;
 }
 for (c=0; c<32; c++) {
    mypal[c+192].g = mypal[c+192].b = c*2;
    mypal[c+192].r = 0;
 }
 for (c=0; c<31; c++)
    mypal[c+224].r = mypal[c+224].g = mypal[c+224].b = c*2;

 mypal[255].r = mypal[255].g = mypal[255].b = 63;
 mypal[0].r = mypal[0].g = mypal[0].b = 0;

 set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);

 buffer = create_bitmap(SCREEN_W, SCREEN_H);
 texture = create_bitmap(32, 32);

 clear(screen);
 textout_centre_ex(screen, font, "Creating RGB table", SCREEN_W/2, SCREEN_H/2, 254, -1);
 rgb_map = malloc(sizeof(RGB_MAP));
 create_rgb_table(rgb_map, mypal, NULL);
 textout_centre_ex(screen, font, "Creating translucency table", SCREEN_W/2, SCREEN_H/2, 254, -1);
 trans_map = malloc(sizeof(COLOR_MAP));
 create_trans_table(trans_map, mypal, 96, 96, 96, NULL);
 textout_centre_ex(screen, font, "Creating lighting table", SCREEN_W/2, SCREEN_H/2, 254, -1);
 light_map = malloc(sizeof(COLOR_MAP));
 create_light_table(light_map, mypal, 0, 0, 0, NULL);
 color_map = light_map;

 set_palette(mypal);

 /* setup texture */
 clear_to_color(texture, 4);
 circlefill(texture, 16, 16, 8, 2);
 circle(texture, 16, 16, 8, 1);
 line(texture, 0, 0, 31, 31, 3);
 line(texture, 31, 0, 0, 31, 3);
 textout_ex(texture, font, "Test", 1, 12, 15, -1);

 clear(screen);
 
 /* set projection parameters */
 set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);
 
 get_camera_matrix_f(&camera,
                     0, 0, 0,             /* eye position */
                     0, 0, 1,             /* front vector */
                     0, -1, 0,            /* up vector */
                     32,                  /* field of view */
                     1);                  /* aspect ratio */
 
 for (;;) {
    clear(buffer);

    /* build a transformation matrix */
    get_transformation_matrix_f(&transform, 1, xr, yr, zr, 0, 0, dist);
 
    /* transform vertices into view space */
    for (c=0; c<NUM_POINTS; c++)
       apply_matrix_f(&transform, point[c].x, point[c].y, point[c].z, &vtx[c].x, &vtx[c].y, &vtx[c].z);
 
    /* check for hidden faces */
    /*
    for (c=0; c<NUM_FACES; c++) {
       if (polygon_z_normal_f(&vtx[face[c].v1],
                              &vtx[face[c].v2],
                              &vtx[face[c].v3]) > 0)
         face[c].visible = TRUE;
       else
         face[c].visible = FALSE;

    }
    */

    for (c=0; c<NUM_FACES; c++) {
       vertex = &vtx[face[c].v1];
       normal = &vtx[face[c].normal];
       nx = normal->x - vertex->x;
       ny = normal->y - vertex->y;
       nz = normal->z - vertex->z;
       if (dot_product_f(vertex->x, vertex->y, vertex->z, nx, ny, nz) < 0)
         face[c].visible = TRUE;
       else
         face[c].visible = FALSE;
    }


    /* project vertices into screen space */
    for (c=0; c<NUM_POINTS; c++) {
       apply_matrix_f(&camera, vtx[c].x, vtx[c].y, vtx[c].z, &vtx[c].x, &vtx[c].y, &vtx[c].z);
       persp_project_f(vtx[c].x, vtx[c].y, vtx[c].z, &vtx[c].x, &vtx[c].y);
    }
 
    /* if mask mode, draw backfaces that may be seen through holes */
    if ((mode == POLYTYPE_ATEX_MASK) || (mode == POLYTYPE_PTEX_MASK)) {
      for (c=0; c<NUM_FACES; c++) {
        if (!face[c].visible) {
           quad3d_f(buffer, mode, texture, 
                    &vtx[face[c].v1], &vtx[face[c].v2],
                    &vtx[face[c].v3], &vtx[face[c].v4]);
        }
      }
    }

    /* draw the cube */
    for (c=0; c<NUM_FACES; c++) {
       if (face[c].visible) {
         quad3d_f(buffer, mode, texture, 
                  &vtx[face[c].v1], &vtx[face[c].v2],
                  &vtx[face[c].v3], &vtx[face[c].v4]);
       }
    }
 
    /* blit to the screen */
    vsync();
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
 
    if (key[KEY_UP])
      xr++;
    if (key[KEY_DOWN])
      xr--;
    if (key[KEY_LEFT])
      yr--;
    if (key[KEY_RIGHT])
      yr++;
    if (key[KEY_ESC])
      goto getout;

    /* handle user input */
    if (keypressed()) {
      c = readkey();
  
      switch (c >> 8) {
        case KEY_SPACE:
             mode++;
             if (mode >= NUM_MODES)
                mode = 0;
             break;
        case KEY_ENTER:
             tile = (tile == 1) ? 2 : 1;
             for (c=0; c<8; c++) {
                if (vtx[c].u)
                  vtx[c].u = 32 * tile - 1;
                if (vtx[c].v)
                  vtx[c].v = 32 * tile - 1;
             }
             break;
  
        default:
             if ((c & 0xFF) == '+') {
               if (dist > 64)
                  dist -= 16;
             }
             else if ((c & 0xFF) == '-') {
               if (dist < 1024)
                  dist += 16;
             }
             break;
      };
    }

 }
 
 getout:
 destroy_bitmap(buffer);
 destroy_bitmap(texture);
 free(rgb_map);
 free(trans_map);
 free(light_map);
 allegro_exit();

 return 0;
}
END_OF_MAIN();
