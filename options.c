/*  options.c  */
/*  part of the fitsblink program  */
/*  routines for input of astrometry options */
/*  Jure Skvarc,  May 1998                   */
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <forms.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
BLINK_FRAME defvals;

void 
logfileC(FL_OBJECT *obj, long val) 

{
  char *format = "Can't open file '%s!";
  char *mes;

  if (state.logfile && strlen(state.logfile) < strlen(fl_get_input(obj))) {
    free(state.logfile);
  }
  state.logfile = strdup((char *) fl_get_input(obj)); 
  trim_spaces(state.logfile);
  if (state.logfp != stdout) {
    fclose(state.logfp);
  }
  state.logfp = fopen(state.logfile, "a");
  if (state.logfp == NULL) {
    mes = (char *) myalloc(sizeof(char) * 
			   (strlen(state.logfile) + strlen(format) + 1),
			   "logfileC", "mes");
    sprintf(mes, format, state.logfile);
    fl_show_message(mes, "Output is redirected to stdout again.", "");
    fl_set_input(obj, "");
    free((void *) mes);
    state.logfp = stdout;
  }
}


/*  Write the minimum value into input field  */
void
set_minconst(float min, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", min);
  fl_set_input(obj, temp);
}

float
proc_min(FL_OBJECT *obj, float min, int nstar)

{
  float max, m;
  char t[80];

  max = 0.5 * (nstar + 1) * nstar;
  m = strtod(fl_get_input(obj), NULL);
  if (m < 5.0) {
    sprintf(t, "I suggest that you enter a number between %.2f and %.2f.", 
	    CONSTMIN, 0.5 * max);
    fl_show_message("Possible wrong input", 
		  "You can enter this number if you really want to\nbut the results will be rather senseless.", t);
  }
  if (m >= max) {
    sprintf(t, "I suggest that you enter a number between %.2f and %.2f.", 
	    CONSTMIN, 0.5 * max);
    fl_show_message("You will never get any match with this value",
		  "because it is too big.", t);
    m = min;
  }
  set_minconst(m, obj);
  return m;
}

/*  Get a minimum value for a constellation to be accepted  */
void minconstC(FL_OBJECT *obj, long val) 

{
  int n = state.control[state.control_num];
  frame[n].match.min = proc_min(obj, frame[n].match.min, frame[n].match.nstar);
}


/*  Write the number of stars used for initial matching  into input field  */
/*-------------------------------------------------------------------------*/
void
set_initconst(int init, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", init);
  fl_set_input(obj, temp);
}

int
proc_firstbright(FL_OBJECT *obj, int firstbright)

{
  int fb;
  fb = strtol(fl_get_input(obj), NULL, 10);
  if (fb < 10 || fb > 100) {
    fl_show_message("You can enter this number if you really want to",
		  "but the results will be rather senseless.",
		  "Please use numbers between 10 and 100");
  }
  set_initconst(fb, obj);
  return fb;
}

void initconstC(FL_OBJECT *obj, long val) 

{
  int n = state.control[state.control_num];
  frame[n].match.firstbright = proc_firstbright(obj, frame[n].match.firstbright);
}


/*  Write a number of stars in a constellation into input field  */
/*---------------------------------------------------------------*/
void
set_starconst(int nstar, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", nstar);
  fl_set_input(obj, temp);
}


int
proc_nstar(FL_OBJECT *obj, int nstar)

{
  int ns;

  ns = strtol(fl_get_input(obj), NULL, 10);
  if (ns < 6 || ns > 15) {
    fl_show_message("You can enter this number if you really want to",
		  "but the results will be rather senseless.",
		  "Please use numbers between 6 and 15");
  }
  set_starconst(ns, obj);
  return ns;
}

void starconstC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].match.nstar = proc_nstar(obj, frame[n].match.nstar);
}

/*  Write the maximal allowed positional error into input field */
/*--------------------------------------------------------------*/
void
set_starerror(float poserrmax, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", poserrmax);
  fl_set_input(obj, temp);
}

float
proc_poserrmax(FL_OBJECT *obj, float poserrmax)

{
  float pos;

  pos = strtod(fl_get_input(obj), NULL);
  if (pos < 1 || pos > 10) {
    fl_show_message("You can enter this number if you really want to",
		  "but the results will be rather senseless.",
		  "Please use numbers between 1 and 10");
  }
  set_starerror(pos, obj);
  return pos;
}

void starerrorC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].match.poserrmax = proc_poserrmax(obj, frame[n].match.poserrmax);
}


/*  Write the maximal allowed positional error into input field */
/*--------------------------------------------------------------*/
void
set_maxres(float res, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", res);
  fl_set_input(obj, temp);
}


float
proc_maxres(FL_OBJECT *obj, float maxres)

{
  float mr;
  mr = strtod(fl_get_input(obj), NULL);
  if (mr < 0.5 || mr > 3) {
    fl_show_message("You can enter this number if you really want to",
		  "but the results will be rather senseless.",
		  "Please use numbers between 0.5 and 3");
  }
  set_maxres(mr, obj);
  return mr;
}


void maxresC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].match.maxres = proc_maxres(obj, frame[n].match.maxres);
}


/*  Write the minimum magnitude  */
/*-------------------------------*/
void
set_starminmag(float minmag, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", minmag);
  fl_set_input(obj, temp);
}

float
proc_minmag(FL_OBJECT *obj, float minmag, float maxmag)

{
  float min;

  min = strtod(fl_get_input(obj), NULL);
  if (min >= maxmag) {
    fl_show_message("The lowest magnitude should have lower",
		  "value than the highest magnitude.",
		  "Correct this, please!");
    min = minmag;
  }
  set_starminmag(min, obj);
  return min;
}

void starminmagC(FL_OBJECT *obj, long val)
{
  int n = state.control[state.control_num];
  frame[n].map.minmag = proc_minmag(obj, frame[n].map.minmag, frame[n].map.maxmag);
}


/*  Write the maximum magnitude  */
/*-------------------------------*/
void
set_starmaxmag(float maxmag, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", maxmag);
  fl_set_input(obj, temp);
}

float
proc_maxmag(FL_OBJECT *obj, float maxmag, float minmag)

{
  float max;
  max = strtod(fl_get_input(obj), NULL);
  if (minmag >= max) {
    fl_show_message("The lowest magnitude should have lower",
		  "value than the highest magnitude.",
		  "Correct this, please!");
    max = maxmag;
  }
  set_starmaxmag(max, obj);
  return max;
}

void starmaxmagC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].map.maxmag = proc_maxmag(obj, frame[n].map.maxmag, frame[n].map.minmag);
}



/*  Write the sensitivity threshold  */
void
set_countsigma(float min, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", min);
  fl_set_input(obj, temp);
}


float
proc_sg_num(FL_OBJECT *obj, float sg_num)

{
  float sg;

  sg = strtod(fl_get_input(obj), NULL);
  if (sg <= 0) {
    fl_show_message("Please type in a positive number!",
		  "Typical good values are between 2 and 5.",
		  "");
    sg = sg_num;
  }
  set_countsigma(sg, obj);
  return sg;
}

void
countsigmaC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.sg_num = proc_sg_num(obj, frame[n].fits.sg_num);
}


/*  Write the minimal accepted object intensity  */
/*===============================================*/
void
set_countminval(int min, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", min);
  fl_set_input(obj, temp);
}

int
proc_minbright(FL_OBJECT *obj, int minbright)

{
  int minb;

  minb = strtol(fl_get_input(obj), NULL, 10);
  if (minb > 200) {
    fl_show_message("Just to let you know that using high",
		  "values of this parameter you may lose",
		  "a lot of good stars.");
  }
  if (minb < 50 ) {
    fl_show_message("You may get a lot of false detections",
		  "if you use this value.",
		  "");
  }
  set_countminval(minb, obj);
  return minb;
}

/*  Input the minimal accepted intensity  */
/*========================================*/
void countminvalC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.minbright = proc_minbright(obj, frame[n].fits.minbright);
}


/*  Write the minimal accepted star size  */
/*========================================*/
void
set_starminsize(int min, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", min);
  fl_set_input(obj, temp);
}


int
proc_minstar(FL_OBJECT *obj, int minstar)

{
  int mins;

  mins = strtol(fl_get_input(obj), NULL, 10);
  if (mins > 2 || mins < 1) {
    fl_show_message("You can experiment if you really want to,",
		  "but the suggested values are 1 and 2",
		  "");
  }
  set_starminsize(mins, obj);
  return mins;
}

/*  Input the minimal accepted star size  */
/*========================================*/
void starminsizelC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.minstar = proc_minstar(obj, frame[n].fits.minstar );
}

/*  Write the grid size  */
/*========================================*/
void
set_gridsize(int grid, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", grid);
  fl_set_input(obj, temp);
}

int
proc_gridsize(FL_OBJECT *obj, int gridsize)

{
  int gsize;

  gsize = strtol(fl_get_input(obj), NULL, 10);
  if (gsize > 100 || gsize < 10) {
    fl_show_message("Please set this value somewhere",
		  "between 10 and 100.", "");
  }
  set_gridsize(gsize, obj);
  return gsize;
}

/*  Input the grid  size  */
/*========================================*/
void stargridC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.maxsize = proc_gridsize(obj, frame[n].fits.maxsize);
}

/*  Write the insensitive edge size  */
/*========================================*/
void
set_edgesize(int edge, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%d", edge);
  fl_set_input(obj, temp);
}


int
proc_border(FL_OBJECT *obj, int border)

{
  int b;
  int n = state.control[state.control_num];

  b = strtol(fl_get_input(obj), NULL, 10);
  if (b > frame[n].fits.width / 2 || 
      b > frame[n].fits.height / 2) {
    fl_show_message("With this value you will get",
		  "0 detections.",
		  "");
    b = border;
  }
  set_edgesize(b, obj);
  return b;
}

/*  Input the edge size  */
/*========================================*/
void staredgeC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.border = proc_border(obj, frame[n].fits.border );
}


/*  Write the inner circle size  */
/*========================================*/
void
set_inner(float size, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", size);
  fl_set_input(obj, temp);
}


float
proc_inner(FL_OBJECT *obj, float inner, float outer)

{
  float inn;
  char a[40];

  inn = strtod(fl_get_input(obj), NULL);
  if (inn < 0.1 || inn >= outer) {
    sprintf(a, "between 0.1 and %.1f", outer);
    fl_show_message("Size of the inner circle radius should be", a, "");
    inn = inner;
  }
  set_inner(inn, obj);
  return inn;
}

/*  Input the size of the inner circle for the aperture astrometry  */
/*========================================*/
void starinnerC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.inner = proc_inner(obj, frame[n].fits.inner, frame[n].fits.outer);
}


/*  Write the outer circle size  */
/*========================================*/
void
set_outer(float size, FL_OBJECT *obj)
{
  char temp[20];
  sprintf(temp, "%.1f", size);
  fl_set_input(obj, temp);
}

float
proc_outer(FL_OBJECT *obj, float outer, float inner)

{
  float out;
  char a[40];

  out = strtod(fl_get_input(obj), NULL);
  if (out > 20.0 || out <= inner) {
    sprintf(a, "between %.1f and 50.0", inner);
    fl_show_message("Size of the outer circle radius should be", a, "");
    out = outer;
  }
  set_outer(out, obj);
  return out;
}


/*  Input the size of the outer circle for the aperture astrometry  */
/*========================================*/
void starouterC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.outer = proc_outer(obj, frame[n].fits.outer, frame[n].fits.inner);
}

/*  Switch on and off the aperture astrometry  */
/*========================================*/
void starapertureC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  frame[n].fits.aperture = fl_get_button(obj);
}


/*  Set default values for matching stars  */
/*-----------------------------------------*/
void 
set_defvals(void)

{
  defvals.match.min = CONSTMIN; 
  defvals.match.nstar = CONSTSTAR; 
  defvals.match.firstbright = FIRSTBRIGHT; 
  defvals.match.poserrmax = POSERRMAX; 
  defvals.match.magerrmax = MAGERRMAX; 
  defvals.match.maxres = MAXRES; 
  defvals.map.minmag = DEFAULT_MINMAG; 
  defvals.map.maxmag = DEFAULT_MAXMAG;
  defvals.fits.sg_num = DEFAULT_SGNUM;
  defvals.fits.inner = DEFAULT_INNER;
  defvals.fits.outer = DEFAULT_OUTER;
  defvals.fits.minbright = DEFAULT_MINBRIGHT;
  defvals.fits.minstar = DEFAULT_MINSTAR;
  defvals.fits.maxsize = DEFAULT_MAXSIZE;
  defvals.fits.border = DEFAULT_BORDER;
  defvals.fits.aperture = DEFAULT_APERTURE;
  defvals.fits.inner = DEFAULT_INNER;
  defvals.fits.outer = DEFAULT_OUTER;
}


void 
set_match_defaults(BLINK_FRAME *frame, int n)

{
  frame[n].match.min = defvals.match.min;
  frame[n].match.nstar = defvals.match.nstar;
  frame[n].match.firstbright = defvals.match.firstbright;
  frame[n].match.poserrmax = defvals.match.poserrmax;
  frame[n].match.magerrmax = defvals.match.magerrmax;
  frame[n].match.maxres = defvals.match.maxres;
  frame[n].map.minmag = defvals.map.minmag;
  frame[n].map.maxmag = defvals.map.maxmag;
  frame[n].fits.sg_num = defvals.fits.sg_num;
  frame[n].fits.minbright = defvals.fits.minbright;
  frame[n].fits.minstar = defvals.fits.minstar;
  frame[n].fits.maxsize = defvals.fits.maxsize;
  frame[n].fits.border = defvals.fits.border;
  frame[n].fits.aperture = defvals.fits.aperture;
  frame[n].fits.inner = defvals.fits.inner;
  frame[n].fits.outer = defvals.fits.outer;
}


void defaultoptionsC(FL_OBJECT *obj, long val)

{
  int n = state.control[state.control_num];
  set_match_defaults(frame, n);
  set_minconst(frame[n].match.min, state.options->minconstW);
  set_initconst(frame[n].match.firstbright, state.options->initconstW);
  set_starconst(frame[n].match.nstar, state.options->starconstW);
  set_starerror(frame[n].match.poserrmax, state.options->starerrorW);
  set_maxres(frame[n].match.maxres, state.options->maxresW);
  set_starminmag(frame[n].map.minmag, state.options->starminmagW);
  set_starmaxmag(frame[n].map.maxmag, state.options->starmaxmagW);
  set_starminsize(frame[n].fits.minstar, state.options->starminsizeW);
  set_countsigma(frame[n].fits.sg_num, state.options->countsigmaW);
  set_countminval(frame[n].fits.minbright, state.options->countminvalW);
  set_gridsize(frame[n].fits.maxsize, state.options->stargridW);
  set_edgesize(frame[n].fits.border, state.options->staredgeW);
  set_inner(frame[n].fits.inner, state.options->starinnerW);
  set_outer(frame[n].fits.outer, state.options->starouterW);
}


void
edit_options(FL_OBJECT *obj, int m)

{
  int n = state.control[state.control_num];
  /*  The window in not created yet, do it now  */
  if (state.options == NULL) {
    state.options = create_form_Options();
    fl_set_form_minsize(state.options->Options, 
		  state.options->Options->w, state.options->Options->h);
    set_match_defaults(frame, n);
  }
  /*  Deactivate OPTIONS item in the menu */
  fl_set_menu_item_mode(obj, m, FL_PUP_GREY);
  fl_show_form(state.options->Options,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Image options");
  set_minconst(frame[n].match.min, state.options->minconstW);
  set_initconst(frame[n].match.firstbright, state.options->initconstW);
  set_starconst(frame[n].match.nstar, state.options->starconstW);
  set_starerror(frame[n].match.poserrmax, state.options->starerrorW);
  set_maxres(frame[n].match.maxres, state.options->maxresW);
  set_starminmag(frame[n].map.minmag, state.options->starminmagW);
  set_starmaxmag(frame[n].map.maxmag, state.options->starmaxmagW);
  fl_set_input(state.options->logfileW, state.logfile);
  set_starminsize(frame[n].fits.minstar, state.options->starminsizeW);
  set_gridsize(frame[n].fits.maxsize, state.options->stargridW); 
  set_edgesize(frame[n].fits.border, state.options->staredgeW); 
  set_countsigma(frame[n].fits.sg_num, state.options->countsigmaW);
  set_countminval(frame[n].fits.minbright, state.options->countminvalW);
  set_inner(frame[n].fits.inner, state.options->starinnerW);
  set_outer(frame[n].fits.outer, state.options->starouterW);
  fl_set_button(state.options->starapertureW, frame[n].fits.aperture);
}

