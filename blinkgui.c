/*  blinkgui.c for fitsblink */
/*  Jure Skvarc, May 1998  */
/* User interface */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <forms.h>
#include <fitsio.h>

#ifdef HPUX
#include <macros.h>
#endif

#include "formblink.h"
#include "functs.h"
#include "consts.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern OBSERVATORY observatory;

extern char *optarg;
extern int optind, opterr, optopt;
/*  Draw given image to the screen  */
void
draw_image(BLINK_FRAME *frame, int n)

{
  GC gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  Window win = FL_ObjWin(state.blinker->blinkW);

  XSetFunction(state.disp, gc, GXcopy);
  XCopyArea(state.disp, frame[n].pixmap, win, gc, 0, 0, 
	    state.blinker->blinkW->w, state.blinker->blinkW->h,
	    state.blinker->blinkW->x, state.blinker->blinkW->y);
  state.doastr = 0;
  fl_set_form_title(state.blinker->blinker, frame[n].name);
  fl_set_button(state.blinker->markersW, frame[n].objshow);
}

/*  Draw a magnified image around the x, y position to a zoom window */
void
draw_zoom(BLINK_FRAME *frame, int n, FL_OBJECT *o, int x, int y)

{
  int xmin, xmax, ymin, ymax;
  int w, h;
  int xq, yq, xt, yt;
  int z;
  int i, j;
  PIXEL v;
  FITS_IMAGE *im;
  Display *disp;

  GC gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  Window win = FL_ObjWin(o);
  disp = state.disp;

  im = &(frame[n].fits);
  if (x >=0 && x < im->width &&
      y >=0 && y < im->height) {  /*  Draw only if the pointer is inside image */
    
    /*  Determine number of pixels in x and y direction  */
    z = state.zoom_width;
    w = o->w / z;
    h = o->h / z;
    /*  Limiting coordinates of the area shown  */
    xmin = max(0, x - w);
    xmax = min(x + w, im->width - 1);
    ymin = max(0, y - h);
    ymax = min(y + h, im->height - 1);
    /*  Do the drawing  */
    yq = o->y;
    yt = y - h / 2;
    for (j = 0; j < h; j++) {
      xq = o->x;
      xt = x - w / 2; 
      for (i = 0; i < w; i++) {
	v = video_point(im, xt++, yt);
	XSetForeground(disp, gc, state.pixel[correct(frame, n, v)]);
	XFillRectangle(disp, win, gc, xq, yq, z, z);
	xq += z;
      }
      yq += z;
      yt++;
    }
    /*  Draw a cross  */
    XSetForeground(disp, gc, fl_get_pixel(FL_RED));
    xt = o->x + w / 2 * z;
    yt = o->y + h / 2 * z;
    w = o->w / 4;
    h = o->h / 4;
    XFillRectangle(disp, win, gc, xt, yt - h, z, h);
    XFillRectangle(disp, win, gc, xt, yt + z, z, h);
    XFillRectangle(disp, win, gc, xt - h, yt, h, z);
    XFillRectangle(disp, win, gc, xt + z, yt, h, z); 
  }
}


/*  Show offsets for the left image  */
void
show_left_offsets(BLINK_FRAME *frame, int n)

{
  char temp[64];

  sprintf(temp, "%.2f", frame[n].dx);
  fl_set_input(state.gui->firstdeltaxW, temp);
  sprintf(temp, "%.2f", frame[n].dy);
  fl_set_input(state.gui->firstdeltayW, temp);
}

/*  Show offsets for the right image  */
void
show_right_offsets(BLINK_FRAME *frame, int n)

{
  char temp[64];

  sprintf(temp, "%.2f", frame[n].dx);
  fl_set_input(state.gui->seconddeltaxW, temp);
  sprintf(temp, "%.2f", frame[n].dy);
  fl_set_input(state.gui->seconddeltayW, temp);
}

/*  Show information about frame n in the control c  */
void
show_frame_info(BLINK_FRAME *frame, int n, int c)

{
  switch (c) {
  case 0:
    fl_set_choice(state.gui->firstnameW, n + 1);
    fl_set_xyplot_data(state.gui->firstgreyW, 
		       frame[n].histx, frame[n].histy, frame[n].nhisto, 
		       "", "", "");
    fl_set_xyplot_data(state.gui->firstgammaW, 
		       frame[n].gamx, frame[n].gamy, frame[n].ngam, 
		       "", "", "");
    fl_set_xyplot_xbounds(state.gui->firstgammaW, frame[n].xmin, frame[n].xmax);
    fl_set_xyplot_xbounds(state.gui->firstgreyW, frame[n].xmin, frame[n].xmax);
    show_left_offsets(frame, n);
    if (state.control_num == 0) {
      draw_image(frame, n);
    }
    break;
  case 1:
    fl_set_choice(state.gui->secondnameW, n + 1);
    fl_set_xyplot_data(state.gui->secondgreyW, 
		       frame[n].histx, frame[n].histy, frame[n].nhisto, 
		       "", "", "");
    fl_set_xyplot_data(state.gui->secondgammaW, 
		       frame[n].gamx, frame[n].gamy, frame[n].ngam, 
		       "", "", "");
    fl_set_xyplot_xbounds(state.gui->secondgammaW, frame[n].xmin, frame[n].xmax);
    fl_set_xyplot_xbounds(state.gui->secondgreyW, frame[n].xmin, frame[n].xmax);
    show_right_offsets(frame, n);
    if (state.control_num == 1) {
      draw_image(frame, n);
    }
    break;
  }
  fl_set_menu_item_mode(state.blinker->astrometryW, MATCH_STARS, (frame[n].detect) ? FL_PUP_NONE : FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, SAVE_STAR_LIST, (frame[n].detect) ? FL_PUP_NONE : FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, DO_ASTROMETRY, (frame[n].matched) ? FL_PUP_NONE : FL_PUP_GREY);
  if (state.parameters && fl_form_is_visible(state.parameters->Parameters)) {
    edit_parameters(state.blinker->astrometryW, EDIT_PARAMETERS);
  }
}

/*************************************/
/*  Callbacks for the control window */
/*************************************/
void
turnimageC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    int n = state.control[val];
    state.control_num = val;
    draw_image(frame, n);
    if (state.parameters && fl_form_is_visible(state.parameters->Parameters)) {
      edit_parameters(state.blinker->astrometryW, EDIT_PARAMETERS);
    }
  }
}

/*  Load a FITS image, calculate a histogram and set some parameters in a
 control  */
void 
loadC(FL_OBJECT *obj, long val)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.[fF]*[tT][sS]*";
  static char *name = "";
  int n;

  fl_use_fselector(1);
  fl_invalidate_fselector_cache();
  fn = fl_show_fselector("Choose input file", curdir, pattern, name);
  if (fn == NULL) return;
  /*  Save the current directory  */
  curdir = (char *) fl_get_directory();
  /*  Save the current pattern */
  pattern = (char *) fl_get_pattern();
  /*  Get index of the current image  */
  n = state.frame_used;
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
  /*  Release space for the previous name  */
  if (frame[n].name) free(frame[n].name);
  /*  copy filename to the structure  */
  frame[n].name = strdup((char *) fn);
  /*  Load the image into the structure */
  if (!load_frame(frame, n)) {
    state.control[val] = n;
    set_image(frame, n);
    /*  Add image name to choices  */
    fl_addto_choice(state.gui->firstnameW, fl_get_filename());
    fl_addto_choice(state.gui->secondnameW, fl_get_filename());
    /*  Write the file name into the input field,  */
    /*  draw histogram, grey level correction curve, translation and rotation  */
    show_frame_info(frame, n, val);
    if (state.frame_used == 1) {
      show_frame_info(frame, n, 1 - val);
    }
  }
  else {
    state.frame_used--;
  }
}

/*  Erase the currently selected filename from the list and release all
    the memory occupied by the image  */
void
eraseC(FL_OBJECT *obj, long val)

{
  FL_OBJECT *ch1, *ch2;
  int i, n, m;

  ch1 = state.gui->firstnameW;
  ch2 = state.gui->secondnameW;
  n = state.control[val];
  m = n + 1;
  state.freeze = 0;
  fl_hide_object(state.blinker->lockedW);
  fl_hide_object(state.blinker->upW);
  fl_hide_object(state.blinker->downW);
  fl_hide_object(state.blinker->rightW);
  fl_hide_object(state.blinker->leftW);
  fl_hide_object(state.blinker->circleW);
  /*  Erase the filename from both choices  */
  if (fl_get_choice_maxitems(ch1) > 0) {
    fl_delete_choice(ch1, m);
    fl_delete_choice(ch2, m);
    /*  Remove the image from the memory  */
    remove_frame(frame, n);
    if (state.blinknum == n) state.blinknum--;
    if (state.frame_used < 2 && state.blink) {
      /*  Stop blinking  */
      state.blink = 0;
      /* Switch off the Blink button */
      fl_set_button(state.blinker->playW, 0);
    }
    if (fl_get_choice_maxitems(ch1) > 0) {
      state.control[0] = fl_get_choice(ch1) - 1;
      state.control[1] = fl_get_choice(ch2) - 1;
      show_frame_info(frame, state.control[0], 0);
      show_frame_info(frame, state.control[1], 1);
    }
    else {
      /*  The last image was erased */
      /*  Erase histogram, grey level correction curve and images  */
      fl_set_xyplot_data(state.gui->firstgreyW, NULL, NULL, 0, "", "", "");
      fl_set_xyplot_data(state.gui->secondgreyW, NULL, NULL, 0, "", "", "");
      fl_set_xyplot_data(state.gui->firstgammaW, NULL, NULL, 0, "", "", "");
      fl_set_xyplot_data(state.gui->secondgammaW, NULL, NULL, 0, "", "", "");
      fl_winset(FL_ObjWin(state.blinker->blinkW));
      fl_set_form_title(state.blinker->blinker, "(no image)");
      fl_rectf(state.blinker->blinkW->x, state.blinker->blinkW->y,
	       state.blinker->blinkW->w, state.blinker->blinkW->h, 
	       state.blinker->blinkW->col1);
      fl_rectf(state.blinker->zoom1W->x, state.blinker->zoom1W->y,
	       state.blinker->zoom1W->w, state.blinker->zoom1W->h, 
	       state.blinker->zoom1W->col1);
      fl_redraw_object(state.blinker->zoom1W);
      fl_rectf(state.blinker->zoom2W->x, state.blinker->zoom2W->y,
	       state.blinker->zoom2W->w, state.blinker->zoom2W->h, 
	       state.blinker->zoom2W->col1);
      fl_redraw_object(state.blinker->zoom2W);
      /*  Disable Astrometry options  */
      for (i = 1; i < EMPTY_ITEM1; i++) {
	fl_set_menu_item_mode(state.blinker->astrometryW, i, FL_PUP_GREY);
      }
      /*  If the Set parameters window is open, close it  */
      if (state.parameters && fl_form_is_visible(state.parameters->Parameters)) {
	fl_hide_form(state.parameters->Parameters);
      }
    }
  }
}

/*  Change control to another image  */
void
nameC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    state.control[val] = fl_get_choice(obj) - 1;
    show_frame_info(frame, state.control[val], val);
  }
}

/******************************************/
/*  Show the contents of the FITS header  */
/******************************************/
void
fitsC(FL_OBJECT *obj, long val)

{
  fitsfile *fptr;
  int status;
  int i;
  char line[81], line1[81];
  int n = state.control[val];

  if (state.frame_used == 0) return;
  if (state.browser[val] == NULL) {
    state.browser[val] = create_form_Edit();
  }
  /*  Deactivate FITS button */
  fl_deactivate_object(obj);
  /*  Show the form  */
  fl_show_form(state.browser[val]->Edit, FL_PLACE_FREE_CENTER, FL_FULLBORDER, frame[n].name);
  /*  Open FITS file  */
  status = 0;
  fits_open_file(&fptr, frame[n].name, READONLY, &status);
  i = 1;
  do {
    /*  Read fits header line */
    fits_read_record(fptr, i, line, &status);
    if (!status) {
      /*  Insert it in the browser  */
      fl_addto_browser(state.browser[val]->browseW, line);
      strcpy(line1, line);
    }
    else if (strncmp("END", line1, 3)) {
      do  {
	fits_read_errmsg(line);
	fl_addto_browser(state.browser[val]->browseW, line);
      } while(strlen(line) > 0);
      status = 1;
    }
    i++;
  } while (status == 0);
  /*  close the file  */
  fits_close_file(fptr, &status);
  state.b[val] = 1;
}  


/***********************/
/*  Histogram          */
/***********************/

/*  This should probably remain empty */
void
greyC(FL_OBJECT *obj, long val)

{
}

void
emptyC(FL_OBJECT *obj, long val)

{
}

/**************************/
/* Grey level correction  */
/**************************/
void
gammaC(FL_OBJECT *obj, long val)

{
  float x, y;
  int i;
  int n, m;

  /*  Get the value which has changed  */
  fl_get_xyplot(obj, &x, &y, &i);
  if (i >= 0) {
    n = state.control[val];
    frame[n].gamx[i] = x;
    frame[n].gamy[i] = y;
    if (i > 0) {
      frame[n].kg[i] = (frame[n].gamy[i] - frame[n].gamy[i - 1]) / 
	(frame[n].gamx[i] - frame[n].gamx[i - 1]);
    }
    if (i < frame[n].ngam - 1) {
      frame[n].kg[i + 1] = (frame[n].gamy[i + 1] - frame[n].gamy[i]) / 
	(frame[n].gamx[i + 1] - frame[n].gamx[i]);
    }
    m = state.control[1 - val];
    if (n == m) {
      switch (val) {
      case 0:
	fl_set_xyplot_data(state.gui->secondgammaW, 
		       frame[m].gamx, frame[m].gamy, frame[m].ngam, 
		       "", "", "");
	break;
      case 1:
	fl_set_xyplot_data(state.gui->firstgammaW, 
		       frame[m].gamx, frame[m].gamy, frame[m].ngam, 
		       "", "", "");
	break;
      }
    }
    set_image(frame, n);
    if (state.control_num == val || state.control[0] == state.control[1]) {
      draw_image(frame, n);
    }
  }
}

void
histadjC(FL_OBJECT *obj, long val)

{
  float gmin, gmax;
  int c, step  = 0;
  
  c = fl_mouse_button();
  switch(c) {
  case FL_LEFT_MOUSE:
    step = 100;
    break;
  case FL_MIDDLE_MOUSE:
    step = 1000;
    break;
  case FL_RIGHT_MOUSE:
    step = 10000;
    break;
  }
  switch (val) {
  case 1:
    fl_get_xyplot_xbounds(state.gui->firstgammaW, &gmin, &gmax);
    gmin += step;
    break;
  case 2:
    fl_get_xyplot_xbounds(state.gui->firstgammaW, &gmin, &gmax);
    gmin -= step;
    break;
  case 3:
    fl_get_xyplot_xbounds(state.gui->firstgammaW, &gmin, &gmax);
    gmax -= step;
    break;
  case 4:
    fl_get_xyplot_xbounds(state.gui->firstgammaW, &gmin, &gmax);
    gmax += step;
    break;
  case 5:
    fl_get_xyplot_xbounds(state.gui->secondgammaW, &gmin, &gmax);
    gmin += step;
    break;
  case 6:
    fl_get_xyplot_xbounds(state.gui->secondgammaW, &gmin, &gmax);
    gmin -= step;
    break;
  case 7:
    fl_get_xyplot_xbounds(state.gui->secondgammaW, &gmin, &gmax);
    gmax -= step;
    break;
  case 8:
    fl_get_xyplot_xbounds(state.gui->secondgammaW, &gmin, &gmax);
    gmax += step;
    break;
  }
  if (val < 5 || state.control[0] == state.control[1]) {
    fl_set_xyplot_xbounds(state.gui->firstgammaW, gmin, gmax);
    fl_set_xyplot_xbounds(state.gui->firstgreyW, gmin, gmax);
    frame[state.control[0]].xmin = gmin;
    frame[state.control[0]].xmax = gmax;
  }
  if (val > 4 || state.control[0] == state.control[1]) {
    frame[state.control[1]].xmin = gmin;
    frame[state.control[1]].xmax = gmax;
    fl_set_xyplot_xbounds(state.gui->secondgammaW, gmin, gmax);
    fl_set_xyplot_xbounds(state.gui->secondgreyW, gmin, gmax);
  }
}


int
cmppixel (const void *a1, const void *b1)

{
  int t = (int) *((PIXEL *) a1) - (int) *((PIXEL *) b1);
  if (t > 0)
    return 1;
  else if (t < 0)
    return -1;
  else
    return 0;
}

void
invert_display(BLINK_FRAME *frame, int n)

{
  int i;
  for (i = 0; i < frame[n].ngam; i++) {
    frame[n].gamy[i] = state.grey_max - frame[n].gamy[i];
    if (i > 0) 
      frame[n].kg[i] = (frame[n].gamy[i] - frame[n].gamy[i - 1]) / 
	(frame[n].gamx[i] - frame[n].gamx[i - 1]);
  }
}

/*===========================================================*/
/*  Do automatic adjustment of grey levels for the image n   */
/*===========================================================*/
void
autolevels(int n)

{
  int i, l, p, ng;
  int j, m;
  double min, max, d;
  PIXEL *temp, *p1, *p2;
  int step;

  /*  Reserve space for a temporary image   */
  l = 50000;
  step = frame[n].fits.width * frame[n].fits.height / l;
  if (step == 0) {
    step = 1;
    l = frame[n].fits.width * frame[n].fits.height;
  }
  temp = (PIXEL *) myalloc(sizeof(PIXEL) * l, "autoC", "temp");
  /*  Copy the image  */
  p1 = frame[n].fits.image;
  p2 = temp;
  for (i = 0; i < l; i++) {
    *p2 = *p1;
    p1 += step;
    p2++;
  }


  /*  Sort values */
  qsort(temp, l, sizeof(PIXEL), cmppixel);
  ng = frame[n].ngam; 
  p = ng / 2 - 1;
#if 1
  frame[n].gamx[p] = temp[(int) (0.03 * l)];
  frame[n].gamy[p] = 0;
  frame[n].gamx[p + 1] = temp[(int) ((1.0 - state.autolevel_high) * l)];
  frame[n].gamy[p + 1] = state.grey_max;
#else
  /*  Median value  */
  m = temp[l / 2];
  d = 0;
  j = 0;
  for (i = 0; i < l / 2; i++) {
    min = (temp[i] - m);
    d += min * min;
    j++;
  }
  min = sqrt(d / j); 
  d = 0;
  j = 0;
  for (i = l / 2; i < l; i++) {
    max = (temp[i] - m);
    d += max * max;
    j++;
  }
  max = sqrt(d / j); 
  frame[n].gamx[p] = m - 4 * min;
  frame[n].gamy[p] = 0;
  frame[n].gamx[p + 1] = m + 1.0 * max;
  frame[n].gamy[p + 1] = state.grey_max;
#endif
  for(i = 0; i < p; i++) {
    frame[n].gamx[i] = i * (frame[n].gamx[p] - frame[n].low) / p;
    frame[n].gamy[i] = 0;
  }
  p += 2;
  for(i = p; i < ng; i++) {
    frame[n].gamx[i] = frame[n].gamx[p - 1] + (i + 1 - p) * 
      (frame[n].high - frame[n].gamx[p - 1]) / (ng - p);
    frame[n].gamy[i] = state.grey_max;
  }
  for (i = 1; i < ng; i++) {
    frame[n].kg[i] = (frame[n].gamy[i] - frame[n].gamy[i - 1]) / 
      (frame[n].gamx[i] - frame[n].gamx[i - 1]);
  }
  free(temp);
  /*  frame[n].xmin = frame[n].gamx[p - 2] * 0.8;
      frame[n].xmax = frame[n].gamx[p  - 1] * 1.2; */

  if (state.inverse) invert_display(frame, n);
}

void
autoC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    int n = state.control[val];
    
    fl_set_oneliner_font(0, 20); 
    fl_show_oneliner("Adjusting grey levels", 100, 100);
    XFlush(state.disp);
    autolevels(n);
    fl_hide_oneliner();
    set_image(frame, n);
    show_frame_info(frame, n, val);
    if (state.control[0] == state.control[1]) {
      show_frame_info(frame, n, 1 - val);
    }
  }
}

/*  Invert the grey level transformation  */
void
invertC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    int n = state.control[val];
    invert_display(frame, n);
    set_image(frame, n);
    show_frame_info(frame, n, val);
    if (state.control[0] == state.control[1]) {
      show_frame_info(frame, n, 1 - val);
    }
  }
}

/**************************************/
/*   Image translation and rotation  */
/**************************************/
void
deltaxC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    int n = state.control[val];
    frame[n].dx = strtod(fl_get_input(obj), NULL);
    show_left_offsets(frame, n);
    show_right_offsets(frame, n);
    set_image(frame, n);
    if (state.control_num == val) {
      draw_image(frame, n);
    }
  }
}

void
deltayC(FL_OBJECT *obj, long val)

{
  if (state.frame_used > 0) {
    int n = state.control[val];
    frame[n].dy = strtod(fl_get_input(obj), NULL);
    show_left_offsets(frame, n);
    show_right_offsets(frame, n);
    set_image(frame, n);
    if (state.control_num == val) {
      draw_image(frame, n);
    }
  }
}

/***********************/
/*  Ending the program */
/***********************/

void
quitC(FL_OBJECT *obj, long val)

{
  if (fl_show_question("Do you really want to quit?", 1)) {
    fl_finish();
    exit(0);
  }
}


void
helpC(FL_OBJECT *obj, long val)

{
  int m;
  char *a;

  m = fl_get_menu(obj);
  switch (m) {
  case 1:
    info();
    break;
  case 2:
    /* readme(obj, 2); */
    if (state.wwwbrowser && state.helpfile) {
      a = (char *) myalloc(sizeof(char) * (strlen(state.wwwbrowser) + 
					   strlen(state.helpfile) + 2), "helpC", "a");
      strcpy(a, state.wwwbrowser);
      strcat(a, " ");
      strcat(a, state.helpfile);
      fl_exe_command(a, 0);
    }
    break;
  }
}


void
info(void)

{
  static FD_info *info;
  char a[30];

  if (info == NULL) {
    info = create_form_info();
    sprintf(a, "Fistblink %s", VERSION);
    fl_set_object_label(info->versionW, a);
  }
  /*  Deactivate FITS button */
  fl_deactivate_all_forms();
  /*  Show the form  */
  fl_show_form(info->info, FL_PLACE_MOUSE | FL_FIX_SIZE, FL_FULLBORDER, "");
  fl_do_forms();
  fl_hide_form(info->info);
  fl_activate_all_forms();
}  

void
readme(FL_OBJECT *obj, int n)

{

  /*  The window in not created yet, do it now  */
  if (state.readme == NULL) {
    state.readme = create_form_help();
    fl_set_form_minsize(state.readme->help, 
		  state.readme->helpW->w, state.readme->helpW->h);
    /*  Read README file  */
    fl_load_browser(state.readme->readmeW, BLINK_README_FILE);
  }
  /*  Deactivate README item in the menu */
  fl_set_menu_item_mode(obj, n, FL_PUP_GREY);
  /*  Show the form  */
  fl_show_form(state.readme->help, FL_PLACE_FREE_CENTER, FL_FULLBORDER, "README");
}

/*  These two remain empty  */
void
zoom1C(FL_OBJECT *obj, long val)

{
}

void
zoom2C(FL_OBJECT *obj, long val)

{
}



/**************************************/
/*  Callbacks for the blinking window */
/**************************************/
void
do_blink(void)

{
  float d;

  draw_image(frame, state.blinknum);
  d = fl_get_counter_value(state.blinker->deltatW);
  delay((int) (100.0 * d));
  state.blinknum++;
  if (state.blinknum == state.frame_used) state.blinknum = 0;
}

/*  Should remain empty  */
void
deltatC(FL_OBJECT *obj, long val)

{
}

void
blinkC(FL_OBJECT *obj, long val)

{
}

/*  Move the right image one pixel up */
void
upC(FL_OBJECT *obj, long val)

{
  if (state.freeze) {
    /*  Get index of the right image  */
    int n = state.control[1];
    /*  Shift it up for one pixel  */
    frame[n].dy++;
    state.yf++;
    draw_zoom(frame, n, state.blinker->zoom2W, state.xf, state.yf);
    show_right_offsets(frame, n);
    if (state.control[0] == state.control[1]) {
      show_left_offsets(frame, n);
      draw_zoom(frame, n, state.blinker->zoom1W, state.xf, state.yf);
    }
  }
}

/*  Move the right image one pixel down */
void
downC(FL_OBJECT *obj, long val)

{
  if (state.freeze) {
    /*  Get index of the right image  */
    int n = state.control[1];
    /*  Shift it down for one pixel  */
    frame[n].dy--;
    state.yf--;
    draw_zoom(frame, n, state.blinker->zoom2W, state.xf, state.yf);
    show_right_offsets(frame, n);
    if (state.control[0] == state.control[1]) {
      show_left_offsets(frame, n);
      draw_zoom(frame, n, state.blinker->zoom1W, state.xf, state.yf);
    }
  }
}

/*  Move the right image one pixel right */
void
rightC(FL_OBJECT *obj, long val)

{
  if (state.freeze) {
    /*  Get index of the right image  */
    int n = state.control[1];
    /*  Shift it right for one pixel  */
    frame[n].dx--;
    state.xf--;
    draw_zoom(frame, n, state.blinker->zoom2W, state.xf, state.yf);
    show_right_offsets(frame, n);
    if (state.control[0] == state.control[1]) {
      show_left_offsets(frame, n);
      draw_zoom(frame, n, state.blinker->zoom1W, state.xf, state.yf);
    }
  }
}

/*  Move the right image one pixel left */
void
leftC(FL_OBJECT *obj, long val)

{
  if (state.freeze) {
    /*  Get index of the right image  */
    int n = state.control[1];
    /*  Shift it left for one pixel  */
    frame[n].dx++;
    state.xf++;
    draw_zoom(frame, n, state.blinker->zoom2W, state.xf, state.yf);
    show_right_offsets(frame, n);
    if (state.control[0] == state.control[1]) {
      show_left_offsets(frame, n);
      draw_zoom(frame, n, state.blinker->zoom1W, state.xf, state.yf);
    }
  }
}

/*  Switch off image shifting mode  */
void
circleC(FL_OBJECT *obj, long val)

{
  state.freeze = 0;
  /*  Hide cursors and LOCKED ! sign  */
  fl_hide_object(state.blinker->lockedW);
  fl_hide_object(state.blinker->upW);
  fl_hide_object(state.blinker->downW);
  fl_hide_object(state.blinker->rightW);
  fl_hide_object(state.blinker->leftW);
  fl_hide_object(state.blinker->circleW);
  set_image(frame, state.control[1]);
  if (!fl_get_button(state.blinker->playW) &&
      state.control_num == 1) {
    draw_image(frame, state.control[1]);
  }
}

/*=================================*/
/*  Open telescope control window  */
/*=================================*/
void
telescope_controlC(FL_OBJECT *obj, long val)

{
  show_telescope_control();
}


int
freeobj_blinkW_handle(FL_OBJECT *d, int event, FL_Coord mx, FL_Coord my, int key, void *xev)

{
  GC gc;
  Window win;
  int w, h, w0, h0, x0, y0;
  float kx, ky;
  static int xp = 0, yp = 0;
  int xmax, ymax;
  int n;
  static int out = 0;
  char temp[20];
  double r, rmin;
  int i, imin;

  if ((state.frame_used == 0 && event != FL_DRAW) || out) return 1;
  gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  win = FL_ObjWin(d);
  fl_winset(win);
  n = state.control[state.control_num];
  /*  Convert mouse coordinates to image coordinates  */
  w = d->w;
  h = d->h;
  xmax = frame[n].fits.width;
  ymax = frame[n].fits.height;
  kx = (float) (xmax - 1) / (w - 1);
  ky = (float) (ymax - 1) / (h - 1);
  if (kx < ky) kx = ky;
  w0 = (xmax - 1) / kx + 0.5;
  h0 = (ymax - 1) / kx + 0.5;
  x0 = d->x + (w - w0) / 2 + 0.5;
  y0 = d->y + (h - h0) / 2 + 0.5;

  switch(event) {
  case FL_STEP:
    break;
  case FL_DRAW:
    if (state.bw != w || state.bh != h) {
      int i;
      out = 1;
      fl_addto_form(state.blinker->blinker);
      /*  Delete free object  */
      fl_delete_object(state.blinker->blinkW);
      /*  Add free object again  */
      fl_get_winsize(FL_ObjWin(state.blinker->zoom1W), &w, &h);
      h = state.blinker->zoom1W->y;
      state.blinker->blinkW = 
	fl_add_free(FL_NORMAL_FREE,10,
		    state.blinker->racoorW->y + state.blinker->racoorW->h + 5, 
		    w - 20, 
		    h - (state.blinker->racoorW->y + state.blinker->racoorW->h) - 10,
		    "",freeobj_blinkW_handle);
      fl_set_object_boxtype(state.blinker->blinkW, FL_FRAME_BOX);
      fl_end_form();
      state.bw = state.blinker->blinkW->w;
      state.bh = state.blinker->blinkW->h;
      fl_get_winsize(win, &state.blinkwidth, &state.blinkheight);
      /*  Free all of the existing pixmaps  */
      for (i = state.frame_used - 1; i >= 0; i--) {
	XFreePixmap(state.disp, frame[i].pixmap); 
      }
      /*  Create pixmaps with new dimensions and draw images into them  */
      for (i = 0; i < state.frame_used; i++) {
	frame[i].pixmap = 
	  XCreatePixmap(state.disp, 
			FL_ObjWin(state.blinker->blinkW), 
			state.bw, state.bh, 
			fl_state[fl_get_form_vclass(state.blinker->blinker)].depth);
	set_image(frame, i); 
      }
      out = 0;
    }
    
    /*  redraw the form  */
    fl_redraw_object(state.blinker->bgroupW);
    if (state.frame_used > 0) {
      if (!state.freeze) {
	draw_zoom(frame, state.control[0], state.blinker->zoom1W, 
		  xp - frame[n].dx + frame[state.control[0]].dx, 
		  yp - frame[n].dy + frame[state.control[0]].dy);
	draw_zoom(frame, state.control[1], state.blinker->zoom2W, 
		  xp - frame[n].dx + frame[state.control[1]].dx, 
		  yp - frame[n].dy + frame[state.control[1]].dy);
      }
      draw_image(frame, n);
    }
    break;
  case FL_MOTION:
    /*  Determine image coordinates from the screen coordinates of the mouse  */
    xp = (mx - x0) * kx + frame[n].dx + 0.5;
    yp = (my - y0) * kx + frame[n].dy + 0.5;
    if (!state.freeze) {
      /*  Draw magnified images around mouse position in the window  */
      draw_zoom(frame, state.control[0], state.blinker->zoom1W, 
		xp - frame[n].dx + frame[state.control[0]].dx, 
		yp - frame[n].dy + frame[state.control[0]].dy);
      draw_zoom(frame, state.control[1], state.blinker->zoom2W, 
		xp - frame[n].dx + frame[state.control[1]].dx, 
		yp - frame[n].dy + frame[state.control[1]].dy);
    }
    fl_drawmode(GXcopy);
    XSetForeground(state.disp, gc, fl_get_pixel(state.blinker->xcoorW->col1));
    sprintf(temp, "%d", xp);
    fl_rectf(state.blinker->xcoorW->x, state.blinker->xcoorW->y,
	   state.blinker->xcoorW->w, state.blinker->xcoorW->h,
	     state.blinker->xcoorW->col1);
    fl_set_input(state.blinker->xcoorW, temp);
    sprintf(temp, "%d", yp);
    fl_rectf(state.blinker->ycoorW->x, state.blinker->ycoorW->y,
	   state.blinker->ycoorW->w, state.blinker->ycoorW->h,
	     state.blinker->ycoorW->col1);
    fl_set_input(state.blinker->ycoorW, temp);
    sprintf(temp, "%u", video_point(&(frame[n].fits), xp, yp));
    fl_rectf(state.blinker->valueW->x, state.blinker->valueW->y,
	   state.blinker->valueW->w, state.blinker->valueW->h,
	     state.blinker->valueW->col1);
    fl_set_input(state.blinker->valueW, temp);

    if (frame[n].fitsfile.wcs) {
      double ra, dec;
      int status = 0;
      fits_pix_to_world(xp + 1, yp + 1, 
	       frame[n].fitsfile.crval1, frame[n].fitsfile.crval2, 
	       frame[n].fitsfile.crpix1, frame[n].fitsfile.crpix2, 
	       frame[n].fitsfile.cdelt1, frame[n].fitsfile.cdelt2, 
	       frame[n].fitsfile.crot,
	       frame[n].fitsfile.ctype,
	       &ra, &dec, &status);
      if (!status) {
	int rah, ram, dech, decm, c;
	double ral, decl;
	ra /= 15.0;
	rah = (int) ra;
	ram = (int) ((ra - rah) * 60);
	ral = 60.0 * (60.0 * (ra - rah) - ram);
	if (dec < 0) {
	  c = '-';
	  dec = -dec;
	}
	else {
	  c = '+';
	}
	dech = (int) dec;
	decm = (int) ((dec - abs(dech)) * 60);
	decl = 60.0 * (60.0 * (dec - abs(dech)) - decm);
	sprintf(temp, "%2d:%02d:%06.3f", rah, ram, ral);
	fl_rectf(state.blinker->racoorW->x, state.blinker->racoorW->y,
	       state.blinker->racoorW->w, state.blinker->racoorW->h,
	       state.blinker->racoorW->col1);
	fl_set_input(state.blinker->racoorW, temp);
	sprintf(temp, "%c%2d:%02d:%05.2f", c, dech, decm, decl);
	fl_rectf(state.blinker->deccoorW->x, state.blinker->deccoorW->y,
	       state.blinker->deccoorW->w, state.blinker->deccoorW->h,
	       state.blinker->deccoorW->col1);
	fl_set_input(state.blinker->deccoorW, temp);
      }
    }
    break;
  case FL_DBLCLICK:
    /*  Double click inside the image freezes images in zoom windows  */
    state.freeze = !state.freeze;
    if (!state.freeze) {
      circleC(0, 0);
    }      
    else {
      /*  Store coordinates  */
      state.xf = (mx - x0) * kx + frame[state.control[1]].dx + 0.5;
      state.yf = (my - y0) * kx + frame[state.control[1]].dy + 0.5;
      /*  Show cursors and LOCKED ! sign  */
      fl_show_object(state.blinker->lockedW);
      fl_show_object(state.blinker->upW);
      fl_show_object(state.blinker->downW);
      fl_show_object(state.blinker->rightW);
      fl_show_object(state.blinker->leftW);
      fl_show_object(state.blinker->circleW);
    }      
    break;
  case FL_PUSH:
    if (state.doastr && key == 1) {   /* If in astrometry mode, find the nearest star  */
      /*  Determine image coordinates from the screen coordinates of the mouse  */
      xp = (mx - x0) * kx + frame[n].dx + 0.5;
      yp = (my - y0) * kx + frame[n].dy + 0.5;
      /* Find the closest star from the star list  */
      rmin = 1e20;
      imin = -1;
      for (i = 0; i < frame[n].imagelist.n; i++) {
	r = fsqr(frame[n].imagelist.s[i].x - xp) + 
	  fsqr(frame[n].imagelist.s[i].y - yp);
	if (r < rmin) {
	  rmin = r;
	  imin = i;
	}
      }
      if (rmin > 25) {
	fl_show_alert("No detected object within circle of r = 5 pixels!", "Try again!", "", 1);
      }
      else if ((frame[n].fits.aperture && 
		showcomet(frame, n, &frame[n].imagelist.s[imin], 0, frame[n].fitsfile.object)) ||
	       (!frame[n].fits.aperture && 
		showstar(frame, n, &frame[n].imagelist.s[imin], 0, frame[n].fitsfile.object))) {
	char a[FLEN_VALUE];
	if (strlen(frame[n].fitsfile.object) > 7) 
	  frame[n].fitsfile.object[8] = '\0';
	if (strlen(frame[n].fitsfile.object) > 0 && 
	    isalpha(frame[n].fitsfile.object[0])) {
	  sprintf(a, "     %-8s %c%4d %02d %8.5f %11s %11s                      %03d", 
		  frame[n].fitsfile.object, 'C', frame[n].year, frame[n].month,
		  frame[n].day + 
		  (frame[n].hour + 
		   (frame[n].minute + frame[n].second / 60.0) / 60.0) / 24.0,
		  format_ra_mpc(frame[n].imagelist.s[imin].ra),
		  format_dec_mpc(frame[n].imagelist.s[imin].dec), observatory.code);
	}
	else {
	  int number = strtol(frame[n].fitsfile.object, NULL, 10);
	  sprintf(a, "%05d         %c%4d %02d %8.5f %11s %11s                      %03d", 
		  number, 'C', frame[n].year, frame[n].month,
		  frame[n].day + 
		  (frame[n].hour + 
		   (frame[n].minute + frame[n].second / 60.0) / 60.0) / 24.0,
		  format_ra_mpc(frame[n].imagelist.s[imin].ra),
		  format_dec_mpc(frame[n].imagelist.s[imin].dec), observatory.code);
	}
	edit_mail(state.blinker->astrometryW, SEND_MAIL);
	fl_addto_browser(state.mail->messageW, a);
      }
    }
    if (key == 3) {   /* Find the nearest star  */
      /*  Determine image coordinates from the screen coordinates of the mouse  */
      xp = (mx - x0) * kx + frame[n].dx + 0.5;
      yp = (my - y0) * kx + frame[n].dy + 0.5;
      /* Find the closest star from the star list  */
      rmin = 1e20;
      imin = -1;
      for (i = 0; i < frame[n].imagelist.n; i++) {
	r = fsqr(frame[n].imagelist.s[i].x - xp) + 
	  fsqr(frame[n].imagelist.s[i].y - yp);
	if (r < rmin) {
	  rmin = r;
	  imin = i;
	}
      }
      if (rmin > 25) {
      }
      else {
	if (frame[n].fits.aperture) {
	  showcomet(frame, n, &frame[n].imagelist.s[imin], 1, frame[n].obj[imin].s);
	}
	else {
	  showstar(frame, n, &frame[n].imagelist.s[imin], 1, frame[n].obj[imin].s);
	}
      }
    }
    break;
  case FL_RELEASE:
    break;
  case FL_MOUSE:  
    break;
  case FL_KEYBOARD:
    break;
  case FL_ENTER:
    fl_set_cursor(win, (state.doastr) ? XC_cross : XC_dotbox);
    break;
  case FL_LEAVE:
    fl_reset_cursor(win);
    XSetForeground(state.disp, gc, fl_get_pixel(state.blinker->xcoorW->col1));
    fl_rectf(state.blinker->xcoorW->x, state.blinker->xcoorW->y,
	   state.blinker->xcoorW->w, state.blinker->xcoorW->h,
	     state.blinker->xcoorW->col1);
    fl_set_input(state.blinker->xcoorW, "");
    fl_rectf(state.blinker->ycoorW->x, state.blinker->ycoorW->y,
	   state.blinker->ycoorW->w, state.blinker->ycoorW->h,
	     state.blinker->ycoorW->col1);
    fl_set_input(state.blinker->ycoorW, "");
    fl_rectf(state.blinker->racoorW->x, state.blinker->racoorW->y,
	   state.blinker->racoorW->w, state.blinker->racoorW->h,
	     state.blinker->racoorW->col1);
    fl_set_input(state.blinker->racoorW, "");
    fl_rectf(state.blinker->deccoorW->x, state.blinker->deccoorW->y,
	   state.blinker->deccoorW->w, state.blinker->deccoorW->h,
	     state.blinker->deccoorW->col1);
    fl_set_input(state.blinker->deccoorW, "");
    fl_rectf(state.blinker->valueW->x, state.blinker->valueW->y,
	   state.blinker->valueW->w, state.blinker->valueW->h,
	     state.blinker->valueW->col1);
    fl_set_input(state.blinker->valueW, "");
    break;
  }
  return 0;
}

void
colormapC(FL_OBJECT *obj, long val)

{
  switch (val) {
  case 1:
    define_greymap();
    break;
  case 2:
    define_colormap();
    break;
  }
}

/**********************************/
/* Switch display of markers      */
/**********************************/
void
markersC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];

  if (state.frame_used > 0) {
    frame[n].objshow = fl_get_button(obj);
    if (!frame[n].objshow && frame[n].objnum > 0) {
      set_image(frame, n);
    }
    else {
      /*  Draw objects to the currently displayed image  */
      draw_objects(frame, n);
    }
    draw_image(frame, n);
  }
}

/*  Define the greymap  */
int
define_greymap(void)

{
  Visual *visual;
  int i, j;
  float k;

  visual = DefaultVisual(state.disp, DefaultScreen((state.disp)));
  /*  Get number of grey levels  */
  state.grey_max = min(visual->map_entries - FL_BUILT_IN_COLS, GREY_WANTED);
  state.grey_max--;
  for (i = 0; i <= state.grey_max; i++) 
    state.pixel[i] = i + FL_FREE_COL1;
  fl_free_pixels(state.pixel, state.grey_max + 1);
  k = 255.0 / state.grey_max;
  for (i = 0; i <= state.grey_max; i++) {
    j = (int) (i * k);
    fl_mapcolor(i + FL_FREE_COL1, j, j, j);
    state.pixel[i] = fl_get_pixel(state.pixel[i]);
  }
  for (i = 0; i < state.frame_used; i++) 
    set_image(frame, i);
  if (state.frame_used > 0)
    draw_image(frame, state.control[state.control_num]);
  state.color = 0;
  return 0;
}

/*  Define the colormap  */
int
define_colormap(void)

{
  Visual *visual;
  int i, j, step;
  static rgb[][3] = {{0,8,16}, {0,12,24}, {0,16,28}, {0,20,32},
		     {4,20,36}, {4,24,40}, {4,28,40}, {4,28,44},
		     {4,32,48}, {4,32,52}, {4,36,52}, {4,36,56},
		     {4,40,56}, {4,40,60}, {4,40,60}, {4,44,64},
		     {4,44,64}, {4,48,68}, {8,48,68}, {8,52,72},
		     {8,52,72}, {8,56,76}, {8,56,76}, {8,60,76},
		     {8,60,80}, {8,60,80}, {8,64,84}, {8,64,84},
		     {12,68,84}, {12,68,88}, {12,72,88}, {12,72,88},
		     {12,76,92}, {12,76,92}, {12,80,92}, {16,80,96},
		     {16,80,96}, {16,84,96}, {16,84,100}, {16,88,100},
		     {16,88,100}, {20,92,104}, {20,92,104}, {20,96,104},
		     {20,96,108}, {20,100,108}, {24,100,108}, {24,104,108},
		     {24,104,112}, {24,108,112}, {28,108,112}, {28,112,116},
		     {28,112,116}, {28,116,116}, {32,116,116}, {32,120,120},
		     {32,120,120}, {32,120,120}, {36,120,120}, {36,124,120},
		     {36,124,116}, {36,124,116}, {36,124,116}, {40,128,116},
		     {40,128,116}, {40,128,116}, {44,128,116}, {44,132,116},
		     {44,132,116}, {44,132,116}, {48,132,116}, {48,136,116},
		     {48,136,116}, {52,136,116}, {52,136,116}, {52,140,116},
		     {52,140,116}, {56,140,116}, {56,140,116}, {56,140,116},
		     {60,144,116}, {60,144,116}, {64,144,116}, {64,144,116},
		     {64,148,116}, {68,148,116}, {68,148,116}, {68,148,116},
		     {72,148,116}, {72,152,116}, {76,152,116}, {76,152,116},
		     {76,152,116}, {80,152,116}, {80,156,116}, {84,156,116},
		     {84,156,112}, {88,156,112}, {88,156,112}, {88,160,112},
		     {92,160,112}, {92,160,112}, {96,160,112}, {96,160,112},
		     {100,164,112}, {100,164,112}, {104,164,112}, {104,164,112},
		     {108,164,112}, {108,168,116}, {112,168,116}, {112,168,116},
		     {116,168,116}, {116,168,116}, {120,172,116}, {120,172,116},
		     {124,172,116}, {124,172,116}, {128,172,116}, {128,172,116},
		     {132,176,116}, {136,176,116}, {136,176,116}, {140,176,120},
		     {140,176,120}, {144,180,120}, {144,180,120}, {148,180,120},
		     {148,180,120}, {152,180,120}, {152,180,120}, {156,184,124},
		     {156,184,124}, {160,184,124}, {160,184,124}, {164,184,124},
		     {164,184,128}, {168,188,128}, {168,188,128}, {172,188,128},
		     {172,188,132}, {176,188,132}, {176,188,132}, {180,192,132},
		     {180,192,132}, {180,192,136}, {184,192,136}, {184,192,136},
		     {188,192,140}, {188,196,140}, {188,196,140}, {192,196,140},
		     {192,196,144}, {192,196,144}, {196,196,144}, {196,200,148},
		     {196,200,148}, {200,200,148}, {200,200,152}, {200,200,152},
		     {200,200,152}, {204,200,152}, {204,200,156}, {204,200,156},
		     {204,200,156}, {204,200,156}, {204,204,160}, {204,204,160},
		     {208,204,160}, {208,204,160}, {208,204,164}, {208,204,164},
		     {208,204,164}, {208,204,168}, {208,204,168}, {212,208,168},
		     {212,208,168}, {212,208,172}, {212,208,172}, {212,208,172},
		     {212,208,176}, {216,212,176}, {216,212,176}, {216,212,180},
		     {216,212,180}, {216,212,180}, {216,216,184}, {216,216,184},
		     {220,216,184}, {220,216,188}, {220,216,188}, {220,220,188},
		     {220,220,192}, {220,220,192}, {220,220,192}, {224,220,196},
		     {224,224,196}, {224,224,196}, {224,224,200}, {224,224,200},
		     {224,224,200}, {224,224,200}, {224,228,204}, {224,228,204},
		     {224,228,204}, {224,228,204}, {228,228,208}, {228,228,208},
		     {228,228,208}, {228,232,208}, {228,232,208}, {228,232,212},
		     {228,232,212}, {228,232,212}, {228,232,212}, {228,232,216},
		     {228,232,216}, {228,236,216}, {228,236,216}, {228,236,216},
		     {228,236,220}, {228,236,220}, {228,236,220}, {228,236,220},
		     {232,240,220}, {232,240,224}, {232,240,224}, {232,240,224},
		     {232,240,224}, {232,240,224}, {232,240,224}, {232,240,228},
		     {232,244,228}, {232,244,228}, {232,244,228}, {232,244,228},
		     {232,244,232}, {232,244,232}, {232,244,232}, {232,244,232},
		     {236,248,232}, {236,248,232}, {236,248,236}, {236,248,236},
		     {236,248,236}, {236,248,236}, {236,248,236}, {236,248,236},
		     {236,252,240}, {236,252,240}, {236,252,240}, {236,252,240},
		     {236,252,240}, {236,252,240}, {236,252,240}};
  
  visual = DefaultVisual(state.disp, DefaultScreen((state.disp)));
  /*  Get number of colors levels  */
  state.grey_max = min(visual->map_entries - FL_BUILT_IN_COLS, GREY_WANTED);
  step = 256 / state.grey_max;
  state.grey_max--;
  for (i = 0; i <= state.grey_max; i++) 
    state.pixel[i] = i + FL_FREE_COL1;
  fl_free_pixels(state.pixel, state.grey_max + 1);
  j = 0;
  for (i = 0; i <= state.grey_max; i++) {
    fl_mapcolor(state.pixel[i], rgb[j][0], rgb[j][1], rgb[j][2]);
    state.pixel[i] = fl_get_pixel(state.pixel[i]);
    j += step;
  }
  for (i = 0; i < state.frame_used; i++) 
    set_image(frame, i);
  if (state.frame_used > 0)
    draw_image(frame, state.control[state.control_num]);
  state.color = 1;
  return 0;
}


void
tgui(int argc, char *argv[])

{
  FL_OBJECT *o;
  XEvent event;
  FL_IOPT opt;
  int op, i;

  opt.sharedColormap = 1;
  fl_set_defaults(FL_PDSharedMap, &opt);
  state.disp = fl_initialize(&argc, argv, "Blink", 0, 0);
  /*  Load the settings  */
  load_settings();
  check_catalogs();
  /*  Create the main form  */
  state.gui = create_form_setting();
  /*  Create the blinker  */
  state.blinker = create_form_blinker();
   /*  Disable ticks in grey level histogram and correction graphs */
  fl_set_xyplot_xtics(state.gui->firstgreyW, -1.0, -1.0);
  fl_set_xyplot_ytics(state.gui->firstgreyW, -1.0, -1.0);
  fl_set_xyplot_xtics(state.gui->firstgammaW, -1.0, -1.0);
  fl_set_xyplot_ytics(state.gui->firstgammaW, -1.0, -1.0);
  fl_set_xyplot_xtics(state.gui->secondgreyW, -1.0, -1.0);
  fl_set_xyplot_ytics(state.gui->secondgreyW, -1.0, -1.0);
  fl_set_xyplot_xtics(state.gui->secondgammaW, -1.0, -1.0);
  fl_set_xyplot_ytics(state.gui->secondgammaW, -1.0, -1.0);

  /*  "Press" the button of the first image  */
  fl_set_button(state.gui->firstimageW, 1); 
  fl_set_button(state.gui->secondimageW, 0);
  /* Set the upper and lower limit for interval between images  */
  fl_set_counter_bounds(state.blinker->deltatW, 0.0, 5.0);
  fl_set_counter_value(state.blinker->deltatW, state.delay);
  fl_set_counter_step(state.blinker->deltatW, 0.1, 0.1);
  /*  Hide the LOCKED ! sign and cursors */
  fl_hide_object(state.blinker->lockedW);
  fl_hide_object(state.blinker->upW);
  fl_hide_object(state.blinker->downW);
  fl_hide_object(state.blinker->rightW);
  fl_hide_object(state.blinker->leftW);
  fl_hide_object(state.blinker->circleW);
  /*  Dectivate input fields with coordinates and pixel value  */
  fl_deactivate_object(state.blinker->xcoorW);
  fl_deactivate_object(state.blinker->ycoorW);
  fl_deactivate_object(state.blinker->racoorW);
  fl_deactivate_object(state.blinker->deccoorW);
  fl_deactivate_object(state.blinker->valueW);
  /*  Set the help menu  */
  fl_set_menu(state.gui->helpW, " About | Manual ");
  /*  Set the astrometry menu  */
  fl_set_menu(state.blinker->astrometryW, " Edit parameters | Detect stars | Match stars | Save star list | Load star list | Do astrometry | | Options | Catalogs | Settings | Mail results | Show report");
  /*  Disable menu options  */
  fl_set_menu_item_mode(state.blinker->astrometryW, EDIT_PARAMETERS, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, DETECT_STARS, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, MATCH_STARS, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, SAVE_STAR_LIST, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, LOAD_STAR_LIST, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, DO_ASTROMETRY, FL_PUP_GREY);
  fl_set_menu_item_mode(state.blinker->astrometryW, EMPTY_ITEM1, FL_PUP_GREY);
  /*  Show the forms  */
  fl_show_form(state.gui->setting, FL_PLACE_MOUSE | FL_FIX_SIZE, FL_FULLBORDER, "Fitsblink");
  fl_set_form_minsize(state.blinker->blinker, 
    state.blinker->blinker->w / 2, state.blinker->blinker->h / 2);
  fl_set_form_size(state.blinker->blinker, state.blinkwidth, state.blinkheight);
  fl_show_form(state.blinker->blinker, FL_PLACE_FREE_CENTER, FL_FULLBORDER, "");
  fl_redraw_form(state.blinker->blinker);
  fl_set_form_title(state.blinker->blinker, "(no image)");
  /*  Store dimensions of the blinking area  */
  state.bw = state.blinker->blinkW->w;
  state.bh = state.blinker->blinkW->h;
  if (state.color) { 
    define_colormap();
    fl_set_button(state.blinker->greybutW, 0);
    fl_set_button(state.blinker->colorbutW, 1);
  }
  else {
    define_greymap();
    fl_set_button(state.blinker->greybutW, 1);
    fl_set_button(state.blinker->colorbutW, 0);
  }


  while ((op = getopt(argc, argv, "i:v")) != -1) {
    switch(op) {
    case 'i':
      /*  load  a .inp file  */
      load_file(optarg);
      break;
    case 'v':
      state.verbose = 1;
      break;
    case '?':
      switch(optopt) {
      default:
	fprintf(stderr, "Unknown option -%c ignored!\n", optopt);
	break;
      }
      break;
    }
  }

  /* Load the images  */
  for (i = optind; i < argc; i++) {
    char *fname;
    int j = i - optind;

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
    frame[j].name = strdup(argv[i]);
    if (load_frame(frame, j)) {
      (state.frame_used)--;
      continue;
    }
    set_image(frame, j);
    /*  Add image name to choices  */
    fname = strrchr(argv[i], '/');
    if (!fname) fname = argv[i];
    else fname++;
    fl_addto_choice(state.gui->firstnameW, fname);
    fl_addto_choice(state.gui->secondnameW, fname);
    if (j == 0 || j == 1) {
      state.control[state.control_num] = j;
      /*  Write the file name into the input field,  */
      /*  draw histogram, grey level correction curve, translation and rotation  */
      show_frame_info(frame, j, 0);
      if (state.frame_used == 1) {
	state.control[1] = 0;
	show_frame_info(frame, j, 1);
      }
    }
  }
  do {
    o = fl_check_forms();
    if (o == FL_EVENT) {
      fl_XNextEvent(&event);
    }
    else if (o == state.blinker->playW) {
      if (state.frame_used > 1 && fl_get_button(o)) {
	state.blink = 1;
	state.blinknum = 0;
      }
      else {
	fl_set_button(o, 0);
	state.blink = 0;
	if (state.frame_used > 0) 
	  draw_image(frame, state.control[state.control_num]);
      }
    } else if (state.b[0] && o == state.browser[0]->closeeditW) {
      fl_clear_browser(state.browser[0]->browseW);
      fl_hide_form(state.browser[0]->Edit);
      fl_activate_object(state.gui->firstfitsW);
      state.b[0] = 0;
    } else if (state.b[1] && o == state.browser[1]->closeeditW) {
      fl_clear_browser(state.browser[1]->browseW);
      fl_hide_form(state.browser[1]->Edit);
      fl_activate_object(state.gui->secondfitsW);
      state.b[1] = 0;
    } else if (state.readme && o == state.readme->closehelpW) {
      fl_hide_form(state.readme->help);
      fl_set_menu_item_mode(state.gui->helpW, 2, FL_PUP_NONE);
    } else if (state.parameters && o == state.parameters->parokW) {
      fl_hide_form(state.parameters->Parameters);
      fl_set_menu_item_mode(state.blinker->astrometryW, EDIT_PARAMETERS, FL_PUP_NONE);
      fl_set_menu_item_mode(state.blinker->astrometryW, DETECT_STARS, FL_PUP_NONE);
    } else if (state.options && o == state.options->acceptoptionsW) {
      fl_hide_form(state.options->Options);
      fl_set_menu_item_mode(state.blinker->astrometryW, OPTIONS, FL_PUP_NONE);
    } else if (state.catalogs && o == state.catalogs->okcatalogsW) {
      fl_hide_form(state.catalogs->Catalog);
      fl_set_menu_item_mode(state.blinker->astrometryW, CATALOGS, FL_PUP_NONE);
    } else if (state.mail && o == state.mail->closemailW) {
      fl_hide_form(state.mail->Mail);
      fl_set_menu_item_mode(state.blinker->astrometryW, SEND_MAIL, FL_PUP_NONE);
    }
    else if (state.report && o == state.report->closereportW) {
      hide_report();
    }
    else if (state.telescope && o == state.telescope->telcloseW) {
      fl_remove_timeout(state.timeout);
      fl_hide_form(state.telescope->Telescope);
    }

    if (state.blink) do_blink();
  } while(1);
}
