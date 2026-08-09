/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} palette_t;

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int posx;
  unsigned int posy;
  unsigned int ncolors;
  unsigned int output_method;
  uint8_t *bitmap; /* Bitmap width * height bytes, one byte per pixel */
  int cur_x, cur_y;
  unsigned int fgcolor, bgcolor;
  unsigned int fgmode, bgmode;
  const palette_t *palette;
} tbm_bitmap_t;

void set_palette(tbm_bitmap_t* bm, unsigned int ncolors);

void redraw_sixel(tbm_bitmap_t* bm);

void update_whc_sixel(unsigned int *w, unsigned int *h, unsigned int *c);

void redraw_kitty(tbm_bitmap_t* bm);

void update_whc_kitty(unsigned int *w, unsigned int *h, unsigned int *c);

void redraw_octant(tbm_bitmap_t* bm);

void update_whc_octant(unsigned int *w, unsigned int *h, unsigned int *c);

void redraw_sextant(tbm_bitmap_t* bm);

void update_whc_sextant(unsigned int *w, unsigned int *h, unsigned int *c);

void redraw_braille(tbm_bitmap_t* bm);

void update_whc_braille(unsigned int *w, unsigned int *h, unsigned int *c);

void redraw_fb(tbm_bitmap_t* bm);

void update_whc_fb(unsigned int *w, unsigned int *h, unsigned int *c);
