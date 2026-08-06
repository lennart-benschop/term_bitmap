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

#include "font8x8_basic.h"


void
tbm_plottext(void *bm, char *s)
{
      tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
      unsigned int plotmode=bms->fgmode;
      unsigned char c;
      unsigned int i,j;
      int x=bms->cur_x;
      int y=bms->cur_y;
      while((c=*s++) != 0) {
	if (c>=32 && c<127) {
	  for (i=0; i<8; i++) {
	    for (j=0; j<8; j++) {
	      if ((font8x8_basic[c][i] >> j) & 1) {
		bms->fgmode = plotmode;
	      } else {
		bms->fgmode = bms->bgmode;
	      }
	      tbm_plotdot(bm,x+j,y+i);
	    }
	  }
	  x+=8;
	}
      }
      bms->cur_x = x;
      bms->cur_y = y;
      bms->fgmode = plotmode;
}
