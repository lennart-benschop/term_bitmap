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

void redraw_braille(tbm_bitmap_t* bms)
{
  unsigned int x,y,code;
  uint8_t byte;
  printf("\033[%d;%dH",bms->posy+1,bms->posx+1); // Position cursor to start graphics
  for (y=0; y<bms->height; y+=4) {
    for (x=0; x<bms->width; x+=2) {
      byte = 0;
      if(bms->bitmap[(y+0)*bms->width+x+0]) byte|=0x1;
      if(bms->bitmap[(y+0)*bms->width+x+1]) byte|=0x8;
      if(bms->bitmap[(y+1)*bms->width+x+0]) byte|=0x2;
      if(bms->bitmap[(y+1)*bms->width+x+1]) byte|=0x10;
      if(bms->bitmap[(y+2)*bms->width+x+0]) byte|=0x4;
      if(bms->bitmap[(y+2)*bms->width+x+1]) byte|=0x20;
      if(bms->bitmap[(y+3)*bms->width+x+0]) byte|=0x40;
      if(bms->bitmap[(y+3)*bms->width+x+1]) byte|=0x80;
      code = 0x2800+byte;
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

void update_whc_braille(unsigned int *w, unsigned int *h, unsigned int *c)
{
  *w = (*w+1)/2 * 2; // Width shall be even.
  *h = (*h+3)/4 * 4; // Heigth shall be multiple of 4.
  *c = 2; // Only support monochrome bitmaps with this.
}
