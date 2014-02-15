/*  astrometry.c  */
/*  part of the fitsblink program  */
/*  routines for calling of astrometry related functions */
/*  Jure Skvarc                              */
#include <stdio.h> 
#include <stdlib.h>
#include "forms.h"

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;

void 
astrometryC(FL_OBJECT *obj, long val) 

{
  int m;
  int x, y, z;

  m = fl_get_menu(obj);
  switch (m) {
  case EDIT_PARAMETERS:
    edit_parameters(obj, m);
    break;
  case DETECT_STARS:
    detect_stars(frame, state.control[state.control_num]);
    break;
  case MATCH_STARS:
    match_stars(frame, state.control[state.control_num]);
    break;
  case SAVE_STAR_LIST:
    star_list(frame, state.control[state.control_num]);
    break;
  case LOAD_STAR_LIST:
    load_star_list(frame, state.control[state.control_num]);
    break;
  case DO_ASTROMETRY:
    state.doastr = 1;
    fl_get_mouse(&x, &y, &z);
    fl_set_mouse(0, 0);
    fl_set_mouse(x, y);
    break;
  case OPTIONS:
    edit_options(obj, m);
    break;
  case CATALOGS:
    edit_catalogs(obj, m);
    break;
  case SAVE_SETTINGS:
    show_settings(obj, m);
    break;
  case SEND_MAIL:
    edit_mail(obj, m);
    break;
  case SHOW_REPORT:
    show_report();
    break;
  }
}

