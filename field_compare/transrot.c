/*  TRANSROT.C  */
/*  Calculate linear transformation between two sets of (x, y) points    */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include "../functs.h"

float
transrot(STAR_PAIR *gs, int *n1, TRANSROT *t, MATCH_OPTIONS *match_options)

{
  int i, j;
  double xx, yy;
  double xt1, yt1, xt2, yt2;
  double x2, y2, xy, lxx, lyx, lxy, lyy, dx, dy;
  double a, b, c, d, e, f, det;
  double sxx, sxy;
  float hi;
  float *th;
  int nold, n;

  n = *n1;
  th = (float *) myalloc(n * sizeof(float), "transrot", "th");
  do {
    nold = n;
    /*  Calculate centers of masses of both sets of points  */
    xt1 = yt1 = xt2 = yt2 = 0.0;
    for (i = 0; i < n; i++) {
      xt1 += gs[i].x1;
      yt1 += gs[i].y1;
      xt2 += gs[i].x2;
      yt2 += gs[i].y2;
    }
    xt1 /= n;
    yt1 /= n;
    xt2 /= n;
    yt2 /= n;
    x2 = 0.0;
    y2 = 0.0;
    xy = 0.0;
    lxx = lyx = lxy = lyy = 0.0;
    /*  Calculate the coefficients of the system of equations  */
    for (i = 0; i < n ; i++) {
      dx = gs[i].x1 - xt1;
      dy = gs[i].y1 - yt1;
      x2 += dx * dx;
      y2 += dy * dy;
      xy += dx * dy;
      lxx += dx * (gs[i].x2 - xt2);
      lyx += dy * (gs[i].x2 - xt2);
      lxy += dx * (gs[i].y2 - yt2);
      lyy += dy * (gs[i].y2 - yt2);
    }
    det = y2 * x2 - xy * xy;
    if (fabs(det) < 1e-30) {
      return -1;
    }
    a = (lxx * y2 - lyx * xy) / det;
    b = (lyx * x2 - lxx * xy) / det;
    c = xt2 - (a * xt1 + b * yt1);
    d = (lxy * y2 - lyy * xy) / det;
    e = (lyy * x2 - lxy * xy) / det;
    f = yt2 - (d * xt1 + e * yt1);
    hi = 0;
    for (i = 0; i < n; i++) {
      xx = a * gs[i].x1 + b * gs[i].y1 + c - gs[i].x2;
      yy = d * gs[i].x1 + e * gs[i].y1 + f - gs[i].y2;
      th[i] = xx * xx + yy * yy;
#ifdef DEBUG1
      printf("xx=%f, yy=%f, hi[%d]=%f\n", xx, yy, i, th[i]);
#endif
      hi += th[i];
    }
    hi = 5 * hi / n;
#ifdef DEBUG1
    printf("hi=%f\n", hi);
#endif
    /*  Preverim, ce je kateri od hi-jev nenavadno velik in ga izlocim  */
    j = 0;
    for (i = 0; i < n; i++) {
      if (j != i) {
	gs[j] = gs[i];
      }
      if (!(sqrt(th[i]) > match_options->poserrmax || th[i] > hi)) {
	j++;
      }
    }
    n = j;
  } while (nold != n && n > 2);
  t->a = a;
  t->b = b;
  t->c = c;
  t->d = d;
  t->e = e;
  t->f = f;
  /*  Find magnitude transformation  */
  t->ma = 0.0;
  t->mb = 0.0;
  sxx = sxy = 0;
  for (i = 0; i < n; i++) {
    sxx += gs[i].mag1 * gs[i].mag1;
    sxy += gs[i].mag1 * gs[i].mag2;
  }
  t->ma = 0.0;
  t->mb = sxy / sxx;
  t->sma = 0;
  t->smb = 0;
  free(th);
  *n1 = n;
  return 0.2 * hi;
}

