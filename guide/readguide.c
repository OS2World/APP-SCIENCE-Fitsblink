/*  readguide.c  */
/*  (c)  Jure Skvarc  March 1998  */
/* A program which reads stars from the GUIDE catalog, which are  */
/*    contained in image with center coordinates */
/*  (ra, dec), rotation angle fi  and dimensions (width, height)     */
/*  and outputs their pixel coordinates          */
/*  TAN projection is assumed                    */
#define _POSIX_C_SOURCE  2
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../consts.h"
#include "../structs.h"
#include "../functs.h"

extern STATE state;

/*  transforms s list of stars in GUIDE format into a list in "STAR" format */
/*  it copies only those which fall within image limits  */
/*  returns 1 if it runs out of space, 2 if user presses cancel  */
int
extract_guide_stars(char *name, STAR *starlist, MAP_IMAGE *image, int plate)

{
  int i, nstar, err, n;
  double xpix, ypix, xinc, yinc;
  GUIDE_STAR *star;
  char *p;
  int wmin, hmin, wmax, hmax;
  int prevn;

  nstar = image->nm;
  xinc = image->width / image->w;
  yinc = image->height / image->h;
  /*  Read all of the stars in file name  */
  i = 0;
  wmin = -0.3 * image->w;
  hmin = -0.3 * image->h;
  wmax =  1.3 * image->w;
  hmax =  1.3 * image->h;
  while (read_sky_region(name, &star, &n, plate) && i != 2) {
    char a[80];
    p = strstr(name, ".gsc") - 10;
    sprintf(a, "Insert CD ROM with %s sky!\n", 
	   (*p == 'n' || (*p == 's' && *(p + 2) == 0)) ? "northern" : "southern");
    if (state.interactive) {
      i = fl_show_choices(a, 2, "OK", "Cancel", "", 1);
    }
    else {
      fprintf(stderr, a);
      getchar();
    }
  }
  if (i == 2) return 2;
  prevn = -1;
  for (i = 0; i < n; i++) {
    /*  Check if the star falls into the image  */
    err = 0;
    fits_world_to_pix(star[i].ra, star[i].dec, image->ra, image->dec, 
		0.5 * image->w, 0.5 * image->h, 
		xinc, yinc, image->fi, "-TAN", 
		&xpix, &ypix, &err);
    if (!err && xpix >= wmin && xpix < wmax && ypix >= hmin && ypix < hmax) {
      if (star[i].mag > image->minmag && star[i].mag < image->maxmag &&
	  star[i].class == 0 /*  Accept only object classified as stars  */
	  && star[i].poserr < 1.0  /*  Do not accept anything with pos. error > 1" */) {
	starlist[nstar].n = star[i].plate * 10000 + star[i].id;
	if (starlist[nstar].n != prevn) {
	  prevn = starlist[nstar].n;
	  starlist[nstar].catalog = 'g';
	  starlist[nstar].m = -1;
	  starlist[nstar].ra = star[i].ra;
	  starlist[nstar].dec = star[i].dec;
	  starlist[nstar].mag = pow(10.0, -0.4 * star[i].mag);
	  starlist[nstar].x = xpix - 1;
	  starlist[nstar].y = ypix - 1;
	  starlist[nstar].q = star[i].class;
	  starlist[nstar].fitmag = star[i].magband;
	  nstar++;
	  if (nstar == MAXSTARNUM) {
	    image->nm = nstar;
	    return 1;
	  }
	}
      }
    }
  }
  free(star);
  image->nm = nstar;
  return 0;
}



int
get_guide_stars(MAP_IMAGE *image, STAR *starlist, 
		char *north_path, char *south_path)

{
  STAR corner[4];
  double declow, dechigh;
  double ralow[2], rahigh[2];
  double ramin, ramax, decmin, decmax;
  double rawidth, rastart, rastep;
  int rarange;
  double xpix, ypix;
  int i, j, k;
  int i1, i2, ind;
  int err;
  int zonelown = 0, zonehighn = 0, zonelows = 0, zonehighs = 0;
  int fsmall;
  char *file;
  int lowindex, highindex;
  int cd;
  LARGE_REGION large[24];
  SMALL_REGION small[732];
  char *lpath, *spath;

  /*  Directory names  */
  char *northdir[] = {"n0000", "n0730", "n1500", "n2230",
		      "n3000", "n3730", "n4500", "n5230",
		      "n6000", "n6730", "n7500", "n8230",
		      "s0000"};
  char *southdir[] = {"s0000", "s0730", "s1500", "s2230",
		      "s3000", "s3730", "s4500", "s5230",
		      "s6000", "s6730", "s7500", "s8230"};

  /*  image->nm counts stars which fall into our image  */
  calculate_corners(image, corner);
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
  fits_world_to_pix(0.0, 90.0, image->ra, image->dec, 
		    0.5 * image->w, 0.5 * image->h, 
	      image->width, image->height, image->fi, "-TAN", 
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
    fits_world_to_pix(0.0, -90.0, image->ra, image->dec, 
		      0.5 * image->w, 0.5 * image->h, 
		      image->width, image->height, image->fi, "-TAN", 
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

  if (declow >= 0) {
    zonelown = (int) (declow / GUIDE_DECWIDTH);
    zonehighn = (int) (dechigh / GUIDE_DECWIDTH - 1e-9);
    cd = 1;    /*  Only use north pole cd  */
  }
  else if (declow > -GUIDE_DECWIDTH) {
    zonelown = -1;
    zonehighn = (int) (dechigh / GUIDE_DECWIDTH - 1e-9);
    cd = 1;    /*  Only use north pole cd  */
  } 
  else if (dechigh <= -GUIDE_DECWIDTH) {
    /* South hemisphere */
    zonelows = (int) (fabs(dechigh) / GUIDE_DECWIDTH) + 12;
    zonehighs = (int) (fabs(declow) / GUIDE_DECWIDTH - 1e-9) + 12;
    cd = 2;
  } else {
    /*  Both hemispheres  */
    zonelown = -1;
    zonehighn = (int) (dechigh / GUIDE_DECWIDTH - 1e-9);
    zonelows = 13;
    zonehighs = (int) (fabs(declow) / GUIDE_DECWIDTH - 1e-9) + 12;
    cd = 3;
  }
  /*  first extract stars from the northern sky  */
  /*  Calculate indices for declination zones  */
  /*  North hemisphere first  */
  if (cd & 1) {
    lpath = (char *) myalloc(sizeof(char) * (strlen(north_path) + 
					     strlen("/tables/lg_reg_x.tbl")) + 1,
			     "readguide", "lpath");
    spath = (char *) myalloc(sizeof(char) * (strlen(north_path) + 
					     strlen("/tables/sm_reg_x.tbl")) + 1,
			     "readguide", "spath");
    sprintf(lpath, "%s/tables/lg_reg_x.tbl", north_path);
    sprintf(spath, "%s/tables/sm_reg_x.tbl", north_path);
    if (read_large_regions(lpath, large)) {
      if (state.interactive) {
	fl_show_alert("Error in reading lg_reg_x.tbl (north)", "Insert the GSC CDROM or write the correct path!", "", 0);
      }
      else {
	fprintf(stderr, "Error in reading lg_reg_x.tbl (north)!\nInsert the GSC CDROM or write the correct path!\n");
      }
      return 1;
    }
    if (read_small_regions(spath, small)) {
      if (state.interactive) {
	fl_show_alert("Error in reading sm_reg_x.tbl (north)", "Insert the GSC CDROM or write the correct path!", "", 0);
      }
      else {
	fprintf(stderr, "Error in reading sm_reg_x.tbl (north)!\nInsert the GSC CDROM or write the correct path!\n");
      }
      return 1;
    }
    /*  Reserve space for the file name  */
    file = (char *) myalloc(sizeof(char) * (strlen(north_path) + 50), 
			  "readguide", "file");
    for (i = zonelown; i <= zonehighn; i++) {
      if (i == -1) ind = 12;
      else ind = i;
      /*  ind is an index to large regions table */      
      for (j = 0; j < rarange; j++) {
	/*  Find lower and upper bound for large regions numbers */
	rawidth = 360.0 / large[ind].numlarge;
	lowindex = ralow[j] / rawidth + large[ind].firstlarge - 1;
	highindex = rahigh[j] / rawidth + large[ind].firstlarge - 1 - 1e-9;
	for (k = lowindex; k <= highindex; k++) {
	  fsmall = small[k].firstsmall;
	  rastart = (k + 1 - large[ind].firstlarge) * rawidth;
	  rastep =  rawidth / (double) small[k].depth;
	  /*  For every subdivision (small region),   */
	  /*  check if any star falls inside  */
	  for (i1 = 0; i1 < small[k].depth; i1++) {
	    if (i == -1) {
	      decmin = (i + (small[k].depth - 1 - i1) / (double) small[k].depth) * GUIDE_DECWIDTH;
	      decmax = decmin + GUIDE_DECWIDTH / small[k].depth;
	    }
	    else {
	      decmin = (i + i1 / 
		      (double) small[k].depth) * GUIDE_DECWIDTH;
	      decmax = decmin + GUIDE_DECWIDTH / small[k].depth;
	    }
	    if (!(decmax < declow || decmin > dechigh)) {
	      for (i2 = 0; i2 < small[k].depth; i2++) {
		ramin = rastart + i2 * rastep;
		ramax = ramin + rastep;
		if (!(ramax < ralow[j] || ramin > rahigh[j])) {
		  int plate = fsmall + i2 + i1 * small[k].depth;
		  sprintf(file, "%s/gsc/%s/%04d.gsc", 
			  north_path, northdir[ind],
			  plate);
		  if (image->nm < MAXSTARNUM) {
		    int k = extract_guide_stars(file, starlist, image, plate);
		    if (k > 0) return 1;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    free(file);
    free(spath);
    free(lpath);
  }

  /*  Southern sky */
  if (cd & 2) {
    lpath = (char *) myalloc(sizeof(char) * (strlen(south_path) + 
					     strlen("/tables/lg_reg_x.tbl")) + 1,
			     "readguide", "lpath");
    spath = (char *) myalloc(sizeof(char) * (strlen(south_path) + 
					     strlen("/tables/sm_reg_x.tbl")) + 1,
			     "readguide", "spath");
    sprintf(lpath, "%s/tables/lg_reg_x.tbl", south_path);
    sprintf(spath, "%s/tables/sm_reg_x.tbl", south_path);
    if (read_large_regions(lpath, large)) {
      if (state.interactive) {
	fl_show_alert("Error in reading lg_reg_x.tbl (south)", "Insert the GSC CDROM or write the correct path!", "", 0);
      }
      else {
	fprintf(stderr, "Error in reading lg_reg_x.tbl (south)!\nInsert the GSC CDROM or write the correct path!\n");
      }
      return 1;
    }
    if (read_small_regions(spath, small)) {
      if (state.interactive) {
	fl_show_alert("Error in reading sm_reg_x.tbl (south)", "Insert the GSC CDROM or write the correct path!", "", 0);
      }
      else {
	fprintf(stderr, "Error in reading sm_reg_x.tbl (south)!\nInsert the GSC CDROM or write the correct path!\n");
      }
      return 1;
    }
    /*  Reserve space for the file name  */
    file = (char *) myalloc(sizeof(char) * (strlen(south_path) + 50), 
			  "readguide", "file");


    for (i = zonelows; i <= zonehighs; i++) {
      ind = i;
      /*  ind is an index to large regions table */      
      for (j = 0; j < rarange; j++) {
	/*  Find lower and upper bound for large regions numbers */
	rawidth = 360.0 / large[ind].numlarge;
	lowindex = ralow[j] / rawidth + large[ind].firstlarge - 1;
	highindex = rahigh[j] / rawidth + large[ind].firstlarge - 1 - 1e-9;
	for (k = lowindex; k <= highindex; k++) {
	  fsmall = small[k].firstsmall;
	  rastart = (k + 1 - large[ind].firstlarge) * rawidth;
	  rastep =  rawidth / (double) small[k].depth;
	  /*  For every subdivision (small region),   */
	  /*  check if any star falls inside  */
	  for (i1 = 0; i1 < small[k].depth; i1++) {
	    decmin = (11 - i + (small[k].depth - 1 - i1) / (double) small[k].depth) * GUIDE_DECWIDTH;
	    decmax = decmin + GUIDE_DECWIDTH / small[k].depth;
	    if (!(decmax < declow || decmin > dechigh)) {
	      for (i2 = 0; i2 < small[k].depth; i2++) {
		ramin = rastart + i2 * rastep;
		ramax = ramin + rastep;
		if (!(ramax < ralow[j] || ramin > rahigh[j])) {
		  int plate = fsmall + i2 + i1 * small[k].depth;
		  sprintf(file, "%s/gsc/%s/%04d.gsc", 
			  south_path, southdir[ind - 12],
			  plate);
		  if (image->nm < MAXSTARNUM) {
		    int k = extract_guide_stars(file, starlist, image, plate);
		    if (k > 0) return 1;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    free(file);
    free(spath);
    free(lpath);
  }
  return 0;
}




