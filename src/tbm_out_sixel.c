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

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} palette_t;

static const palette_t palette_2[2] = {
  {0, 0, 0},
  {100, 100, 100},
};

static const palette_t palette_4[4] = {
  {0, 0, 0},
  {100, 0, 0},
  {100, 100, 0},
  {100, 100, 100},
};

static const palette_t palette_8[8] = {
  {0, 0, 0},
  {100, 0, 0},
  {0, 100, 0},
  {100, 100, 0},
  {0, 0, 100},
  {100, 0, 100},
  {0, 100, 100},
  {100, 100, 100},
};

static const palette_t palette_16[16] = {
  {0, 0, 0},
  {50, 0, 0},
  {0, 50, 0},
  {50, 25, 0},
  {0, 0, 50},
  {50, 0, 50},
  {0, 50, 50},
  {50, 50, 50},
  {25, 25, 25},
  {100, 0, 0},
  {0, 100, 0},
  {100, 100, 0},
  {0, 0, 100},
  {100, 0, 100},
  {0, 100, 100},
  {100, 100, 100},
};


void redraw_sixel(tbm_bitmap_t* bms)
{
  const palette_t *pl;
  unsigned int i,x,y;
  uint8_t c;
  switch(bms->ncolors) {
  case 2:
    pl = palette_2;
    break;
  case 4:
    pl = palette_4;
    break;
  case 8:
    pl = palette_8;
    break;
  default:
    pl = palette_16;
    break;
  }
  printf("\033[%d;%dH",bms->posy+1,bms->posx+1); // Position cursor to start graphics
  printf("\033Pq"); // Enter sixel mode
  // Write out the colors of the palette.
  for (i=0; i<bms->ncolors; i++) {
    printf("#%d;2;%d;%d;%d",i,pl[i].r,pl[i].g,pl[i].b);
  }
  for (y=0; y<bms->height; y+=6) {
    for (i=0; i<bms->ncolors; i++) {
      printf("#%d",i);
      for (x=0; x<bms->width; x++) {
	c = 0;
	if (bms->bitmap[(y+0)*bms->width+x] == i) c|=0x01;
	if (bms->bitmap[(y+1)*bms->width+x] == i) c|=0x02;
	if (bms->bitmap[(y+2)*bms->width+x] == i) c|=0x04;
	if (bms->bitmap[(y+3)*bms->width+x] == i) c|=0x08;
	if (bms->bitmap[(y+4)*bms->width+x] == i) c|=0x10;
	if (bms->bitmap[(y+5)*bms->width+x] == i) c|=0x20;
	putchar(c+63);
      }
      if (i==bms->ncolors-1) printf("-\n"); else printf("$\n"); // End of line.
    }
  }
  printf("\033\\\n"); // Exit sixel mode
  //fflush(stdout);
}

void update_whc_sixel(unsigned int *w, unsigned int *h, unsigned int *c)
{
  if (*c >=16)
    *c = 16;
  else if (*c >= 8)
    *c = 8;
  else if (*c >= 4)
    *c = 4;
  else 
    *c = 2;
  // Force height to be a multiple of 6.
  *h = (*h + 5)/6*6;
}
