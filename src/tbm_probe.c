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
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

static struct termios oldterm;
static struct termios newterm;
static int tflags;

static void tbm_term_setraw(void)
{
  tcsetattr(0,TCSAFLUSH,&newterm);
}

static void tbm_term_restore(void)
{
  tcsetattr(0,TCSAFLUSH,&oldterm);
  fcntl(0,F_SETFL,tflags);
}

static void tbm_term_init(void)
{
    tcgetattr(0,&oldterm);
    newterm=oldterm;
    newterm.c_iflag = newterm.c_iflag & ~INLCR & ~ICRNL;
    newterm.c_lflag = newterm.c_lflag & ~ECHO & ~ICANON;
    newterm.c_cc[VMIN]=1;  
    newterm.c_cc[VTIME]=0;
    tflags=fcntl(0,F_GETFL,0);
}


void tbm_term_setxy(unsigned int x, unsigned int y)
{
  char tmpbuf[30];
  sprintf(tmpbuf,"\033[%d;%dH",y,x);
  write(1,tmpbuf,strlen(tmpbuf));
}



void tbm_term_getxy(unsigned int *x, unsigned int *y)
{
  char numbuf[14];
  char c;
  int i;
  *x=-1;
  *y=-1;
  tbm_term_setraw();
  write(1,"\033[6n",4); // Escape sequence to read cursor position.
  while (getchar() != 0x1b)
    ;
  if (getchar() != '[') 
    goto fun_exit;
  i=0;
  while (isdigit(c=getchar())) {
    if (i<13) {
      numbuf[i++]=c;
    }
  }
  numbuf[i]=0;
  *y=atoi(numbuf);
  if (c != ';') goto fun_exit;
  i=0;
  while (isdigit(c=getchar())) {
    if (i<13) {
      numbuf[i++]=c;
    }
  }
  numbuf[i]=0;
  *x=atoi(numbuf);
  if (c != 'R') goto fun_exit;
 fun_exit:
  tbm_term_restore();
}


static void tbm_get_screen_params(unsigned int *pwidth,
				  unsigned int *pheight,
				  unsigned int *cwidth,
				  unsigned int *cheight)
{
  unsigned int oldx, oldy;
  char c;
  int i=0;
  char numbuf[14];
  *pwidth=0;
  *pheight=0;
  tbm_term_setraw();
  // Send a command to obtain width/height in pixels, not all terms support this
  write(1,"\033[14t",5);
  // Wait for escape, which may not come.
  fcntl(0,F_SETFL,tflags|O_NONBLOCK);
  while ((c=getchar()) != 0x1b && i<10) {
    usleep(20000);
    i++;
  }
  if (c!=0x1b) goto fun_exit;
  // Read and parse the entire response.
  fcntl(0,F_SETFL,tflags);
  if (getchar() != '[') 
    goto fun_exit;
  if (getchar() != '4') 
    goto fun_exit;
  if (getchar() != ';') 
    goto fun_exit;
  i=0;
  while (isdigit(c=getchar())) {
    if (i<13) {
      numbuf[i++]=c;
    }
  }
  numbuf[i]=0;
  *pheight=atoi(numbuf);
  if (c != ';') goto fun_exit;
  i=0;
  while (isdigit(c=getchar())) {
    if (i<13) {
      numbuf[i++]=c;
    }
  }
  numbuf[i]=0;
  *pwidth=atoi(numbuf);
  if (c != 't') goto fun_exit;  
 fun_exit:
  tbm_term_restore();
  // Use a portable way to probe the screen size in characters
  // Put the cursor at the extreme lower right position and read it.
  tbm_term_getxy(&oldx,&oldy);
  tbm_term_setxy(999,999);
  tbm_term_getxy(cwidth,cheight);
  tbm_term_setxy(oldx,oldy);
}

/* Number of text lines that should not be used by the bitmap */
unsigned int unused_rows = 2;

void tbm_set_unused(unsigned int r)
{
  unused_rows = r;
}

void
tbm_get_recommended(unsigned int *width,
		    unsigned int *height,
		    unsigned int *ncolors,
		    unsigned int *mode)
{
  char *p = getenv("TBM_MODE");
  unsigned int ccellheight,pwidth,pheight,cwidth,cheight;
  tbm_term_init();
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
      *mode = 100;
#ifdef USE_LINUX_FB      
    } else if (!strcmp(p,"linux")) {
      *mode = BITMAP_FB;
#endif      
    } else if (!strcmp(p,"xterm-kitty")) {
      *mode = BITMAP_KITTY;
    } else {
      *mode = 100;
    }
  }
  tbm_get_screen_params(&pwidth,&pheight,&cwidth,&cheight);
  if (unused_rows >= cheight)
    unused_rows = cheight - 1;
  //printf("Screen params pw=%d ph=%d cw=%d ch=%d ur=%d\n",pwidth,pheight,cwidth,cheight,unused_rows);
  //exit(0);
  
  if (*mode == 100) {
    unsigned int val;
    probe_sixel(&val);
    if (val>0) {
      *mode = BITMAP_SIXEL;      
    }  else if (pwidth!=0) {
      *mode = BITMAP_SEXTANT;
    } else {
      *mode = BITMAP_BRAILLE;
    }
  }
  
  switch(*mode) {
  case BITMAP_SIXEL:
    if (pheight == 0) {
      probe_sixel(&ccellheight);
      //printf("CCELLHEIGTH=%d\n",ccellheight);
      pheight=cheight*ccellheight;
      pwidth=cwidth*(ccellheight/2); 
    }
    ccellheight=pheight/cheight;
    *width = pwidth;
    *height = ((cheight-unused_rows)*ccellheight/6)*6;
    *ncolors = 256;
    break; 
  case BITMAP_KITTY:
    ccellheight=pheight/cheight;
    *width = pwidth;
    *height = (cheight-unused_rows)*ccellheight;
    *ncolors = 256;
    break;
#ifdef USE_LINUX_FB
  case BITMAP_FB:
    *width = cwidth*8;
    *height = (cheight-unused_rows)*16;
    *ncolors = 256;
    break;
#endif    
  case BITMAP_OCTANT:
    *width = cwidth*2;
    *height = (cheight-unused_rows)*4;
    *ncolors = 2;
    break;
  case BITMAP_SEXTANT:
    *width = cwidth*2;
    *height = (cheight-unused_rows)*3;
    *ncolors = 2;
    break;
  case BITMAP_BRAILLE:
    *width = cwidth*2;
    *height = (cheight-unused_rows)*4;
    *ncolors = 2;
    break;
  }
  //printf("width=%d height=%d nc=%d\n",*width,*height,*ncolors);
  //exit(0);
}
