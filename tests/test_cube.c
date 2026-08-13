/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.

   Spinning cube demo, inspired by a BBC Basic program by Dean Belfield.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "term_bitmap.h"


static float sind(float x)
{
  return sinf(x*M_PI/180.0);
}

static float cosd(float x)
{
  return cosf(x*M_PI/180.0);
}


typedef struct {
  float x,y,z;
} point3_t;

point3_t vertices[8] = {
  {-1,1,1},
  {1,1,1},
  {-1,-1,1},
  {1,-1,1},
  {-1,1,-1},
  {1,1,-1},
  {-1,-1,-1},
  {1,-1,-1},
};

typedef struct {
  int v1,v2,v3,v4; 
} face;

/* List of vertices (index) for each of the six cube faces. */
face faces[6] = {
  {0,1,3,2},
  {6,7,5,4},
  {1,5,7,3},
  {2,6,4,0},
  {2,3,7,6},
  {0,4,5,1},
};

typedef struct {
  float x,y;
} point2_t;


int main(void)
{
  unsigned int width;
  unsigned int height;
  unsigned int ncolors;
  unsigned int mode;
  void *bm;
  float theta=0.0,psi=0.0,phi=0.0;
  float observer_dist=3.2,scale;
  point2_t proj[8],disp;
  int i;
  tbm_get_recommended(&width, &height, &ncolors, &mode);
  if (ncolors > 8) ncolors = 8;
  bm = tbm_new_screen(width,height,0,0,ncolors,mode);
  if (!bm) return -1;
  disp.x=0;
  disp.y=0;
  scale = height;
  observer_dist = 6.4;
  printf("\033[2J\n"); // Clear the screen first

  while (phi<10000.0) {
    tbm_clear(bm);
    // Rotate the vertices.
    for (i=0; i<8; i++) {
      point3_t Pa = vertices[i];
      float x,y;

      y   =Pa.y*cosd(phi)  -Pa.z*sind(phi);
      Pa.z=Pa.y*sind(phi)  +Pa.z*cosd(phi);
      x   =Pa.x*cosd(theta)-Pa.z*sind(theta);
      Pa.z=Pa.x*sind(theta)+Pa.z*cosd(theta);
      Pa.x=   x*cosd(psi)     -y*sind(psi);
      Pa.y=   x*sind(psi)     +y*cosd(psi);
      // Translate
      Pa.x+=disp.x;
      Pa.y+=disp.y;
      // Projection
      proj[i].x=Pa.x*scale/(observer_dist-Pa.z)+width/2;
      proj[i].y=Pa.y*scale/(observer_dist-Pa.z)+height/2;
    }
    // Draw each face of the cube.
    for (i=0; i<6; i++) {
      point2_t p1,p2,p3,p4; // 4 points of current face.
      p1=proj[faces[i].v1];
      p2=proj[faces[i].v2];
      p3=proj[faces[i].v3];
      p4=proj[faces[i].v4];
      if (p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y) <= 0.0) {
	// face is not seen from behind, therefore not hidden.
	if (ncolors < 8) {
	  // Plot wireframe
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_lineto(bm,p2.x,p2.y);
	  tbm_lineto(bm,p3.x,p3.y);
	  tbm_lineto(bm,p4.x,p4.y);
	  tbm_lineto(bm,p1.x,p1.y);
	} else {
	  // Plot solid polygon consiting of 2 triangles.
	  tbm_setfg(bm,i+1);
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_triangle(bm,p2.x,p2.y,p3.x,p3.y);
	  tbm_moveto(bm,p3.x,p3.y);
	  tbm_triangle(bm,p4.x,p4.y,p1.x,p1.y);
	}
      }
    }
    phi+=4.0;
    theta-=1.0;
    psi+=0.0;
    tbm_redraw(bm);
    usleep(20000);
  }
  
  tbm_delete(bm);
  return 0;
}
