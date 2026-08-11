/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/

#include "term_bitmap.h"
#include "tbm_internal.h"
#include <string.h>

static const palette_t palette_2[2] = {
  {0, 0, 0},
  {255, 255, 255},
};

static const palette_t palette_4[4] = {
  {0, 0, 0},
  {255, 0, 0},
  {255, 255, 0},
  {255, 255, 255},
};

static const palette_t palette_8[8] = {
  {0, 0, 0},
  {255, 0, 0},
  {0, 255, 0},
  {255, 255, 0},
  {0, 0, 255},
  {255, 0, 255},
  {0, 255, 255},
  {255, 255, 255},
};

static const palette_t palette_16[16] = {
  {0, 0, 0},
  {128, 0, 0},
  {0, 128, 0},
  {128, 64, 0},
  {0, 0, 128},
  {128, 0, 128},
  {0, 128, 128},
  {128, 128, 128},
  {64, 64, 64},
  {255, 0, 0},
  {0, 255, 0},
  {255, 255, 0},
  {0, 0, 255},
  {255, 0, 255},
  {0, 255, 255},
  {255, 255, 255},
};

static palette_t palette_64[64];
static palette_t palette_256[256];

void set_palette(tbm_bitmap_t* bm)
{
  unsigned int i;
  if (palette_256[255].r == 0) {
    for (i=0; i<256; i++) {
      palette_256[i].r = (i & 7)*255/7;
      palette_256[i].g = ((i>>3)& 7)*255/7;
      palette_256[i].b = ((i>>6)& 3)*255/3;
    }
  }
  if (palette_64[63].r == 0) {
    for (i=0; i<64; i++) {
      palette_64[i].r = (i & 3)*255/3;
      palette_64[i].g = ((i>>2) & 3)*255/3;
      palette_64[i].b = ((i>>4) & 3)*255/3;
    }
  }
  if(bm->ncolors >= 256) {
    bm->ncolors = 256;
    bm->palette = palette_256;
  } else if(bm->ncolors >= 64){
    bm->ncolors = 64;
    bm->palette = palette_64;
  } else if(bm->ncolors >= 16){
    bm->ncolors = 16;
    bm->palette = bm->small_palette;
    memcpy(bm->small_palette,palette_16, sizeof(palette_16));
  } else if(bm->ncolors >= 8){
    bm->ncolors = 8;
    bm->palette = bm->small_palette;
    memcpy(bm->small_palette,palette_8, sizeof(palette_8));
  } else if(bm->ncolors >= 4){
    bm->ncolors = 4;
    bm->palette = bm->small_palette;
    memcpy(bm->small_palette,palette_4, sizeof(palette_4));
  } else {
    bm->ncolors = 2;
    bm->palette = bm->small_palette;
    memcpy(bm->small_palette,palette_2, sizeof(palette_2));
  }
}

/* tbm_set_palette

   Change colour index i in the palette of the indicated screen.
   Only works if number of columns is 16 or less
*/
void
tbm_set_palette(void *bm,
		unsigned int i,
		unsigned int r,
		unsigned int g,
		unsigned int b)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  if (i<16) {
    bms->small_palette[i].r = r;
    bms->small_palette[i].g = g;
    bms->small_palette[i].b = b;
  }
}

void
tbm_get_palette(void *bm,
		unsigned int i,
		unsigned int *r,
		unsigned int *g,
		unsigned int *b)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  if (i<bms->ncolors) {
    *r =bms->palette[i].r;
    *g =bms->palette[i].g;
    *b =bms->palette[i].b;
  }
}
