/* Simple terminal bitmap output library.
   Copyright 2026 L.C. Benschop, Vught, The Netherlands.
   The program is released under the MIT license.
   There is NO WARRANTY.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "term_bitmap.h"

static int randint(int max)
{
  return (int)(((float)rand() / RAND_MAX)*max);
}

int main(void)
{
  unsigned int width;
  unsigned int height;
  unsigned int ncolors;
  unsigned int mode;
  void *bm;
  tbm_get_recommended(&width, &height, &ncolors, &mode);
  bm = tbm_new_screen(width,height,0,0,ncolors,mode);
  if (!bm) return -1;

  tbm_moveto(bm,0,0);
  tbm_lineto(bm,width-1,height-1);

  tbm_moveto(bm,width-1,0);
  tbm_lineto(bm,0,height-1);

  tbm_redraw(bm);

  tbm_moveto(bm,width-1,0);
  tbm_lineto(bm,width-20,height-1);
  tbm_moveto(bm,0,height/2);
  // Line in green horizontal
  tbm_setpen(bm,10,0,DRAW_MODE_FG,DRAW_MODE_INVIS);
  tbm_lineto(bm,width-1,height/2);

  // Text in yellow.
  tbm_moveto(bm,0,height/4);
  tbm_setpen(bm,11,0,DRAW_MODE_FG,DRAW_MODE_BG);
  tbm_plottext(bm,"Hello, world!");
  
  tbm_redraw(bm);

  printf("Press ENTER to continue\n");
  while(getchar()!='\n');

  tbm_clear(bm);
  // Text in various colours
  {
    int i;
    char strbuf[80];
    for (i=0; i<ncolors; i++) {      
      tbm_setpen(bm, i, 0, DRAW_MODE_FG, DRAW_MODE_BG);
      if (ncolors==2)
	tbm_moveto(bm, 0, i*32);
      else
	tbm_moveto(bm, 0, i*16);
      sprintf(strbuf,"Foreground colour %d",i);
      tbm_plottext(bm,strbuf);
      tbm_setpen(bm, i, ncolors-1, DRAW_MODE_FG, DRAW_MODE_BG);
      if (ncolors==2)
	tbm_moveto(bm, 0, i*32+8);
      else
	tbm_moveto(bm, 160, i*16);
      tbm_plottext(bm,strbuf);
      tbm_setpen(bm,0, i, DRAW_MODE_FG, DRAW_MODE_BG);
      if (ncolors==2)
	tbm_moveto(bm, 0, i*32+16);
      else
	tbm_moveto(bm, 0, i*16+8);
      sprintf(strbuf,"Background colour %d",i);
      tbm_plottext(bm,strbuf);
      tbm_setpen(bm,ncolors-1, i, DRAW_MODE_FG, DRAW_MODE_BG);
      if (ncolors==2)
	tbm_moveto(bm, 0, i*32+24);
      else
	tbm_moveto(bm, 160, i*16+8);
      tbm_plottext(bm,strbuf);
    }
    tbm_redraw(bm);
  }

  printf("Press ENTER to continue\n");
  while(getchar()!='\n');

  // Random lines
  tbm_clear(bm);
  {
    int x1,x2,y1,y2,i,c;
    for (i=0;i<100;i++) {
      c=randint(ncolors-1)+1;
      tbm_setpen(bm,c,0,DRAW_MODE_FG,DRAW_MODE_BG);
      x1 = randint(width);
      x2 = randint(width);
      y1 = randint(height);
      y2 = randint(height);
      tbm_moveto(bm,x1,y1);
      tbm_lineto(bm,x2,y2);
    }
    tbm_redraw(bm);
  }
  
  printf("Press ENTER to continue\n");
  while(getchar()!='\n');

  tbm_clear(bm);
  // Lissajous figure 
  {
    unsigned int i,x,y; 
    tbm_setpen(bm,10,0,DRAW_MODE_FG,DRAW_MODE_INVIS);
    for (i=0;i<=1000; i++) {
      x = width/2 + (width*0.35)*cos((double)i * M_PI * 6 / 1000);
      y = height/2 + (height*0.49)* sin((double)i * M_PI * 2 / 1000);
      if (i==0)
	tbm_moveto(bm,x,y);
      else
	tbm_lineto(bm,x,y);
    }      
    tbm_redraw(bm);
  }
  
  printf("Press ENTER to continue\n");
  while(getchar()!='\n');

  tbm_clear(bm);
  // My favourite xor-lines pattern.
  tbm_setpen(bm,15,0,DRAW_MODE_INVERT,DRAW_MODE_INVIS);
  {
    unsigned int i,j;
    for (i=0;i<width;i++) {
      for (j=0; j<width; j++) {
	tbm_moveto(bm, i, 0);
	tbm_lineto(bm, j, height-1);
      }
      tbm_redraw(bm);
    }
  }
  
  printf("Press ENTER to continue\n");
  while(getchar()!='\n');

  tbm_delete(bm);
  return 0;
}
