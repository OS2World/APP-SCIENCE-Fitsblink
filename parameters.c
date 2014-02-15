/*  parameters.c  */
/*  part of the fitsblink program  */
/*  routines for input of image parameters  */
/*  Jure Skvarc, May 1998                   */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <forms.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;



/*  Write right ascension is in HHhMMmSS.SSs format to the input field */
/*  Input ra is in decimal degrees  */
void
set_rainput(FL_OBJECT *obj, double ra)

{
  char a[20];
  int h, m;
  float s;

  ra /= 15.0;
  h = (int) ra;
  ra -= h;
  ra *= 60.0;
  m = (int) (ra + 1e-9);
  s = (ra - m + 1e-9) * 60;
  sprintf(a, "%dh%02dm%05.2fs", h, m, s);
  fl_set_input(obj, a);
}

/*  Accept right ascension of the centre pixel in format hh:mm:ss.ss  */
void 
rainputC(FL_OBJECT *obj, long val)

{
  char *r;
  int n;

  r = (char *) fl_get_input(obj);
  n = state.control[state.control_num];
  frame[n].ra = parse_ra(r);
  frame[n].fitsfile.crval1 = frame[n].ra;
  set_rainput(obj, frame[n].ra);
}


/*  Write declination is in +DDdMMmSS.Ss format to the input field */
/*  Input dec is in decimal degrees  */
void
set_decinput(FL_OBJECT *obj, double dec)

{
  char a[20];
  int d, m, c;
  float s;
  
  if (dec < 0) {
    c = '-';
    dec = -dec;
  }
  else {
    c = '+';
  }
  d = (int) dec;
  dec -= d;
  dec *= 60.0;
  m = (int) (dec + 1e-9);
  s = (dec - m + 1e-9) * 60;
  sprintf(a, "%c%dd%02d'%05.2f\"", c, d, m, s);
  fl_set_input(obj, a);
}


/*  Accept declination of the centre pixel in format +dd:mm:ss.s */

void
decinputC(FL_OBJECT *obj, long val)

{
  char *r;
  int n;

  r = (char *) fl_get_input(obj);
  n = state.control[state.control_num];
  frame[n].dec = parse_dec(r);
  frame[n].fitsfile.crval2 = frame[n].dec;
  set_decinput(obj, frame[n].dec);
}


/*  Write a double value with a format format to the input field */
void
set_doubleval(FL_OBJECT *obj, char *format, double val)

{
  char temp[20];
  sprintf(temp, format, val);
  fl_set_input(obj, temp);
}

/*  Set pixel size in x direction  */
void
xpixelC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].xsize = strtod(fl_get_input(obj), NULL);
  frame[n].fitsfile.cdelt1 = frame[n].xsize / 3600.0;
  set_doubleval(obj, "%.3f", frame[n].xsize);
}


/*  Set pixel size in y direction  */
void
ypixelC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].ysize = strtod(fl_get_input(obj), NULL);
  frame[n].fitsfile.cdelt2 = frame[n].ysize / 3600.0;
  set_doubleval(obj, "%.3f", frame[n].ysize);
}


/*  Set rotation angle (in decimal degrees)  */
void
rotationC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fi = fmod(strtod(fl_get_input(obj), NULL), 360);
  frame[n].fitsfile.crot = frame[n].fi;
  set_doubleval(obj, "%.3f", frame[n].fi);
}


/*  Write year, month, day, hour or minute to the input field */
void
set_intval(FL_OBJECT *obj, int val)

{
  char temp[20];
  sprintf(temp, "%d", val);
  fl_set_input(obj, temp);
}

/*  Set year  */
void
timeyearC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].year = strtol(fl_get_input(obj), NULL, 10);
  set_intval(obj, frame[n].year);
}

/*  Set month  */
void
timemonthC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].month = strtol(fl_get_input(obj), NULL, 10);
  set_intval(obj, frame[n].month);
}

/*  Write a day in a decimal format */
void
write_decimal_day(BLINK_FRAME *frame, int n)

{
  double decday = frame[n].day + (frame[n].hour + (frame[n].minute +
					    frame[n].second / 60.0) / 60.0) / 24.0; 
  set_doubleval(state.parameters->timedecdayW, "%10.7f", decday);
}

/*  Set day */
void
timedayC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].day = strtol(fl_get_input(obj), NULL, 10);
  set_intval(obj, frame[n].day);
  write_decimal_day(frame, n);
}

/* Set hour  */
void
timehourC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].hour = strtol(fl_get_input(obj), NULL, 10);
  set_intval(obj, frame[n].hour);
  write_decimal_day(frame, n);
}

/* Set minute  */
void 
timeminuteC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].minute = strtol(fl_get_input(obj), NULL, 10);
  set_intval(obj, frame[n].minute);
  write_decimal_day(frame, n);
}


void
timesecondC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].second = strtod(fl_get_input(obj), NULL);
  set_doubleval(obj, "%6.3f", frame[n].second);
  write_decimal_day(frame, n);
}


void
write_timedecday(BLINK_FRAME *frame, int n)

{
  write_decimal_day(frame, n);
  set_intval(state.parameters->timedayW, frame[n].day);
  set_intval(state.parameters->timehourW, frame[n].hour);
  set_intval(state.parameters->timeminuteW, frame[n].minute);
  set_doubleval(state.parameters->timesecondW, "%6.3f", frame[n].second);
}


/*  Set time in decimal days  */
void
timedecdayC(FL_OBJECT *obj, long val)

{
  double decday;

  int n = state.control[state.control_num];
  decday = fmod(fabs(strtod(fl_get_input(obj), NULL) - 1), 31.0) + 1.0;
  /*  Extract separate days. hours, minutes and seconds from decimal days */
  frame[n].day = decday;
  decday -= frame[n].day;
  decday *= 24.0;
  frame[n].hour = decday;
  decday -= frame[n].hour;
  decday *= 60.0;
  frame[n].minute = decday;
  decday -= frame[n].minute;
  frame[n].second = 60.0 * decday; 
  write_timedecday(frame, n);
}


/*   Object name  */
void
objectC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  strcpy(frame[n].fitsfile.object, fl_get_input(obj));
}

/*  Observer name  */
void
observerC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  strcpy(frame[n].fitsfile.observer, fl_get_input(obj));
}

/*  Telescope  */
void
telescopeC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  strcpy(frame[n].fitsfile.telescope, fl_get_input(obj));
}

/*  Instrument  */
void
instrumentC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  strcpy(frame[n].fitsfile.instrument, fl_get_input(obj));
}




/*  Edit image parameters */
void
edit_parameters(FL_OBJECT *obj, int m)

{
  int n;

  /*  The window in not created yet, do it now  */
  if (state.parameters == NULL) {
    state.parameters = create_form_Parameters();
    fl_set_form_minsize(state.parameters->Parameters, 
		  state.parameters->Parameters->w, state.parameters->Parameters->h);
  }
  /*  Deactivate PARAMETERS item in the menu */
  fl_set_menu_item_mode(obj, m, FL_PUP_GREY);
  if (!fl_form_is_visible(state.parameters->Parameters)) {
    fl_show_form(state.parameters->Parameters,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Image parameters");
  }
  /*  Write current parameter values into the form */
  n = state.control[state.control_num];
  set_rainput(state.parameters->rainputW, frame[n].ra);
  set_decinput(state.parameters->decinputW, frame[n].dec);
  set_doubleval(state.parameters->xpixelW, "%.3f", frame[n].xsize);
  set_doubleval(state.parameters->ypixelW, "%.3f", frame[n].ysize);
  set_doubleval(state.parameters->rotationW, "%.3f", frame[n].fi);
  write_timedecday(frame, n);
  set_intval(state.parameters->timeyearW, frame[n].year);
  set_intval(state.parameters->timemonthW, frame[n].month);
  fl_set_input(state.parameters->objectW, frame[n].fitsfile.object);
  fl_set_input(state.parameters->observerW, frame[n].fitsfile.observer);
  fl_set_input(state.parameters->telescopeW, frame[n].fitsfile.telescope);
  fl_set_input(state.parameters->instrumentW, frame[n].fitsfile.instrument);
  strcpy(frame[n].fitsfile.ctype, "-TAN");
  frame[n].fitsfile.wcs = 1;
}
