/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/

#include "term_bitmap.h"
#include "tbm_internal.h"

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

void set_palette(tbm_bitmap_t* bm, unsigned int ncolors)
{
  switch(bm->ncolors) {
  case 2:
    bm->palette = palette_2;
    break;
  case 4:
    bm->palette = palette_4;
    break;
  case 8:
    bm->palette = palette_8;
    break;
  default:
    bm->palette = palette_16;
    break;
  }
}

