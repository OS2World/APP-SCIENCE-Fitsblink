/*  aperture.c   */
/*  Part of the fitsblink program */
/*  Jure Skvarc, November 1998  */
/*  Given coordinates of the center point and two radii, it calculates */
/*  star or comet intensity inside the inner circle and subtracts the  */
/*  median intensity between the circles  */
/*  It assumes that the inner radius is smaller than the outer radius  */

#include <stdio.h>
#define __USE_BSD 
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <forms.h>
#include <forms.h>
#include "../functs.h"
extern POINT stack[];
extern STATE state;
extern GAUSSF gaussf;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/*  Find out how many vertices of the pixel are inside the given circle */
/*  and return flags, which tell which points are inside the circle     */
/*  index       coordinates    */
/*  0           xt - 0.5, yt - 0.5  */         
/*  1           xt + 0.5, yt - 0.5  */         
/*  2           xt + 0.5, yt + 0.5  */         
/*  3           xt - 0.5, yt + 0.5  */
int
inside(double x, double y, double r, double xt, double yt, double *inside)

{
  double xm, xv, ym, yv;
  int i, k = 0;

  r *= r;
  xt -= x;
  yt -= y;
  xm = (xt - 0.5);
  xm *= xm;
  xv = (xt + 0.5);
  xv *= xv;
  ym = (yt - 0.5);
  ym *= ym;
  yv = (yt + 0.5);
  yv *= yv;
  inside[0] = r - xm - ym;
  inside[1] = r - xv - ym;
  inside[2] = r - xv - yv;
  inside[3] = r - xm - yv;
  for (i = 0; i <= 3; i++) 
    if (inside[i] > 0) k++;
  return k;
}

/*  Find the pixel with the highest value inside the circle with */
/*  the radius r and center coordinates x and y.  Return the  */
/*  pixel coordinates in xf and yf    */
void
find_peak(FITS_IMAGE *slika, double x, double y, double r, int *xf, int *yf)

{
  int i, imin, imax, i2;
  int j, jmin, jmax;
  int vmax = -10000000, v;
  double dist[4];

  r += 0.5;
  imin = x - r;
  imax = x + r;
  *xf = x;
  *yf = y;
  for (i = imin; i <= imax; i++) {
    i2 = i - x;
    i2 *= i2;
    jmax = (int) (sqrt(r * r - i2) + 0.5);
    jmin = y - jmax;
    jmax += y;
    for (j = jmin; j <= jmax; j++) {
      if (inside(x, y, r, (double) i, (double) j, dist) > 0) {
	v = video_point(slika, i, j);
	if (v > vmax) {
	  vmax = v;
	  *xf = i;
	  *yf = j;
	}
      }
    }
  }
}


int
vrh3(FITS_IMAGE *sl, int x, int y, int v, int back, float sg) 

{
  int v1;

  v1 = (long) video_point(sl, x, y) - back;
  return v1 > sl->sg_num * sg && (v - back) + 0.2 * v1 / sg > v1;
}

/*  Subroutine which visits all points which are above the given threshold */
/*  and within a certain distance from the starting point  */
/*  The algorithm uses stack  */
int
aperture_visit(FITS_IMAGE *slika, double x0, double y0, float r, int thresh, int back, float sg, OBJEKT *obj)
{
  int sp = 0;
  int v;
  SHADOW senca;
  int side = 2 * r + 4;
  double ins[4];
  int x1, y1;
  int xmin, xmax, ymin, ymax;

  xmin = x0 - r - 1;
  xmax = xmin + side - 1;
  ymin = y0 - r - 1;
  ymax = ymin + side - 1;
  reserve_shadow(&senca, side, side, (int) (x0 - r - 1), (int) (y0 - r - 1));
  clear_shadow(&senca);
  /*  Postavim zacetne podatke o objektu   */
  obj->area = 0;
  obj->grey = 0;
  obj->x1 = obj->y1 = obj->xt = obj->yt = 0;
  stack[sp].x = x0;
  stack[sp++].y = y0;
  do {
    int x, y;
    sp--;
    x = stack[sp].x;
    y = stack[sp].y;
    if (!test(&senca, x, y)) {
      /*  Oznacim, da je tocka obiskana  */
      set(&senca, x, y);
      v = video_point(slika, x, y);
      obj->area++;
      obj->grey += v;
      obj->xt += v * x;
      obj->yt += v * y;
      obj->x1 += x;
      obj->y1 += y;
      /*  Pogledam, ali lahko stopim na okoliske tocke */
      y1 = y - 1;
      if (y > ymin && video_point(slika, x, y1) > thresh && 
	  vrh3(slika, x, y1, v, back, sg) &&
	  inside(x0, y0, r, (double) x, (double) y1, ins) > 0) {
	stack[sp].x = x;
	stack[sp++].y = y1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      y1 = y + 1;
      if (y < ymax && video_point(slika, x, y1) > thresh &&
	  vrh3(slika, x, y1, v, back, sg) &&
	  inside(x0, y0, r, (double) x, (double) y1, ins) > 0) {
	stack[sp].x = x;
	stack[sp++].y = y1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      x1 = x - 1;
      if (x > xmin && video_point(slika, x1, y) > thresh &&
	  vrh3(slika, x1, y, v, back, sg) &&
	  inside(x0, y0, r, (double) x1, (double) y, ins) > 0) {
	stack[sp].x = x1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      x1 = x + 1;
      if (x < xmax && video_point(slika, x1, y) > thresh &&
	  vrh3(slika, x1, y, v, back, sg) &&
	  inside(x0, y0, r, (double) x1, (double) y, ins) > 0) {
	stack[sp].x = x1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
    }
  } while(sp > 0);
  free_shadow(&senca);
  return 0;
}





/*  Make an aperture photometry of the object with the brightest pixel  */
/*  at coordinate x0, y0.  Add values inside a circle with a radius inner */
/*  Subtract background calculated as a median value of the pixels between */
/*  circles with radii inner and outer */
double
aperture(FITS_IMAGE *sl, double x0, double y0, double inner, double outer, 
    double *sx, double *sy)

{
  double v,  background, ball;
  double bx = 0.0, by = 0.0;
  int x, y;
  double sg;
  int inarea, outarea;
  double i, j, i2;
  int k;
  int rmax, imin, imax, jmin, jmax;
  double dist[4];
  int size;
  static int bsize = 0;
  static PIXEL *backg = NULL;
  double o2 = outer * outer;
  double sv;
  int above_thresh;
  double thresh;
  OBJEKT obj;

  /*  radius of the circle to be examined  */
  rmax = outer + 0.5;
  /*  areas of the inner circle and of the outer minus inner circle  */
  inarea = outarea = 0;
  /*  range of x coordinates to be examined  */
  imin = x0 - rmax;
  imax = x0 + rmax;
  /*  Calculate the size needed for the storage of the background pixels */
  size = M_PI * o2;
  if (size > bsize) {
    if (backg) {
      backg = (PIXEL *) realloc(backg, sizeof(PIXEL) * size);
      if (!backg) {
	fprintf(stderr, "Out of memory, function aperture, backg.\n");
	exit(1);
      }
    }
    else {
      backg = (PIXEL *) myalloc(sizeof(PIXEL) * size, "aperture", "backg");
    }
    bsize = size;
  }
  for (i = imin; i <= imax; i++) {
    i2 = i - x0;
    i2 *= i2;
    jmax = (int) (sqrt(o2 - i2) + 0.5);
    jmin = y0 - jmax;
    jmax += y0;
    for (j = jmin; j <= jmax; j++) {
      if (inside(x0, y0, inner, i, j, dist) > 0) {
	v = video_point(sl, (int) i, (int) j);
	/* this pixel is inside the inner circle with at least one vertex */
	inarea++;
      }
	/* this pixel is inside the outer circle with at least one vertex */
      else if (inside(x0, y0, outer, i, j, dist) > 0) {
	v = video_point(sl, (int) i, (int) j);
	/*  This pixel is between the inner and outer circle  */
	backg[outarea] = v;
	outarea++;
	if (outarea == bsize) {
	  bsize += 100;	
	  backg = (PIXEL *) realloc(backg, sizeof(PIXEL) * bsize);
	  if (!backg) {
	    fprintf(stderr, "Out of memory, function aperture, backg.\n");
	    exit(1);
	  }
	}
      }
    }
  }
  /*  Calculate the median of the background pixels  */
  qsort(backg, outarea, sizeof(PIXEL), cmppixel);
  /*  Calculate the sigma  */
  sg = 0;
  i2 = 0;
  background = 0;
  for (k = 0.1 * outarea; k < outarea * 0.9; k++) {
    background += backg[k];
    sg += backg[k] * backg[k];
    i2++;
  }
  background /= i2;
  sg = sqrt(sg / i2 - background * background);
  thresh = background + sl->sg_num * sg;
  /*  Calculate the intensity and centroid of the pixel larger than background  */
  /*  for sg_num * sigma  */
  find_peak(sl, (double) x0, (double) y0, inner, &x, &y);
  aperture_visit(sl, (double) x, (double) y, inner, thresh, background, sg, &obj);
  above_thresh = obj.area;
  sv = obj.grey;
  *sx = obj.xt;
  *sy = obj.yt;
  bx = obj.x1;
  by = obj.y1;
  if (above_thresh > 0) {
    ball =  background * above_thresh;
    /*  Subtract the background from the signal  */
    sv -= ball;
    *sx = (*sx - bx * background) / sv;
    *sy = (*sy - by * background) / sv;
  }
  else {
    sv = video_point(sl, (int) x0, (int) y0) - background;
    *sx = x0;
    *sy = y0;
  }
  return sv;
}
