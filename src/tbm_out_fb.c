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
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


void redraw_fb(tbm_bitmap_t* bms)
{
  // Keep the frame buffer mapped after the first successful call.
  // the OS should take care of cleaing it up when the program exits.
  static uint32_t *mapped_fb;
  static unsigned int line_length,fb_len;
  static struct fb_var_screeninfo fbi; 
  const palette_t *pl = bms->palette;
  unsigned int x,y;
  unsigned int startx=bms->posx*8;
  unsigned int starty=bms->posy*16;
  if (mapped_fb == NULL) {
    int fd = open("/dev/fb0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
      perror("Cannot open frame buffer device\n");
      return;
    }
    if(ioctl(fd, FBIOGET_VSCREENINFO, &fbi)) {
      perror("Cannot get frame buffer device info\n");
      close(fd);
      return;
    }
    if (fbi.bits_per_pixel != 16 && fbi.bits_per_pixel != 32) {
      perror("Only support 32 or 16 bits per pixel\n");
      close(fd);
      return;
    }
    fb_len = fbi.xres*fbi.yres*fbi.bits_per_pixel/8;
    line_length = fbi.xres;
    mapped_fb = (uint32_t*)mmap(NULL, fb_len, PROT_READ|PROT_WRITE,
								MAP_SHARED, fd, 0);
    if (mapped_fb == NULL) {
      perror("Unable to map frame buffer device\n");
      close(fd);
      return;
    }
  }
  if (startx + bms->width > fbi.xres || starty + bms->height > fbi.yres) {
    perror("Bitmap does not fit on screen\n");
    return;
  }
  printf("\033[%d;%dHa\n",bms->posy+1,bms->posx+1); // Do some fb output
  printf("\033[%d;%dH",bms->posy+1+(bms->height+15)/16,1); // Position on line just after graphics bitmap.
  for (y=0;y<bms->height;y++) {
    for (x=0;x<bms->width;x++) {
      uint8_t c = bms->bitmap[y*bms->width+x];
      if (fbi.bits_per_pixel == 32) {
	uint32_t pixcol = (pl[c].r << 16) | (pl[c].g << 8)| (pl[c].b);
	mapped_fb[(starty+y)*line_length + startx + x] = pixcol;
      } else {
	uint16_t * mapped_fb16 = (uint16_t*)mapped_fb;
	uint16_t pixcol = ((pl[c].r>>3) << 11) | ((pl[c].g >> 2) << 5)| (pl[c].b>> 3);
	mapped_fb16[(starty+y)*line_length + startx + x] = pixcol;	
      }
    }
  }
  //munmap(mapped_fb, fb_len);
  //close(fd);
}

void update_whc_fb(unsigned int *w, unsigned int *h, unsigned int *c)
{
  if (*c >=16)
    *c = 16;
  else if (*c >= 8)
    *c = 8;
  else if (*c >= 4)
    *c = 4;
  else 
    *c = 2;
}
