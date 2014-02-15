/*  error.c for Fitsblink 1.2  */
/* User interface */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

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


void
show_errors(int errnum, char **errors) 

{
  int i;

  if (state.error == NULL) {
    state.error = create_form_Error();
  }
  /*  Deactivate all forms */
  fl_deactivate_all_forms();
  /*  Show the form  */
  fl_show_form(state.error->Error, FL_PLACE_FREE_CENTER, FL_FULLBORDER, "");
  for (i = 0; i < errnum; i++) {
    fl_addto_browser(state.error->errorW, errors[i]);
  }
  fl_do_forms();
  fl_clear_browser(state.error->errorW);
  fl_hide_form(state.error->Error);
  fl_activate_all_forms();
}
