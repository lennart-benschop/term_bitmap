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
