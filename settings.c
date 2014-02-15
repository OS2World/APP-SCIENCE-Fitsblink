/*  settings.c  */
/*  part of the fitsblink program  */
/*  routines for loading and saving of some settings  */
/*  Jure Skvarc, May 1998                   */
#define __USE_SVID 1
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <forms.h>

#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern CATALOG catalog[NUMBER_OF_CATALOGS];
extern OBSERVATORY observatory;
extern TELESCOPE telescope;
extern MAIL mail;
extern BLINK_FRAME defvals;

/*  Save some default values used by Fitsblink  */
int
save_settings(void)

{
  char *home;
  char *rcpath;
  char *rcname = "/.fitsblinkrc";
  FILE *fp;

  /*  Get home directory  */
  home = getenv("HOME");
  /*  Reserve space for full name  */
  rcpath = (char *) myalloc(sizeof(char) * (strlen(home) + strlen(rcname) + 1),
			    "save_settings", "rcpath");
  strcpy(rcpath, home);
  strcat(rcpath, rcname);
  if ((fp = fopen(rcpath, "w")) == NULL) {
    fl_show_alert("Can't open parameter file", "", "", 1);
    return 1;
  }
  fprintf(fp, "USNO_PATH %s\n", catalog[CAT_USNO].path);
  /*  fprintf(fp, "PPM_PATH %s\n", catalog[CAT_PPM].path); */
  fprintf(fp, "GUIDE_NORTH_PATH %s\n", catalog[CAT_GUIDE].path);
  fprintf(fp, "GUIDE_SOUTH_PATH %s\n", catalog[CAT_GUIDE_SOUTH].path);
  fprintf(fp, "OBSERVATORY %d\n", observatory.code);
  fprintf(fp, "OBSERVER %s\n", observatory.observer);
  fprintf(fp, "TELESCOPE %s\n", observatory.telescope);
  fprintf(fp, "INSTRUMENT %s\n", observatory.instrument);
  fprintf(fp, "LONGITUDE %.6f\n", observatory.longitude);
  fprintf(fp, "LATITUDE %.6f\n", observatory.latitude);
  fprintf(fp, "LOGFILE %s\n", state.logfile);
  fprintf(fp, "TO_ADDRESS %s\n", mail.to);
  fprintf(fp, "CC_ADDRESS %s\n", mail.cc);
  fprintf(fp, "MAIL_TITLE %s\n", mail.title);
  fprintf(fp, "FILE_TO_MAIL %s\n", mail.file);
  fprintf(fp, "DISPLAY_COLOR %d\n", (fl_get_button(state.set_display->color_setW)) ? 1 : 0);
  fprintf(fp, "DISPLAY_INVERSE %d\n", (fl_get_button(state.set_display->inverse_setW)) ? 1 : 0);
  fprintf(fp, "DISPLAY_DELAY %.1f\n", fl_get_counter_value(state.set_display->set_deltatW));
  fprintf(fp, "DISPLAY_AUTOLEVEL_HIGH %.3f\n", state.autolevel_high);
  fprintf(fp, "DISPLAY_WIDTH %d\n", state.blinkwidth);
  fprintf(fp, "DISPLAY_HEIGHT %d\n", state.blinkheight);
  fprintf(fp, "TEL_START %s\n", telescope.start);
  fprintf(fp, "TEL_POINT %s\n", telescope.point);
  fprintf(fp, "TEL_PARK %s\n", telescope.park);
  fprintf(fp, "TEL_WHERE %s\n", telescope.where);
  fprintf(fp, "TEL_ZERO %s\n", telescope.zero);
  fprintf(fp, "TEL_CENTER %s\n", telescope.center);
  fprintf(fp, "CCD_SETTEMP %s\n", telescope.settemp);
  fprintf(fp, "CCD_GETTEMP %s\n", telescope.gettemp);
  fprintf(fp, "CCD_EXPOSE %s\n", telescope.expose);
  fprintf(fp, "COUNT_SIGMA %.2f\n", defvals.fits.sg_num);
  fprintf(fp, "COUNT_MINBRIGHT %.2f\n", defvals.fits.minbright);
  fprintf(fp, "COUNT_STARSIZE %d\n", defvals.fits.minstar);
  fprintf(fp, "COUNT_GRIDSIZE %d\n", defvals.fits.maxsize);
  fprintf(fp, "COUNT_EDGE %d\n", defvals.fits.border);
  fprintf(fp, "COUNT_APERTURE %d\n", defvals.fits.aperture);
  fprintf(fp, "COUNT_INNER %.2f\n", defvals.fits.inner);
  fprintf(fp, "COUNT_OUTER %.2f\n", defvals.fits.outer);
  fprintf(fp, "MATCH_MIN %.1f\n", defvals.match.min);
  fprintf(fp, "MATCH_FIRSTBRIGHT %d\n", defvals.match.firstbright);
  fprintf(fp, "MATCH_NSTAR %d\n", defvals.match.nstar);
  fprintf(fp, "MATCH_POSERRMAX %.1f\n", defvals.match.poserrmax);
  fprintf(fp, "MATCH_MAXRES %.1f\n", defvals.match.maxres);
  fprintf(fp, "MAP_MINMAG %.1f\n", defvals.map.minmag);
  fprintf(fp, "MAP_MAXMAG %.1f\n", defvals.map.maxmag);
  if (state.wwwbrowser) fprintf(fp, "BROWSER %s\n", state.wwwbrowser);
  if (state.helpfile) fprintf(fp, "HELPFILE %s\n", state.helpfile);
  if (fclose(fp) != 0) {
    fl_show_alert("Error in closing parameter file", "", "", 1);
  }
  free(rcpath);
  return 0;
}


/*  Load some default values used by Fitsblink  */
int
load_settings(void)

{
  char *home;
  char t[256], *p, *r;
  char *rcpath;
  char *rcname = "/.fitsblinkrc";
  FILE *fp;

  /*  Get home directory  */
  home = getenv("HOME");
  /*  Reserve space for full name  */
  rcpath = (char *) myalloc(sizeof(char) * (strlen(home) + strlen(rcname) + 1),
			    "load_settings", "rcpath");
  strcpy(rcpath, home);
  strcat(rcpath, rcname);
  if ((fp = fopen(rcpath, "r")) == NULL) {
    fl_show_alert("Can't open parameter file", "", "", 1);
    return 1;
  }
  while (!feof(fp)) {
    /*  Read one line */
    if (!fgets(t, 256, fp)) break;
    p = t;
    p = strtok(p, " \t\n");
    if (!p) continue;
    if (!strcmp("USNO_PATH", p)) {
      r = strtok(NULL, " \t");
      if (r) {
	trim_spaces(r);
	strncpy(catalog[CAT_USNO].path, r, PATHLENGTH);
      }
    }
    else if (!strcmp("PPM_PATH", p)) {
      r = strtok(NULL, " \t");
      if (r) {
	trim_spaces(r);
	strcpy(catalog[CAT_PPM].path, r);
      }
    }
    else if (!strcmp("GUIDE_NORTH_PATH", p)) {
      r = strtok(NULL, " \t");
      if (r) {
	trim_spaces(r);
	strcpy(catalog[CAT_GUIDE].path, r);
      }
    }
    else if (!strcmp("GUIDE_SOUTH_PATH", p)) {
      r = strtok(NULL, " \t");
      if (r) {
	trim_spaces(r);
	strcpy(catalog[CAT_GUIDE_SOUTH].path, r);
      }
    }
    else if (!strcmp("OBSERVATORY", p)) {
      r = strtok(NULL, " \t");
      if (r) {
	sscanf(r, "%d", &observatory.code);
      }
    }
    else if (!strcmp("OBSERVER", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(observatory.observer, r, CARDLENGTH);
      }
    }
    else if (!strcmp("TELESCOPE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(observatory.telescope, r, CARDLENGTH);
      }
    }
    else if (!strcmp("INSTRUMENT", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(observatory.instrument, r, CARDLENGTH);
      }
    }
    else if (!strcmp("LONGITUDE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	observatory.longitude = strtod(r, NULL);
      }
    }
    else if (!strcmp("LATITUDE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	observatory.latitude = strtod(r, NULL);
      }
    }
    else if (!strcmp("LOGFILE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.logfile = strdup(r);
      }
    }
    else if (!strcmp("TO_ADDRESS", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	mail.to = strdup(r);
      }
    }
    else if (!strcmp("CC_ADDRESS", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	mail.cc = strdup(r);
      }
    }
    else if (!strcmp("MAIL_TITLE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	mail.title = strdup(r);
      }
    }
    else if (!strcmp("FILE_TO_MAIL", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	mail.file = strdup(r);
      }
    }
    else if (!strcmp("COLOR", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.color = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("DISPLAY_COLOR", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.color = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("INVERSE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.inverse = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("DISPLAY_INVERSE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.inverse = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("DELAY", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.delay = strtod(r, NULL);
      }
    }
    else if (!strcmp("DISPLAY_DELAY", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.delay = strtod(r, NULL);
      }
    }
    else if (!strcmp("DISPLAY_AUTOLEVEL_HIGH", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.autolevel_high = strtod(r, NULL);
      }
    }
    else if (!strcmp("DISPLAY_WIDTH", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.blinkwidth = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("DISPLAY_HEIGHT", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.blinkheight = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("TEL_START", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.start, r, 255);
      }
    }
    else if (!strcmp("TEL_POINT", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.point, r, 255);
      }
    }
    else if (!strcmp("TEL_WHERE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.where, r, 255);
      }
    }
    else if (!strcmp("TEL_ZERO", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.zero, r, 255);
      }
    }
    else if (!strcmp("TEL_CENTER", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.center, r, 255);
      }
    }
    else if (!strcmp("TEL_PARK", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.park, r, 255);
      }
    }
    else if (!strcmp("CCD_GETTEMP", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.gettemp, r, 255);
      }
    }
    else if (!strcmp("CCD_SETTEMP", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.settemp, r, 255);
      }
    }
    else if (!strcmp("CCD_EXPOSE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	strncpy(telescope.expose, r, 255);
      }
    }
    else if (!strcmp("BROWSER", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.wwwbrowser = strdup(r);
      }
    }
    else if (!strcmp("HELPFILE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	state.helpfile = strdup(r);
      }
    }
    else if (!strcmp("COUNT_SIGMA", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.sg_num = strtod(r, NULL);
      }
    }
    else if (!strcmp("COUNT_MINBRIGHT", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.minbright = strtod(r, NULL);
      }
    }
    else if (!strcmp("COUNT_STARSIZE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.minstar = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("COUNT_GRIDSIZE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.maxsize = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("COUNT_EDGE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.border = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("COUNT_APERTURE", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.aperture = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("COUNT_INNER", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.inner = strtod(r, NULL);
      }
    }
    else if (!strcmp("COUNT_OUTER", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.fits.outer = strtod(r, NULL);
      }
    }
    else if (!strcmp("MATCH_MIN", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.match.min = strtod(r, NULL);
      }
    }
    else if (!strcmp("MATCH_FIRSTBRIGHT", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.match.firstbright = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("MATCH_NSTAR", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.match.nstar = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("MATCH_POSERRMAX", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.match.poserrmax = strtol(r, NULL, 10);
      }
    }
    else if (!strcmp("MATCH_MAXRES", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.match.maxres = strtod(r, NULL);
      }
    }
    else if (!strcmp("MAP_MINMAG", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.map.minmag = strtod(r, NULL);
      }
    }
    else if (!strcmp("MAP_MAXMAG", p)) {
      r = strtok(NULL, "\n");
      if (r) {
	trim_spaces(r);
	defvals.map.maxmag = strtod(r, NULL);
      }
    }
    else {
      trim_spaces(p);
      if (strlen(p) > 0) fprintf(stderr, "Unknown parameter %s ignored.\n", p);
    }
  }
  return 0;   
}             
              

/*  Catalog paths  */
/*******************/
void 
usno_path_setC(FL_OBJECT *obj, long val)

{
  usnodirC(obj, val);
}

void 
ppm_path_setC(FL_OBJECT *obj, long val)

{
  ppmdirC(obj, val);
}

void 
gsc_north_path_setC(FL_OBJECT *obj, long val)

{
  guidedirC(obj, val);
}

void 
gsc_south_path_setC(FL_OBJECT *obj, long val)

{
  guidedirsouthC(obj, val);
}


/*  Observatory data  */
/**********************/
void 
telescope_setC(FL_OBJECT *obj, long val)

{
  strncpy(observatory.telescope, fl_get_input(obj), CARDLENGTH - 1);
}

void 
observatory_setC(FL_OBJECT *obj, long val)

{
  int c;
  char a[10];

  c = strtol(fl_get_input(obj), NULL, 10);
  sprintf(a, "%d", c);
  fl_set_input(obj, a);
  observatory.code = c;
}

void 
observer_setC(FL_OBJECT *obj, long val)

{
  strncpy(observatory.observer, fl_get_input(obj), CARDLENGTH - 1);
}

void 
instrument_setC(FL_OBJECT *obj, long val)

{
  strncpy(observatory.instrument, fl_get_input(obj), CARDLENGTH - 1);
}

void 
observatory_longitudeC(FL_OBJECT *obj, long val)

{
  char a[15];

  observatory.longitude = strtod(fl_get_input(obj), NULL);
  sprintf(a, "%.6f", observatory.longitude);
  fl_set_input(obj, a);
}

void 
observatory_latitudeC(FL_OBJECT *obj, long val)

{
  char a[15];

  observatory.latitude = strtod(fl_get_input(obj), NULL);
  sprintf(a, "%.6f", observatory.latitude);
  fl_set_input(obj, a);
}

/*=================*/
/*  Mail defaults  */
/*=================*/
void 
to_setC(FL_OBJECT *obj, long val)

{
  toaddressC(obj, val);
}

void 
cc_setC(FL_OBJECT *obj, long val)

{
  ccaddressC(obj, val);
}

void 
subject_setC(FL_OBJECT *obj, long val)

{
  mailtitleC(obj, val);
}

void 
file_setC(FL_OBJECT *obj, long val)

{
  mailfileC(obj, val);
}

/*====================*/
/*  Display settings  */
/*====================*/
void 
color_setC(FL_OBJECT *obj, long val){}

void 
inverse_setC(FL_OBJECT *obj, long val){}

void 
set_autolevel_highC(FL_OBJECT *obj, long val)

{
  float autol;

  autol = strtod(fl_get_input(obj), NULL);
  if (autol >= 0.02 || autol < 0.003) {
    fl_show_message("Suggestested values for this parameter are between",
		  "0.003 and 0.02.",
		  "");
  }
  state.autolevel_high = autol;
}

void 
blinkerwidthC(FL_OBJECT *obj, long val){}

void 
blinkerheightC(FL_OBJECT *obj, long val){}

/*==============================================*/
/*  Commands for camera and telescope control   */
/*==============================================*/

void
comstartC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.start, fl_get_input(obj));
}

void
compointC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.point, fl_get_input(obj));
}

void
comparkC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.park, fl_get_input(obj));
}

void
comwhereC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.where, fl_get_input(obj));
}

void
comzeroC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.zero, fl_get_input(obj));
}

void
comcenterC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.center, fl_get_input(obj));
}

void
comgettempC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.gettemp, fl_get_input(obj));
}

void
comsettempC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.settemp, fl_get_input(obj));
}

void
comexposeC(FL_OBJECT *obj, long val)

{
  strcpy(telescope.expose, fl_get_input(obj));
}

/*==============================*/
/*  Star detection parameters   */
/*==============================*/

void
set_sg_numC(FL_OBJECT *obj, long val)

{
  defvals.fits.sg_num = proc_sg_num(obj, defvals.fits.sg_num);
}

void
set_minbrightC(FL_OBJECT *obj, long val)

{
  defvals.fits.minbright = proc_minbright(obj, defvals.fits.minbright);
}

void
set_minstarC(FL_OBJECT *obj, long val)

{
  defvals.fits.minstar = proc_minstar(obj, defvals.fits.minstar);
}

void
set_gridsizeC(FL_OBJECT *obj, long val)

{
  defvals.fits.maxsize = proc_gridsize(obj, defvals.fits.maxsize);
}

void
set_borderC(FL_OBJECT *obj, long val)

{
  defvals.fits.border = proc_border(obj, defvals.fits.border);
}

void
set_apertureC(FL_OBJECT *obj, long val)

{
  defvals.fits.aperture = fl_get_button(obj);
}

void
set_innerC(FL_OBJECT *obj, long val)

{
  defvals.fits.inner = proc_inner(obj, defvals.fits.inner, defvals.fits.outer);
}

void
set_outerC(FL_OBJECT *obj, long val)

{
  defvals.fits.outer = proc_outer(obj, defvals.fits.outer, defvals.fits.inner);
}

/*===================*/
/* Catalog matching  */
/*===================*/
void
set_minconstC(FL_OBJECT *obj, long val)

{
  defvals.match.min = proc_min(obj, defvals.match.min, defvals.match.nstar);
}

void
set_initconstC(FL_OBJECT *obj, long val)

{
  defvals.match.firstbright = proc_firstbright(obj, defvals.match.firstbright);
}

void
set_starconstC(FL_OBJECT *obj, long val)

{
  defvals.match.nstar = proc_nstar(obj, defvals.match.nstar);
}


void
set_starerrorC(FL_OBJECT *obj, long val)

{
  defvals.match.poserrmax = proc_poserrmax(obj, defvals.match.poserrmax);
}


void
set_maxresC(FL_OBJECT *obj, long val)

{
  defvals.match.maxres = proc_maxres(obj, defvals.match.maxres);
}

void
set_minmagC(FL_OBJECT *obj, long val)

{
  defvals.map.minmag = proc_minmag(obj, defvals.map.minmag, defvals.map.maxmag);
}

void
set_maxmagC(FL_OBJECT *obj, long val)

{
  defvals.map.maxmag = proc_maxmag(obj, defvals.map.maxmag, defvals.map.minmag);
}



/***************************/
/*  WWW browser path       */
/***************************/
void
setbrowserC(FL_OBJECT *obj, long val)

{
  if (state.wwwbrowser) free(state.wwwbrowser);
  state.wwwbrowser = strdup((char *)fl_get_input(obj));
}

void
sethelpfileC(FL_OBJECT *obj, long val)

{
  if (state.helpfile) free(state.helpfile);
  state.helpfile = strdup((char *)fl_get_input(obj));
}



void
show_settings(FL_OBJECT *obj, int m)

{
  char a[10];
  int out;
  XEvent event;
  FL_OBJECT *o;

  /*  The window in not created yet, do it now  */
  if (state.settings == NULL) {
    state.settings = create_form_Settings();
    state.set_catalogs = create_form_Set_catalogs();
    state.set_observatory = create_form_Set_observatory();
    state.set_display = create_form_Set_display();
    state.set_mail = create_form_Set_mail();
    state.set_help = create_form_Set_help();
    state.set_commands = create_form_Commands();
    state.set_ccd = create_form_Set_ccd();
    state.set_count = create_form_Set_count();
    state.set_match = create_form_Set_match();
    fl_addto_tabfolder(state.settings->setfolderW, "Catalogs", 
		       state.set_catalogs->Set_catalogs);
    fl_addto_tabfolder(state.settings->setfolderW, "Observatory", 
		       state.set_observatory->Set_observatory);
    fl_addto_tabfolder(state.settings->setfolderW, "Display", 
		       state.set_display->Set_display);
    fl_addto_tabfolder(state.settings->setfolderW, "Mail", 
		       state.set_mail->Set_mail);
    fl_addto_tabfolder(state.settings->setfolderW, "Telescope", 
		       state.set_commands->Commands);
    fl_addto_tabfolder(state.settings->setfolderW, "Camera", 
		       state.set_ccd->Set_ccd);
    fl_addto_tabfolder(state.settings->setfolderW, "Count", 
		       state.set_count->Set_count);
    fl_addto_tabfolder(state.settings->setfolderW, "Match", 
		       state.set_match->Set_match);
    fl_addto_tabfolder(state.settings->setfolderW, "Help", 
		       state.set_help->Set_help);
    fl_set_counter_bounds(state.set_display->set_deltatW, 0.0, 5.0);
    fl_set_counter_value(state.set_display->set_deltatW,  
			 fl_get_counter_value(state.blinker->deltatW));
    fl_set_counter_step(state.set_display->set_deltatW, 0.1, 0.1);
  }
  fl_show_form(state.settings->Settings,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Image options");
  /*==============*/
  /*  Catalogs    */
  /*==============*/
  fl_set_input(state.set_catalogs->usno_pathW, catalog[CAT_USNO].path);
  fl_set_input(state.set_catalogs->ppm_pathW, catalog[CAT_PPM].path);
  fl_hide_object(state.set_catalogs->ppm_pathW);
  fl_set_input(state.set_catalogs->gsc_north_pathW, catalog[CAT_GUIDE].path);
  fl_set_input(state.set_catalogs->gsc_south_pathW, catalog[CAT_GUIDE_SOUTH].path);
  /*==============*/
  /*  Observatory */
  /*==============*/
  fl_set_input(state.set_observatory->telescopeW, observatory.telescope);
  sprintf(a, "%d", observatory.code);
  fl_set_input(state.set_observatory->observatoryW, a);
  fl_set_input(state.set_observatory->observerW, observatory.observer);
  fl_set_input(state.set_observatory->instrumentW, observatory.instrument);
  sprintf(a, "%.6f", observatory.longitude);
  fl_set_input(state.set_observatory->observatory_longitudeW, a);
  sprintf(a, "%.6f", observatory.latitude);
  fl_set_input(state.set_observatory->observatory_latitudeW, a);
  /*==============*/
  /*  Mail        */
  /*==============*/
  fl_set_input(state.set_mail->toW, mail.to);
  fl_set_input(state.set_mail->ccW, mail.cc);
  fl_set_input(state.set_mail->subjectW, mail.title);
  fl_set_input(state.set_mail->fileW, mail.file);
  /*==============*/
  /*  Display     */
  /*==============*/
  fl_set_button(state.set_display->color_setW, state.color);
  fl_set_button(state.set_display->inverse_setW, state.inverse);
  fl_set_button(state.set_display->set_deltatW, state.delay);
  sprintf(a, "%.3f", state.autolevel_high);
  fl_set_input(state.set_display->set_autolevel_highW, a);
  sprintf(a, "%d", state.blinkwidth);
  fl_set_input(state.set_display->blinkerwidthW, a);
  fl_deactivate_object(state.set_display->blinkerwidthW);
  sprintf(a, "%d", state.blinkheight);
  fl_set_input(state.set_display->blinkerheightW, a);
  fl_deactivate_object(state.set_display->blinkerheightW);
  /*========================*/
  /*  Telescope and camera  */
  /*========================*/
  fl_set_input(state.set_commands->comstartW, telescope.start);
  fl_set_input(state.set_commands->comparkW, telescope.park);
  fl_set_input(state.set_commands->compointW, telescope.point);
  fl_set_input(state.set_commands->comwhereW, telescope.where);
  fl_set_input(state.set_commands->comzeroW, telescope.zero);
  fl_set_input(state.set_commands->comcenterW, telescope.center);
  fl_set_input(state.set_ccd->comgettempW, telescope.gettemp);
  fl_set_input(state.set_ccd->comsettempW, telescope.settemp);
  fl_set_input(state.set_ccd->comexposeW, telescope.expose);
  /*===================*/
  /*  Star detection   */
  /*===================*/
  sprintf(a, "%.1f", defvals.fits.sg_num);
  fl_set_input(state.set_count->set_sg_numW, a);
  sprintf(a, "%.0f", defvals.fits.minbright);
  fl_set_input(state.set_count->set_minbrightW, a);
  sprintf(a, "%d", defvals.fits.maxsize);
  fl_set_input(state.set_count->set_gridsizeW, a);
  sprintf(a, "%d", defvals.fits.minstar);
  fl_set_input(state.set_count->set_minstarW, a);
  sprintf(a, "%d", defvals.fits.border);
  fl_set_input(state.set_count->set_borderW, a);
  fl_set_button(state.set_count->set_apertureW, defvals.fits.aperture);
  sprintf(a, "%.1f", defvals.fits.inner);
  fl_set_input(state.set_count->set_innerW, a);
  sprintf(a, "%.1f", defvals.fits.outer);
  fl_set_input(state.set_count->set_outerW, a);
  /*===================*/
  /*  Catalog matching */
  /*===================*/
  sprintf(a, "%.1f", defvals.match.min);
  fl_set_input(state.set_match->set_minconstW, a);
  sprintf(a, "%d", defvals.match.firstbright);
  fl_set_input(state.set_match->set_initconstW, a);
  sprintf(a, "%d", defvals.match.nstar);
  fl_set_input(state.set_match->set_starconstW, a);
  sprintf(a, "%.1f", defvals.match.poserrmax);
  fl_set_input(state.set_match->set_starerrorW, a);
  sprintf(a, "%.1f", defvals.match.maxres);
  fl_set_input(state.set_match->set_maxresW, a);
  sprintf(a, "%.1f", defvals.map.minmag);
  fl_set_input(state.set_match->set_minmagW, a);
  sprintf(a, "%.1f", defvals.map.maxmag);
  fl_set_input(state.set_match->set_maxmagW, a);
  /*==============*/
  /*  Help        */
  /*==============*/
  if (state.wwwbrowser) fl_set_input(state.set_help->setbrowserW, state.wwwbrowser);
  if (state.helpfile) fl_set_input(state.set_help->sethelpfileW, state.helpfile);

  out = 0;
  do {
    o = fl_check_forms();
    if (o == FL_EVENT) {
      fl_XNextEvent(&event);
    }
    else if (o == state.settings->savesettingsW) {
      save_settings();
      out = 1;
    }
    else if (o == state.settings->closesettingsW) {
      out = 1;
    }
  } while(!out);
  fl_hide_form(state.settings->Settings);
}
