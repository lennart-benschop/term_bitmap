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


static const char base64_chars[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";


void redraw_kitty(tbm_bitmap_t* bms)
{
  const palette_t *pl = bms->palette;
  unsigned int x,y;
  unsigned int npix=0;
  printf("\033[%d;%dH",bms->posy+1,bms->posx+1); // Position cursor to start graphics
  //printf("\033_Ga=d;\033\\"); /* Delete the previous image */
  printf("\033_Gf=24,s=%d,v=%d,a=T,m=1;",bms->width,bms->height); // Put terminal in Kitty graphics mode.
  for (y=0;y<bms->height;y++) {
    for (x=0;x<bms->width;x++) {
      uint8_t c = bms->bitmap[y*bms->width+x];
      uint32_t pixcol = (pl[c].r << 16) | (pl[c].g << 8)| (pl[c].b);
      putchar(base64_chars[(pixcol>>18)&0x3f]);
      putchar(base64_chars[(pixcol>>12)&0x3f]);
      putchar(base64_chars[(pixcol>>6)&0x3f]);
      putchar(base64_chars[pixcol&0x3f]);
      if (++npix == 1024) { // End current data chunk and immediately start the next one.
	printf("\033\\\033_Gm=1;");
	npix = 0;
      }
    }
  }
  printf("\033\\\033_Gm=0;\033\\\n"); // End current chunk and send final empty chunk
}

void update_whc_kitty(unsigned int *w, unsigned int *h, unsigned int *c)
{
}
