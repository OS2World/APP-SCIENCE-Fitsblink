/*  readusno.c  */
/*  (c)  Jure Skvarc  March 1998  */
/* A program which reads stars from the USNO_SA catalog, which are  */
/*    contained in image with center coordinates */
/*  (ra, dec), rotation angle fi  and dimensions (width, height)     */
/*  and outputs their pixel coordinates          */
/*  TAN projection is assumed                    */
#define _POSIX_C_SOURCE  2
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <fitsio.h>

#include "../consts.h"
#include "../structs.h"
#include "../functs.h"

int byte_order[4];

extern STATE state;

/*  Calculate corners of the image  */
/*  Arguments:  */
/*  ra  -  right ascension of center point in dec. degrees  */
/*  dec -  declination of the center point in dec. degrees  */
/*  fi  -  rotation from N through E in degrees  */
/*  width -  width of the image in degrees   */
/*  height - height of the image in degrees  */
/*  coords -  array of 4 corner coordinates  */
/*  return 1 if some error happens  */
int
calculate_corners(MAP_IMAGE *map, STAR *coords)
{
  double xinc, yinc;
  int i;
  int err;
  static double x[] = {0.0, 1.0, 1.0, 0.0};
  static double y[] = {0.0, 0.0, 1.0, 1.0};

  /*  Check height and width  */
  if (map->width <= 0 || map->height <= 0) {
    fprintf(stderr, "Wrong image dimensions: width=%f, height=%f\n", 
	    map->width, map->height);
    return 1;
  }
  /*  Pretend that we have image one pixel wide and high */
  xinc = map->width / map->w;
  yinc = map->height / map->h;
  /*  Therefore center coordinates are 0.5 map->w, 0.5 map->h */
  for (i = 0; i < 4; i++) {
    /*  Sky coordinates of all corners */
    err = 0;
    fits_pix_to_world(x[i] * map->w, y[i] * map->h, map->ra, map->dec, 
		      0.5 * map->w, 0.5 * map->h, 
		      xinc, yinc, map->fi, "-TAN",
		      &(coords[i].ra), &(coords[i].dec), &err);
    if (err) {
      fprintf(stderr, "Wrong transform: x=%f, y=%f\n", x[i], y[i]);
      /*      return 1; */
    }
  }
  return 0;
}

/*  A function which reads an accelerator file name  */
/*  name contains full path to the accelerator file  */
int
read_accelerator(char *name, ACCELERATOR *acc)

{
  FILE *fp;
  int i;
  char a[80];

  if ((fp = fopen(name, "r")) == NULL) {
    sprintf(a, "Can't read accelerator file %s\n", name);
    if (state.interactive) {
      fl_show_alert(a, "", "", 1);
    }
    else {
      fprintf(stderr, a);
    }
    return 1;
  }
  for (i = 0; i < 96; i++) {
    if (fscanf(fp, "%f%d%d\n", &(acc[i].ra), &(acc[i].index), &(acc[i].length)) != 3) {
      sprintf(a, "Failed reading accelerator file %s, line %d\n", name, i + 1);
      if (state.interactive) {
	fl_show_alert(a, "", "", 1); 
      }
      else {
	fprintf(stderr, a);
      }
      return 1;
    }
    /*  Subtract 1 to get a C index instead of Fortran  */
    acc[i].index--;  
  }
  fclose(fp);
  return 0;
}

/*  Read all accelerator files  */
int
get_accelerators(ACCELERATOR acc[24][96], char *path)

{
  char *file;
  int i;
  char a[80];

  /*  Make space for file names  */
  if ((file = malloc(sizeof(char) * (strlen(path) + 13))) == NULL) {
    sprintf(a, "Can't reserve space for file name in get_accelerator\n");
    if (state.interactive) {
      fl_show_alert(a, "", "", 1); 
    }
    else {
      fprintf(stderr, a);
    }
    return 1;
  }
  for (i = 0 ; i < 24; i++) {
    sprintf(file, "%s/zone%04d.acc", path, i * 75);
    if (read_accelerator(file, acc[i])) {
      return 1;
    }
  }
  return 0;
}

/*  Find the index of a star which has right ascension less than ra  */
int
find_lowindex(double ra, ACCELERATOR *acc)

{
  int ind;

  ind = (ra / 15.0 - 1e-9) / USNO_RADIST;
  return acc[ind].index ;  
}


/*  Find the index of a star which has right ascension more than ra  */
int
find_highindex(double ra, ACCELERATOR *acc)

{
  int ind;

  ind = (ra / 15.0 - 1e-9) / USNO_RADIST;
  return acc[ind].index + acc[ind].length;  
}

/*   Convert integers in USNO_STAR structure from big_endian format */
/*   to the one used by this machine  */
void
convert_integers(USNO_STAR *star, int *ra, int *dec, int *mag)

{
  *ra = star->a[3] + 256 * (star->a[2] + 256 * (star->a[1] + 256 * star->a[0]));
  *dec = star->a[7] + 256 * (star->a[6] + 256 * (star->a[5] + 256 * star->a[4]));
  *mag = star->a[11] + 256 * (star->a[10] + 256 * (star->a[9] + 256 * star->a[8]));
}


/*  transforms s list of stars in USNO format into a list in "STAR" format */
/*  it copies only those which fall within image limits  */
/*  return 1 if it runs out of space  */
int
extract_usno_stars(USNO_STAR *starbuf, int nrec, STAR *starlist, MAP_IMAGE *map)

{
  int i, nstar, err;
  char a[12];
  double ra, dec;
  double xpix, ypix, xinc, yinc;
  int ira, idec, imag;
  int wmin, hmin, wmax, hmax;

  nstar = map->nm;
  xinc = map->width / map->w;
  yinc = map->height / map->h;
  wmin = -0.2 * map->w;
  hmin = -0.2 * map->h;
  wmax =  1.2 * map->w;
  hmax =  1.2 * map->h;
  for (i = 0; i < nrec; i++) {
    convert_integers(&starbuf[i], &ira, &idec, &imag);
    ra = ira / 360000.0;
    dec = idec / 360000.0 - 90.0;
    /*  Check if the star falls into the image  */
    err = 0;
    fits_world_to_pix(ra, dec, map->ra, map->dec, 0.5 * map->w, 0.5 * map->h, 
		xinc, yinc, map->fi, "-TAN", 
		&xpix, &ypix, &err);
    if (!err && xpix >= wmin && xpix < wmax && ypix >= hmin && ypix < hmax) {
      sprintf(a, "%+011d", imag);
      starlist[nstar].redmag = strtol(a + 8, NULL, 10) / 10.0;
      a[8] = '\0';
      starlist[nstar].bluemag = strtol(a + 5, NULL, 10) / 10.0;
      if (starlist[nstar].redmag > map->minmag && 
	  starlist[nstar].redmag < map->maxmag) {
	starlist[nstar].catalog = 'u';
	starlist[nstar].n = nstar;
	starlist[nstar].ra = ra;
	starlist[nstar].dec = dec;
	starlist[nstar].x = xpix - 1;
	starlist[nstar].y = ypix - 1;
	starlist[nstar].mag = pow(10.0, -0.4 * starlist[nstar].redmag);
	starlist[nstar].q = a[1] - 48;
	starlist[nstar].m = -1;
	nstar++;
	if (nstar == MAXSTARNUM) {
	  map->nm = nstar;
	  return 1;
	}
      }
    }
  }
  map->nm = nstar;
  return 0;
}


/*  Test for byte order of this machine  */
void
get_byte_order(void)

{
  long t;
  char *a;
  int i;

  t = 0 + 256 * (1 + 256 * (2 + 256 * 3));
  a = (char *) &t;
  for (i = 0; i < 4; i++) {
    byte_order[i] = a[i];
  }
}



int
get_usno_stars(MAP_IMAGE *map, STAR *starlist, char *catalog_path) 

{
  STAR corner[4];
  double declow, dechigh;
  double ralow[2], rahigh[2];
  int rarange;
  double xpix, ypix;
  int i, j;
  int nrec;
  int err;
  ACCELERATOR acc[24][96];
  int zonelow, zonehigh;
  char *file;
  int lowindex, highindex, ranum;
  USNO_STAR *starbuf;
  FILE *fp;
  char a[80];

  get_byte_order();
  /*  Reserve space for the file name  */
  if ((file = malloc(sizeof(char) * (strlen(catalog_path) + 13))) == NULL) {
    fprintf(stderr, "Can't reserve space for file name in main\n");
    return 1;
  }
  /*  Reserve space for the buffer  */
  if ((starbuf = (USNO_STAR *) malloc(sizeof(USNO_STAR) * BUFSIZE)) == NULL) {
    fprintf(stderr, "Can't reserve space for star buffer in main\n");
    return 1;
  }
  if (get_accelerators(acc, catalog_path)) return 1;
  if (calculate_corners(map, corner)) return 1;
  /*  Find the highest and the lowest declination value  */
  declow = dechigh = corner[0].dec;
  ralow[0] = rahigh[0] = corner[0].ra;
  for (i = 1; i < 4; i++) {
    if (corner[i].dec < declow) declow = corner[i].dec;
    if (corner[i].dec > dechigh) dechigh = corner[i].dec;
    if (corner[i].ra < ralow[0]) ralow[0] = corner[i].ra;
    if (corner[i].ra > rahigh[0]) rahigh[0] = corner[i].ra;
  }
  /* Check if the north pole falls inside the rectangle  */
  err = 0;
  fits_world_to_pix(0.0, 90.0, map->ra, map->dec, 0.5 * map->w, 0.5 * map->h, 
	      map->width, map->height, map->fi, "-TAN", 
	      &xpix, &ypix, &err);
  if (!err && (xpix >= 0.0 && xpix <= 1.0 && ypix >= 0.0 && ypix <= 1.0)) {
      ralow[0] = 0;
      rahigh[0] = 360;
      rarange = 1;
      dechigh = 90.0;
  }
  else {
    /* Check if the south pole falls inside the rectangle  */
    err = 0;
    fits_world_to_pix(0.0, -90.0, map->ra, map->dec, 0.5 * map->w, 0.5 * map->h, 
		map->width, map->height, map->fi, "-TAN", 
		&xpix, &ypix, &err);
    if (!err && (xpix >= 0.0 && xpix <= 1.0 && ypix >= 0.0 && ypix <= 1.0)) {
      ralow[0] = 0;
      rahigh[0] = 360;
      rarange = 1;
      declow = -90.0;
    }
    else {
      if (rahigh[0] - ralow[0] >= 180) {
	ralow[1] = rahigh[0];
	rahigh[1] = 360;
	rahigh[0] = ralow[0];
	ralow[0] = 0;
	rarange = 2;
      }
      else {
	rarange = 1;
      }
    }
  }


  /* convert declination to South Polar Distance (SPD) */
  declow += 90;
  dechigh += 90;  
  /*  Calculate indices for declination zones  */
  zonelow = (int) (declow / USNO_DECWIDTH);
  zonehigh = (int) (dechigh / USNO_DECWIDTH - 1e-9);
  /*  map->nm counts stars which fall in our image  */
  for (i = zonelow; i <= zonehigh; i++) {
    /*  Construct catalog file name  */
    sprintf(file, "%s/zone%04d.cat", catalog_path, i * 75);
    /*  Open catalog file */
    if ((fp = fopen(file, "r")) == NULL) {
      sprintf(a, "Can't open catalog file %s\n", file);
      if (state.interactive) {
	fl_show_alert(a, "", "", 1);
      }
      else {
	fprintf(stderr, a);
      }
      return 1;
    }
    for (j = 0; j < rarange; j++) {
      /*  Find lower and upper bound for indices for ra range  */
      lowindex = find_lowindex(ralow[j], acc[i]);
      highindex = find_highindex(rahigh[j], acc[i]);
      /*  Number of stars between the boundaries  */
      ranum = highindex - lowindex;
      lowindex *= USNO_RECORDLENGTH;
      /*  Go to the position of the first record  */
      fseek(fp, lowindex, SEEK_SET);
      do {
	nrec = min(BUFSIZE, ranum);
	/*  Read star data in chunks of no more than BUFSIZE  */
	err = fread(starbuf, sizeof(USNO_STAR), nrec, fp);
	if (err != nrec) {
	  sprintf(a, "Error in reading file %s\n", file);
	  if (state.interactive) {
	    fl_show_alert(a, "", "", 1);
	  }
	  else {
	    fprintf(stderr, a);
	  }
	  return 1;
	}
	if (map->nm < MAXSTARNUM) {
	  extract_usno_stars(starbuf, nrec, starlist, map);
	}
	ranum -= nrec;
      } while(ranum > 0);
    }
    fclose(fp);
  }
  free(starbuf);
  free(file);
  return 0;
}
