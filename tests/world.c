/* 3D test world    */
/* Gregory McIntyre */
/* 1998             */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <allegro.h>


/* display a nice 8x8 chessboard grid */
#define GRID_SIZE    16
#define TILE_SIZE     1

/* convert radians to degrees */
#define DEG(n)    ((n) * 180.0 / M_PI)


/* parameters controlling the camera and projection state */
int fov = 32;
float aspect = 1;
float xpos = 0;
float ypos = -0.8;
float zpos = -4;
float heading = 0;
float pitch = 0;
float roll = 0;


/* render a tile of the grid */
void draw_square(BITMAP *bmp, MATRIX_f *camera, int x, int z)
{
   V3D_f v[4];
   int c;

   /* set up four vertices with the world-space position of the tile */
   v[0].x = x - GRID_SIZE/2;
   v[0].y = 0;
   v[0].z = z - GRID_SIZE/2;

   v[1].x = x - GRID_SIZE/2 + TILE_SIZE;
   v[1].y = 0;
   v[1].z = z - GRID_SIZE/2;

   v[2].x = x - GRID_SIZE/2 + TILE_SIZE;
   v[2].y = 0;
   v[2].z = z - GRID_SIZE/2 + TILE_SIZE;

   v[3].x = x - GRID_SIZE/2;
   v[3].y = 0;
   v[3].z = z - GRID_SIZE/2 + TILE_SIZE;

   /* for each vertex... */
   for (c=0; c<4; c++) {
      /* apply the camera matrix, translating world space -> view space */
      apply_matrix_f(camera, v[c].x, v[c].y, v[c].z, &v[c].x, &v[c].y, &v[c].z);

      /* reject anything too close to us or behind us. Really this ought
       * to do a proper 3d clip, but I have yet to write the code for that :-)
       */
      if (v[c].z < 0.01)
	 return;

      /* project view space -> screen space */
      persp_project_f(v[c].x, v[c].y, v[c].z, &v[c].x, &v[c].y);
   }

   if (v[0].c = (((x + z) / TILE_SIZE) & 1)) {
     /* every second square */
     v[0].c = 1;
     v[1].c = 43;
     v[2].c = 85;
     v[3].c = 127;
   }
   else {
     v[0].c = 127 + 1;
     v[1].c = 127 + 43;
     v[2].c = 127 + 85;
     v[3].c = 127 + 127;
   }

   /* render the square */
   quad3d_f(bmp, POLYTYPE_GCOL, NULL, &v[0], &v[1], &v[2], &v[3]);
}


/* draw everything */
void render(BITMAP *bmp)
{
   char buf[80];
   MATRIX_f roller, camera;
   int x, y, w, h;
   float xfront, yfront, zfront;
   float xup, yup, zup;

   /* clear the background */
   clear(bmp);

   set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

   /* calculate the in-front vector */
   xfront = sin(heading) * cos(pitch);
   yfront = sin(pitch);
   zfront = cos(heading) * cos(pitch);

   /* rotate the up vector around the in-front vector by the roll angle */
   get_vector_rotation_matrix_f(&roller, xfront, yfront, zfront, roll*128.0/M_PI);
   apply_matrix_f(&roller, 0, -1, 0, &xup, &yup, &zup);

   /* build the camera matrix */
   get_camera_matrix_f(&camera,
		       xpos, ypos, zpos,        /* camera position */
		       xfront, yfront, zfront,  /* in-front vector */
		       xup, yup, zup,           /* up vector */
		       fov,                     /* field of view */
		       aspect);                 /* aspect ratio */

   /* draw the grid of squares */
   for (x=0; x<GRID_SIZE; x++)
      for (y=0; y<GRID_SIZE; y++)
	 draw_square(bmp, &camera, x * TILE_SIZE, y * TILE_SIZE);

   /* overlay some text */
   set_clip_rect(bmp, 0, 0, bmp->w, bmp->h);
   sprintf(buf, "Field of view: %d (f/F changes)", fov);
   textout_ex(bmp, font, buf, 0, 16, 255, -1);
   sprintf(buf, "Aspect ratio: %.2f (a/A changes)", aspect);
   textout_ex(bmp, font, buf, 0, 24, 255, -1);
   sprintf(buf, "X position: %.2f (x/X changes)", xpos);
   textout_ex(bmp, font, buf, 0, 32, 255, -1);
   sprintf(buf, "Y position: %.2f (y/Y changes)", ypos);
   textout_ex(bmp, font, buf, 0, 40, 255, -1);
   sprintf(buf, "Z position: %.2f (z/Z changes)", zpos);
   textout_ex(bmp, font, buf, 0, 48, 255, -1);
   sprintf(buf, "Heading: %.2f deg (left/right changes)", DEG(heading));
   textout_ex(bmp, font, buf, 0, 56, 255, -1);
   sprintf(buf, "Pitch: %.2f deg (pgup/pgdn changes)", DEG(pitch));
   textout_ex(bmp, font, buf, 0, 64, 255, -1);
   sprintf(buf, "Roll: %.2f deg (r/R changes)", DEG(roll));
   textout_ex(bmp, font, buf, 0, 72, 255, -1);
   sprintf(buf, "Front vector: %.2f, %.2f, %.2f", xfront, yfront, zfront);
   textout_ex(bmp, font, buf, 0, 80, 255, -1);
   sprintf(buf, "Up vector: %.2f, %.2f, %.2f", xup, yup, zup);
   textout_ex(bmp, font, buf, 0, 88, 255, -1);
}


/* deal with user input */
void process_input()
{
   if (key[KEY_F]) {
      if (key_shifts & KB_SHIFT_FLAG) {
	 if (fov < 96)
	    fov++;
      }
      else {
	 if (fov > 16)
	    fov--;
      }
   }

   if (key[KEY_A]) {
      if (key_shifts & KB_SHIFT_FLAG) {
	 aspect += 0.05;
	 if (aspect > 2)
	    aspect = 2;
      }
      else {
	 aspect -= 0.05;
	 if (aspect < .1)
	    aspect = .1;
      }
   }

   if (key[KEY_X]) {
      if (key_shifts & KB_SHIFT_FLAG)
	 xpos += 0.1;
      else
	 xpos -= 0.1;
   }

   if (key[KEY_Y]) {
      if (key_shifts & KB_SHIFT_FLAG)
	 ypos += 0.1;
      else
	 ypos -= 0.1;
   }

   if (key[KEY_Z]) {
      if (key_shifts & KB_SHIFT_FLAG)
	 zpos += 0.1;
      else
	 zpos -= 0.1;
   }

   if (key[KEY_LEFT])
      heading -= 0.04;

   if (key[KEY_RIGHT])
      heading += 0.04;

   if (key[KEY_PGUP])
      if (pitch > -M_PI/4)
	 pitch -= 0.03;

   if (key[KEY_PGDN])
      if (pitch < M_PI/4)
	 pitch += 0.03;

   if (key[KEY_R]) {
      if (key_shifts & KB_SHIFT_FLAG) {
	 if (roll < M_PI/4)
	    roll += 0.03;
      }
      else {
	 if (roll > -M_PI/4)
	    roll -= 0.03;
      }
   }

   if (key[KEY_UP]) {
      xpos += sin(heading) / 7;
      zpos += cos(heading) / 7;
   }

   if (key[KEY_DOWN]) {
      xpos -= sin(heading) / 7;
      zpos -= cos(heading) / 7;
   }
}


int main()
{
   BITMAP *buffer;
   PALETTE pal;
   int c;

   allegro_init();
   install_keyboard();

   if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
      allegro_exit();
      printf("Error setting graphics mode\n%s\n\n", allegro_error);
      return 1;
   }

   /* tile 1 colour */
   for (c = 1; c != 128; c++) {
      pal[c].r = c * 49/127;
      pal[c].g = 0;
      pal[c].b = c * 63/127;
   }
   /* tile 2 colour */
   for (c = 0; c != 127; c++) {
      pal[c + 128].r = 0;
      pal[c + 128].g = 0;
      pal[c + 128].b = c * 63/127;
   }

   pal[0].r = pal[0].g = pal[0].b = 0;
   pal[255].r = pal[255].g = pal[255].b = 63;

   set_pallete(pal);
   buffer = create_bitmap(SCREEN_W, SCREEN_H);

   while (!key[KEY_ESC]) {
      render(buffer);
      vsync();
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      process_input();
   }

   destroy_bitmap(buffer);
   return 0;
}
END_OF_MAIN();
