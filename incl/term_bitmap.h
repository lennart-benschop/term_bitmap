/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/


#include <stdint.h>
#include <stdbool.h>

// The supported bitmap drawing modes.

/* Use 2x3 'sextant' characters 2x3 pixels per character.
   These are in the Unicode 'symbols for legacy computing' range.
   Many terminals have dedicated code to render them, so they look right
   even when the used font does not have them. */
#define BITMAP_SEXTANT 0 
/* Use 2x4 'octant' characters 2x4 pixels per character.
   These are a more recent addition to Unicode. Some terminals have dedicated
   support for rendering these. */
#define BITMAP_OCTANT  1 
/* Use unicode 'braille' characters, all 2x4 dot patterns are supported.
   It depends on the font how they look. Many Unicode fonts include these
   characters. */
#define BITMAP_BRAILLE 2
/* Use DEC vt340 'sixel' protocol to draw high resolution bitmaps.
   Supported on the original xterm when invoked with the proper terminal
   emulation option.
   -ti vt340 will work.

   Also supported on KDE Konsole and some other modern terminal emulators.
 */
#define BITMAP_SIXEL   3
/* Use kitty graphics, originally defined for the 'kitty' terminal, but
   also supported on KDE Konsole and some modern terminal emulators. */
#define BITMAP_KITTY   4
/* Use the frame buffer directly. Works on the Linux console, outside
   the X or Wayland desktop. 
 */
#define BITMAP_FB      5

/* The character based modes support only monochrome bitmaps, while
   the true bitmap modes (sixel, kitty, fb) support full colour.
*/

/* tbm_new_screen

   Create a new bitmap screen.
   width: width in pixels.
   height: height in pixels.
   posx: character X position (starts at 0) of the top left corner.
   posy: character Y position (starts at 0) of the top left corner.
   ncolors: number of colours on the screen (values 2, 4, 8 and 16 supported)
            support for 256 colours will likely be added in the future.
   output_method: drawing mode, one of the BITMAP_* values.

   Return an opaque pointer to the newly created object, NULL if failure.
   Pss this pointer as 'bm' parameter to other API functions.
 */

void *
tbm_new_screen(unsigned int width,
	       unsigned int height,
	       unsigned int posx,
	       unsigned int posy,
	       unsigned int ncolors,
	       unsigned int output_method);

/* tbm_delete

   Delete the bitmap screen created by tbm_new_screen.
 */
void
tbm_delete(void *bm);

/* tbm_redraw

   Redraw the bitmap represented by bm
*/
void
tbm_redraw(void *bm);


/*
  Pixel coordinates (x,y) start at (0,0) at the top left position of the bitmap.
  The bootm right position is (width-1,height-1).
  x runs from left to right.
  y runs from top to bottom.

  x and y increment by 1 for each visible pixel.
  
  x and y values can be outside of the range (0..width-1, 0..heigth-1),
  including negative values. Lines and other objects can extend outside
  the bitmap, but no actual pixels will be drawn for those values.

  Each bitmap has a single graphics cursor position associated with it.
*/


/* Drawing modes:
   DRAW_MODE_INVIS  Invisible, do not change any pixels.
   DRAW_MODE_FG     draw in foreground colour.
   DRAW_MODE_INVERT invert the pixel that is on the screen.
   DRAW_MODE_BG     draw in background colour.
*/
#define DRAW_MODE_INVIS 0
#define DRAW_MODE_FG 1
#define DRAW_MODE_INVERT 2
#define DRAW_MODE_BG 3

/* tbm_setpen

   Set colours and drawing mode.
   bm: selected bitmap
   fg: foreground colour, in range 0..ncolors-1.
   bg: background colour, in range 0..ncolors-1.
   fg_mode: drawing mode for foreground pixels. These are all drwan pixels
            in tbm_plotdot, tbm_lineto etc. Except for the background pixels
	    of drawn text.
   bg_mode: drawing mode for background pixels. These are the background
            pixels of drawn text. DRAW_MODE_BG plots them in background colour
	    DRAW_MODE_INVIS does not change these pixels.

	    May in the future also apply to dotted/dashed lines.
 */
void
tbm_setpen(void *bm,
	   unsigned int fg,
	   unsigned int bg,
	   unsigned int fg_mode,
	   unsigned int bg_mode);

/* tbm_setfg

   Set only foregrond colour.
 */
void
tbm_setfg(void * bm,
	  unsigned int fg);

/* tbm_clear

   Fill the entire bitmap with background colour.
 */
void
tbm_clear(void *bm);

/* tbm_moveto

   Set the graphics cursor position in the given bitmap.
 */
void
tbm_moveto(void *bm, int x, int y);

/* tbm_moveto

   Set the graphics cursor position in the given bitmap
   and plot a single pixel there.
 */
void
tbm_plotdot(void *bm, int x, int y);

/* tbm_moveto

   Return the pixel value at the given x,y coordinate.
 */
uint8_t
tbm_getdot(void *bm, int x, int y);

/* tbm_moveto

   Draw a straight line from the current graphics cursor to the given
   (x,y) coordinate. Update the graphics cursor to that (x,y) coordinate.
 */
void
tbm_lineto(void *bm, int x, int y);

/* tbm_triangle

   Draw a solid triangle between the current position, x2,y2 and x3,y3
   The new current position will now be x3,y3
*/
void
tbm_triangle(void *bm, int x2, int y2, int x3, int y3);

/* tbm_circle

   Draw a circle with x,y as centre and radius r.
   Draw the outline (fill=false) or a solid circle (fill=true).
 */
void
tbm_circle(void *bm, int x, int y, unsigned int r, bool fill);

/* tbm_getpos
   Draw a null-terminated text string in the given bitmap.
   - Text is in an 8x8 monospaced font.
   - Only ASCII characters will be drawn.
   - The graphics cursor will be moved to the right by 8 times the number of
     drawn characters. No wrap-around to the next line and no scrolling.
 */
void
tbm_plottext(void *bm, char *s);

/* tbm_getpos
   Return the current x,y graphics cursor position in the given bitmap
*/
void
tbm_getpos(void *bm, int *x, int *y);

/* tbm_getparams

   Return the widht, height and ncolors parameters of a given bitmap.
   bm: bitmap object to use.
   w: width
   h: height
   c: number of colours
 */
void
tbm_getparams(void *bm, unsigned int *w, unsigned int *h, unsigned int *c);

/* tbm_get_recommended

   Determine the recommended graphics mode and some reasonable
   width, height and ncolors parameters for that mode.

   The recommended mode will be derived from the following:
   - If envirnoment variable TBM_MODE has one of the values 'kitty', 'sixel',
     'fb', 'braille', 'octant' or 'sextant', the corresponding mode will be
      selected.
   - Otherwise, if environment variable TERM has the value 'linux', select
     BITMAP_FB, if it has the value 'xterm-kitty', select BITMAP_KITTY.
   - Otherwise fall back to BITMAP_SEXTANT.

   The actual algorithm will be refined in the future, possible including
   some probes to check whether 'kitty' or 'sixel' mode is supported by
   the terminal and to derive sensible values for width and height within the
   given terminal.
 */
void
tbm_get_recommended(unsigned int *width,
		    unsigned int *height,
		    unsigned int *ncolors,
		    unsigned int *mode);
