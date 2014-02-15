/*  STETJE.C  */
/*  Podprogrami za stetje objektov na sliki  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <forms.h>

#include "../functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
POINT stack[STACKMAX];
POINT *pixellist, *pixellist2;
int star_size, star_size2;
int maxsize2;
float **avg, **sgm;

int
background(FITS_IMAGE *sl, int x, int y)

{
  float v1;
  int xp, yp, xc, yc;
  int k;
  double x0, y0, z0, x1, y1, z1, x2, y2, z2;
  double e, ca, cb, cc;

  xp = (x - sl->maxsize / 2) / sl->maxsize;
  yp = (y - sl->maxsize / 2) / sl->maxsize;
  if (x < sl->maxsize / 2) xp = -1;
  if (y < sl->maxsize / 2) yp = -1;
  xc = sl->maxsize / 2 + xp * sl->maxsize;
  yc = sl->maxsize / 2 + yp * sl->maxsize;
  if (xp < 0) xp = 0;
  if (yp < 0) yp = 0;
  x0 = xc;
  y0 = yc;
  z0 = avg[xp][yp];
  x1 = xc + sl->maxsize;
  y1 = yc + sl->maxsize;
  z1 = avg[xp + 1][yp + 1];
  if ((y1 - y) * (x1 - x0) - (x1 - x) * (y1 - y0) < 0) {
    x2 = x0;
    y2 = y1;
    z2 = avg[xp][yp + 1];
    k = 1;
  }
  else {
    x2 = x1;
    y2 = y0;
    z2 = avg[xp + 1][yp];
    k = 0;
  }
  if (z0 == 0 && z1 == 0 && z2 == 0) {
    v1 = 0;
  }
  else {
    float g1, g2, e1, e2, f1, f2;

    g1 = - z0 * x1 + z1 * x0;
    g2 = - z1 * x2 + z2 * x1;
    f1 = x1 - x0;
    f2 = x2 - x1;
    e1 = y0 * x1 - y1 * x0;
    e2 = y1 * x2 - x1 * y2;
    e = f1 * e2 - f2 * e1;
    cc = (g1 * e2 - g2 * e1) / e;
    cb = (g2 * f1 - g1 * f2) / e;
    if (fabs(x0) > fabs(x1)) {
      ca = (- z0 - cb * y0 - cc) / x0;
    }
    else if (x1 != 0) {
      ca = (- z1 - cb * y1 - cc) / x1;
    }
    else {
      ca = (- z2 - cb * y2 - cc) / x2;
    }
    v1 = -(ca * x + cb * y + cc);
  }
  return v1 ;
}

int
above_threshold(FITS_IMAGE *sl, int x, int y, float *sg1)
{
  int v;
  float v1, sg;
  int xp, yp, xc, yc;
  int k;
  double x0, y0, z0, x1, y1, z1, x2, y2, z2;
  double e, ca, cb, cc;

  v = video_point(sl, x, y);
  xp = (x - sl->maxsize / 2) / sl->maxsize;
  yp = (y - sl->maxsize / 2) / sl->maxsize;
  if (x < sl->maxsize / 2) xp = -1;
  if (y < sl->maxsize / 2) yp = -1;
  xc = sl->maxsize / 2 + xp * sl->maxsize;
  yc = sl->maxsize / 2 + yp * sl->maxsize;
  if (xp < 0) xp = 0;
  if (yp < 0) yp = 0;
  x0 = xc;
  y0 = yc;
  z0 = avg[xp][yp];
  x1 = xc + sl->maxsize;
  y1 = yc + sl->maxsize;
  z1 = avg[xp + 1][yp + 1];
  if ((y1 - y) * (x1 - x0) - (x1 - x) * (y1 - y0) < 0) {
    x2 = x0;
    y2 = y1;
    z2 = avg[xp][yp + 1];
    k = 1;
  }
  else {
    x2 = x1;
    y2 = y0;
    z2 = avg[xp + 1][yp];
    k = 0;
  }
  if (z0 == 0 && z1 == 0 && z2 == 0) {
    v1 = 0;
  }
  else {
    float g1, g2, e1, e2, f1, f2;

    g1 = - z0 * x1 + z1 * x0;
    g2 = - z1 * x2 + z2 * x1;
    f1 = x1 - x0;
    f2 = x2 - x1;
    e1 = y0 * x1 - y1 * x0;
    e2 = y1 * x2 - x1 * y2;
    e = f1 * e2 - f2 * e1;
    cc = (g1 * e2 - g2 * e1) / e;
    cb = (g2 * f1 - g1 * f2) / e;
    if (fabs(x0) > fabs(x1)) {
      ca = (- z0 - cb * y0 - cc) / x0;
    }
    else if (x1 != 0) {
      ca = (- z1 - cb * y1 - cc) / x1;
    }
    else {
      ca = (- z2 - cb * y2 - cc) / x2;
    }
    v1 = -(ca * x + cb * y + cc);
  }
  if (k) {
    z0 = sgm[xp][yp];
    z1 = sgm[xp + 1][yp + 1];
    z2 = sgm[xp][yp + 1];
  }
  else {
    z0 = sgm[xp][yp];
    z1 = sgm[xp + 1][yp + 1];
    z2 = sgm[xp + 1][yp];
  }
  if (z0 == 0 && z1 == 0 && z2 == 0) {
    sg = 0;
  }
  else {
    float g1, g2, e1, e2, f1, f2;

    g1 = - z0 * x1 + z1 * x0;
    g2 = - z1 * x2 + z2 * x1;
    f1 = x1 - x0;
    f2 = x2 - x1;
    e1 = y0 * x1 - y1 * x0;
    e2 = y1 * x2 - x1 * y2;
    e = f1 * e2 - f2 * e1;
    cc = (g1 * e2 - g2 * e1) / e;
    cb = (g2 * f1 - g1 * f2) / e;
    if (fabs(x0) > fabs(x1)) {
      ca = (- z0 - cb * y0 - cc) / x0;
    }
    else if (x1 != 0) {
      ca = (- z1 - cb * y1 - cc) / x1;
    }
    else {
      ca = (- z2 - cb * y2 - cc) / x2;
    }
    sg = -(ca * x + cb * y + cc);
  }
  *sg1 = sg;
  return v > v1 + sl->sg_num * sg;
}  


int
vrh1(FITS_IMAGE *sl, int x, int y) 

{
  float sg;

  return above_threshold(sl, x, y, &sg);
}

int
vrh2(FITS_IMAGE *sl, int x, int y, int v) 

{
  int v1;
  float sg;
  int t;

  v1 = (long) video_point(sl, x, y) - background(sl, x, y);
  t = above_threshold(sl, x, y, &sg);

  return t && v + 0.2 * v1 / sg > v1;
}


/*  Podprogram za obhod objekta   */
/*  Uporabljam algoritem s skladom  */
int
obhod(FITS_IMAGE *slika, SHADOW *senca, POINT *robne, int x, int y, int n, OBJEKT *seznam)
{
  int sp = 0;
  OBJEKT obj;
  int v, bg;
  int i;

  /*  Postavim zacetne podatke o objektu   */
  obj.x1 = obj.x2 = obj.xt = obj.xf = obj.xu = x;
  obj.y1 = obj.y2 = obj.yt = obj.yf = y;
  obj.area = 0;
  obj.grey = 0;
  obj.bg = 0;
  obj.inner = pixellist2;
  stack[sp].x = x;
  stack[sp++].y = y;
  do {
    sp--;
    x = stack[sp].x;
    y = stack[sp].y;
    /*  Here we visit only points which were already visited before  */
    if (test(senca, x, y)) {
      /* Check if this point is in the list  */
      for (i = 0; i < seznam[n].area; i++) {
	if (x == seznam[n].inner[i].x && y == seznam[n].inner[i].y) break;
      }
      if (i == seznam[n].area) continue;
      /*  Oznacim, da je tocka obiskana  */
      clear(senca, x, y);
      v = video_point(slika, x, y) - (bg = background(slika, x, y));
      obj.inner[obj.area].x = x;
      obj.inner[obj.area].y = y;
      obj.inner[obj.area].v = v;
      obj.area++;
      if (obj.area == star_size2) {
	star_size2 += DEFAULT_STAR_AREA;
	obj.inner = pixellist2 = 
	  (POINT *) realloc(obj.inner, sizeof(POINT) * star_size2);
	if (obj.inner == NULL) {
	  fprintf(stderr, "Too big a star!");
	  exit(1);
	}
      }
      obj.x1 = min(x, obj.x1);
      if (obj.y1 > y) {
	obj.y1 = y;
	obj.xu = x;
      }
      obj.x2 = max(x, obj.x2);
      obj.y2 = max(y, obj.y2);
      obj.grey += v;
      obj.bg += bg;
      /*  Pogledam, ali lahko stopim na okoliske tocke */
      /*  Pogoja:    */
      /*           tocka je nad pragom  */
      /*           tocka ni vecja od trenutne tocke za vec kot sg_num */
      if (y > slika->y1 && vrh2(slika, x, y - 1, (int) v)) {
	stack[sp].x = x;
	stack[sp++].y = y - 1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (y < slika->y2 && vrh2(slika, x, y + 1, (int) v)) {
	stack[sp].x = x;
	stack[sp++].y = y + 1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (x > slika->x1 && vrh2(slika, x - 1, y, (int) v)) {
	stack[sp].x = x - 1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (x < slika->x2 && vrh2(slika, x + 1, y, (int) v)) {
	stack[sp].x = x + 1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
    }
  } while(sp > 0);
  /*  Now reverse the state of all the shadow pixels  */
  for (i = 0; i < seznam[n].area; i++) {
    reverse(senca, seznam[n].inner[i].x, seznam[n].inner[i].y);
  }
  obj.perim = obj.area;
  obj.bg /= obj.area;
  seznam[n] = obj;
  return(0);
}


int
find_top(FITS_IMAGE *slika, SHADOW *senca, POINT *robne, int x, int y, int n, OBJEKT *seznam)
{
  int sp = 0;
  OBJEKT obj;
  int v, bg, vmax, xmax = 0, ymax = 0;

  /*  Postavim zacetne podatke o objektu   */
  obj.x1 = obj.x2 = obj.xt = obj.xf = obj.xu = x;
  obj.y1 = obj.y2 = obj.yt = obj.yf = y;
  obj.area = 0;
  obj.grey = 0;
  obj.bg = 0;
  obj.inner = pixellist;
  stack[sp].x = x;
  stack[sp++].y = y;
  vmax = -100000;
  do {
    sp--;
    x = stack[sp].x;
    y = stack[sp].y;
    /*  Check if the object is touching the edge of image  */
    if (x == slika->x1 || x == slika->x2
	|| y == slika->y1 || y == slika->y2) {
      /*  return if the object is on the edge  */
      return 1;
    }
    if (!test(senca, x, y)) {
      /*  Oznacim, da je tocka obiskana  */
      set(senca, x, y);
      v = video_point(slika, x, y) - (bg = background(slika, x, y));
      obj.inner[obj.area].x = x;
      obj.inner[obj.area].y = y;
      obj.inner[obj.area].v = v;
      /*  Store coordinates of the brightest pixel  */
      if (v > vmax) {
	vmax = v;
	xmax = x;
	ymax = y;
      }
      obj.area++;
      if (obj.area == star_size) {
	star_size += DEFAULT_STAR_AREA;
	obj.inner = pixellist = 
	  (POINT *) realloc(obj.inner, sizeof(POINT) * star_size);
	if (obj.inner == NULL) {
	  fprintf(stderr, "Too big a star!");
	  exit(1);
	}
      }
      obj.x1 = min(x, obj.x1);
      if (obj.y1 > y) {
	obj.y1 = y;
	obj.xu = x;
      }
      obj.x2 = max(x, obj.x2);
      obj.y2 = max(y, obj.y2);
      obj.grey += v;
      obj.bg += bg;
      /*  Pogledam, ali lahko stopim na okoliske tocke */
      /*  Pogoja:    */
      /*           tocka je nad pragom  */
      /*           tocka ni vecja od trenutne tocke za vec kot sg_num */
      if (y > slika->y1 && vrh1(slika, x, y - 1)) {
	stack[sp].x = x;
	stack[sp++].y = y - 1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (y < slika->y2 && vrh1(slika, x, y + 1)) {
	stack[sp].x = x;
	stack[sp++].y = y + 1;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (x > slika->x1 && vrh1(slika, x - 1, y)) {
	stack[sp].x = x - 1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
      if (x < slika->x2 && vrh1(slika, x + 1, y)) {
	stack[sp].x = x + 1;
	stack[sp++].y = y;
	if (sp == STACKMAX) {
	  fprintf(stderr, "Stack overflow in line %d of file %s\n", 
		  __LINE__, __FILE__);
	  sp--;
	}
      }
    }
  } while(sp > 0);
  obj.perim = obj.area;
  obj.bg /= obj.area;
  obj.xf = xmax; 
  obj.yf = ymax;
  seznam[n] = obj;
  return 0;
}



/*  Podprogram, ki presteje objekte na sliki */
/*  slika --  struktura s podatki o sliki
    senca --  kazalec na strukturo, ki kaze na pomozno sliko
    sezpoint -- kazalec na seznam podatkov o posameznih sledeh
    rw -- zastavica, ki pove, ali prikazujem delo programa
    step -- korak pri skeniranju slike
    */
int
stetje(FITS_IMAGE slika, int *nstar, STAR **star1)
     
{
  int x, y, i;
  int err;
  OBJEKT obj;
  int xmax, ymax;
  float *xp1, *xp2, *yval;
  float avgall, sgmall;
  int np, npmax;
  FL_OBJECT *o;
  XEvent event;
  float kx, ky, w0, h0;
  int x0, y0, w, h;
  float bgthresh;
  SHADOW senca;
  STAR *star = *star1;
  char a[80];
  time_t t;
  int starcount = 0;
  int xborder = slika.width - slika.border;
  int yborder = slika.height - slika.border;


  reserve_shadow(&senca, slika.width, slika.height, 0, 0);
  /*  Sfiltriram sliko  */
  /* blur(&slika);  */
  t = time(NULL);
  fprintf(state.logfp, "==================================================\n");
  fprintf(state.logfp, "Fitsblink %s starcount %s\n", VERSION, asctime(gmtime(&t)));
  fprintf(state.logfp, "    Image file name:%s\n", slika.name);
  fprintf(state.logfp, "    Image analysis parameters:\n");
  fprintf(state.logfp, "      subimage size: %d pixels\n", slika.maxsize);
  fprintf(state.logfp, "      sensitivity threshold: %.2f sigma\n", slika.sg_num);
  fprintf(state.logfp, "      minimal brightness: %.2f\n", slika.minbright);
  fprintf(state.logfp, "      insensitive edge: %d\n", slika.border);
  if (state.interactive) {
    w = state.blinker->blinkW->w;
    h = state.blinker->blinkW->h;
    kx = (float) (slika.width - 1) / (w - 1);
    ky = (float) (slika.height - 1) / (h - 1);
    if (kx < ky) kx = ky;
    w0 = (slika.width - 1) / kx + 1.5;
    h0 = (slika.height - 1) / kx + 1.5;
    x0 = state.blinker->blinkW->x + (w - w0) / 2 + 0.5;
    y0 = state.blinker->blinkW->y + (h - h0) / 2 + 0.5;
    open_message("Star counting");
  }

  npmax = slika.maxsize * slika.maxsize / 4 * PI;
  xp1 = (float *) myalloc(sizeof(float) * (npmax + 1), "stetje", "xp1");
  xp2 = (float *) myalloc(sizeof(float) * (npmax + 1), "stetje", "xp2");
  yval = (float *) myalloc(sizeof(float) * (npmax + 1), "stetje", "yval");
  maxsize2 = 2 * slika.maxsize;
  xmax = slika.width / slika.maxsize + 2;
  ymax = slika.height / slika.maxsize + 2;
  avg = float_array(xmax, ymax);
  sgm = float_array(xmax, ymax);
  /*  space for pixel lists  */
  star_size2 = star_size = DEFAULT_STAR_AREA;
  pixellist = (POINT *) myalloc(sizeof(POINT) * star_size, "stetje", "pixellist");
  pixellist2 = (POINT *) myalloc(sizeof(POINT) * star_size2, "stetje", "pixellist2");
  /*  Calculate average background levels in different parts of the image  */
  if (povprecja(&slika, avg, sgm, &avgall, &sgmall)) goto end_stetje;
  fprintf(state.logfp, "    Average background=%.2f\n", avgall);
  fprintf(state.logfp, "    Average deviation=%.2f\n", sgmall);
  bgthresh = avgall + 5.0 * slika.sg_num * sgmall;
  /*	Obrobim sliko  */
  rob(&slika, (int) avgall);

  if (state.verbose) {
    for (x = 0; x < xmax; x++) {
      for (y = 0; y < ymax; y++) {
	fprintf(state.logfp, "%d  ", (int) avg[x][y]);
      }
      fprintf(state.logfp, "\n");
    }
    fprintf(state.logfp, "\n");
    for (x = 0; x < xmax; x++) {
      for (y = 0; y < ymax; y++) {
	fprintf(state.logfp, "%d  ", (int) sgm[x][y]);
      }
    fprintf(state.logfp, "\n");
    }
  }

  clear_shadow(&senca);
  for (y = slika.y1; y <= slika.y2; y++) {
    if (state.interactive) {
      sprintf(a, "Line %d of %d", y, slika.y2);
      write_message(a);
    }
    for (x = slika.x1; x <= slika.x2; x++) {
      /*  Ce pika se ni obiskana, jo obiscem in oznacim  */
      if (!test(&senca, x, y) && vrh1(&slika, x, y)) {
	/*  Obhodim objekt  */
	if (find_top(&slika, &senca, NULL, x, y, 0, &obj) == 1) continue;
	if (obj.x2 - obj.x1 < slika.minstar || 
	    obj.y2 - obj.y1 < slika.minstar) continue;  /*  Ignore very small objects  */
	err = obhod(&slika, &senca, NULL, obj.xf, obj.yf, 0, &obj);
	if (slika.aperture) {
	  obj.grey = (long) aperture(&slika, (double) obj.xf, (double) obj.yf, 
				     frame[state.control[state.control_num]].fits.inner, 
				     frame[state.control[state.control_num]].fits.outer,
				     &obj.xt, &obj.yt);
	}
	else {
	  np = 0;
	  obj.xt = 0;
	  obj.yt = 0;
	  obj.grey = 0;
	  for (i = 0; i < obj.area; i++) {
	    np++;
	    if (np >= npmax) break;
	    yval[np] = obj.inner[i].v;
	    if (yval[np] > 0) {   /*  Take only values that make sense  */
	      xp1[np] = obj.inner[i].x;
	      xp2[np] = obj.inner[i].y;
	      obj.xt += xp1[np] * yval[np];
	      obj.yt += xp2[np] * yval[np];
	      obj.grey += obj.inner[i].v;
	    }
	    else {
	      np--;
	    }
	  }
	  obj.xt /= obj.grey;
	  obj.yt /= obj.grey;
	}
	if (obj.xt > slika.border && obj.xt < xborder &&
	    obj.yt > slika.border && obj.yt < yborder /* &&  */
#if 0
	    np > 3) {
#endif
	  ) {
	  int fail = 0;
	  /* float koef[7], hi2;
	     fit(xp1, xp2, yval, np, koef, &hi2, avgall, &fail); */
	  star[starcount].n = starcount;
	  star[starcount].x = obj.xt;
	  star[starcount].y = obj.yt;
	  star[starcount].mag = obj.grey;
	  star[starcount].m = -1;
	  star[starcount].q = 0;
	  if (!fail) {
	    /* star[starcount].fitmag =  koef[2] / (koef[3] * koef[5]) * PI; */
	    if (star[starcount].mag > slika.minbright && obj.bg < bgthresh
		/* || (star[starcount].fitmag / star[starcount].mag < slika.fitmeasure &&
		   star[starcount].mag / star[starcount].fitmag < slika.fitmeasure && */
		) {
	      /*		fprintf(fp, "%d %.2f %.2f %.2f %d\n", starcount, star[starcount].x,
				star[starcount].y, star[starcount].mag, obj.area); */
	      starcount++;
	      if (starcount == *nstar) {
		*nstar += 250;
		star = (STAR *) realloc(star, sizeof(STAR) * *nstar);
		if (star == NULL) {
		  fprintf(stderr, "Too many stars!");
		  exit(1);
		}
	      }
	    }
	  }
	  else {
	    /*	      printf("Fit failed\n"); */
	    star[starcount].fitmag =  -1;
	  }
	}
      }
    }
    if (!test(&senca, x, y)) {
      x--;
    }
    if (state.interactive && y % 50 == 0) {
      o = fl_check_forms();
      if (o == FL_EVENT) {
	fl_XNextEvent(&event);
      }
      else if (o == state.message->messagecancelW) {
	break;
      }
    }
  }
 end_stetje:
  t = time(NULL);
  fprintf(state.logfp, "    Number of stars counted: %d\n", starcount);
  fprintf(state.logfp, "Fitsblink %s starcount - END %s\n", VERSION, asctime(gmtime(&t)));
  fprintf(state.logfp, "++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  free(pixellist2);
  free(pixellist);
  free_float_array(sgm, xmax); 
  free_float_array(avg, xmax); 
  free((void *) yval);
  free((void *) xp2);
  free((void *) xp1);
  free_shadow(&senca);
  *star1 = star;
  if (state.interactive) {
    close_message();
  }
  return starcount;
}


void
rob(FITS_IMAGE *slika, int v)
     
{
  int x, t1, t2;
  
  t1 = slika->x1 - 1;
  t2 = slika->x2 + 1;
  for (x = slika->y1; x <= slika->y2; x++) {
    set_point(slika, t1, x, v);
    set_point(slika, t2, x, v);
  }
  t1 = slika->y1 - 1;
  t2 = slika->y2 + 1;
  for (x = slika->x1 - 1; x <= slika->x2 + 1; x++) {
    set_point(slika, x, t1, v);
    set_point(slika, x, t2, v);
  }
}
