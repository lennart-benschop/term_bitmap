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
tbm_get_recommended(unsigned int *width,
		    unsigned int *height,
		    unsigned int *ncolors,
		    unsigned int *mode)
{
  char *p = getenv("TBM_MODE");
  if (p==NULL) {
    *mode = 100;
  } else if (!strcmp(p,"kitty")) {
    *mode = BITMAP_KITTY;
  } else if (!strcmp(p,"sixel")) {
    *mode = BITMAP_SIXEL;
  } else if (!strcmp(p,"fb")) {
    *mode = BITMAP_FB;
  } else if (!strcmp(p,"octant")) {
    *mode = BITMAP_OCTANT;
  } else if (!strcmp(p,"sextant")) {
    *mode = BITMAP_SEXTANT;
  } else if (!strcmp(p,"braille")) {
    *mode = BITMAP_BRAILLE;
  } else {
    *mode = 100;
  }
  if (*mode == 100) {
    p = getenv("TERM");
    if (p==NULL) {
      *mode = BITMAP_SEXTANT;
    } else if (!strcmp(p,"linux")) {
      *mode = BITMAP_FB;
    } else if (!strcmp(p,"xterm-kitty")) {
      *mode = BITMAP_KITTY;
    } else {
      *mode = BITMAP_SEXTANT;
    }
  }

  switch(*mode) {
  case BITMAP_SIXEL:
    *width = 640;
    *height = 384;
    *ncolors = 16;
    break; 
  case BITMAP_KITTY:
    *width = 640;
    *height = 384;
    *ncolors = 256;
    break;
  case BITMAP_FB:
    *width = 640;
    *height = 480;
    *ncolors = 256;
    break;
  case BITMAP_OCTANT:
    *width = 160;
    *height = 96;
    *ncolors = 2;
    break;
  case BITMAP_SEXTANT:
    *width = 160;
    *height = 72;
    *ncolors = 2;
    break;
  case BITMAP_BRAILLE:
    *width = 160;
    *height = 96;
    *ncolors = 2;
    break;
  } 
}
