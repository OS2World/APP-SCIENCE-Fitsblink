/*  FIELD_COMPARE  */
/*  Poiscem ujemajoce zvezde iz dveh seznamov */
/*  (c) Jure Skvarc, oktober 1996  */
#define _POSIX_C_SOURCE  2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "../functs.h"

extern STATE state;
TRANSROT trglob;

int
field_compare(STARS *list1, STARS *list2, MATCH_OPTIONS match_options, 
	      BLINK_FRAME *frame, int n)

{
  int i, j;
  time_t t;
  int s1sizetemp, s2sizetemp;
  int temp;
  TRANSROT tr, tr1;
  STAR_PAIR *gs;
  int nmatched;
  double xx, yy, x, y;
  double det;
  float kf;
  double xinc, yinc, xc, yc;
  int mm;
  double x1, y1;
  int err;
  int refit, frefit;
  float res;


  list1->sp = 0;
  for (i = 0; i < list1->n; i++) {
    list1->sp += log(list1->s[i].mag);
  }
  list1->sp = 0.9 * exp(list1->sp / i); 
  /*  Constants for fits_pixel_to_world routine  */
  xinc = frame[n].map.width / frame[n].map.w;
  yinc = frame[n].map.height / frame[n].map.h;
  xc = 0.5 * frame[n].map.w;
  yc = 0.5 * frame[n].map.h;

  t = time(NULL);
  fprintf(state.logfp, "==================================================\n");
  fprintf(state.logfp, "Fitsblink %s field_compare %s", VERSION, asctime(gmtime(&t)));
  fprintf(state.logfp, "  - START\n");
  fprintf(state.logfp, "    minimal value to accept constellation: %f\n", match_options.min);
  fprintf(state.logfp, "    number of neighbouring stars in a constellation: %d\n", match_options.nstar);
  fprintf(state.logfp, "    number of stars used for initial matching: %d\n", match_options.firstbright);
  fprintf(state.logfp, "    maximum allowed positional error for a star in constellation: %.1f\n", match_options.poserrmax);
  fprintf(state.logfp, "    maximum allowed magnitude error for a star in constellation: %.1f\n", match_options.magerrmax);
  fprintf(state.logfp, "    Number of stars in the first list:%d\n", list1->n);
  fprintf(state.logfp, "    Number of stars in the second list:%d\n", list2->n);
  
  /*  Sort first list on magnitudes  */
  qsort((void *) list1->s, (size_t) list1->n, sizeof(STAR), compare_starmag);
  /*  Now sort first match_options.firstbright stars on y coordinates  */
  s1sizetemp  =  min(match_options.firstbright, list1->n);
  qsort((void *) list1->s, (size_t) s1sizetemp, sizeof(STAR), compare_star);

  frame[n].objnum += s1sizetemp;
  frame[n].objmax = frame[n].objnum;
  if ((frame[n].obj = realloc(frame[n].obj, sizeof(OBJECT) * frame[n].objmax)) == NULL) {
    fprintf(stderr, "Out of memory, function field_compare, variable obj\n");
    exit(1);
  }
  kf = 3.0 / log(list1->s[0].mag);
  for (j = 0, i = frame[n].objnum - s1sizetemp; j < s1sizetemp; j++, i++) {
    frame[n].obj[i].x = list1->s[j].x;
    frame[n].obj[i].y = list1->s[j].y;
    frame[n].obj[i].r = 1.5 * kf * log(list1->s[j].mag);
    frame[n].obj[i].color = FL_BLUE;
    frame[n].obj[i].shape = OBJ_BOX;
    frame[n].obj[i].s = "";
  }
  temp = list1->n;
  list1->n = s1sizetemp;
  s1sizetemp = temp;

  /*  Sort second list on magnitudes */
  qsort((void *) list2->s, (size_t) list2->n, sizeof(STAR), compare_starmag);
  /*  Now sort first match_options.firstbright stars on y coordinates  */
  s2sizetemp  =  min(2 * match_options.firstbright, list2->n);
  qsort((void *) list2->s, (size_t) s2sizetemp, sizeof(STAR), compare_star);

  temp = list2->n;
  list2->n = s2sizetemp;
  s2sizetemp = temp;
  /*  Do constellation matching on bright stars only  */
  if (list1->n < list2->n) {
    constellations(list1, list2, 0, &tr, match_options);
  }
  else {
    constellations(list2, list1, 0, &tr1, match_options);
    det = tr1.a * tr1.e - tr1.b * tr1.d;
    tr.a = tr1.e / det;
    tr.b = -tr1.b / det;
    tr.c = (tr1.f * tr1.b - tr1.e * tr1.c) / det;
    tr.d = -tr1.d / det;
    tr.e = tr1.a / det;
    tr.f = (tr1.c * tr1.d - tr1.a * tr1.f) / det;
    tr.hi = tr1.hi;
    tr.sma = 0;
    tr.smb = 0;
  }
  list1->n = s1sizetemp;
  list2->n = s2sizetemp;
  if (tr.hi < 0) {
    return -1;
  }
  /*  Reset matched objects  */
  for (i = 0; i < list1->n; i++) list1->s[i].m = -1;
  for (i = 0; i < list2->n; i++) list2->s[i].m = -1;
  fprintf(state.logfp, "    Coordinate system transformation:\n");
  fprintf(state.logfp, "       x2 = %f x1 %+f y1 %+f\n", tr.a, tr.b, tr.c);
  fprintf(state.logfp, "       y2 = %f x1 %+f y1 %+f\n", tr.d, tr.e, tr.f);
  fprintf(state.logfp, "       Chi squared / point=%.2f\n", tr.hi);

  fprintf(state.logfp, "    Magnitude transformation:\n");
  fprintf(state.logfp, "       mag2 = %f %+f mag1\n", tr.ma, tr.mb);
  fprintf(state.logfp, "       sigma_a = %f, sigma_b = %f\n", tr.sma, tr.smb);
  /*  Now sort both arrays entirely on y coordinate  */
  qsort((void *) list1->s, (size_t) list1->n, sizeof(STAR), compare_star);
  qsort((void *) list2->s, (size_t) list2->n, sizeof(STAR), compare_star);
  /*  Do constellation matching on all stars */
  constellations(list1, list2, 1, &tr, match_options);
  /*  Check how many stars are successfully matched  */
  nmatched = 0;
  for (i = 0; i < list1->n; i++) {
    if (list1->s[i].m != -1) nmatched++;
  }
  fprintf(state.logfp, "    Number of matched stars:%d\n", nmatched);
  /*  Reserve space for pairs of stars  */
  gs = (STAR_PAIR *) myalloc(sizeof(STAR_PAIR) * nmatched, "field_compare", "gs");
  frefit = 0;
  /*  Sort first list on magnitudes  */
  qsort((void *) list1->s, (size_t) list1->n, sizeof(STAR), compare_starmag);
  /*  Correct pointers from the second list  */
  for (i = 0; i < list1->n; i++) {
    int mm = list1->s[i].m;
    if (mm != -1) {
      list2->s[mm].m = i;
    }
  }
  do {
    refit = 0;
    /*  Fill the gs list with matched pairs of stars  */
    j = 0;
    for (i = 0; i < list1->n && j < match_options.firstbright; i++) {
      int mm = list1->s[i].m;
      if (mm != -1) {
	gs[j].n1 = i;
	gs[j].x1 = list1->s[i].x;
	gs[j].y1 = list1->s[i].y;
	gs[j].mag1 = list1->s[i].mag;
	gs[j].n2 = mm;
	gs[j].x2 = list2->s[mm].x;
	gs[j].y2 = list2->s[mm].y;
	gs[j].mag2 = list2->s[mm].mag;
	j++;
      }
    }
    nmatched = j;
    /*  Find coordinate transformation  */
    tr.hi = transrot(gs, &nmatched, &tr, &match_options);
    if (tr.hi < 0 || tr.hi > match_options.poserrmax) {
      return -1;
    }
    /*  Mark stars which were used for coordinate fit */
    for (i = 0; i < nmatched; i++) {
      list1->s[gs[i].n1].q = 1;
    }
    /*  Now check if errors are small enough  */
    for (i = 0; i < list1->n; i++) {
      mm = list1->s[i].m;
      if (mm != -1) {
	/*  Transform from image pixel coordinates to catalog pixel coordinates */
	x1 = tr.a * list1->s[i].x + tr.b * list1->s[i].y + tr.c;
	y1 = tr.d * list1->s[i].x + tr.e * list1->s[i].y + tr.f;
	/*  Transform from pixel coordinates to RA-DEC  */
	err = 0;
	fits_pix_to_world(x1 + 1, y1 + 1, frame[n].map.ra, frame[n].map.dec, 
			  xc, yc, xinc, yinc, 
			  frame[n].map.fi, "-TAN",
			  &list1->s[i].ra, &list1->s[i].dec, &err);
	/*  Calculate distance between matched stars */
	res = sqrt(fsqr(cos(list1->s[i].dec) * (list1->s[i].ra - list2->s[mm].ra)) +
		   fsqr(list1->s[i].dec - list2->s[mm].dec)) * 3600.0;
	/*  Check if this is too big  */
	if (res > match_options.maxres) {
	  list1->s[i].m = -1;  /*  remove matched pair  */
	  refit = 1;
	}
      }
    }
    if (frefit == 0 && refit == 0) {
      frefit = 1;
      refit = 1;
    }
  } while (refit);

  fprintf(state.logfp, "    Second coordinate system transformation:\n");
  fprintf(state.logfp, "       Number of points:%d\n", nmatched);
  fprintf(state.logfp, "       x2 = %f x1 %+f y1 %+f\n", tr.a, tr.b, tr.c);
  fprintf(state.logfp, "       y2 = %f x1 %+f y1 %+f\n", tr.d, tr.e, tr.f);
  fprintf(state.logfp, "       Chi squared / point=%.2f\n", tr.hi);

  fprintf(state.logfp, "    Magnitude transformation:\n");
  fprintf(state.logfp, "       mag2 = %f %+f mag1\n", tr.ma, tr.mb);
  fprintf(state.logfp, "       sigma_a = %f, sigma_b = %f\n", tr.sma, tr.smb);
  free(gs);
  /*  Calculate inverse coordinate transformation  */
  det = tr.a * tr.e - tr.b * tr.d;
  tr1.a = tr.e / det;
  tr1.b = -tr.b / det;
  tr1.c = (tr.f * tr.b - tr.e * tr.c) / det;
  tr1.d = -tr.d / det;
  tr1.e = tr.a / det;
  tr1.f = (tr.c * tr.d - tr.a * tr.f) / det;
  nmatched = 0;

  for (i = 0; i < list1->n; i++) {
    mm = list1->s[i].m;
    /*  Transform from image pixel coordinates to catalog pixel coordinates */
    x1 = tr.a * list1->s[i].x + tr.b * list1->s[i].y + tr.c;
    y1 = tr.d * list1->s[i].x + tr.e * list1->s[i].y + tr.f;
    if (mm != -1) {
      nmatched++;
      x = tr1.a * list2->s[mm].x + tr1.b * list2->s[mm].y + tr1.c;
      y = tr1.d * list2->s[mm].x + tr1.e * list2->s[mm].y + tr1.f;
      xx = x1 - list2->s[mm].x;
      yy = y1 - list2->s[mm].y;
      list2->s[mm].x = x;
      list2->s[mm].y = y;
      /*  Transform from pixel coordinates to RA-DEC  */
      err = 0;
      fits_pix_to_world(x1 + 1, y1 + 1, frame[n].map.ra, frame[n].map.dec, 
		      xc, yc, xinc, yinc, 
		      frame[n].map.fi, "-TAN",
		      &list1->s[i].ra, &list1->s[i].dec, &err);
      /*  Calculate distance between stars  */
      list1->s[i].err = list2->s[mm].err =
	sqrt(fsqr(cos(list1->s[i].dec) * (list1->s[i].ra - list2->s[mm].ra)) +
	     fsqr(list1->s[i].dec - list2->s[mm].dec)) * 3600.0;
      list1->s[i].catalog = list2->s[mm].catalog;
    }
    else {
      /*  Transform from pixel coordinates to RA-DEC  */
      err = 0;
      fits_pix_to_world(x1 + 1, y1 + 1, frame[n].map.ra, frame[n].map.dec, 
			xc, yc, xinc, yinc, 
			frame[n].map.fi, "-TAN",
			&list1->s[i].ra, &list1->s[i].dec, &err);
      list1->s[i].catalog = '0';
    }
  }
  fprintf(state.logfp, "    RESULTS:\n");
  fprintf(state.logfp, "      Number of successfully matched stars:%d\n", nmatched);

  t = time(NULL);
  fprintf(state.logfp, "Fitsblink %s field_compare %s - END\n", VERSION, asctime(gmtime(&t)));
  fprintf(state.logfp, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  if (state.interactive) {
    newwcs(frame, n, tr);
  }
  trglob = tr;
  frame[n].objnum += nmatched;
  frame[n].objmax = frame[n].objnum;
  if ((frame[n].obj = realloc(frame[n].obj, sizeof(OBJECT) * frame[n].objmax)) == NULL) {
    fprintf(stderr, "Out of memory, function field_compare, variable obj\n");
    exit(1);
  }
  return nmatched;
}


