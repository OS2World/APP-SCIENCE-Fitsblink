/*  telescope.c  */
/*  part of the fitsblink program  */
/*  Control panel for a telescope and CCD camera */
/*  Jure Skvarc                                   */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
 
#include <forms.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern TELESCOPE telescope;
extern OBSERVATORY observatory;
extern int daylight;
extern long int timezone;


/*=========================================================*/
/*  Execute a command by append string a to a tx command   */
/*=========================================================*/
void
execute(char *a)

{
  char *b;

  b = (char *) myalloc(sizeof(char) * (strlen(a) + 4), "execute", "b");
  strcpy(b, "tx ");
  strcat(b, a);
  fl_exe_command(b, 1);
  free(b);
}

/* Calculate altitude and azimuth from the given ra and dec and location */
void
equtoaltaz(TELESCOPE *tel, OBSERVATORY *obs)

{
  double sr, ha, fi;
  double jd;
  double sfi, cfi, del, cha;
  struct tm *gmt;
  time_t t;

  t = time(NULL);
  gmt = gmtime(&t);   /* Current gmt  */
  /*  calculate Julian day */
  jd = julian_day(1900 + gmt->tm_year, gmt->tm_mon + 1, (double) gmt->tm_mday
		  + gmt->tm_hour / 24.0 + gmt->tm_min / 1440.0 + 
		  gmt->tm_sec / 86400.0);
  /*  Calculate local hour angle for tel->ra  */
  sr = fmod(sidereal(jd, obs->longitude), 360.0);
  if (sr < 0) {
    sr = (360 + sr);
  }
  ha = fmod(sr - tel->ra, 360.0);
  if (ha < 0) {
    ha = DEGRAD * (360 + ha);
  }
  else {
    ha = DEGRAD * ha;
  }
  cha = cos(ha);
  fi = obs->latitude * DEGRAD;
  sfi = sin(fi); cfi = cos(fi);
  del = tel->dec * DEGRAD;
  tel->az = RADDEG * atan2(sin(ha), cha * sfi - tan(del) * cfi) + 180.0;
  tel->alt = RADDEG * asin(sfi * sin(del) + cfi * cos(del) * cha);
  tel->jd = jd;
  tel->sr = sr;
}

/* Calculate ra and dec from the given altitude and azimuth and location */
void
altaztoequ(TELESCOPE *tel, OBSERVATORY *obs)

{
  double sr, ha, fi;
  double jd;
  double sfi, cfi, del, cha;
  struct tm *gmt;
  time_t t;

  t = time(NULL);
  gmt = gmtime(&t);   /* Current gmt  */
  /*  calculate Julian day */
  jd = julian_day(1900 + gmt->tm_year, gmt->tm_mon + 1, (double) gmt->tm_mday + 
		  gmt->tm_hour / 24.0 + gmt->tm_min / 1440.0 + 
		  gmt->tm_sec / 86400.0);
  sr = fmod(sidereal(jd, obs->longitude), 360.0);
  if (sr < 0) {
    sr = (360 + sr);
  }
  /*  Calculate local hour angle for tel->ra  */
  ha = DEGRAD * fmod(sr - tel->ra, 360.0);
  if (ha < 0) {
    ha = DEGRAD * (360 + ha);
  }
  else {
    ha = DEGRAD * ha;
  }
  cha = cos(DEGRAD * tel->az);
  fi = obs->latitude * DEGRAD;
  sfi = sin(fi); cfi = cos(fi);
  del = tel->alt * DEGRAD;
  tel->ra = RADDEG * atan2(sin(DEGRAD * tel->az), cha * sfi + tan(del) * cfi);
  tel->ra = RADDEG * (ha - tel->ra);
  if (tel->ra < 0) tel->ra += 360.0;
  tel->dec = RADDEG * asin(sfi * sin(del) - cfi * cos(del) * cha);
  tel->jd = jd;
  tel->sr = sr;
}

void
update_position(XEvent *xev, void *t)

{
  char a[20];
  equtoaltaz(&telescope, &observatory);
  fl_set_input(state.telescope->telaltW, format_dec(telescope.alt + 1E-9));
  fl_set_input(state.telescope->telaziW, format_az(telescope.az + 1E-9));
  sprintf(a, "%.5f", telescope.jd);
  fl_set_input(state.telescope->teljulianW, a);
  fl_set_input(state.telescope->telsiderealW, format_ra(telescope.sr + 1E-9));
  state.timeout = fl_add_timeout(500, update_position, NULL);
}

void
teldecC(FL_OBJECT *obj, long val)

{
  double dec;

  dec = parse_dec_c((char *)fl_get_input(obj));
  fl_set_input(obj, format_dec(dec + 1E-9));
  telescope.dec = dec;
}

void
telraC(FL_OBJECT *obj, long val)

{
  double ra;

  ra = parse_ra_c((char *)fl_get_input(obj));
  fl_set_input(obj, format_ra(ra + 1E-9));
  telescope.ra = ra;
}

void
telaziC(FL_OBJECT *obj, long val)

{
}

void
telaltC(FL_OBJECT *obj, long val)

{
}

void
telbrowsecatC(FL_OBJECT *obj, long val)

{
  char a[256], *p;
  char ra[20], dec[20];

  strncpy(a, fl_get_browser_line(obj, fl_get_browser(obj)), 255);
  /*  Extract entries no 4 and 5  */
  p = a;
  p = strtok(p, " \t");
  p = strtok(NULL, " \t");
  p = strtok(NULL, " \t");
  p = strtok(NULL, " \t");
  strcpy(ra, p);
  p = strtok(NULL, " \t");
  strcpy(dec, p);
  telescope.ra = parse_ra_c(ra);
  telescope.dec = parse_dec_c(dec);
  fl_set_input(state.telescope->telraW, format_ra(telescope.ra + 1E-9));
  fl_set_input(state.telescope->teldecW, format_dec(telescope.dec + 1E-9));
}

void
teltempC(FL_OBJECT *obj, long val)

{
  double t;
  char a[10];

  t = strtod(fl_get_input(obj), NULL);
  if (fabs(t) > 270) {
    t = 0;
  }
  sprintf(a, "%.2f", t);
  telescope.temp = t;
  fl_set_input(obj, a);
}

void
telreadtempC(FL_OBJECT *obj, long val)

{
  FD_CMDLOG *cmdlog;
  int m;
  char *a, b[20];

  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(telescope.gettemp);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
  cmdlog = fl_get_command_log_fdstruct();
  m = fl_get_browser_maxline(cmdlog->browser) - 3;
  a = strdup((char *) fl_get_browser_line(cmdlog->browser, m));
  if (a == NULL) return;
  sscanf(a, "ccd done temp=%lf\n", &telescope.temp);
  sprintf(b, "%.2f", telescope.temp);
  fl_set_input(state.telescope->teltempW, b);
  free(a);
}

void
telsettempC(FL_OBJECT *obj, long val)

{
  char a[270], *p, b[20];

  strcpy(a, telescope.settemp);
  p = strstr(a, "TEMP");
  if (p == NULL) {
    fl_show_alert("Missing TEMP template in the set temperature  command!", "", "", 1);
    return;
  }
  *p = '\0';
  sprintf(b, "%.2f", telescope.temp);
  strcat(a, b);
  strcat(a, (char *) telescope.settemp + (p - a) + 4);
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(a);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}

/*========================*/
/*   Set exposure time    */
/*========================*/
void
telexposureC(FL_OBJECT *obj, long val)

{
  double t;
  char a[10];

  t = strtod(fl_get_input(obj), NULL);
  if (t < 0) {
    t = 0;
  }
  sprintf(a, "%.2f", t);
  telescope.exptime = t;
  fl_set_input(obj, a);

}

/*==========================*/
/*  Make a CCD exposure     */
/*==========================*/
void
telstartexposureC(FL_OBJECT *obj, long val)

{
  char a[270], *p, b[270];
  
  strcpy(a, telescope.expose);
  p = strstr(a, "FILE");
  if (p == NULL) {
    fl_show_alert("Missing FILE template in the expose command!", "", "", 1);
    return;
  }
  *p = '\0';
  if (telescope.filename == NULL || strlen(telescope.filename) == 0) {
    fl_show_alert("Please enter file name!", "", "", 1);
    return;
  }
  sprintf(b, "%s", telescope.filename);
  strcat(a, b);
  strcat(a, (char *) telescope.expose + (p - a) + 4);
  p = strstr(p + 1, "TIME");
  if (p == NULL) {
    fl_show_alert("Missing TIME template in the expose command!", "", "", 1);
    return;
  }
  *p = '\0';
  sprintf(b, "%.3f %s ", telescope.exptime, (telescope.dark) ? "dark" : "");
  strcat(a, b);
  strcat(a, (char *) telescope.expose + 
	 (strstr(telescope.expose, "TIME") - telescope.expose) + 4);
  fl_deactivate_form(state.telescope->Telescope);
  /*  fl_show_object(state.telescope->exponW); */
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(a);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  /*fl_hide_object(state.telescope->exponW);*/
  fl_activate_form(state.telescope->Telescope);
}

/*================================*/
/*  Center star in the CCD field  */
/*================================*/
void
telcenterC(FL_OBJECT *obj, long val)

{
  char a[270], *p, b[270];
  
  strcpy(a, telescope.center);
  p = strstr(a, "TIME");
  if (p == NULL) {
    fl_show_alert("Missing TIME template in the center command!", "", "", 1);
    return;
  }
  *p = '\0';
  sprintf(b, "%.3f", telescope.exptime);
  strcat(a, b);
  strcat(a, (char *) telescope.center + 
	 (strstr(telescope.center, "TIME") - telescope.center) + 4);
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(a);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}



void
telfileC(FL_OBJECT *obj, long val)

{
  if (telescope.filename) free(telescope.filename);
  telescope.filename = strdup((char *) fl_get_input(obj));
}

void
telfbrowseC(FL_OBJECT *obj, long val)

{
}

void
teldarkC(FL_OBJECT *obj, long val)

{
  telescope.dark = fl_get_button(obj);
}


void
telstartC(FL_OBJECT *obj, long val)

{
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(telescope.start);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}


void
telparkC(FL_OBJECT *obj, long val)

{
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(telescope.park);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}

/*===============================================*/
/*  Read and display current telescope position  */
/*===============================================*/
void
telreadposC(FL_OBJECT *obj, long val)

{
  FD_CMDLOG *cmdlog;
  int m;
  char *a;
  char ra[20], dec[20];

  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(telescope.where);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
  cmdlog = fl_get_command_log_fdstruct();
  m = fl_get_browser_maxline(cmdlog->browser) - 3;
  a = strdup((char *) fl_get_browser_line(cmdlog->browser, m));
  if (a == NULL) return;
  sscanf(a, "where done ra=%s dec=%s\n", ra, dec);
  telescope.ra = parse_ra_c(ra);
  telescope.dec = parse_dec_c(dec);
  fl_set_input(state.telescope->telraW, format_ra(telescope.ra + 1E-9));
  fl_set_input(state.telescope->teldecW, format_dec(telescope.dec + 1E-9));
  free(a);
}


/*===========================================*/
/*  Point telescope to the desired position  */
/*===========================================*/
void
telgoC(FL_OBJECT *obj, long val)

{
  char a[270], *p;

  strcpy(a, telescope.point);
  p = strstr(a, "RA");
  if (p == NULL) {
    fl_show_alert("Missing RA template in the point command!", "", "", 1);
    return;
  }
  *p = '\0';
  strcat(a, format_ra(telescope.ra + 1E-9));
  strcat(a, (char *) telescope.point + (p - a) + 2);
  p = strstr(p + 1, "DEC");
  if (p == NULL) {
    fl_show_alert("Missing DEC template in the point command!", "", "", 1);
    return;
  }
  *p = '\0';
  strcat(a, format_dec(telescope.dec + 1E-9));
  strcat(a, (char *) telescope.point + 
	 (strstr(telescope.point, "DEC") - telescope.point) + 3);
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(a);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}

/*=======================================*/
/*  Set telescope position to RA, DEC    */
/*=======================================*/
void
telzeroC(FL_OBJECT *obj, long val)

{
  char a[270], *p;

  strcpy(a, telescope.zero);
  p = strstr(a, "RA");
  if (p == NULL) {
    fl_show_alert("Missing RA template in the zero command!", "", "", 1);
    return;
  }
  *p = '\0';
  strcat(a, format_ra(telescope.ra + 1E-9));
  strcat(a, (char *) telescope.zero + (p - a) + 2);
  p = strstr(p + 1, "DEC");
  if (p == NULL) {
    fl_show_alert("Missing DEC template in the zero command!", "", "", 1);
    return;
  }
  *p = '\0';
  strcat(a, format_dec(telescope.dec + 1E-9));
  strcat(a, (char *) telescope.zero + 
	 (strstr(telescope.zero, "DEC") - telescope.zero) + 3);
  fl_deactivate_form(state.telescope->Telescope);
  fl_set_object_color(obj, FL_RED, FL_BUTTON_COL2);
  execute(a);
  fl_set_object_color(obj, FL_BUTTON_COL1, FL_BUTTON_COL2);
  fl_activate_form(state.telescope->Telescope);
}


void
telchoosecatC(FL_OBJECT *obj, long val)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.lst";
  static char *name = "";

  fl_use_fselector(0);
  fl_invalidate_fselector_cache();
  fn = fl_show_fselector("Choose a catalog", curdir, pattern, name);
  if (fn == NULL) return;
  /*  Save the current directory  */
  curdir = (char *) fl_get_directory();
  /*  Save the current pattern */
  pattern = (char *) fl_get_pattern();
  name = (char *) fl_get_filename();
  fl_load_browser(state.telescope->telbrowsecatW, fn);
}
/*  Show the telescope control window */
void
show_telescope_control(void)

{
  int n;
  time_t t;
  char a[20];

  /*  The window in not created yet, do it now  */
  if (state.telescope == NULL) {
    state.telescope = create_form_Telescope();
    t = time(NULL);
    localtime(&t);
    /* fl_set_clock_adjustment(state.telescope->telutW, timezone); */
    fl_set_browser_fontsize(state.telescope->telbrowsecatW, 14);
    fl_set_browser_fontstyle(state.telescope->telbrowsecatW, FL_FIXED_STYLE);
    fl_deactivate_object(state.telescope->teljulianW);
    fl_deactivate_object(state.telescope->telsiderealW);
    fl_deactivate_object(state.telescope->telaltW);
    fl_deactivate_object(state.telescope->telaziW);
  }
  fl_show_form(state.telescope->Telescope,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Telescope control");
  state.timeout = fl_add_timeout(500, update_position, NULL);
  /*  Write current parameter values into the form */
  n = state.control[state.control_num];

  fl_set_button(state.telescope->teldarkW, telescope.dark);
  fl_set_input(state.telescope->telraW, format_ra(telescope.ra));
  fl_set_input(state.telescope->teldecW, format_dec(telescope.dec));
  sprintf(a, "%.2f\n", telescope.temp);
  fl_set_input(state.telescope->teltempW, a);
  sprintf(a, "%.3f\n", telescope.exptime);
  fl_set_input(state.telescope->telexposureW, a);
  equtoaltaz(&telescope, &observatory);
  fl_set_input(state.telescope->telfileW, telescope.filename);
}





