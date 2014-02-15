/*  match_stars.c  */
/*  part of the fitsblink program  */
/*  routines for matching of stars from the catalog and stars from an image  */
/*  A lot of previously independent programs meet here  */
/*  Jure Skvarc                                 */
#include <stdio.h> 
#include <math.h>
#include <forms.h>
#include <malloc.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern CATALOG catalog[];


/*  Detect stars in the image  */
/*=============================*/
int
detect_stars(BLINK_FRAME *frame, int n)

{
  int i;
  int nstar;
  float kf, kx, ky;

  /*  Free the previous image list, if it exists */
  if (frame[n].imagelist.s != NULL) {
    if (frame[n].obj) free(frame[n].obj);
    frame[n].objnum = 0;
    free(frame[n].imagelist.s);
    if (state.interactive) {
      set_image(frame, n);
      draw_image(frame, n);
    }
  }
  /*  Reserve some space for stars  */
  nstar = NSTAR;
  frame[n].imagelist.s = (STAR *) myalloc(sizeof(STAR) * nstar, "detect_stars", "frame[n].imagelist.s");
  /*  Count the stars  */
  frame[n].imagelist.n = stetje(frame[n].fits, &nstar, &(frame[n].imagelist.s));
  if (state.interactive && frame[n].imagelist.n > 0) {
  /*  Reserve space for the list of objects  */
    frame[n].objnum = frame[n].objmax = frame[n].imagelist.n;
    frame[n].obj = (OBJECT *) myalloc(sizeof(OBJECT) * frame[n].objmax, 
					      "match_stars", "frame[n].obj");
    /*  Set the object list  */
    kx = (float) (frame[n].fits.width - 1) / (state.blinker->blinkW->w - 1);
    ky = (float) (frame[n].fits.height - 1) / (state.blinker->blinkW->h - 1);
    if (kx < ky) kx = ky;
    /*  Find the brightest star  */
    kf = 0;
    for (i = 0; i < frame[n].imagelist.n; i++) {
      if (frame[n].imagelist.s[i].mag > kf) 
	kf = frame[n].imagelist.s[i].mag;
    }
    kf = 6.0 / log(kf);
    for (i = 0; i < frame[n].objnum; i++) {
      frame[n].obj[i].x = frame[n].imagelist.s[i].x;
      frame[n].obj[i].y = frame[n].imagelist.s[i].y;
      frame[n].obj[i].r = kf * log(frame[n].imagelist.s[i].mag);
      frame[n].obj[i].color = FL_RED;
      frame[n].obj[i].s = "";
      frame[n].obj[i].shape = OBJ_CIRCLE;
    }
    /*  Draw star outlines  */
    draw_objects(frame, n);
    draw_image(frame, n);
    /*  Allow star matching and output of star list  */
    if (state.interactive) {
      fl_set_menu_item_mode(state.blinker->astrometryW, MATCH_STARS, FL_PUP_NONE);
      fl_set_menu_item_mode(state.blinker->astrometryW, SAVE_STAR_LIST, FL_PUP_NONE);
    }
    frame[n].detect = 1;
  }
  return frame[n].imagelist.n;
}



/*  Match stars from a star list to the stars from a catalogue */
/*=============================================================*/
int
match_stars(BLINK_FRAME *frame, int n)

{
  int nm, i, j, k;
  char r1[15], r2[15], d2[15], d1[15];
  char a[80];
  float errp, magmax;

  /*  Copy input image parameters into MAP_IMAGE structure  */
  frame[n].map.ra = frame[n].ra;    /* R.A. of the map center  */
  frame[n].map.dec = frame[n].dec;  /* Declination of the map center  */
  frame[n].map.fi = 0; /* frame[n].fi;  */  /* Map rotation              */
  frame[n].map.width = fabs(frame[n].xsize) * frame[n].fitsfile.nax[0] / 3600.0;  /*  Map width in decimal degrees */
  frame[n].map.height = fabs(frame[n].ysize) * frame[n].fitsfile.nax[1] / 3600.0;  /*  Map height in decimal degrees */
  frame[n].map.w = frame[n].fitsfile.nax[0];
  frame[n].map.h = frame[n].fitsfile.nax[1];
  
  /*  Reserve space for the star list  */
  if (!frame[n].cataloglist.s) {
    frame[n].cataloglist.s = (STAR *) myalloc(sizeof(STAR) * MAXSTARNUM,
					      "match_stars", "cataloglist");
  }
  if (!catalog[CAT_USNO].use && !catalog[CAT_PPM].use && !catalog[CAT_GUIDE].use
      && !catalog[CAT_GUIDE_SOUTH].use) {
    if (state.interactive) 
      fl_show_alert("Please, select at least one catalog in Astrometry menu!", "", "", 1);
    else return 2;
  }
  else {
    frame[n].map.nm = 0;
    if (catalog[CAT_USNO].use) {
      get_usno_stars(&(frame[n].map), frame[n].cataloglist.s, catalog[CAT_USNO].path);
    }
    if (catalog[CAT_GUIDE].use || catalog[CAT_GUIDE_SOUTH].use) {
      get_guide_stars(&(frame[n].map), frame[n].cataloglist.s, catalog[CAT_GUIDE].path, catalog[CAT_GUIDE_SOUTH].path);
    }
    frame[n].cataloglist.n = frame[n].map.nm;
    if (frame[n].cataloglist.n == 0) {
      if (state.interactive) {
	fl_show_alert("No stars from the catalog found!", "", "", 1);
      }
      return 1;
    }
    fprintf(state.logfp, "    Number of stars from the catalog:%d\n", frame[n].map.nm);
    nm = field_compare(&(frame[n].imagelist), &(frame[n].cataloglist), 
		  frame[n].match, frame, n);
    if (nm > 3) {
      errp = 0;
      k = 0;
      frame[n].matched = 1;
      if (state.interactive) {
	double kf;
	fl_set_menu_item_mode(state.blinker->astrometryW, DO_ASTROMETRY, FL_PUP_NONE);
	/*  Show coordinates of the matched stars */
	if (!(state.report && fl_form_is_visible(state.report->Report))) 
	  show_report();
	fl_clear_browser(state.report->reportW);
	fl_addto_browser(state.report->reportW, 
			 "Image no.     RA       DEC          Cat. no.       RA        DEC        err");
	j = frame[n].objnum - nm;
	magmax = 1e10;
	for (i = 0; i < frame[n].imagelist.n; i++) {
	  int m = frame[n].imagelist.s[i].m;
	  if (m != -1 && frame[n].imagelist.s[i].q) {
	    if (frame[n].cataloglist.s[m].mag < magmax) {
	      magmax = frame[n].cataloglist.s[m].mag;
	    }
	  }
	}
	kf = 3.0 / log(magmax);
	magmax = log(magmax) + 2;
	for (i = 0; i < frame[n].imagelist.n; i++) {
	  int m = frame[n].imagelist.s[i].m;
	  if (m != -1 && frame[n].imagelist.s[i].q) {
	    strcpy(r1, format_ra(frame[n].imagelist.s[i].ra));
	    strcpy(d1, format_dec(frame[n].imagelist.s[i].dec));
	    strcpy(r2, format_ra(frame[n].cataloglist.s[m].ra));
	    strcpy(d2, format_dec(frame[n].cataloglist.s[m].dec));
	    sprintf(a, "%08d %s %s    %08d %s %s %6.2f", 
		    frame[n].imagelist.s[i].n, r1, d1, 
		    frame[n].cataloglist.s[m].n, r2, d2, 
		    frame[n].imagelist.s[i].err);
	    fl_addto_browser(state.report->reportW, a);
	    
	    frame[n].obj[j].x = frame[n].cataloglist.s[m].x;
	    frame[n].obj[j].y = frame[n].cataloglist.s[m].y;
	    frame[n].obj[j].r = 2.0 * kf * 
	      (log(frame[n].cataloglist.s[m].mag) - magmax);
	    frame[n].obj[j].color = FL_YELLOW;
	    frame[n].obj[j].shape = OBJ_DIAMOND;
	    frame[n].obj[j].s = "";
	    errp += frame[n].imagelist.s[i].err;
	    j++;
	    k++;
	  }
	}
	errp /= k;
	fl_addto_browser(state.report->reportW, 
			 "---------------------------------------------------------------------------");
	sprintf(a, "Number of reference stars: %d", k);
	fl_addto_browser(state.report->reportW, a);
	sprintf(a, "Average residual: %.2f\"", errp);
	fl_addto_browser(state.report->reportW, a);
	fprintf(state.logfp, "Number of reference stars: %d\n", k);
	fprintf(state.logfp, "Average residual: %.2f\"\n", errp);
	
	frame[n].objnum += (k - nm);
	/*  Draw star outlines  */
	set_image(frame, n);
	draw_objects(frame, n);
	draw_image(frame, n);
      }
      else {
	for (i = 0; i < frame[n].imagelist.n; i++) {
	  int m = frame[n].imagelist.s[i].m;
	  if (m != -1 && frame[n].imagelist.s[i].q) {
	    errp += frame[n].imagelist.s[i].err;
	    k++;
	  }
	}
	errp /= k;
	fprintf(state.logfp, "Number of reference stars: %d\n", k);
	fprintf(state.logfp, "Average residual: %.2f\"\n", errp);
      }
    }
    else {
      if (state.interactive) {
	fl_show_alert("Matching failed!", "", "", 1);
      }
      return 1;
    }
  }
  return 0;
}



/*  Output the detected stars to a file  */
int
star_list(BLINK_FRAME *frame, int n)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.dat";
  static char *name = "";
  FILE *fp;
  int i;

  make_output_filename(frame[n].fits.name, &name, "dat");
  if (state.interactive) {
    fl_use_fselector(3);
    fl_invalidate_fselector_cache();
    fn = fl_show_fselector("Choose output file", curdir, pattern, name);
    if (fn == NULL) return 1;
    /*  Save the current directory  */
    curdir = (char *) fl_get_directory();
    /*  Save the current pattern */
    pattern = (char *) fl_get_pattern();
    name = (char *) fl_get_filename();
  }
  else {
    fn = name;
  }
  if (!(fp = fopen(fn, "w"))) {
    if (state.interactive) {
      fl_show_alert("Can not open file", fn, "for writing!", 0);
      return 1;
    }
    else {
      fprintf(stderr, "Can not open file %s for writing!\n", fn);
      exit(1);
    }
  }
  fprintf(fp, "Input file name: %s\n", frame[n].fits.name);
  for (i = 0; i < frame[n].imagelist.n; i++) {
    fprintf(fp, "%5d %7.2f %7.2f %11.2f", i, frame[n].imagelist.s[i].x,
	    frame[n].imagelist.s[i].y, frame[n].imagelist.s[i].mag);
    if (frame[n].matched) {
      int m;
      fprintf(fp, " %9.6f %9.6f", 
	      frame[n].imagelist.s[i].ra, frame[n].imagelist.s[i].dec);
      m = frame[n].imagelist.s[i].m;
      if (m != -1) {
	fprintf(fp, " %c %08d %6.2f %6.2f", frame[n].cataloglist.s[m].catalog,
		frame[n].cataloglist.s[m].n, frame[n].cataloglist.s[m].err,
		-2.5 * log10(frame[n].cataloglist.s[m].mag));
      }
    }
    fprintf(fp, "\n");
  }
  fprintf(state.logfp, "Star list output written to: %s\n", fn);
  fclose(fp);
  return 0;
}


/*  Read star positions and intensities from a file  */
int
load_star_list(BLINK_FRAME *frame, int n)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.dat";
  static char *name = "";
  char line[81];
  FILE *fp;
  int nstar;
  int i, m;
  float kf;
  int was = 0;

  make_output_filename(frame[n].fits.name, &name, "dat");
  fn = name;
  if (state.interactive) {
    fl_use_fselector(3);
    fl_invalidate_fselector_cache();
    fn = fl_show_fselector("Choose input file", curdir, pattern, name);
    if (fn == NULL) return 1;
    /*  Save the current directory  */
    curdir = (char *) fl_get_directory();
    /*  Save the current pattern */
    pattern = (char *) fl_get_pattern();
    name = (char *) fl_get_filename();
  }
  if (!(fp = fopen(fn, "r"))) {
    if (state.interactive) {
      fl_show_alert("Can not open file", fn, "for reading!", 0);
      return 1;
    }
  }
  if (frame[n].imagelist.s != NULL) {
    if (frame[n].obj) free(frame[n].obj);
    frame[n].objnum = 0;
    free(frame[n].imagelist.s);
    was = 1;
  }
  /*  reserve space for some stars  */
  nstar = NSTAR;
  frame[n].imagelist.s = (STAR *) myalloc(sizeof(STAR) * nstar, "load_star_list", "frame[n].imagelist.s");
  /*  Set star counter to zero  */
  i = frame[n].imagelist.n = 0;
  /*  Skip the first line in the input file  */
  while (!feof(fp) && fgetc(fp) != '\n');
  /*  Read all stars  */
  while (!feof(fp)) {
    /*  Get a line  */
    fgets(line, 80, fp);
    if (!feof(fp)) {
      char c;
      m = sscanf(line, "%d %lf %lf %f %lf %lf %c %d, %f", &frame[n].imagelist.s[i].n, 
		 &frame[n].imagelist.s[i].x, &frame[n].imagelist.s[i].y, 
		 &frame[n].imagelist.s[i].mag, &frame[n].imagelist.s[i].ra, 
		 &frame[n].imagelist.s[i].dec,  &c,
		 &frame[n].imagelist.s[i].m, &frame[n].imagelist.s[i].err);
      frame[n].imagelist.s[i].catalog = c;
      if (m >= 4) {
	frame[n].imagelist.n++;
	i++;
	if (i == nstar) {
	  nstar += NSTAR;
	  frame[n].imagelist.s = (STAR *) realloc(frame[n].imagelist.s,
						  sizeof(STAR) * nstar);
	  if (frame[n].imagelist.s == NULL) {
	    fprintf(stderr, "Out of memory, function load_star_list, frame[%d].imagelist.s", n);
	    exit(1);
	  }
	}
      }
    }
  }
  fclose(fp);
  /*  Return space  */
  frame[n].imagelist.s = (STAR *) realloc(frame[n].imagelist.s,
					  sizeof(STAR) * frame[n].imagelist.n);
  frame[n].objnum = frame[n].objmax = frame[n].imagelist.n;
  frame[n].obj = (OBJECT *) myalloc(sizeof(OBJECT) * frame[n].objmax, 
				    "load_star_list", "frame[n].obj");
  /*  Find the brightest star  */
  kf = 0;
  for (i = 0; i < frame[n].imagelist.n; i++) {
    if (frame[n].imagelist.s[i].mag > kf) 
      kf = frame[n].imagelist.s[i].mag;
  }
  kf = 6.0 / log(kf);
  for (i = 0; i < frame[n].objnum; i++) {
    frame[n].obj[i].x = frame[n].imagelist.s[i].x;
    frame[n].obj[i].y = frame[n].imagelist.s[i].y;
    frame[n].obj[i].r = kf * log(frame[n].imagelist.s[i].mag);
    frame[n].obj[i].color = FL_RED;
    frame[n].obj[i].s = "";
    frame[n].obj[i].shape = OBJ_CIRCLE;
  }
  if (frame[n].objnum > 0) {
    if (state.interactive) {
      fl_set_menu_item_mode(state.blinker->astrometryW, MATCH_STARS, FL_PUP_NONE);
      fl_set_menu_item_mode(state.blinker->astrometryW, SAVE_STAR_LIST, FL_PUP_NONE);
    }
    frame[n].detect = 1;
  }
  if (state.interactive) {
    if (was) {
      set_image(frame, n);
    }
    else {
      draw_objects(frame, n);
    }      
    draw_image(frame, n);
  }
  return 0;
}

