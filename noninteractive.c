/*  noninteractive.c  */
/*  part of the fitsblink program  */
/*  Jure Skvarc, July 1998  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/time.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "functs.h"
#include "consts.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern CATALOG catalog[NUMBER_OF_CATALOGS];
extern OBSERVATORY observatory;
extern MAIL mail;
extern FITS_IMAGE count_options;
extern MATCH_OPTIONS match_options;
extern TRANSROT trglob;
extern char *optarg;
extern int optopt, optind, opterr;
/*  Measure star positions and output them to a file in non-interactive mode  */

void
starcount(int argc, char *argv[])

{
  int i;
  int opt;

  while ((opt = getopt(argc, argv, "l:s:m:i:c:b:n:o:agv")) != -1) {
    switch(opt) {
    case 'l':
      /*  Name of the log file  */
      state.logfile = strdup(optarg);
      if ((state.logfp = fopen(state.logfile,"a") )== NULL) {
	fprintf(stderr, "Can not open log file!\n");
	exit(1);
      }
      break;
    case 'm':
      count_options.maxsize = strtol(optarg, NULL, 10);
      if (count_options.maxsize < 10) count_options.maxsize = 10;
      break;
    case 'b':
      count_options.border = strtol(optarg, NULL, 10);
      break;
    case 'c':
      count_options.minstar = strtol(optarg, NULL, 10);
      break;
    case 's':
      /*  Sensitivity expressed in number of standard deviations above the 
          background  */
      count_options.sg_num = strtod(optarg, NULL);
      if (count_options.sg_num < 0) count_options.sg_num = 2.0;
      break;
    case 'i':
      /*  The minimum acceptable intensity  */
      count_options.minbright = strtod(optarg, NULL);
      break;
    case 'n':
      /*  The radius of the inner circle for the aperture astrometry  */
      count_options.inner = strtod(optarg, NULL);
      break;
    case 'o':
      /*  The radius of the outer circle for the aperture astrometry  */
      count_options.outer = strtod(optarg, NULL);
      break;
    case 'a':
      /*  Aperture astrometry  */
      count_options.aperture = 1;
      break;
    case 'g':
      /*  "Above sigma astrometry" */
      count_options.aperture = 0;
      break;
    case 'v':
      /*  Verbose mode */
      state.verbose = 1;
      break;
    case '?':
      switch(optopt) {
      default:
	fprintf(stderr, "Unknown option -%c ignored!\n", optopt);
	break;
      }
      break;
    }
  }
  for (i = optind; i < argc; i++) {
    frame[0].name = strdup(argv[i]);
    load_frame(frame, 0);
    detect_stars(frame, 0);
    star_list(frame, 0);
    remove_frame(frame, 0);
  }
}



/*  Update WCS values in the FITS file  */
int
update_wcs(BLINK_FRAME *frame, int n, TRANSROT tr)

{  
  fitsfile *fptr;
  int err = 0;
  double xinc, yinc;
  double ac, dc, xc, yc, nw, nh, fi;

  /*  Open file for writing  */
  fits_open_file(&fptr, frame[n].name, READWRITE, &err);
  if (err != 0) {
    if (!state.interactive) {
      fprintf(stderr, "Can't open %s for writing!\n", frame[n].name);
      return 1;
    }
  }
  /*  Calculate WCS values  */
  xinc = frame[n].map.width / frame[n].map.w;
  yinc = frame[n].map.height / frame[n].map.h;
  xc = 0.5 * frame[n].map.w;
  yc = 0.5 * frame[n].map.h;
  cal_new_wcs(tr, frame[n].map.ra, frame[n].map.dec, frame[n].map.fi, 
	      xc, yc, xinc, yinc, 
	      &ac, &dc, &nw, &nh, &fi);
  fits_update_key(fptr, TSTRING, "CTYPE1", "RA---TAN", "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CTYPE1 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRPIX1", &xc, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CRPIX1 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRVAL1", &ac, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CRVAL1 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CDELT1", &nw, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CDELT1 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CROTA1", &fi, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CROTA1 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TSTRING, "CTYPE2", "DEC--TAN", "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CTYPE2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRPIX2", &yc, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CRPIX2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CRVAL2", &dc, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CRVAL2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CDELT2", &nh, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CDELT2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_update_key(fptr, TDOUBLE, "CROTA2", &fi, "Inserted by fitsblink", &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not update keyword CROTA2 for file %s\n", frame[n].name);
    return 1;
  }
  fits_close_file(fptr, &err);
  if (err != 0) {
    fprintf(stderr, "Disaster: could not close file %s\n", frame[n].name);
    return 1;
  }
  return 0;
}

/*  Read a star list, field center coordinates and pixel size (or fits header), */
/*  compare it to catalog and output celestial coordinates along with any  */
/*  cross ID's                                                             */
void
make_catalog(int argc, char *argv[])

{
  int opt;
  int wcs = 0;
  char *starlist = NULL;
  double ra, dec, p1, p2;
  FILE *fp;
  char a[256], fn[256];
  char *cat = NULL, *path;
  int nstar, n, i;
  int xw, yw;
  int writewcs = 0;

  match_options.firstbright = FIRSTBRIGHT;
  match_options.nstar = CONSTSTAR;
  match_options.min = CONSTMIN;
  match_options.poserrmax = POSERRMAX;
  match_options.magerrmax = MAGERRMAX;
  match_options.maxres = MAXRES;
  
  while ((opt = getopt (argc, argv, "b:l:n:d:a:w:h:m:e:i:r:f:c:x:y:vW")) != -1) {
    switch(opt) {
    case 'l':
      /*  Name of the log file  */
      state.logfile = strdup(optarg);
      if ((state.logfp = fopen(state.logfile,"a") )== NULL) {
	fprintf(stderr, "Can not open log file!\n");
	exit(1);
      }
      break;
    case 'b':
      match_options.firstbright = strtol(optarg, NULL, 10);
      if (match_options.firstbright < MINFIRSTBRIGHT) match_options.firstbright = MINFIRSTBRIGHT;
      break;
    case 'a':
      /* Get right ascension  */
      ra = strtod(optarg, NULL);
      wcs++;
      break;
    case 'd':
      /* Get declination */
      dec = strtod(optarg, NULL);
      wcs++;
      break;
    case 'w':
      /* Get pixel width */
      p1 = strtod(optarg, NULL);
      wcs++;
      break;
    case 'h':
      /* Get pixel height */
      p2 = strtod(optarg, NULL);
      wcs++;
      break;
    case 'x':
      /*  Number of pixels in x direction  */
      xw = strtol(optarg, NULL, 10);
      wcs++;
      break;
    case 'y':
      /*  Number of pixels in y direction  */
      yw = strtol(optarg, NULL, 10);
      wcs++;
      break;
    case 'f':
      /*  Name of the star list file  */
      starlist = strdup(optarg);
      if ((fp = fopen(starlist, "r") )== NULL) {
	fprintf(stderr, "Can not open starlist file: %s!\n", starlist);
	exit(1);
      }
    case 'n':
      match_options.nstar = strtol(optarg, NULL, 10);
      break;
    case 'm':
      match_options.min = strtod(optarg, NULL);
      break;
    case 'e':
      match_options.poserrmax = strtod(optarg, NULL);
      break;
    case 'i':
      match_options.magerrmax = strtod(optarg, NULL);
      break;
    case 'r':
      match_options.maxres = strtod(optarg, NULL);
      break;
    case 'c':
      /*  Extract catalog name and path  */
      cat = optarg;
      path = strtok(cat, ":");
      path = strtok(NULL, ":");
      break;
    case 'v':
      state.verbose = 1;
      break;
    case 'W':
      writewcs = 1;
      break;
    case '?':
      switch(optopt) {
      default:
	fprintf(stderr, "Unknown option -%c ignored!\n", optopt);
	break;
      }
      break;
    }
  }

  if (starlist == 0) {
    fprintf(stderr, "No file name!\n");
    exit(1);
  }
  /*  In the first file line we look for the image file name  */
  fscanf(fp, "Input file name: %s", fn);
  frame[0].name = strdup(fn);
  /*  Load the image  */
  if (load_frame(frame, 0)) {
    fprintf(stderr, "Can't load %s\n", frame[0].name);
    exit(1);
  }
  if (wcs != 6) {
    if (!frame[0].fitsfile.wcs) {
      fprintf(stderr, "No image coordinates!\n");
      exit(1);
    }
  }
  else {
    if (!frame[0].fitsfile.wcs) {
      frame[0].fitsfile.nax[0] = xw;
      frame[0].fitsfile.nax[1] = yw;
    }
    /*  Write or override image coordinates and pixel dimensions  */
    frame[0].ra = frame[0].fitsfile.crval1 = ra;
    frame[0].dec = frame[0].fitsfile.crval2 = dec;
    frame[0].fitsfile.crpix1 = 0.5 * frame[0].fitsfile.nax[0];
    frame[0].fitsfile.crpix2 = 0.5 * frame[0].fitsfile.nax[1];
    frame[0].xsize = p1; 
    frame[0].ysize = p2; 
    frame[0].fitsfile.cdelt1 = p1 / 3600.0;
    frame[0].fitsfile.cdelt2 = p2 / 3600.0;
    frame[0].fitsfile.crot = 0;
    strcpy(frame[0].fitsfile.ctype, "-TAN");
    frame[0].fitsfile.wcs = 1;
  }
  /*  reserve some space for the star list  */
  nstar = NSTAR;
  frame[0].imagelist.s = (STAR *) myalloc(sizeof(STAR) * nstar, "make_catalog", 
					  "frame[0].star.s");
  /*  Read a star list  */
  /*====================*/
  i = 0;
  while (!feof(fp)) {
    fgets(a, 256, fp);    /*  Read a complete line   */
    n = sscanf(a, "%d %lf %lf %f", &frame[0].imagelist.s[i].n,
	   &frame[0].imagelist.s[i].x, &frame[0].imagelist.s[i].y,
	   &frame[0].imagelist.s[i].mag);
    if (n == 4 && !feof(fp)) {
      i++;
      if (i == nstar) {
	nstar += 250;
	frame[0].imagelist.s = (STAR *) 
	  realloc(frame[0].imagelist.s, sizeof(STAR) * nstar);
	if (frame[0].imagelist.s == NULL) {
	  fprintf(stderr, "Too many stars!");
	  exit(1);
	}
      }
    }
  }
  frame[0].imagelist.n = i;
  fclose(fp);
  if (cat == NULL) {
    fprintf(stderr, "Missing catalog name!\n");
    exit(1);
  }
  /*  Check which catalog we have  */
  /*===============================*/
  if (strcmp(cat, "usno") == 0) {
    catalog[CAT_USNO].use = 1;
    catalog[CAT_USNO].path = strdup(path);
    catalog[CAT_USNO].lpath = strlen(path);

  } 
  else if (strcmp(cat, "gscn") == 0) {
    catalog[CAT_GUIDE].use = 1;
    catalog[CAT_GUIDE].path = strdup(path);
    catalog[CAT_GUIDE].lpath = strlen(path);
  } 
  else if (strcmp(cat, "gscs") == 0) {
    catalog[CAT_GUIDE_SOUTH].use = 1;
    catalog[CAT_GUIDE_SOUTH].path = strdup(path);
    catalog[CAT_GUIDE_SOUTH].lpath = strlen(path);
  } 
  /*  else if (strcmp(cat, "ppm") == 0) {
    catalog[CAT_PPM].use = 1;
    catalog[CAT_PPM].path = strdup(path);
    catalog[CAT_PPM].lpath = strlen(path);
    } */
  else {
    fprintf(stderr, "Unknown catalog\n");
    fprintf(stderr, "Try one of these:\n");
    fprintf(stderr, "  usno -- USNO SA 1.0\n");
    fprintf(stderr, "  gscn -- GSC 1.1 from 90 to -7.5 declination\n");
    fprintf(stderr, "  gscs -- GSC 1.1 from -7.5 to -90 declination\n");
    /*    fprintf(stderr, "  ppm -- PPM\n"); */
    exit(1);
  }
  frame[0].match = match_options;
  /*  Match a list with a star catalog  */
  if (match_stars(frame, 0)) {
    fprintf(stderr, "Matching failed!\n");
    exit(1);
  }
  /*  Output star list  */
  star_list(frame, 0);
  if (writewcs) {
    update_wcs(frame, 0, trglob);
  }
  remove_frame(frame, 0);
}
