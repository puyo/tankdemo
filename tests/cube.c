/* 3D math test     */
/* Gregory McIntyre */
/* 1998             */


/* Polygon, 3D, quad3d */
/* c:\djgpp\allegro\allegro.txt */


#include <stdio.h>
#include <stdlib.h>
#include "allegro.h"

#define NUM_VERTICES       8     /* a cube has eight corners */
#define NUM_FACES          6     /* a cube has six faces */

#define DRAW_TYPE          POLYTYPE_GCOL

#define XSPEED             0
#define YSPEED             0
#define ZSPEED             0


/*** STRUCTURE DEFINITIONS ***********************************************/

typedef struct VTX
{
   fixed x, y, z;
} VTX;


typedef struct QUAD              /* four vertices makes a quad */
{
   VTX *vtxlist;
   int v1, v2, v3, v4;
} QUAD;


typedef struct SHAPE             /* store position of a shape */
{
   fixed x, y, z;                /* x, y, z position */
   fixed rx, ry, rz;             /* rotations */
   fixed dz;                     /* speed of movement */
   fixed drx, dry, drz;          /* speed of rotation */
} SHAPE;



/*** GLOBAL DATA *********************************************************/

VTX points[] =                   /* a cube, centered on the origin */
{
   /* vertices of the cube */
   { -40 << 16, -40 << 16, -40 << 16 },
   { -32 << 16,  32 << 16, -32 << 16 },
   {  32 << 16,  32 << 16, -32 << 16 },
   {  32 << 16, -32 << 16, -32 << 16 },
   { -32 << 16, -32 << 16,  32 << 16 },
   { -32 << 16,  32 << 16,  32 << 16 },
   {  32 << 16,  32 << 16,  32 << 16 },
   {  32 << 16, -32 << 16,  32 << 16 },
};


QUAD faces[] =                   /* group the vertices into polygons */
{
   { points, 0, 3, 2, 1 },
   { points, 4, 5, 6, 7 },
   { points, 0, 1, 5, 4 },
   { points, 2, 3, 7, 6 },
   { points, 0, 4, 7, 3 },
   { points, 1, 2, 6, 5 }
};


SHAPE shape;

VTX current_points[NUM_VERTICES];
QUAD current_faces[NUM_FACES];


BITMAP *buffer;


/*** FUNCTION DEFINITIONS ************************************************/


/* draw a quad */
void draw_quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4, int mode)
{
   int col;

   /* four vertices */
   V3D vtx1 = { v1->x, v1->y, v1->z, 0, 0, 0 };
   V3D vtx2 = { v2->x, v2->y, v2->z, 0, 0, 0 };
   V3D vtx3 = { v3->x, v3->y, v3->z, 0, 0, 0 };
   V3D vtx4 = { v4->x, v4->y, v4->z, 0, 0, 0 };

   /* cull backfaces */
   if (polygon_z_normal(&vtx1, &vtx2, &vtx3) < 0)
      return;

   /* set up the vertex color, differently for each rendering mode */
   switch (mode) {

      case POLYTYPE_FLAT:
         col = MID(128, 255 - fixtoi(v1->z+v2->z) / 16, 255);
         vtx1.c = vtx2.c = vtx3.c = vtx4.c = col;
         break;

      case POLYTYPE_GCOL:
	 vtx1.c = 208;
	 vtx2.c = 128;
	 vtx3.c = vtx2.c + 1;
	 vtx4.c = vtx1.c + 1;
/*	 vtx1.c = 208;
	 vtx2.c = 128;
	 vtx3.c = vtx2.c + 48;
	 vtx4.c = vtx1.c + 48;*/
         break;

      case POLYTYPE_ATEX_LIT:
      case POLYTYPE_PTEX_LIT:
         vtx1.c = MID(0, 255 - fixtoi(v1->z) / 4, 255);
         vtx2.c = MID(0, 255 - fixtoi(v2->z) / 4, 255);
         vtx3.c = MID(0, 255 - fixtoi(v3->z) / 4, 255);
         vtx4.c = MID(0, 255 - fixtoi(v4->z) / 4, 255);
         break; 
   }

   /* draw the quad */
   quad3d(buffer, mode, NULL, &vtx1, &vtx2, &vtx3, &vtx4);
}



/* draw the shapes calculated by translate_shapes() */
void draw()
{
   int c;
   QUAD *face = current_faces;
   VTX *v1, *v2, *v3, *v4;

   clear(buffer);

   for (c=0; c < NUM_FACES; c++) {
      /* find the vertices used by the face */
      v1 = face->vtxlist + face->v1;
      v2 = face->vtxlist + face->v2;
      v3 = face->vtxlist + face->v3;
      v4 = face->vtxlist + face->v4;

      draw_quad(buffer, v1, v2, v3, v4, DRAW_TYPE);
      face++;
   }

   blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}



/* initialise shape position */
void init_shape()
{
 int c;

 shape.x = SCREEN_W/2;
 shape.y = SCREEN_H/2;
 shape.z = itofix(191); /*shape.z = itofix(768);*/
 shape.rx = 0;
 shape.ry = 0;
 shape.rz = 0;
 shape.dz =  0; /*(random() & 0xFFFFF) - 0x80000;*/
 shape.drx = itofix(XSPEED);
 shape.dry = itofix(YSPEED);
 shape.drz = itofix(ZSPEED);
}


/* update shape positions */
void move_shape()
{
   int c;

   shape.z += shape.dz;

   if ((shape.z > itofix(1024)) || (shape.z < itofix(192)))
     shape.dz = -shape.dz;

   shape.drx = 0;
   shape.dry = 0;
   shape.drz = 0;

   if (key[KEY_UP])
     shape.drx = itofix(1);
   if (key[KEY_DOWN])
     shape.drx = itofix(-1);
   if (key[KEY_LEFT])
     shape.dry = itofix(1);
   if (key[KEY_RIGHT])
     shape.dry = itofix(-1);
   if (key[KEY_COMMA])
     shape.drz = itofix(1);
   if (key[KEY_STOP])
     shape.drz = itofix(-1);

   shape.rx += shape.drx;
   shape.ry += shape.dry;
   shape.rz += shape.drz;
}


/* translate shape from 3d world space to 2d screen space */
void translate_shape()
{
   int d;
   MATRIX matrix;
   VTX *outpoint = current_points;
   QUAD *outface = current_faces;

   /* build a transformation matrix */
   get_transformation_matrix(&matrix, itofix(1),
                             shape.rx, shape.ry, shape.rz,
                             shape.x, shape.y, shape.z);

   /* output the vertices */
   for (d=0; d<NUM_VERTICES; d++) {
      apply_matrix(&matrix, points[d].x, points[d].y, points[d].z, &outpoint[d].x, &outpoint[d].y, &outpoint[d].z);
      persp_project(outpoint[d].x, outpoint[d].y, outpoint[d].z, &outpoint[d].x, &outpoint[d].y);
   }

   /* output the faces */
   for (d=0; d<NUM_FACES; d++) {
      outface[d] = faces[d];
      outface[d].vtxlist = outpoint;
   }

   outpoint += NUM_VERTICES;
   outface += NUM_FACES;
}



/*** MAIN FUNCTION *******************************************************/

int main(void)
{
 allegro_init();
 install_timer();
 install_keyboard();

 rgb_map = malloc(sizeof(RGB_MAP));
 create_rgb_table(rgb_map, desktop_palette, NULL);

 set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);

 buffer = create_bitmap(SCREEN_W, SCREEN_H);
 set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

 init_shape();

 while (!key[KEY_ESC]) {
   move_shape();
   translate_shape();
   vsync();
   draw();
 }

 destroy_bitmap(buffer);
 free(rgb_map);
 allegro_exit();
 return 0;
}
END_OF_MAIN();
