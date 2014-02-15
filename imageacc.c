/*  imageacc.c  for Fitsblink 1.2  */
/*  Functions for access and modification of image elements  */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <forms.h>
#include "formblink.h"
#include "functs.h"

extern FITS_IMAGE count_options;

/*   Set basic image parameters */
void
set_image_parameters(FITS_IMAGE *slika, int xmax, int ymax, PIXEL *image)
     
{
  int i;

  slika->width = xmax;
  slika->height = ymax;
  slika->x1 = 1;
  slika->y1 = 1;
  slika->x2 = slika->width - 2;
  slika->y2 = slika->height - 2;
  slika->image = image;
  slika->pi = (PIXEL **) myalloc(ymax * sizeof(PIXEL *), 
				 "set_image_parameters", "pi");
  /*  Set array of pointers to rows */
  for (i = 0; i < ymax; i++) {
    slika->pi[i] = image + i * xmax;
  }    
  slika->maxsize = DEFAULT_MAXSIZE;
  slika->sg_num = DEFAULT_SGNUM;
  slika->minbright = DEFAULT_MINBRIGHT;
  slika->minstar = DEFAULT_MINSTAR;
  slika->border = DEFAULT_BORDER;
  if (count_options.maxsize != -999) {
    slika->maxsize = count_options.maxsize;
  }
  if (count_options.sg_num != -999) {
    slika->sg_num = count_options.sg_num;
  }
  if (count_options.minstar != -999) {
    slika->minstar = count_options.minstar;
  }
  if (count_options.minbright != -999) {
    slika->minbright = count_options.minbright;
  }
  if (count_options.fitmeasure != -999) {
    slika->fitmeasure = count_options.fitmeasure;
  }
  if (count_options.border != -999) {
    slika->border = count_options.border;
  }
  if (count_options.inner != -999) {
    slika->inner = count_options.inner;
  }
  if (count_options.outer != -999) {
    slika->outer = count_options.outer;
  }
}


/*   Return value of pixel at coordinates (x, y)   */
PIXEL
  video_point(FITS_IMAGE *slika, int x, int y)
    
{
  if (x >= 0 && x < slika->width &&
      y >= 0 && y < slika->height)
    return *(slika->pi[y] + x);
  else return 0;
}


/*   Set pixel with coordinates x, y to value  v  */
void
  set_point(FITS_IMAGE *slika, int x, int y, PIXEL v)
{
  PIXEL *p;
  p = slika->pi[y] + x;
  *p = v;
}

void
  xor_point(FITS_IMAGE *slika, int x, int y)
{
  PIXEL *p;
  p = slika->pi[y] + x;
  *p ^= 65535;
}




/*  Make a convolution in point (x, y) with a function, defined in structure g  */
float
convolve(GAUSSF *g, FITS_IMAGE *sl, int x, int y)

{
  int i, j, i1, j1;
  int f2n = g->n / 2;
  float s = 0;
  int v, vall = 0;

  for (i1 = 0, i = x - f2n; i1 < g->n; i1++, i++) {
    if (i > sl->x1 && i < sl->x2) {
      for (j1 = 0, j = y - f2n; j1 < g->n; j1++, j++) {
	if (j > sl->y1 && j < sl->y2) {
	  v = video_point(sl, i, j);
	  s += g->g[i1][j1] * v;
	  vall += v;
	}
      }
    }
  }
  return s / vall;
}

/*  Make a simple filtering of the image  */
void
blur(FITS_IMAGE *sl) 

{
  int x, y;
  PIXEL *row, *row1;

  row = myalloc(sizeof(PIXEL) * sl->width, "blur", "row");
  row1 = myalloc(sizeof(PIXEL) * sl->width, "blur", "row1");
  /*  Copy the first row */
  for (x = 0; x < sl->width; x++) row[x] = video_point(sl, x, 0);
  for (y = 1; y < sl->height - 1; y++) {
    row1[0] = video_point(sl, 0, y);
    for (x = 1; x < sl->width - 1; x++) {
      row1[x] = video_point(sl, x, y);
      set_point(sl, x, y, (video_point(sl, x - 1, y) + video_point(sl, x + 1, y) +
			   video_point(sl, x, y - 1) + video_point(sl, x - 1, y + 1)) / 4);
    }
    row1[0] = video_point(sl, sl->width - 1, y);
    for (x = 0; x < sl->width; x++) row[x] = row1[x];
  }
  free(row1);
  free(row);
}





/*  Calculate the coefficients of the Gaussian filter  */
void
set_gaussian_filter(GAUSSF *f, float sigma, float r)

{
  float x, y, x2, y2, r2, s2;
  int i, j;
  int fn2;

  f->n = 2 * (r + 1) + 1;  /*  Array size  */
  f->r = r;              /*  Radius inside which the coefficients are valid */
  f->sigma = sigma;      /*  Half-width of the Gaussian  */
  /*  Reserve space for the coefficient array  */
  f->g = myalloc(sizeof(float *) * f->n, "set_gaussian_filter", "f->g");
  for (i = 0; i < f->n; i++) {
    f->g[i] = myalloc(sizeof(float) * f->n, "set_gaussian_filter", "f->g[i]");
  }
  fn2 = f->n / 2;
  r2 = r * r;
  s2 = sigma * sigma;
  for (i = 0; i < f->n; i++) {
    x = (i - fn2);
    x2 = x * x;
    for (j = 0; j < f->n; j++) {
      y = (j - fn2);
      y2 = y * y;
      if (x2 + y2 <= r2) {
	/*  Laplace of the gaussian  */
	f->g[i][j] = (x2 + y2 - 2 * s2) / (s2 * s2) * exp(-(x2 + y2) / (2 * s2));
      }
      else {
	f->g[i][j] = 0;
      }
    }
  }
}


