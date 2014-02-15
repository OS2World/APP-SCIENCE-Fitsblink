/*  blink.c  for fitsblink */
#ifdef HPUX
#define _INCLUDE_HPUX_SOURCE
#define _INCLUDE_XOPEN_SOURCE
#define _INCLUDE_POSIX_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/time.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "functs.h"
#include "consts.h"

STATE state;
BLINK_FRAME *frame;
CATALOG catalog[NUMBER_OF_CATALOGS];
OBSERVATORY observatory;
MAIL mail;
FITS_IMAGE count_options;
MATCH_OPTIONS match_options;
TELESCOPE telescope;
GAUSSF gaussf;
/*****************/
/*  MYALLOC      */
/*****************/
void *
myalloc(unsigned int size, char *function, char *variable)
     
{
  void *p;
  if (size == 0) {
    fprintf(stderr, "Zero size, function %s, variable %s.\n",
           function, variable);
  }
  p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Out of memory, function %s, variable %s.\n",
           function, variable);
    exit(1);
  }
  return(p);
}


/*      DELAY                            */
/*      Wait for  t hundreths of second */
void
delay(int t)
     
{
  fd_set set;
  struct timeval timeout;
  
  FD_ZERO (&set);
  /* Initialize the timeout data structure.  */
  timeout.tv_sec = t / 100;
  timeout.tv_usec = (t % 100) * 10000;
  select(FD_SETSIZE, &set, NULL, NULL, &timeout);
}

/*  Transform pixel value according to values in arrays gamx and gamy  */
int
correct(BLINK_FRAME *frame, int n, PIXEL x)

{
  int i;
  float *gamx = frame[n].gamx;
  float *gamy = frame[n].gamy;
  for (i = 1; i < frame[n].ngam; i++) {
    if (x <= gamx[i] && x >= gamx[i - 1]) {
      return (int) (gamy[i - 1] + (x - gamx[i - 1]) * frame[n].kg[i]);
    }
  }
  return 0;
}

/*  Draw objects which belong to the image */
void
draw_objects(BLINK_FRAME *frame, int n)

{
  int j;
  Pixmap pixmap = frame[n].pixmap;
  Display *disp = state.disp;
  GC gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  OBJECT *obj = frame[n].obj;
  float dx = frame[n].dx, dy = frame[n].dy;
  int x0, y0, w0, h0;
  int xmax, ymax;
  float kx, ky;
  float ta, tb, tc;
  XPoint xpoints[10];

  if (!frame[n].objshow) return;
  xmax = frame[n].fits.width;
  ymax = frame[n].fits.height;
  kx = (float) (xmax - 1) / (state.blinker->blinkW->w - 1);
  ky = (float) (ymax - 1) / (state.blinker->blinkW->h - 1);
  if (kx < ky) kx = ky;
  w0 = (xmax - 1) / kx + 0.5;
  h0 = (ymax - 1) / kx + 0.5;
  x0 = (state.blinker->blinkW->w - w0) / 2 + 0.5;
  y0 = (state.blinker->blinkW->h - h0) / 2 + 0.5;
  /*  Draw additonal objects  */
  for (j = 0; j < frame[n].objnum; j++) {
    XSetState(disp, gc, fl_get_pixel(obj[j].color), 0, GXcopy, AllPlanes);
    switch (obj[j].shape) {
    case OBJ_CIRCLE:
      XDrawArc(disp, pixmap, gc, 
	       x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5), 
	       y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5), 
	       (int) (2 * obj[j].r), (int) (2 * obj[j].r), 0, 23040);
      XDrawArc(disp, pixmap, gc, 
	       x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 1.5), 
	       y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 1.5), 
	       (int) (2 * obj[j].r + 2), (int) (2 * obj[j].r + 2), 0, 23040);
      break;
    case OBJ_BOX:
      XDrawRectangle(disp, pixmap, gc, 
		     x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5), 
		     y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5), 
		     (int) (2 * obj[j].r), (int) (2 * obj[j].r));
      XDrawRectangle(disp, pixmap, gc, 
		     x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 1.5), 
		     y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 1.5), 
		     (int) (2 * obj[j].r + 2), (int) (2 * obj[j].r + 2));
      break;
    case OBJ_DIAMOND:
      xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
      xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5);
      xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + obj[j].r + 0.5);
      xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx + 0.5);
      xpoints[2].x = xpoints[0].x;
      xpoints[2].y = y0 + (int) ((obj[j].y - dy) / kx + obj[j].r + 0.5);
      xpoints[3].x = x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5);
      xpoints[3].y = xpoints[1].y;
      xpoints[4].x = xpoints[0].x;
      xpoints[4].y = xpoints[0].y;
      xpoints[5].x = xpoints[0].x;
      xpoints[5].y = xpoints[0].y - 1;
      xpoints[6].x = xpoints[1].x + 1;
      xpoints[6].y = xpoints[1].y;
      xpoints[7].x = xpoints[2].x;
      xpoints[7].y = xpoints[2].y + 1;
      xpoints[8].x = xpoints[3].x - 1;
      xpoints[8].y = xpoints[3].y;
      xpoints[9].x = xpoints[5].x;
      xpoints[9].y = xpoints[5].y;
      XDrawLines(disp, pixmap, gc, xpoints, 10, CoordModeOrigin);
      break;
      case OBJ_UPTRIANGLE:
	tb = 0.5773502 * obj[j].r;
	tc = 0.5 * tb;
	ta = 0.5 * obj[j].r;
	xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
	xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx - tb - 0.5);
	xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + ta + 0.5);
	xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx + tc + 0.5);
	xpoints[2].x = x0 + (int) ((obj[j].x - dx) / kx - ta - 0.5);
	xpoints[2].y = xpoints[1].y;
	xpoints[3].x = xpoints[0].x;
	xpoints[3].y = xpoints[0].y;
	xpoints[4].x = xpoints[0].x;
	xpoints[4].y = xpoints[0].y - 1;
	xpoints[5].x = xpoints[1].x + 1;
	xpoints[5].y = xpoints[1].y + 1;
	xpoints[6].x = xpoints[2].x - 1;
	xpoints[6].y = xpoints[5].y;
	xpoints[7].x = xpoints[4].x;
	xpoints[7].y = xpoints[4].y;
	XDrawLines(disp, pixmap, gc, xpoints, 8, CoordModeOrigin);
	break;
      case OBJ_DOWNTRIANGLE:
	tb = 0.5773502 * obj[j].r;
	tc = 0.5 * tb;
	ta = 0.5 * obj[j].r;
	xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
	xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx + tb - 0.5);
	xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + ta + 0.5);
	xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx - tb + 0.5);
	xpoints[2].x = x0 + (int) ((obj[j].x - dx) / kx - ta - 0.5);
	xpoints[2].y = xpoints[1].y;
	xpoints[3].x = xpoints[0].x;
	xpoints[3].y = xpoints[0].y;
	xpoints[4].x = xpoints[0].x;
	xpoints[4].y = xpoints[0].y + 1;
	xpoints[5].x = xpoints[1].x + 1;
	xpoints[5].y = xpoints[1].y + 1;
	xpoints[6].x = xpoints[2].x - 1;
	xpoints[6].y = xpoints[2].y - 1;
	xpoints[7].x = xpoints[4].x;
	xpoints[7].y = xpoints[4].y;
	XDrawLines(disp, pixmap, gc, xpoints, 8, CoordModeOrigin);
	break;
    }
  }
  XSetState(disp, gc, state.pixel[0], 0, GXcopy, AllPlanes);
}

/*  Determine the size of pixmap  */
int
pixmap_size(int width, int height, int depth)

{
  int count;
  int bits;
  XPixmapFormatValues *pfv;

  pfv = XListPixmapFormats(state.disp,  &count);
  for (count--; count > 0; count--) {
    if (pfv[count].depth != depth) continue;
    bits = width * pfv[count].bits_per_pixel;
    if (bits % pfv[count].scanline_pad) {
      bits -= bits % pfv[count].scanline_pad;
      bits += pfv[count].scanline_pad;
    }
    XFree(pfv);
    return (bits / 8) * height;
  }
  return 0;
}


/*  Draw the image with current "gamma" settings to the memory  */
void
set_image(BLINK_FRAME *frame, int n)

{
  float kx, ky;
  int w, h;
  int x, y, x0, y0, w0, h0, xp, yp;
  int ys, xi, yi;
  float dx, dy;
  GC gc;
  int xmax, ymax;

  Window win = FL_ObjWin(state.blinker->blinkW);
  XImage *xim;
  XWindowAttributes xwa;
  unsigned char *data = NULL;


  Pixmap pixmap = frame[n].pixmap;
  Display *disp = state.disp;
  PIXEL *image = frame[n].fits.image;

  fl_set_oneliner_font(0, 20); 
  fl_show_oneliner("Setting the image", 100, 100);
  XFlush(state.disp);
  xmax = frame[n].fits.width;
  ymax = frame[n].fits.height;
  if (xmax == 0 || ymax == 0) return;
  dx = frame[n].dx;
  dy = frame[n].dy;
  gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  w = state.blinker->blinkW->w;
  h = state.blinker->blinkW->h;

  kx = (float) (xmax - 1) / (w - 1);
  ky = (float) (ymax - 1) / (h - 1);
  if (kx < ky) kx = ky;
  w0 = (xmax - 1) / kx + 0.5;
  h0 = (ymax - 1) / kx + 0.5;
  x0 = (w - w0) / 2 + 0.5;
  y0 = (h - h0) / 2 + 0.5;
  XSetState(disp, gc, state.pixel[0], 0, GXcopy, AllPlanes);
  XFillRectangle(disp, pixmap, gc, 0, 0, w, h); 


  XGetWindowAttributes(state.disp, win, &xwa);
  data = myalloc(pixmap_size(w0, h0, xwa.depth), "set_image", "data");
  xim = XCreateImage(state.disp, xwa.visual, xwa.depth, ZPixmap, 0, data, w0, h0, 8, 0);

  /*  Draw the image  */
  yp = y0;
  for (y = 0; y < h0; y++, yp++) {
    yi = (int) (kx * y + dy + 0.5);
    if (yi >=0 && yi < ymax) {
      ys = yi * xmax;
      xp = x0;
      for (x = 0; x < w0; x++, xp++) {
	xi = (int) (kx * x + dx + 0.5);
	if (xi >= 0 && xi < xmax) {
	  XPutPixel(xim, x, y, state.pixel[correct(frame, n, image[xi + ys])]); 
	}
      }
    }
  }
  XPutImage(state.disp, pixmap, gc, xim, 0, 0, x0, y0, w0, h0);
  XDestroyImage(xim);
  draw_objects(frame, n);
  fl_hide_oneliner();
  fl_winset(FL_ObjWin(state.blinker->blinkW));
}


/*  Calculate values for linear grey level correction  */
void
set_lin(BLINK_FRAME *frame, int n)

{
  float dg, dx;
  int i;

  dx = frame[n].high - frame[n].low;
  dg = dx / (frame[n].ngam - 1.0);
  /*  Set tables for brightness correction  */
  for (i = 0; i < frame[n].ngam; i++) {
    frame[n].gamx[i] = frame[n].low + i * dg;
    frame[n].gamy[i] = (state.grey_max * i) / (frame[n].ngam - 1.0);
    if (i > 0) {
      frame[n].kg[i] = (frame[n].gamy[i] - frame[n].gamy[i - 1]) / 
	(frame[n].gamx[i] - frame[n].gamx[i - 1]);
    }
  }
}


int
load_frame(BLINK_FRAME *frame, int n)

{
  int xmax, ymax;
  PIXEL *image;
  int i, j, low, high, g;
  float dg;
  int wh;
  char *errors[ERROR_MAX];
  int errnum;

  if ((errnum = read_fits(frame[n].name, &image, &xmax, &ymax, 
			  &(frame[n].fitsfile), errors)) > 0) {
    if (state.interactive) {
      show_errors(errnum, errors);
      return 1;
    }
    else {
      for (i = 0; i < errnum; i++) {
	fprintf(stderr, errors[i]);
      }
      exit(1);
    }
  }
  frame[n].fits.name = frame[n].name;
  frame[n].imagelist.s = NULL;
  frame[n].imagelist.n = 0;
  frame[n].cataloglist.s = NULL;
  frame[n].cataloglist.n = 0;
  frame[n].detect = 0;
  frame[n].matched = 0;
  frame[n].map.minmag = DEFAULT_MINMAG;
  frame[n].map.maxmag = DEFAULT_MAXMAG;
  /*  If WCS coordinates are present, calculate coordinates of the image center */
  /*  Enable the Set parameters option in the astrometry menu  */
  if (state.interactive) {
    fl_set_menu_item_mode(state.blinker->astrometryW, EDIT_PARAMETERS, FL_PUP_NONE);
    fl_set_menu_item_mode(state.blinker->astrometryW, LOAD_STAR_LIST, FL_PUP_NONE);
  }
  if (frame[n].fitsfile.wcs) {
    int status = 0;
    fits_pix_to_world(0.5 * xmax, 0.5 * ymax,
	     frame[n].fitsfile.crval1, frame[n].fitsfile.crval2,
	     frame[n].fitsfile.crpix1, frame[n].fitsfile.crpix2,
	     frame[n].fitsfile.cdelt1, frame[n].fitsfile.cdelt2,
	     frame[n].fitsfile.crot,
	     frame[n].fitsfile.ctype,
	     &(frame[n].ra), &(frame[n].dec), &status);
    frame[n].fi = frame[n].fitsfile.crot;
    frame[n].xsize = fabs(frame[n].fitsfile.cdelt1 * 3600.0);
    frame[n].ysize = fabs(frame[n].fitsfile.cdelt2 * 3600.0);
  }
  else {
    frame[n].ra = 0.0;
    frame[n].dec = 0.0;
    frame[n].fi = 0.0;
    frame[n].xsize = 1.92;
    frame[n].ysize = 1.92;
  }
  if (state.interactive) 
    fl_set_menu_item_mode(state.blinker->astrometryW, DETECT_STARS, FL_PUP_NONE);
  /*  If possible, calculate time and date of mid-exposure  */
  calculate_midexposure(&frame[n].fitsfile, &frame[n].year, &frame[n].month,
			&frame[n].day, &frame[n].hour, 
			&frame[n].minute, &frame[n].second);
  /*  Set default options for star matching  */
  set_match_defaults(frame, n);
  set_image_parameters(&frame[n].fits, xmax, ymax, image);
  /* awx = state.blinker->zoom1W.x / state.zoom_width;
  awy = state.blinker->zoom1W.y / state.zoom_width;
  frame[n].astrwin.image = (PIXEL *) myalloc(sizeof(PIXEL) * awx * awy, 
					     "load_frame", "astrwin");
  set_image_parameters(&frame[n].astrwin, awx, awy,
  state.blinker->zoom1W.y / state.zoom_width, );  */
  /*  Translation and rotation of the image  */
  frame[n].dx = frame[n].dy = frame[n].alfa = 0.0;
  if (state.interactive) {
    /*  Create Pixmap for image  */
    frame[n].pixmap = 
      XCreatePixmap(state.disp, 
		    FL_ObjWin(state.blinker->blinkW), 
		    state.bw, state.bh, 
		    fl_state[fl_get_form_vclass(state.blinker->blinker)].depth);

    /*  Find the lowest and the highest grey level  */
    low = 65536;
    high = -65536;
    for (i = 0; i < xmax * ymax; i++) {
      g = image[i];
      if (low > g) low = g;
      if (high < g) high = g;
    }
    frame[n].xmin = frame[n].low = low;
    frame[n].xmax = frame[n].high = high;
    /*  Set number of points for grey level correction  */
    frame[n].ngam = NGAMMA;
    frame[n].gamx = myalloc(frame[n].ngam * sizeof(float), 
			    "load_frame", "frame[n].gamx");
    frame[n].gamy = myalloc(frame[n].ngam * sizeof(float), 
			    "load_frame", "frame[n].gamy");
    frame[n].kg = myalloc(frame[n].ngam * sizeof(float), 
			  "load_frame", "frame[n].kg");
    /*  Width of the histogram box  */
    frame[n].nhisto = wh = HISTOGRAM_WIDTH;
    /* reserve space for histogram  */
    frame[n].histx = myalloc(wh * sizeof(float), 
			     "load_frame", "frame[n].histx");
    frame[n].histy = myalloc(wh * sizeof(float), 
			     "load_frame", "frame[n].histy");
    for (i = 0; i < wh; i++) {
      frame[n].histx[i] = i / (double) (wh - 1) * (high - low);
      frame[n].histy[i] = 0;
    }
    /*  Calculate histogram  */
    dg = (high - low) / (wh - 1.0);
    for (i = 0; i < xmax * ymax; i++) {
      j = (image[i] - low) / dg;
      frame[n].histy[j]++;
    }
    for (i = 0; i < wh; i++) {
      frame[n].histy[i] = log(frame[n].histy[i] + 1);
    }
    /*  Number of markers  */
    frame[n].objnum = 0;
    frame[n].objshow = 1;
    /*  Make automatic grey level correction  */
    autolevels(n);
  }
  return 0;
}

/*  Remove all the data belonging to frame n from the memory  */
int
remove_frame(BLINK_FRAME *frame, int n)

{
  int i;

  if (state.interactive) {
    free(frame[n].histy);
    free(frame[n].histx);
    free(frame[n].kg);
    free(frame[n].gamy);
    free(frame[n].gamx);
    /*  Free the Pixmap  */
    XFreePixmap(state.disp, frame[n].pixmap);
  }
  /*  Free space for the image  */
  free(frame[n].fits.image);
  if (frame[n].imagelist.s) free(frame[n].imagelist.s);
  if (frame[n].cataloglist.s) free(frame[n].cataloglist.s);
  if (frame[n].obj) free(frame[n].obj);
  /* Move data about other images so that they fill the hole which appeared */
  (state.frame_used)--;
  for (i = n; i < state.frame_used; i++) frame[i] = frame[i + 1];
  /*  Set pointers to NULL */
  frame[i].gamx = NULL;
  frame[i].gamy = NULL;
  frame[i].kg = NULL;
  frame[i].histx = NULL;
  frame[i].histy = NULL;
  frame[i].name = NULL;
  frame[i].fits.image = NULL;
  frame[i].cataloglist.s = NULL;
  frame[i].imagelist.s = NULL;
  return 0;
}


/*  Find color index from the string  */
int
find_object_color(char *color)

{
  char *c[] = {"red", "yellow", "blue", "green", "magenta", "cyan"};
  int cl[] = {FL_RED, FL_YELLOW, FL_BLUE, FL_GREEN, FL_MAGENTA, FL_CYAN, FL_RED};
  int i;

  if (strlen(color) == 0) return FL_RED;
  strlwr(color);
  for (i = 0; i < sizeof(c); i++) {
    if (strcmp(color, c[i]) == 0) break;
  }
  return cl[i];
}


/*  Find shape index from the string  */
int
find_object_shape(char *shape)

{
  char *c[] = {"circle", "box", "diamond", "uptriangle", "downtriangle"};
  int cl[] = {OBJ_CIRCLE, OBJ_BOX, OBJ_DIAMOND, OBJ_UPTRIANGLE, OBJ_DOWNTRIANGLE,
	      OBJ_CIRCLE};
  int i;

  if (strlen(shape) == 0) return OBJ_CIRCLE;
  strlwr(shape);
  for (i = 0; i < sizeof(c); i++) {
    if (strcmp(shape, c[i]) == 0) break;
  }
  return cl[i];
}



int
load_file(char *pname)

{
  char line[80], *line1, *p, name[80], *fname;
  FILE *fp;
  int i, i1, j, n;
  float dx, dy;
  GC gc;
  int xmax, ymax;
  int w, h, x0, y0, w0, h0;
  float kx, ky;
  float ta, tb, tc;
  Pixmap pixmap;
  OBJECT *obj = NULL;
  char color[80], shape[80];
  XPoint xpoints[10];

  Display *disp = state.disp;
  gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  w = state.blinker->blinkW->w;
  h = state.blinker->blinkW->h;
  
  if ((fp = fopen(pname, "r")) == NULL) {
    fprintf(stderr, "Can not open file %s\n", pname);
    exit(1);
  }
  i = 0; /*  Number of currently loaded images  */
  while(!feof(fp)) {
    n = 0;
    /*  Read a line from file  */
    fgets(line, 80, fp);
    if (feof(fp)) break;
    line1 = (char *) strdup(line);
    do {
      p = strtok(line1, " \t");
      if (p != NULL) n++;
      line1 = NULL;
    } while (p != NULL);
    /*  n contains a number of items in the current line  */
    switch (n) {
    case 3:
      /*    case 7: */
      if (n == 3) {
	  /*  Three items: get file name and offsets in x and y direction */
	  /****************************************************************/
	  sscanf(line, "%s %f %f", name, &dx, &dy);
	  frame[i].a = 0.0;
	  frame[i].b = 0.0;
	  frame[i].d = 0.0;
	  frame[i].e = 0.0;
      }
      else {
	/*  Seven items: */
	  /****************************************************************/
	/*	  sscanf(line, "%s %f %f %f %f %f %f %f", name, &dx, &dy,
		 &frame[i].a, &frame[i].b, &frame[i].d, &frame[i].e);  */
      }
      frame[i].name = (char *)strdup(name);
      (state.frame_used)++;
      if (state.frame_used == state.frame_max) {
	int i;
	state.frame_max += FRAME_MAX;
	if ((frame = (BLINK_FRAME *) 
	     realloc(frame, sizeof(BLINK_FRAME) * state.frame_max)) == NULL) {
	  fprintf(stderr, "Out of memory, function loadC, variable frame\n");
	  exit(1);
	}
	for (i = state.frame_used; i < state.frame_max; i++) {
	  frame[i].gamx = NULL;
	  frame[i].gamy = NULL;
	  frame[i].kg = NULL;
	  frame[i].histx = NULL;
	  frame[i].histy = NULL;
	  frame[i].name = NULL;
	  frame[i].fits.image = NULL;
	}
      }
      /*  Load the image into the structure */
      if (load_frame(frame, i)) {
	(state.frame_used)--;
	break;
      }
      frame[i].dx = dx;
      frame[i].dy = dy;
      frame[i].objmax = OBJECT_NUM;
      obj = frame[i].obj = (OBJECT *) myalloc(sizeof(OBJECT) * frame[i].objmax, 
					      "load_file", "obj");
      frame[i].imagelist.s = (STAR *) myalloc(sizeof(OBJECT) * frame[i].objmax, 
					      "load_file", "imagelist.s");
      set_image(frame, i);
      /*  Add image name to choices  */
      fname = strrchr(name, '/');
      if (!fname) fname = name;
      else fname++;
      fl_addto_choice(state.gui->firstnameW, fname);
      fl_addto_choice(state.gui->secondnameW, fname);
      if (i == 0 || i == 1) {
	state.control[state.control_num] = i;
	/*  Write the file name into the input field,  */
	/*  draw histogram, grey level correction curve, translation and rotation  */
	show_frame_info(frame, i, 0);
	if (state.frame_used == 1) {
	  state.control[1] = 0;
	  show_frame_info(frame, i, 1);
	}
      }
      i++;
      break;
    case 4:
    case 5:
    case 6:
      /*  Info about an additional object  */
      if (state.frame_used == 0) {
	fprintf(stderr, "No image loaded yet: ignoring line %s", line);
	break;
      }
      i1 = i - 1;
      j = frame[i1].objnum;
      obj[j].s = (char *) myalloc(sizeof(char) * 81, "load_image", "obj[j].s");
      color[0] = '\0';
      shape[0] = '\0';
      if (n == 4) {
	if (sscanf(line, "%f %f %f %s", &(obj[j].x), &(obj[j].y),
		   &(obj[j].r), obj[j].s) != 4) break;
	frame[i1].imagelist.s[j].x = obj[j].x;
	frame[i1].imagelist.s[j].y = obj[j].y;
	frame[i1].imagelist.s[j].mag = obj[j].r;
	frame[i1].imagelist.n = j + 1;
      }
      else if (n == 5) {
	if (sscanf(line, "%f %f %f %s %s", &(obj[j].x), &(obj[j].y),
		   &(obj[j].r), obj[j].s, color) != 5) break;
	frame[i1].imagelist.s[j].x = obj[j].x;
	frame[i1].imagelist.s[j].y = obj[j].y;
	frame[i1].imagelist.s[j].mag = obj[j].r;
	frame[i1].imagelist.n = j + 1;
      }
      else if (n == 6) {
	if (sscanf(line, "%f %f %f %s %s %s", &(obj[j].x), &(obj[j].y),
		   &(obj[j].r), obj[j].s, color, shape) != 6) break;
	frame[i1].imagelist.s[j].x = obj[j].x;
	frame[i1].imagelist.s[j].y = obj[j].y;
	frame[i1].imagelist.s[j].mag = obj[j].r;
	frame[i1].imagelist.n = j + 1;
      }
      obj[j].color = find_object_color(color);
      obj[j].shape = find_object_shape(shape);
      frame[i1].objnum++;
      if (frame[i1].objnum == frame[i1].objmax) {
	frame[i1].objmax += OBJECT_NUM;
	if ((obj = frame[i1].obj = (OBJECT *)
	     realloc(obj, sizeof(OBJECT) * frame[i1].objmax)) == NULL) {
	  fprintf(stderr, "Out of memory, function load_image, variable obj\n");
	  exit(1);
	}
	if ((frame[i1].imagelist.s = (STAR *)
	     realloc(frame[i1].imagelist.s, sizeof(OBJECT) * frame[i1].objmax)) == NULL) {
	  fprintf(stderr, "Out of memory, function load_image, variable obj\n");
	  exit(1);
	}
      }
      pixmap = frame[i1].pixmap;
      xmax = frame[i1].fits.width;
      ymax = frame[i1].fits.height;
      dx = frame[i1].dx;
      dy = frame[i1].dy;
      kx = (float) (xmax - 1) / (w - 1);
      ky = (float) (ymax - 1) / (h - 1);
      if (kx < ky) kx = ky;
      w0 = (xmax - 1) / kx + 0.5;
      h0 = (ymax - 1) / kx + 0.5;
      x0 = (w - w0) / 2 + 0.5;
      y0 = (h - h0) / 2 + 0.5;
      XSetState(disp, gc, fl_get_pixel(obj[j].color), 0, GXcopy, AllPlanes);
      switch (obj[j].shape) {
      case OBJ_CIRCLE:
	XDrawArc(disp, pixmap, gc, 
		 x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5), 
		 y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5), 
	   (int) (2 * obj[j].r), (int) (2 * obj[j].r), 0, 23040);
	XDrawArc(disp, pixmap, gc, 
	       x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 1.5), 
		 y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 1.5), 
	       (int) (2 * obj[j].r + 2), (int) (2 * obj[j].r + 2), 0, 23040);
	break;
      case OBJ_BOX:
	XDrawRectangle(disp, pixmap, gc, 
		 x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5), 
		       y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5), 
	   (int) (2 * obj[j].r), (int) (2 * obj[j].r));
	XDrawRectangle(disp, pixmap, gc, 
		 x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 1.5), 
		       y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 1.5), 
	   (int) (2 * obj[j].r + 2), (int) (2 * obj[j].r + 2));
	break;
      case OBJ_DIAMOND:
	xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
	xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx - obj[j].r - 0.5);
	xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + obj[j].r + 0.5);
	xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx + 0.5);
	xpoints[2].x = xpoints[0].x;
	xpoints[2].y = y0 + (int) ((obj[j].y - dy) / kx + obj[j].r + 0.5);
	xpoints[3].x = x0 + (int) ((obj[j].x - dx) / kx - obj[j].r - 0.5);
	xpoints[3].y = xpoints[1].y;
	xpoints[4].x = xpoints[0].x;
	xpoints[4].y = xpoints[0].y;
	xpoints[5].x = xpoints[0].x;
	xpoints[5].y = xpoints[0].y - 1;
	xpoints[6].x = xpoints[1].x + 1;
	xpoints[6].y = xpoints[1].y;
	xpoints[7].x = xpoints[2].x;
	xpoints[7].y = xpoints[2].y + 1;
	xpoints[8].x = xpoints[3].x - 1;
	xpoints[8].y = xpoints[3].y;
	xpoints[9].x = xpoints[5].x;
	xpoints[9].y = xpoints[5].y;
	XDrawLines(disp, pixmap, gc, xpoints, 10, CoordModeOrigin);
	break;
      case OBJ_UPTRIANGLE:
	tb = 0.5773502 * obj[j].r;
	tc = 0.5 * tb;
	ta = 0.5 * obj[j].r;
	xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
	xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx - tb - 0.5);
	xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + ta + 0.5);
	xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx + tc + 0.5);
	xpoints[2].x = x0 + (int) ((obj[j].x - dx) / kx - ta - 0.5);
	xpoints[2].y = xpoints[1].y;
	xpoints[3].x = xpoints[0].x;
	xpoints[3].y = xpoints[0].y;
	xpoints[4].x = xpoints[0].x;
	xpoints[4].y = xpoints[0].y - 1;
	xpoints[5].x = xpoints[1].x + 1;
	xpoints[5].y = xpoints[1].y + 1;
	xpoints[6].x = xpoints[2].x - 1;
	xpoints[6].y = xpoints[5].y;
	xpoints[7].x = xpoints[4].x;
	xpoints[7].y = xpoints[4].y;
	XDrawLines(disp, pixmap, gc, xpoints, 8, CoordModeOrigin);
	break;
      case OBJ_DOWNTRIANGLE:
	tb = 0.5773502 * obj[j].r;
	tc = 0.5 * tb;
	ta = 0.5 * obj[j].r;
	xpoints[0].x = x0 + (int) ((obj[j].x - dx) / kx + 0.5);
	xpoints[0].y = y0 + (int) ((obj[j].y - dy) / kx + tb - 0.5);
	xpoints[1].x = x0 + (int) ((obj[j].x - dx) / kx + ta + 0.5);
	xpoints[1].y = y0 + (int) ((obj[j].y - dy) / kx - tb + 0.5);
	xpoints[2].x = x0 + (int) ((obj[j].x - dx) / kx - ta - 0.5);
	xpoints[2].y = xpoints[1].y;
	xpoints[3].x = xpoints[0].x;
	xpoints[3].y = xpoints[0].y;
	xpoints[4].x = xpoints[0].x;
	xpoints[4].y = xpoints[0].y + 1;
	xpoints[5].x = xpoints[1].x + 1;
	xpoints[5].y = xpoints[1].y + 1;
	xpoints[6].x = xpoints[2].x - 1;
	xpoints[6].y = xpoints[2].y - 1;
	xpoints[7].x = xpoints[4].x;
	xpoints[7].y = xpoints[4].y;
	XDrawLines(disp, pixmap, gc, xpoints, 8, CoordModeOrigin);
	break;
      }
      break;
    }
  }
  fclose(fp);
  XSetState(disp, gc, state.pixel[0], 0, GXcopy, AllPlanes);
  if (state.frame_used > 0) {
    show_frame_info(frame, state.control[state.control_num], state.control_num);
  }
  return 0;
}


int
main(int argc, char *argv[])

{
  int i;
  char *np;

  /*  reserve space for FRAME_MAX descriptions of images  */
  frame = (BLINK_FRAME *) myalloc(sizeof(BLINK_FRAME) * FRAME_MAX,
				  "main", "frame");
  state.frame_max = FRAME_MAX;
  for (i = 0; i < state.frame_max; i++) {
    frame[i].gamx = NULL;
    frame[i].gamy = NULL;
    frame[i].kg = NULL;
    frame[i].histx = NULL;
    frame[i].histy = NULL;
    frame[i].name = NULL;
    frame[i].fits.image = NULL;
  }
  /*===============================================*/
  /*  Set defaults for the program state variable  */
  /*===============================================*/
  state.browser[0] = NULL;
  state.browser[1] = NULL;
  state.b[0] = 0;
  state.b[1] = 0; 
  state.error = NULL; 
  state.readme = NULL;
  state.parameters = NULL;
  state.report = NULL;
  state.options = NULL;
  state.catalogs = NULL;
  state.mail = NULL;
  state.newwcs = NULL;
  state.settings = NULL;
  state.set_catalogs = NULL;
  state.set_observatory = NULL;
  state.set_display = NULL;
  state.set_mail = NULL;
  state.set_commands = NULL;
  state.set_ccd = NULL;
  state.set_count = NULL;
  state.set_match = NULL;
  state.showstar = NULL;
  state.showcomet = NULL;
  state.telescope = NULL;
  state.message = NULL;
  state.blinkwidth = 1000;
  state.blinkheight = 910;
  state.frame_used = 0;
  state.control_num = 0;
  state.zoom_width = 4;
  state.freeze = 0;
  state.blink = 0;
  state.blinknum = 0;
  state.delay = DEFAULT_DELAY;
  state.autolevel_high = DEFAULT_AUTOLEVEL_HIGH;
  state.logfile = NULL;
  state.logfp = stdout;
  state.interactive = 1;
  state.verbose = 0;
  state.doastr = 0;
  state.color = 0;
  state.inverse = 0;
  state.wwwbrowser = NULL;
  state.helpfile = NULL;
  /*======================================================*/
  /*  Set default values for star counting and matching   */
  /*======================================================*/
  set_defvals();
  /*====================================*/
  /*  Mark count_options variables      */
  /*====================================*/
  count_options.maxsize = -999;
  count_options.sg_num = -999;
  count_options.fitmeasure = -999;
  count_options.border = -999;
  count_options.minbright = -999;
  count_options.minstar = -999;
  count_options.inner = -999;
  count_options.outer = -999;
  count_options.aperture = -999;
  /*  Define supported catalogs  */
  /*====================================*/
  /*  Mark match_options variables      */
  /*====================================*/
  match_options.firstbright = -999;
  match_options.nstar = -999;
  match_options.min = -999;
  match_options.poserrmax = -999;
  match_options.magerrmax = -999;
  match_options.maxres = -999;
  /*=============================*/
  catalog[CAT_USNO].use = 0;
  catalog[CAT_USNO].name = "USNO-SA 1.0";
  catalog[CAT_USNO].lpath = PATHLENGTH;
  if ((catalog[CAT_USNO].path = (char *) malloc(catalog[CAT_USNO].lpath)) == NULL) {
    fprintf(stderr, "Out of memory, path for USNO catalog.\n");
    exit(1);
  }
  catalog[CAT_USNO].path[0] = '\0';
  catalog[CAT_PPM].use = 0;
  catalog[CAT_PPM].name = "PPM";
  catalog[CAT_PPM].lpath = PATHLENGTH;
  if ((catalog[CAT_PPM].path = (char *) malloc(catalog[CAT_PPM].lpath)) == NULL) {
    fprintf(stderr, "Out of memory, path for PPM catalog.\n");
    exit(1);
  }
  catalog[CAT_PPM].path[0] = '\0';
  catalog[CAT_GUIDE].use = 0;
  catalog[CAT_GUIDE].name = "GSC 1.1";
  catalog[CAT_GUIDE].lpath = PATHLENGTH;
  if ((catalog[CAT_GUIDE].path = (char *) malloc(catalog[CAT_GUIDE].lpath)) == NULL) {
    fprintf(stderr, "Out of memory, path for GUIDE NORTH catalog.\n");
    exit(1);
  }
  catalog[CAT_GUIDE].path[0] = '\0';

  catalog[CAT_GUIDE_SOUTH].use = 0;
  catalog[CAT_GUIDE_SOUTH].name = "GSC 1.1";
  catalog[CAT_GUIDE_SOUTH].lpath = PATHLENGTH;
  if ((catalog[CAT_GUIDE_SOUTH].path = (char *) malloc(catalog[CAT_GUIDE_SOUTH].lpath)) == NULL) {
    fprintf(stderr, "Out of memory, path for GUIDE SOUTH catalog.\n");
    exit(1);
  }
  catalog[CAT_GUIDE_SOUTH].path[0] = '\0';

  /*=================================*/
  /*  Define observatory parameters  */
  /*=================================*/
  observatory.code = 0;
  observatory.observer[0] = '\0';
  observatory.telescope[0] = '\0';
  observatory.instrument[0] = '\0';

  /*=================================*/
  /*  Telescope control defaults     */
  /*=================================*/
  telescope.ra = 0;
  telescope.dec = 0;
  telescope.exptime = 1;
  telescope.dark = 0;
  telescope.temp = 0;
  telescope.filename = NULL;

  /*  Mail structure  */
  if ((mail.to = (char *) malloc(1)) == NULL) {
    fprintf(stderr, "Out of memory, mail.to string.\n");
    exit(1);
  }
  mail.to[0] = '\0';
  if ((mail.cc = (char *) malloc(1)) == NULL) {
    fprintf(stderr, "Out of memory, mail.cc string.\n");
    exit(1);
  }
  mail.cc[0] = '\0';
  if ((mail.title = (char *) malloc(1)) == NULL) {
    fprintf(stderr, "Out of memory, mail.title string.\n");
    exit(1);
  }
  mail.title[0] = '\0';
  if ((mail.file = (char *) malloc(1)) == NULL) {
    fprintf(stderr, "Out of memory, mail.file string.\n");
    exit(1);
  }
  mail.file[0] = '\0';

  /*  Find out name of the program  */
  /*================================*/
  np = strrchr(argv[0], '/');  /* check first if there is any path  */
  if (np == NULL) np = argv[0];  /*  No path, just name  */
  else np++;
  if (strcmp(np, "starcount") == 0) {
    state.interactive = 0;
    starcount(argc, argv);
  } 
  else if (strcmp(np, "catalog") == 0) {
    state.interactive = 0;
    make_catalog(argc, argv);
  } else {
    tgui(argc, argv);
  }
  return 0;
}
