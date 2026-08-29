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



int main(void)
{
  unsigned int width;
  unsigned int height;
  unsigned int ncolors;
  unsigned int mode;
  void *bm;
  double xmin =-2.00;
  double xmax =+1;
  double ymin =-1.12;
  double ymax =+1.12;
  //double xmin =-.8;
  //double xmax =+.47;
  //double ymin =-.3;
  //double ymax =+.3;
  double x0,y0,x,y,t;
  int nx,ny;
  int iter;
  tbm_get_recommended(&width, &height, &ncolors, &mode);
  if (ncolors > 8) ncolors = 256;
  bm = tbm_new_screen(width,height,0,0,ncolors,mode);
  if (!bm) return -1;
  printf("\033[2J\n"); // Clear the screen first

  for (ny=0;ny<height;ny++) {
    y0=ymin+(ymax-ymin)*ny/height;
    for (nx=0; nx<width; nx++) {
      x0=xmin + (xmax-xmin)*nx/width;
      x=0.0;
      y=0.0;
      iter = 0;
      while (x*x + y*y < 4.0 && iter<255) {
	t=x*x - y*y+x0;
	y=2*x*y+y0;
	x=t;
	iter++;
      }
      tbm_setfg(bm,255-iter);
      tbm_plotdot(bm,nx,ny);
    }
  }
  
  
  tbm_redraw(bm);
  tbm_delete(bm);
  return 0;
}
