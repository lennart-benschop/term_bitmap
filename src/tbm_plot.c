/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/

#include "term_bitmap.h"
#include "tbm_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void
tbm_clear(void *bm)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    memset(bms->bitmap, 0, bms->width*bms->height);
}

void
tbm_moveto(void *bm, int x, int y)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    bms->cur_x = x;
    bms->cur_y = y;
}

void
tbm_getpos(void *bm, int *x, int *y)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    *x = bms->cur_x;
    *y = bms->cur_y;
}

void
tbm_plotdot(void *bm, int x, int y)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    bms->cur_x = x;
    bms->cur_y = y;
    if (x < 0 || x >= bms->width || y < 0 || y >= bms->height)
      return;
    switch(bms->fgmode) {
    case DRAW_MODE_FG:
      bms->bitmap[y*bms->width + x] = bms->fgcolor;
      break;
    case DRAW_MODE_BG:
      bms->bitmap[y*bms->width + x] = bms->bgcolor;
      break;
    case DRAW_MODE_INVERT:
      bms->bitmap[y*bms->width + x] = bms->ncolors - bms->bitmap[y*bms->width + x] - 1;
      break;
    }
}

uint8_t 
tbm_getdot(void *bm, int x, int y)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    if (x < 0 || x >= bms->width || y < 0 || y >= bms->height)
      return 0;
    else
      return bms->bitmap[y*bms->width + x];
}

void
tbm_lineto(void *bm, int x, int y)
{
    tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
    int x1 = bms->cur_x;
    int y1 = bms->cur_y;
    int x2 = x;
    int y2 = y;
    int i, j, k, t;
    int dx, dy,xs,ys;
    if (x1 < x2) {
      dx = x2 - x1;
      xs = 1;
    } else {
      dx = x1 - x2;
      xs = -1;
    }
    if (y1 < y2) {
      dy = y2 - y1;
      ys = 1;
    } else {
      dy = y1 - y2;
      ys = -1;
    }
    if (dx > dy) { /* Horizontal line */
      t=-dx/2;
      j=y1;
      i=x1;
      for (k=0; k<=dx; k++) {
	tbm_plotdot(bm, i, j);
	i+=xs;
	t+=dy;
	if (t>0) {
	  j=j+ys;
	  t=t-dx;
	}
      }
    } else {
      t=-dy/2;
      j=y1;
      i=x1;
      for (k=0; k<=dy; k++) {
	tbm_plotdot(bm, i, j);
	j+=ys;
	t+=dx;
	if (t>0) {
	  i=i+xs;
	  t=t-dy;
	}
      }
    }              
}


int interpolate (int y, int x1, int y1, int x2, int y2)
{
  if (y1==y2)
    return x2;
  else
    return (int)((y-y1)/((float)y2-y1+1)*(x2-x1)+x1+1);

}

void
tbm_triangle(void *bm, int x2, int y2, int x3, int y3)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  int t;
  int x1=bms->cur_x;
  int y1=bms->cur_y;
  int y, xleft, xright,old_x3,old_y3;
  old_x3 = x3;
  old_y3 = y3;
  // Order the pointx x1,y1, x2,y2 and x3,y3 by increasing y coordinate.
  if (y1 > y2) {
    t=x1, x1=x2, x2=t;
    t=y1, y1=y2, y2=t;
  }
  if (y1 > y3) {
    t=x1, x1=x3, x3=t;
    t=y1, y1=y3, y3=t;
  }
  if (y2 > y3) {
    t=x2, x2=x3, x3=t;
    t=y2, y2=y3, y3=t;
  }
  //printf("x1,y1=%d,%d x2,y2=%d,%dd x3,y3=%d,%d\n",x1,y1,x2,y2,x3,y3);
  
  for (y=y1; y<=y3; y++) {
    xleft = interpolate(y, x1, y1, x3, y3);
    if (y < y2)
      xright = interpolate(y, x1, y1, x2, y2);
    else if (y2==y3)
      xright = x2;
    else
      xright = interpolate(y, x2, y2, x3, y3);
    //printf("xl=%d, xr=%d y=%d\n",xleft, xright, y);
    
    tbm_moveto(bm, xleft, y);
    tbm_lineto(bm, xright, y);    
  }  
  //getchar();
  bms->cur_x = old_x3;
  bms->cur_y = old_y3;
}

void
tbm_circle(void *bm, int x, int y, unsigned int r, bool fill)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  int i,old_x,new_x;
  bms->cur_x = x;
  bms->cur_y = y;
  old_x = r;
  for (i=0; i<=r; i++) {
    new_x = sqrt(r*r-i*i);
    if (fill) {
      tbm_moveto(bm,x-new_x,y-i);
      tbm_lineto(bm,x+new_x,y-i);
      tbm_moveto(bm,x-new_x,y+i);
      tbm_lineto(bm,x+new_x,y+i);
    } else {
      tbm_moveto(bm,x-new_x,y-i);
      tbm_lineto(bm,x-old_x,y-i);
      tbm_moveto(bm,x+old_x,y-i);
      tbm_lineto(bm,x+new_x,y-i);
      tbm_moveto(bm,x-new_x,y+i);
      tbm_lineto(bm,x-old_x,y+i);
      tbm_moveto(bm,x+old_x,y+i);
      tbm_lineto(bm,x+new_x,y+i);      
    }
    old_x = new_x;
  }
  bms->cur_x = x;
  bms->cur_y = y;  
}
