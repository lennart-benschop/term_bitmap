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

void *
tbm_new_screen(unsigned int width,
	       unsigned int height,
	       unsigned int posx,
	       unsigned int posy,
	       unsigned int ncolors,
	       unsigned int output_method)
{
  unsigned int w = width;
  unsigned int h = height;
  unsigned int c = ncolors;
  tbm_bitmap_t * bms;
  uint8_t *bmd;
  
  if (w > 1920 || h > 1200 || c > 256 || c < 2) {
    return NULL;
  }
    
  switch(output_method) {
    case BITMAP_SIXEL:
      update_whc_sixel(&w,&h,&c);
      break;
    case BITMAP_KITTY:
      update_whc_kitty(&w,&h,&c);
      break;
    case BITMAP_FB:
      update_whc_fb(&w,&h,&c);
      break;
    case BITMAP_OCTANT:
      update_whc_octant(&w,&h,&c);
      break;
    case BITMAP_SEXTANT:
      update_whc_sextant(&w,&h,&c);
      break;
    case BITMAP_BRAILLE:
      update_whc_braille(&w,&h,&c);
      break;
    default:
      return NULL;
  }

  bmd = malloc(w*h);
  if (!bmd)
    return NULL;
  bms = malloc(sizeof(tbm_bitmap_t));
  if (!bms) {
    free(bmd);
    return NULL;
  }
  memset(bmd, 0, w*h);
  bms->width = w;
  bms->height = h;
  bms->posx = posx;
  bms->posy = posy;
  bms->ncolors = c;
  bms->output_method = output_method;
  bms->bitmap = bmd;
  bms->cur_x = 0;
  bms->cur_y = 0;
  bms->bgcolor = 0;
  bms->fgcolor = c - 1;
  bms->fgmode = DRAW_MODE_FG;
  bms->bgmode = DRAW_MODE_INVIS;
  return (void*)bms;
}

void
tbm_delete(void *bm)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  free(bms->bitmap);
  free(bms);
}

void
tbm_redraw(void *bm)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  switch(bms->output_method) {
    case BITMAP_SIXEL:
      redraw_sixel(bms);
      break;
    case BITMAP_KITTY:
      redraw_kitty(bms);
      break;
    case BITMAP_FB:
      redraw_fb(bms);
      break;
    case BITMAP_OCTANT:
      redraw_octant(bms);
      break;
    case BITMAP_SEXTANT:
      redraw_sextant(bms);
      break;
    case BITMAP_BRAILLE:
      redraw_braille(bms);
      break;
  }
}

void
tbm_setpen(void *bm,
	   unsigned int fg,
	   unsigned int bg,
	   unsigned int fg_mode,
	   unsigned int bg_mode)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  bms->fgcolor = (fg < bms->ncolors) ? fg: bms->ncolors-1;
  bms->bgcolor = (bg < bms->ncolors) ? bg: bms->ncolors-1;
  bms->fgmode = fg_mode;
  bms->bgmode = bg_mode;
}

void
tbm_getparams(void *bm, unsigned int *w, unsigned int *h, unsigned int *c)
{
  tbm_bitmap_t * bms = (tbm_bitmap_t*) bm;
  *w = bms->width;
  *h = bms->height;
  *c = bms->ncolors;
}

