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


/* The unicode code points for 2x3 sextants Note that this range skips
   block patterns that are already defined elsewhere */
static const unsigned int sextant_table[64] = {
  0x00020,
  0x1fb00,
  0x1fb01,
  0x1fb02,
  0x1fb03,
  0x1fb04,
  0x1fb05,
  0x1fb06,
  0x1fb07,
  0x1fb08,
  0x1fb09,
  0x1fb0a,
  0x1fb0b,
  0x1fb0c,
  0x1fb0d,
  0x1fb0e,
  0x1fb0f,
  0x1fb10,
  0x1fb11,
  0x1fb12,
  0x1fb13,
  0x0258c,
  0x1fb14,
  0x1fb15,
  0x1fb16,
  0x1fb17,
  0x1fb18,
  0x1fb19,
  0x1fb1a,
  0x1fb1b,
  0x1fb1c,
  0x1fb1d,
  0x1fb1e,
  0x1fb1f,
  0x1fb20,
  0x1fb21,
  0x1fb22,
  0x1fb23,
  0x1fb24,
  0x1fb25,
  0x1fb26,
  0x1fb27,
  0x02590,
  0x1fb28,
  0x1fb29,
  0x1fb2a,
  0x1fb2b,
  0x1fb2c,
  0x1fb2d,
  0x1fb2e,
  0x1fb2f,
  0x1fb30,
  0x1fb31,
  0x1fb32,
  0x1fb33,
  0x1fb34,
  0x1fb35,
  0x1fb36,
  0x1fb37,
  0x1fb38,
  0x1fb39,
  0x1fb3a,
  0x1fb3b,
  0x02588,
};

void redraw_sextant(tbm_bitmap_t* bms)
{
  unsigned int x,y,code;
  uint8_t byte;
  printf("\033[%d;%dH",bms->posy+1,bms->posx+1); // Position cursor to start graphics
  for (y=0; y<bms->height; y+=3) {
    for (x=0; x<bms->width; x+=2) {
      byte = 0;
      if(bms->bitmap[(y+0)*bms->width+x+0]) byte|=0x1;
      if(bms->bitmap[(y+0)*bms->width+x+1]) byte|=0x2;
      if(bms->bitmap[(y+1)*bms->width+x+0]) byte|=0x4;
      if(bms->bitmap[(y+1)*bms->width+x+1]) byte|=0x8;
      if(bms->bitmap[(y+2)*bms->width+x+0]) byte|=0x10;
      if(bms->bitmap[(y+2)*bms->width+x+1]) byte|=0x20;
      code = sextant_table[byte];
      if (code < 0x80) {
	putchar(code);
      } else if (code < 0x800) {
	putchar(0xC0+(code>>6));
	putchar(0x80+(code & 0x3f));
      } else if (code < 0x10000) {
	putchar(0xE0+(code>>12));
	putchar(0x80+((code>>6) & 0x3f));
	putchar(0x80+(code & 0x3f));
      } else {
	putchar(0xF0+(code>>18));
	putchar(0x80+((code>>12) & 0x3f));
	putchar(0x80+((code>>6) & 0x3f));
	putchar(0x80+(code & 0x3f));
      }
    }
    putchar('\n');
  }
}

void update_whc_sextant(unsigned int *w, unsigned int *h, unsigned int *c)
{
  *w = (*w+1)/2 * 2; // Width shall be even.
  *h = (*h+2)/3 * 3; // Heigth shall be multiple of 3.
  *c = 2; // Only support monochrome bitmaps with this.
}
