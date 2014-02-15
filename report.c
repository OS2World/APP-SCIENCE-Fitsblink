/*  report.c  */
/*  part of the fitsblink program  */
/*  routines for the output of astrometry data  */
/*  Jure Skvarc                                 */
#include <stdio.h> 
#include <forms.h>


#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;

void reportC(FL_OBJECT *obj, long val)

{
}


void savereportC(FL_OBJECT *obj, long val)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.rep";
  static char *name = "";
  FILE *fp;
  int i, n;

  fl_use_fselector(4);
  fl_invalidate_fselector_cache();
  fn = fl_show_fselector("Choose output file", curdir, pattern, name);
  if (fn == NULL) return;
  /*  Save the current directory  */
  curdir = (char *) fl_get_directory();
  /*  Save the current pattern */
  pattern = (char *) fl_get_pattern();
  name = (char *) fl_get_filename();
  if (!(fp = fopen(fn, "w"))) {
    fl_show_alert("Can not open file", name, "for writing!", 0);
    return;
  }
  n = fl_get_browser_maxline(state.report->reportW);
  for (i = 1; i <= n; i++) {
    fprintf(fp, "%s\n", fl_get_browser_line(state.report->reportW, i));
  }
  fclose(fp);
}


void
show_report(void)

{
  /*  The window in not created yet, do it now  */
  if (state.report == NULL) {
    state.report = create_form_Report();
    fl_set_form_minsize(state.report->Report, 
		  state.report->Report->w, state.report->Report->h);
    fl_set_browser_fontsize(state.report->reportW, 16);
    fl_set_browser_fontstyle(state.report->reportW, FL_FIXED_STYLE);
  }
  if (!fl_form_is_visible(state.report->Report)) {
    fl_show_form(state.report->Report,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Astrometry report");
  }
}


void
hide_report(void)

{
  fl_hide_form(state.report->Report);
}

