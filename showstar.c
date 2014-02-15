/*  showstar.c  */
/*  part of the fitsblink program  */
/*  Jure Skvarc                    */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include <forms.h>
#include <string.h>
#include <fitsio.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern CATALOG catalog[];

STAR *cstar;

void
objstarC(FL_OBJECT *obj, long val)

{
  fl_trigger_object(state.showstar->acceptstarW);
  fl_trigger_object(state.showstar->okstarW);
}


void
draw_object(FL_OBJECT *o, STAR *z, int zoom, FITS_IMAGE *im, BLINK_FRAME *frame, int n)

{
  int xq, yq, yt, xt, r;
  int v, h, w;
  int i, j, x, y;
  GC gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  Window win = FL_ObjWin(o);

  x = z->x;
  y = z->y;
  /*  Number of pixels in x and y direction  */
  w = o->w / zoom;
  h = o->h / zoom;
  /*  Do the drawing  */
  yq = o->y;
  yt = y - h / 2;
  for (j = 0; j < h; j++) {
    xq = o->x;
    xt = x - w / 2; 
    for (i = 0; i < w; i++) {
      v = video_point(im, xt++, yt);
      XSetForeground(state.disp, gc, state.pixel[correct(frame, n, v)]);
      XFillRectangle(state.disp, win, gc, xq, yq, zoom, zoom);
      xq += zoom;
    }
    yq += zoom;
    yt++;
  }
  /*  Draw a cross  */
  xt = z->x - w / 2;
  xt = (z->x - (float) xt + 0.5) * zoom + o->x;
  yt = z->y - h / 2;
  yt = (z->y - (float) yt + 0.5) * zoom + o->y;
  w = o->w / 4;
  h = o->h / 4;
  fl_line(xt, o->y, xt, o->y + o->h - 1, FL_RED);
  fl_line(o->x, yt, o->x + o->w - 1, yt, FL_RED);
  if (im->aperture) {
    r = (int) (frame[n].fits.inner * zoom + 0.5);
    fl_ovall(xt - r, yt - r, 2 * r, 2 * r, FL_WHITE);
    fl_ovall(xt - r - 1, yt - r - 1, 2 * (r + 1), 2 * (r + 1), FL_BLACK);
    r = (int) (frame[n].fits.outer * zoom + 0.5);
    fl_ovall(xt - r, yt - r, 2 * r, 2 * r, FL_WHITE);
    fl_ovall(xt - r - 1, yt - r - 1, 2 * (r + 1), 2 * (r + 1), FL_BLACK);
  }
}
/*  Show an object */
int
showstar(BLINK_FRAME *frame, int n, STAR *z, int ok, char *name)

{
  double ac, dc;
  char a[25];
  int out, err = 0;
  XEvent event;
  FL_OBJECT *o;
  double xc, yc, xinc, yinc;

  int w, h;
  int xq, yq, xt, yt, x, y;
  int zoom;
  int i, j;
  PIXEL v;
  FITS_IMAGE *im;
  Display *disp;
  GC gc = fl_state[fl_get_form_vclass(state.blinker->blinker)].gc[0];
  Window win;

  cstar = z;
  xinc = frame[n].map.width / frame[n].map.w;
  yinc = frame[n].map.height / frame[n].map.h;
  xc = 0.5 * frame[n].map.w;
  yc = 0.5 * frame[n].map.h;
  /*  The window in not created yet, do it now  */
  if (state.showstar == NULL) {
    state.showstar = create_form_Star();
    fl_set_object_shortcut(state.showstar->acceptstarW, "^m", 1);
    fl_set_object_shortcut(state.showstar->okstarW, "^m", 1);
  }
  if (ok) {
    fl_hide_object(state.showstar->acceptstarW);
    fl_hide_object(state.showstar->rejectstarW);
    fl_show_object(state.showstar->okstarW);
    fl_deactivate_object(state.showstar->objstarW);
  }
  else {
    fl_show_object(state.showstar->acceptstarW);
    fl_show_object(state.showstar->rejectstarW);
    fl_hide_object(state.showstar->okstarW);
    fl_activate_object(state.showstar->objstarW);
  }
  fl_show_form(state.showstar->Star,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Clicked object");
  /*  Write object coordinates in the window  */
  sprintf(a, "x = %.2f", z->x);
  fl_set_object_label(state.showstar->xstarW, a);
  sprintf(a, "y = %.2f", z->y);
  fl_set_object_label(state.showstar->ystarW, a);
  sprintf(a, "v = %.0f", z->mag);
  fl_set_object_label(state.showstar->vstarW, a);
  if (frame[n].matched) {
    sprintf(a, "RA = %s", format_ra(z->ra));
    fl_set_object_label(state.showstar->rastarW, a);
    sprintf(a, "DEC = %s", format_dec(z->dec));
    fl_set_object_label(state.showstar->decstarW, a);
  }
  else if (frame[n].fitsfile.wcs) {
    fits_pix_to_world(z->x + 1, z->y + 1, 
		      frame[n].fitsfile.crval1, frame[n].fitsfile.crval2, 
		      frame[n].fitsfile.crpix1, frame[n].fitsfile.crpix2, 
		      frame[n].fitsfile.cdelt1, frame[n].fitsfile.cdelt2, 
		      frame[n].fitsfile.crot,
		      frame[n].fitsfile.ctype,
		      &ac, &dc, &err);
    sprintf(a, "RA = %s", format_ra(ac));
    fl_set_object_label(state.showstar->rastarW, a);
    sprintf(a, "DEC = %s", format_dec(dc));
    fl_set_object_label(state.showstar->decstarW, a);
  }
  else {
    fl_set_object_label(state.showstar->rastarW, " ");
    fl_set_object_label(state.showstar->decstarW, " ");
  }
  if (name != NULL) {
    fl_set_input(state.showstar->objstarW, name);
  }
  /*  Draw a small subimage centered around the chosen object  */
  /*=====================================================*/
  o = state.showstar->windstarW;
  win = FL_ObjWin(o);
  disp = state.disp;
  x = z->x;
  y = z->y;
  im = &(frame[n].fits);
  /*  Determine number of pixels in x and y direction  */
  zoom = 5;
  w = o->w / zoom;
  h = o->h / zoom;
  /*  Do the drawing  */
  yq = o->y;
  yt = y - h / 2;
  for (j = 0; j < h; j++) {
    xq = o->x;
    xt = x - w / 2; 
    for (i = 0; i < w; i++) {
      v = video_point(im, xt++, yt);
      XSetForeground(disp, gc, state.pixel[correct(frame, n, v)]);
      XFillRectangle(disp, win, gc, xq, yq, zoom, zoom);
      xq += zoom;
    }
    yq += zoom;
    yt++;
  }
  /*  Draw a cross  */
  xt = z->x - w / 2;
  xt = (z->x - xt + 0.5) * zoom + o->x;
  yt = z->y - h / 2;
  yt = (z->y - yt + 0.5) * zoom + o->y;
  w = o->w / 4;
  h = o->h / 4;
  fl_line(xt, o->y, xt, o->y + o->h - 1, FL_RED);
  fl_line(o->x, yt, o->x + o->w - 1, yt, FL_RED);

  out = 0;
  fl_deactivate_all_forms();
  fl_activate_form(state.showstar->Star);
  do {
    o = fl_check_forms();
    if (o == FL_EVENT) {
      fl_XNextEvent(&event);
    }
    else if (o == state.showstar->acceptstarW) {
      out = 2;
      strncpy(name, fl_get_input(state.showstar->objstarW), FLEN_VALUE);
    }
    else if (o == state.showstar->rejectstarW) {
      out = 1;
    }
    else if (o == state.showstar->okstarW) {
      out = 3;
    }
  } while(!out);
  fl_hide_form(state.showstar->Star);
  fl_activate_all_forms();
  return out - 1;
}



void
innercircleC(FL_OBJECT *obj, long val)

{
  int zoom = 5;
  FL_OBJECT *o;
  int n = state.control[state.control_num];
  char a[20];

  frame[n].fits.inner = fl_get_counter_value(obj);
  o = state.showcomet->windcometW;
  fl_set_counter_bounds(state.showcomet->outercircleW, frame[n].fits.inner + 1.0, 50.0);
  cstar->mag = aperture(&frame[n].fits, cstar->x, cstar->y, 
	   frame[n].fits.inner, frame[n].fits.outer,
	   &cstar->x, &cstar->y);
  draw_object(o, cstar, zoom, &frame[n].fits, frame, n);
  sprintf(a, "x = %.2f", cstar->x);
  fl_set_object_label(state.showcomet->xcometW, a);
  sprintf(a, "y = %.2f", cstar->y);
  fl_set_object_label(state.showcomet->ycometW, a);
  sprintf(a, "v = %.0f", cstar->mag);
  fl_set_object_label(state.showcomet->vcometW, a);
}


void
outercircleC(FL_OBJECT *obj, long val)

{
  int zoom = 5;
  FL_OBJECT *o;
  int n = state.control[state.control_num];
  char a[20];

  frame[n].fits.outer = fl_get_counter_value(obj);
  o = state.showcomet->windcometW;
  fl_set_counter_bounds(state.showcomet->innercircleW, 0.1, frame[n].fits.outer - 1.0);
  cstar->mag = aperture(&frame[n].fits, cstar->x, cstar->y, 
	   frame[n].fits.inner, frame[n].fits.outer,
	   &cstar->x, &cstar->y);
  draw_object(o, cstar, zoom, &frame[n].fits, frame, n);
  sprintf(a, "x = %.2f", cstar->x);
  fl_set_object_label(state.showcomet->xcometW, a);
  sprintf(a, "y = %.2f", cstar->y);
  fl_set_object_label(state.showcomet->ycometW, a);
  sprintf(a, "v = %.0f", cstar->mag);
  fl_set_object_label(state.showcomet->vcometW, a);
}

/*  Show an object */
int
showcomet(BLINK_FRAME *frame, int n, STAR *z, int ok, char *name)

{
  double ac, dc;
  char a[25];
  int out, err = 0;
  XEvent event;
  FL_OBJECT *o;
  double xc, yc, xinc, yinc;

  int zoom = 5;
  FITS_IMAGE *im;

  cstar = z;
  xinc = frame[n].map.width / frame[n].map.w;
  yinc = frame[n].map.height / frame[n].map.h;
  xc = 0.5 * frame[n].map.w;
  yc = 0.5 * frame[n].map.h;
  /*  The window in not created yet, do it now  */
  if (state.showcomet == NULL) {
    state.showcomet = create_form_Comet();
    fl_set_object_shortcut(state.showcomet->acceptcometW, "^m", 1);
    fl_set_object_shortcut(state.showcomet->okcometW, "^m", 1);
  }
  if (ok) {
    fl_hide_object(state.showcomet->acceptcometW);
    fl_hide_object(state.showcomet->rejectcometW);
    fl_show_object(state.showcomet->okcometW);
    fl_deactivate_object(state.showcomet->objcometW);
  }
  else {
    fl_show_object(state.showcomet->acceptcometW);
    fl_show_object(state.showcomet->rejectcometW);
    fl_hide_object(state.showcomet->okcometW);
    fl_activate_object(state.showcomet->objcometW);
  }
  fl_set_counter_bounds(state.showcomet->innercircleW, 0.1, frame[n].fits.outer - 1.0);
  fl_set_counter_step(state.showcomet->innercircleW, 0.1, 1);
  fl_set_counter_bounds(state.showcomet->outercircleW, frame[n].fits.inner + 1.0, 15.0);
  fl_set_counter_step(state.showcomet->outercircleW, 0.1, 1);
  fl_set_counter_value(state.showcomet->innercircleW, frame[n].fits.inner);
  fl_set_counter_value(state.showcomet->outercircleW, frame[n].fits.outer);
  fl_show_form(state.showcomet->Comet,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Clicked object");
  /*  Write object coordinates in the window  */
  sprintf(a, "x = %.2f", z->x);
  fl_set_object_label(state.showcomet->xcometW, a);
  sprintf(a, "y = %.2f", z->y);
  fl_set_object_label(state.showcomet->ycometW, a);
  sprintf(a, "v = %.0f", z->mag);
  fl_set_object_label(state.showcomet->vcometW, a);
  if (frame[n].matched) {
    sprintf(a, "RA = %s", format_ra(z->ra));
    fl_set_object_label(state.showcomet->racometW, a);
    sprintf(a, "DEC = %s", format_dec(z->dec));
    fl_set_object_label(state.showcomet->deccometW, a);
  }
  else if (frame[n].fitsfile.wcs) {
    fits_pix_to_world(z->x + 1, z->y + 1, 
		      frame[n].fitsfile.crval1, frame[n].fitsfile.crval2, 
		      frame[n].fitsfile.crpix1, frame[n].fitsfile.crpix2, 
		      frame[n].fitsfile.cdelt1, frame[n].fitsfile.cdelt2, 
		      frame[n].fitsfile.crot,
		      frame[n].fitsfile.ctype,
		      &ac, &dc, &err);
    sprintf(a, "RA = %s", format_ra(ac));
    fl_set_object_label(state.showcomet->racometW, a);
    sprintf(a, "DEC = %s", format_dec(dc));
    fl_set_object_label(state.showcomet->deccometW, a);
  }
  else {
    fl_set_object_label(state.showcomet->racometW, " ");
    fl_set_object_label(state.showcomet->deccometW, " ");
  }
  if (name != NULL) {
    fl_set_input(state.showcomet->objcometW, name);
  }
  /*  Draw a small subimage centered around the chosen object  */
  /*=====================================================*/
  o = state.showcomet->windcometW;
  im = &(frame[n].fits);
  draw_object(o, z, zoom, im, frame, n);
  out = 0;
  fl_deactivate_all_forms();
  fl_activate_form(state.showcomet->Comet);
  do {
    o = fl_check_forms();
    if (o == FL_EVENT) {
      fl_XNextEvent(&event);
    }
    else if (o == state.showcomet->acceptcometW) {
      out = 2;
      strncpy(name, fl_get_input(state.showcomet->objcometW), 80);
    }
    else if (o == state.showcomet->rejectcometW) {
      out = 1;
    }
    else if (o == state.showcomet->okcometW) {
      out = 3;
    }
  } while(!out);
  fl_hide_form(state.showcomet->Comet);
  fl_activate_all_forms();
  return out - 1;
}

