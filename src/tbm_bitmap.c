
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

/* tbm_mono_bitmap_put
 */
void tbm_mono_bitmap_put(
			 void *bm,
			 uint8_t * bitmap_p,
			 int x,
			 int y,
			 int xsize,
			 int ysize)
{
      tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
      unsigned int plotmode=bms->fgmode;
      unsigned int bytes_per_line=(xsize+7)/8;
      unsigned int i,j;
      int xs=bms->cur_x;
      int ys=bms->cur_y;
      for (i=0; i<ysize; i++) {
	for (j=0; j<xsize; j++) {
	  if (bitmap_p[i*bytes_per_line+(j>>3)] & (0x80>>(j & 0x7))) {
	    bms->fgmode = plotmode;
	  } else {
	    bms->fgmode = bms->bgmode;
	  }
	  tbm_plotdot(bm,x+j,y+i);
	}
      }
      bms->cur_x = xs;
      bms->cur_y = ys;
      bms->fgmode = plotmode;
}

/* tbm_bitmap_put
 */
void tbm_bitmap_put(
		    void *bm,
		    uint8_t * bitmap_p,
		    int x,
		    int y,
		    int xsize,
		    int ysize,
		    bool is_transparent)
{
      tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
      uint8_t oldfg = bms->fgcolor;
      unsigned int i,j;
      uint8_t c;
      int xs=bms->cur_x;
      int ys=bms->cur_y;
      for (i=0; i<ysize; i++) {
	for (j=0; j<xsize; j++) {
	  c=bitmap_p[i*xsize+j];
	  if (c!=bms->bgcolor || !is_transparent) {
	    bms->fgcolor = c;
	    tbm_plotdot(bm,x+j,y+i);
	  }
	}
      }
      bms->cur_x = xs;
      bms->cur_y = ys;
      bms->fgcolor = oldfg;
}

/* tbm_bitmap_get
 */
void tbm_bitmap_get(
		    void *bm,
		    uint8_t * bitmap_p,
		    int x,
		    int y,
		    int xsize,
		    int ysize)
{
    unsigned int i,j;
    uint8_t c;
    for (i=0; i<ysize; i++) {
      for (j=0; j<xsize; j++) {
	c = tbm_getdot(bm,x+j,y+i);
	bitmap_p[i*xsize+j]=c;
      }
    }
}
