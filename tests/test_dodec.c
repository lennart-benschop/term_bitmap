/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.

   Spinning dodecahedron demo, inspired by a BBC Basic program by Dean Belfield.
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


#define PHI 1.618033988749895
#define IPHI 0.6180339887498949

typedef struct {
  float x,y,z;
} point3_t;

point3_t vertices[20] = {
  {IPHI, 0, PHI}, // 0
  {-IPHI, 0, PHI}, // 1
  {-IPHI, 0, -PHI}, // 2
  {IPHI, 0, -PHI}, // 3
  {0, PHI, IPHI}, // 4
  {0, -PHI, IPHI}, // 5
  {0, -PHI, -IPHI}, // 6
  {0, PHI, -IPHI}, // 7
  {PHI, IPHI, 0}, // 8
  {-PHI, IPHI, 0}, // 9
  {-PHI, -IPHI, 0}, // 10
  {PHI, -IPHI, 0}, // 11
  {1, 1, 1}, // 12
  {-1, 1, 1}, // 13
  {-1, -1, 1}, // 14
  {1, -1, 1}, // 15
  {1, -1, -1}, // 16
  {1, 1, -1}, // 17
  {-1, 1, -1}, // 18
  {-1, -1, -1}, // 19
};

typedef struct {
  int v1,v2,v3,v4,v5; 
} face;

/* List of vertices (index) for each of the six cube faces. */
face faces[12] = {
  {0, 1, 14, 5, 15}, // Dark red
  {1, 0, 12, 4, 13}, // Dark green
  {3, 2, 18, 7, 17}, // Brown
  {2, 3, 16, 6, 19}, // Dark blue
  {4, 7, 18, 9, 13}, // Dark Magenta
  {7, 4, 12, 8, 17}, // Dark Cyan
  {5, 6, 16, 11, 15}, // Light grey
  {6, 5, 14, 10, 19}, // Dark grey
  {8, 11, 16, 3, 17}, // Bright red +
  {11, 8, 12, 0, 15}, // Bright green +
  {9, 10, 14, 1, 13}, // Yellow
  {10, 9, 18, 2, 19}, // Bright blue 
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
  point2_t proj[20],disp;
  int i;
  tbm_get_recommended(&width, &height, &ncolors, &mode);
  if (ncolors > 16) ncolors = 16;
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
    for (i=0; i<20; i++) {
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
    for (i=0; i<12; i++) {
      point2_t p1,p2,p3,p4,p5; // 5 points of current face.
      p1=proj[faces[i].v1];
      p2=proj[faces[i].v2];
      p3=proj[faces[i].v3];
      p4=proj[faces[i].v4];
      p5=proj[faces[i].v5];
      if (p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y) <= 0.0) {
	// face is not seen from behind, therefore not hidden.
	if (ncolors < 16) {
	  // Plot wireframe
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_lineto(bm,p2.x,p2.y);
	  tbm_lineto(bm,p3.x,p3.y);
	  tbm_lineto(bm,p4.x,p4.y);
	  tbm_lineto(bm,p5.x,p5.y);
	  tbm_lineto(bm,p1.x,p1.y);
	} else {
	  // Plot solid polygon consiting of 2 triangles.
	  tbm_setfg(bm,i+1);
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_triangle(bm,p2.x,p2.y,p3.x,p3.y);
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_triangle(bm,p3.x,p3.y,p4.x,p4.y);
	  tbm_moveto(bm,p1.x,p1.y);
	  tbm_triangle(bm,p4.x,p4.y,p5.x,p5.y);
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
