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

#define MIN_RL 4

void redraw_sixel(tbm_bitmap_t* bms)
{
  const palette_t *pl = bms->palette;
  unsigned int i,j,x,y;
  uint8_t c,oldc;
  unsigned int rlcount;
  bool color_used;
  printf("\033[%d;%dH",bms->posy+1,bms->posx+1); // Position cursor to start graphics
  printf("\033Pq"); // Enter sixel mode
  // Write out the colors of the palette.
  for (i=0; i<bms->ncolors; i++) {
    printf("#%d;2;%d;%d;%d",i,pl[i].r*100/255,pl[i].g*100/255,pl[i].b*100/255);
  }
  for (y=0; y<bms->height; y+=6) {
    for (i=0; i<bms->ncolors; i++) {
      color_used = false;
      for (j=0; j<bms->width*6; j++) {
	if (bms->bitmap[y*bms->width+j] == i) {
	  color_used=true;
	  break;
	}
      }
      if (color_used) {
	printf("#%d",i);
	oldc = 0;
	rlcount = 0;
	for (x=0; x<bms->width; x++) {
	  c = 0;
	  if (bms->bitmap[(y+0)*bms->width+x] == i) c|=0x01;
	  if (bms->bitmap[(y+1)*bms->width+x] == i) c|=0x02;
	  if (bms->bitmap[(y+2)*bms->width+x] == i) c|=0x04;
	  if (bms->bitmap[(y+3)*bms->width+x] == i) c|=0x08;
	  if (bms->bitmap[(y+4)*bms->width+x] == i) c|=0x10;
	  if (bms->bitmap[(y+5)*bms->width+x] == i) c|=0x20;
	  if (oldc != c || x+1==bms->width) {
	    if (oldc==c) rlcount++;
	    if (rlcount < MIN_RL) {
	      for (j=0;j<rlcount;j++) putchar(oldc+63);
	  } else {
	      printf("!%d%c",rlcount,oldc+63);
	    }
	    if (oldc!=c && x+1==bms->width) putchar(c+63);
	    rlcount = 1;
	  oldc = c;
	  } else {
	    rlcount ++;
	  }
	}
      }
      if (i==bms->ncolors-1) printf("-\n"); else if (color_used) printf("$\n"); // End of line.
    }
  }
  printf("\033\\\n"); // Exit sixel mode
  //fflush(stdout);
}

void update_whc_sixel(unsigned int *w, unsigned int *h, unsigned int *c)
{
  // Force height to be a multiple of 6.
  *h = (*h + 5)/6*6;
}
