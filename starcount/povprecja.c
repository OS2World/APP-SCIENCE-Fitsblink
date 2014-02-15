/*  POVPRECJA.C  */
/*  part of the fitsblink program  */
/*  routine which calculates background  */
/*  Jure Skvarc, May 1998                   */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <forms.h>

#include "../functs.h"
extern STATE state;

int
povprecja(FITS_IMAGE *slika, float **avg, float **sgm, float *avgall, float *sgmall)

{
  int i, j, kk;
  int x, y, xp, yp, xc, yc;
  int xmax, ymax;
  double s, s2, sall = 0, s2all, sp;
  int w2, w2all;
  PIXEL *dat;
  int all, count = 0;
  FL_OBJECT *o;
  XEvent event;

  dat = myalloc(4 * slika->maxsize * slika->maxsize * sizeof(PIXEL), "povprecja", "dat");
  *avgall = 0.0;
  *sgmall = 0.0;
  kk = 0;
  /*  Size of the area, where the average is calculated  */
  xmax = slika->width / slika->maxsize + 1;
  ymax = slika->height / slika->maxsize + 1;
  all = (xmax + 1) * (ymax + 1);
  for (xp = 0; xp <= xmax; xp++) {
    xc = slika->maxsize / 2 + xp * slika->maxsize;
    for (yp = 0; yp <= ymax; yp++) {
      if (state.interactive) {
	char a[30];
	count++;
	sprintf(a, "Background %d of %d", count, all);
	write_message(a);
      }
      yc = slika->maxsize / 2 + yp * slika->maxsize;
      /*  First we find a median value in given rectangle  */
      sall = 0;
      s2all = 0;
      w2all = 0;
      for (i = -slika->maxsize; i < slika->maxsize; i++) {	
	x = xc + i;
	if (x >= slika->x1 && x <= slika->x2) {
	  for (j = -slika->maxsize; j < slika->maxsize; j++) {
	    y = yc + j;
	    if (y >= slika->y1 && y <= slika->y2) {
		dat[w2all] = video_point(slika, x, y);
		w2all++;
	    }
	  }
	}
      }
      /*  Sort the values in the rectangle  */
      median(dat, w2all, sizeof(PIXEL), cmppixel);
      sall = dat[w2all / 2];
      median(dat, w2all / 2, sizeof(PIXEL), cmppixel);
      median(dat, w2all / 4, sizeof(PIXEL), cmppixel);
      median(dat, w2all / 8, sizeof(PIXEL), cmppixel);
      /*  Find sigma of pixels excluding */
      /* the brightest and the darkest 1 / (2 * slika->maxsize + 1) */
      sp = 0;
      s2 = 0;
      w2 = 0;
      for (i = 0.125 * w2all; i < w2all / 2; i++) {
	s = sall - dat[i];
	sp += s;
	s2 += s * s;
	w2++;
      }
      if (w2 > 0) {
	sp /= w2;
	*avgall += sall;
	kk++;
	avg[xp][yp] = sall;
	*sgmall += (sgm[xp][yp] = sqrt(s2 / w2 - sp * sp));
      }
      else {
	avg[xp][yp] = 0;
	sgm[xp][yp] = 0;
      }
    }
    if (state.interactive) {
      o = fl_check_forms();
      if (o == FL_EVENT) {
	fl_XNextEvent(&event);
      }
      else if (o == state.message->messagecancelW) {
	free(dat);
	return 1;
      }
    }
  }
  if (kk > 0)  {
    *avgall /= kk;
    *sgmall /= kk;
  }
  else {
    *avgall = sall;
    *sgmall = 0;
  }
  free(dat);
  return 0;
}
      
