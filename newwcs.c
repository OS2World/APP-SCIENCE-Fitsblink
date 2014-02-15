/*  newwcs.c  */
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
extern TRANSROT trglob;

/*  Calculate  new WCS values */
void
cal_new_wcs(TRANSROT tr, double ra, double dec, double fi1, 
	    double xc, double yc, double xinc, double yinc, 
	    double *ac, double *dc, double *nw, double *nh, double *fi)

{
  double xc2, yc2, x1, y1, x2, y2;
  double ax1, ax2, dx1, dx2, ay1, ay2, dy1, dy2;
  double ax, ay, bx, by;
  int err = 0;

  /*  Calculate new reference (center) coordinates into *ac, dc */
  x1 = xc2 = tr.a * (xc - 1.0) + tr.b * (yc - 1.0) + tr.c;
  y1 = yc2 = tr.d * (xc - 1.0) + tr.e * (yc - 1.0) + tr.f;
  fits_pix_to_world(xc2 + 1, yc2 + 1, ra, dec, xc, yc, xinc, yinc, 
		    fi1, "-TAN",
		    ac, dc, &err);
  xc2 = tr.a * (xc - 1.5) + tr.b * (yc - 1.0) + tr.c;
  yc2 = tr.d * (xc - 1.5) + tr.e * (yc - 1.0) + tr.f;
  fits_pix_to_world(xc2 + 1, yc2 + 1, ra, dec, 
		    xc, yc, xinc, yinc, 
		    fi1, "-TAN",
		    &ax1, &dx1, &err);
  x2 = xc2 = tr.a * (xc - 0.5) + tr.b * (yc - 1.0) + tr.c;
  y2 = yc2 = tr.d * (xc - 0.5) + tr.e * (yc - 1.0) + tr.f;
  fits_pix_to_world(xc2 + 1, yc2 + 1, ra, dec, 
		    xc, yc, xinc, yinc, 
		    fi1, "-TAN",
		    &ax2, &dx2, &err);
  xc2 = tr.a * (xc - 1.0) + tr.b * (yc - 1.5) + tr.c;
  yc2 = tr.d * (xc - 1.0) + tr.e * (yc - 1.5) + tr.f;
  fits_pix_to_world(xc2 + 1, yc2 + 1, ra, dec, 
		    xc, yc, xinc, yinc, 
		    fi1, "-TAN",
		    &ay1, &dy1, &err);
  xc2 = tr.a * (xc - 1.0) + tr.b * (yc - 0.5) + tr.c;
  yc2 = tr.d * (xc - 1.0) + tr.e * (yc - 0.5) + tr.f;
  fits_pix_to_world(xc2 + 1, yc2 + 1, ra, dec, 
		    xc, yc, xinc, yinc, 
		    fi1, "-TAN",
		    &ay2, &dy2, &err);
  *nw = sqrt(fsqr((ax2 - ax1) * cos(*dc / 180 * PI)) + fsqr(dx2 - dx1));
  *nh = sqrt(fsqr((ay2 - ay1) * cos(*dc / 180 * PI)) + fsqr(dy2 - dy1));
  if (tr.a * tr.e - tr.b * tr.d < 0) *nh = -*nh;
  ax = x2 - x1;
  ay = y2 - y1;
  bx = cos(fi1 / 180.0 * PI);
  by = sin(fi1 / 180.0 * PI);
  *fi = 180.0 / PI * atan2(-ax * by + ay * bx, ax * bx + ay * by);
}


int
update_wcs_interactive(BLINK_FRAME *frame, int n, 
		       double ac, double dc, double xc, double yc,
		       double nw, double nh, double fi)

{
  fitsfile *fptr;
  int err = 0;

  /*  Open file for writing  */
  fits_open_file(&fptr, frame[n].name, READWRITE, &err);
  if (err != 0) {
    if (state.interactive) {
      char a[100];
      sprintf(a, "Can't open %s for writing!\n", frame[n].name);
      fl_show_alert(a, "Aborting!", "", 1);
      return 1;
    }
  }
  fits_update_key(fptr, TSTRING, "CTYPE1", "RA---TAN", "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CTYPE1 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRPIX1", &xc, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CRPIX1 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRVAL1", &ac, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CRVAL1 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CDELT1", &nw, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CDELT1 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CROTA1", &fi, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CROTA1 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TSTRING, "CTYPE2", "DEC--TAN", "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CTYPE2 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRPIX2", &yc, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CRPIX2 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRVAL2", &dc, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CRVAL2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CDELT2", &nh, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CDELT2 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CROTA2", &fi, "Inserted by fitsblink", &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not update keyword CROTA2 for file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  fits_close_file(fptr, &err);
  if (err != 0) {
    char a[100];
    sprintf(a, "Disaster: could not close file %s\n", frame[n].name);
    fl_show_alert(a, "Aborting!", "", 1);
    return 1;
  }
  return 0;
  
}

void
newwcs(BLINK_FRAME *frame, int n, TRANSROT tr)

{
  double ac, dc;
  double xc, yc, xinc, yinc;
  double nw, nh, fi;
  char a[25];
  int out;
  XEvent event;
  FL_OBJECT *o;

  xinc = frame[n].map.width / frame[n].map.w;
  yinc = frame[n].map.height / frame[n].map.h;
  xc = 0.5 * frame[n].map.w;
  yc = 0.5 * frame[n].map.h;
  /*  If the window in not created yet, do it now  */
  if (state.newwcs == NULL) {
    state.newwcs = create_form_Wcs();
    /*  Deactivate all input fields  */
    fl_deactivate_object(state.newwcs->crval1oldW);
    fl_deactivate_object(state.newwcs->crval2oldW);
    fl_deactivate_object(state.newwcs->crpix1oldW);
    fl_deactivate_object(state.newwcs->crpix2oldW);
    fl_deactivate_object(state.newwcs->cdelt1oldW);
    fl_deactivate_object(state.newwcs->cdelt2oldW);
    fl_deactivate_object(state.newwcs->crota2oldW);
    fl_deactivate_object(state.newwcs->crval1newW);
    fl_deactivate_object(state.newwcs->crval2newW);
    fl_deactivate_object(state.newwcs->crpix1newW);
    fl_deactivate_object(state.newwcs->crpix2newW);
    fl_deactivate_object(state.newwcs->cdelt1newW);
    fl_deactivate_object(state.newwcs->cdelt2newW);
    fl_deactivate_object(state.newwcs->crota2newW);
  }
  fl_show_form(state.newwcs->Wcs,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "WCS values");

  /*  Calculate new values of WCS keywords  */
  cal_new_wcs(tr, frame[n].map.ra, frame[n].map.dec, frame[n].map.fi, 
	      xc, yc, xinc, yinc, 
	      &ac, &dc, &nw, &nh, &fi);

  fl_set_input(state.newwcs->crval1oldW, format_ra(frame[n].fitsfile.crval1));
  fl_set_input(state.newwcs->crval2oldW, format_dec(frame[n].fitsfile.crval2));
  sprintf(a, "%7.2f", frame[n].fitsfile.crpix1);
  fl_set_input(state.newwcs->crpix1oldW, a);
  sprintf(a, "%7.2f", frame[n].fitsfile.crpix2);
  fl_set_input(state.newwcs->crpix2oldW, a);
  sprintf(a, "%7.3f", frame[n].xsize);
  fl_set_input(state.newwcs->cdelt1oldW, a);
  sprintf(a, "%7.3f", frame[n].ysize);
  fl_set_input(state.newwcs->cdelt2oldW, a);
  sprintf(a, "%7.2f", frame[n].fitsfile.crot);
  fl_set_input(state.newwcs->crota2oldW, a);

  fl_set_input(state.newwcs->crval1newW, format_ra(ac));
  fl_set_input(state.newwcs->crval2newW, format_dec(dc));
  sprintf(a, "%7.2f", xc);
  fl_set_input(state.newwcs->crpix1newW, a);
  sprintf(a, "%7.2f", yc);
  fl_set_input(state.newwcs->crpix2newW, a);
  sprintf(a, "%7.3f", nw * 3600.0);
  fl_set_input(state.newwcs->cdelt1newW, a);
  sprintf(a, "%7.3f", nh * 3600.0);
  fl_set_input(state.newwcs->cdelt2newW, a);
  sprintf(a, "%7.2f", fi);
  fl_set_input(state.newwcs->crota2newW, a);
  out = 0;
  fl_deactivate_all_forms();
  fl_activate_form(state.newwcs->Wcs);
  do {
    o = fl_check_forms();
    if (o == FL_EVENT) {
      fl_XNextEvent(&event);
    }
    else if (o == state.newwcs->wcsyesW) {
      frame[n].ra = frame[n].fitsfile.crval1 = ac;
      frame[n].dec = frame[n].fitsfile.crval2 = dc;
      frame[n].fitsfile.crpix1 = xc;
      frame[n].fitsfile.crpix2 = yc;
      frame[n].xsize = fabs((frame[n].fitsfile.cdelt1 = nw) * 3600.0);
      frame[n].ysize = fabs((frame[n].fitsfile.cdelt2 = nh) * 3600.0);
      frame[n].fi = frame[n].fitsfile.crot = fi;
      if (state.parameters &&
	  fl_form_is_visible(state.parameters->Parameters)) {
	set_rainput(state.parameters->rainputW, ac);
	set_decinput(state.parameters->decinputW, dc);
	set_doubleval(state.parameters->xpixelW, "%.3f", xc);
	set_doubleval(state.parameters->ypixelW, "%.3f", yc);
	set_doubleval(state.parameters->rotationW, "%.3f", fi);
      }
      out = 1;
    }
    else if (o == state.newwcs->writetofileW) {
      update_wcs_interactive(frame, n, ac, dc, xc, yc, nw, nh, fi);
      out = 1;
    }
    else if (o == state.newwcs->wcsnoW) {
      out = 1;
    }
  } while(!out);
  fl_hide_form(state.newwcs->Wcs);
  fl_activate_all_forms();
}
