/*  fits.c  for Fitsblink 1.2 */
#ifdef HPUX
#define _INCLUDE_HPUX_SOURCE
#define _INCLUDE_XOPEN_SOURCE
#define _INCLUDE_POSIX_SOURCE
#endif
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include <fitsio.h>
#include "../functs.h"

/*  Read the file lg_reg_x.tbl  */
/*==============================*/
int
read_large_regions(char *name, LARGE_REGION *large)
     
{
  int status = 0;
  char comment[81];
  int verbose = 0, any;
  int i;
  short tsht[24];
  double tdoub[24];
  fitsfile *fptr;
  int simple, type;

  /*  open the file  */
  fits_open_file(&fptr, name, READONLY, &status);
  if (status != 0)  {
    return status;
  }
  /*  Check if the file is simple fits file  */
  fits_read_key_log(fptr, "SIMPLE", &simple, comment, &status);
  if (verbose)  {
    printf("SIMPLE=%c\n", simple ? 'T' : 'F');
  }
  if (!simple) {
    return status;
  }
  /*  Move to the first extension  */
  fits_movabs_hdu(fptr, 2, &type, &status);
  /*   Read centers of declination bands (first column) */
  fits_read_col_dbl(fptr, 1, 1, 1, 24, 0.0, tdoub, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 24; i++) {
    large[i].center = tdoub[i];
  }
  
  /*   Read numbers of large regions in declination bands (2. column) */
  fits_read_col_sht(fptr, 2, 1, 1, 24, 0, tsht, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 24; i++) {
    large[i].numlarge = tsht[i];
  }
  
  /*   Read numbers of large regions in declination bands (3. column) */
  fits_read_col_sht(fptr, 3, 1, 1, 24, 0, tsht, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 24; i++) {
    large[i].firstlarge = tsht[i];
  }
  
  fits_close_file(fptr, &status);
  return status;
}


/*  Read the file lg_reg_x.tbl  */
/*==============================*/
int
read_small_regions(char *name, SMALL_REGION *small)
     
{
  int status = 0;
  char comment[81];
  int verbose = 0, any;
  int i;
  short tsht[732];
  fitsfile *fptr;
  int simple, type;

  /*  open the file  */
  fits_open_file(&fptr, name, 0, &status);
  if (status != 0)  {
    return status;
  }
  /*  Check if the file is simple fits file  */
  fits_read_key_log(fptr, "SIMPLE", &simple, comment, &status);
  if (verbose)  {
    printf("SIMPLE=%c\n", simple ? 'T' : 'F');
  }
  if (!simple) {
    return status;
  }
  /*  Move to the first extension  */
  fits_movabs_hdu(fptr, 2, &type, &status);
  /*   Read centers of declination bands (first column) */
  fits_read_col_sht(fptr, 1, 1, 1, 732, 0, tsht, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 732; i++) {
    small[i].largeid = tsht[i];
  }
  
  /*   Read numbers of small regions in declination bands (2. column) */
  fits_read_col_sht(fptr, 2, 1, 1, 732, 0, tsht, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 732; i++) {
    small[i].firstsmall = tsht[i];
  }
  
  /*   Read numbers of small regions in declination bands (3. column) */
  fits_read_col_sht(fptr, 3, 1, 1, 732, 0, tsht, &any, &status);
  if (status != 0) {
    return status;
  }
  for (i = 0; i < 732; i++) {
    small[i].depth = tsht[i];
  }
  
  fits_close_file(fptr, &status);
  return status;
}

/*  Read the file lg_reg_x.tbl  */
/*==============================*/
int
read_sky_region(char *name, GUIDE_STAR **star, int *n, int plate)
     
{
  int status = 0;
  char comment[81];
  int verbose = 0, any;
  int i, j;
  long *tint;
  long nrows, tsize, crow, toread;
  double *tdoub;
  fitsfile *fptr;
  int simple, type;

  /*  open the file  */
  fits_open_file(&fptr, name, 0, &status);
  if (status != 0)  {
    return status;
  }
  /*  Check if the file is simple fits file  */
  fits_read_key_log(fptr, "SIMPLE", &simple, comment, &status);
  if (verbose)  {
    printf("SIMPLE=%c\n", simple ? 'T' : 'F');
  }
  if (!simple) {
    return status;
  }
  /*  Move to the first extension  */
  fits_movabs_hdu(fptr, 2, &type, &status);
  /*  Read the total number of rows  */
  fits_read_key_lng(fptr, "NAXIS2", &nrows, comment, &status);
  /*  Allocate memory for star list  */
  if ((*star = (GUIDE_STAR *) malloc(sizeof(GUIDE_STAR) * nrows)) == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(1);
  }
  /*  Determine the optimum number of rows to be read at once  */
  fits_get_rowsize(fptr, &tsize, &status);
  /*  Allocate memory for tables  */
  if ((tdoub = malloc(sizeof(double) * tsize)) == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(1);
  }
  if ((tint = malloc(sizeof(int) * tsize)) == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(1);
  }
  crow = 1;
  j = 0;
  toread = nrows;
  if (toread < tsize) tsize = toread;
  while (crow <= nrows) {
    /*  Read GSC ID's  */
    fits_read_col_lng(fptr, 1, crow, 1, tsize, 0, tint, &any, &status);
    /*  Read ra (in decimal degrees)  */
    fits_read_col_dbl(fptr, 2, crow, 1, tsize, 0, tdoub, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].plate = plate;
      (*star)[j].id = tint[i];
      (*star)[j].ra = tdoub[i];
      j++;
    }
    j -= tsize;
    /*  Read dec (in decimal degrees)  */
    fits_read_col_dbl(fptr, 3, crow, 1, tsize, 0, tdoub, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].dec = tdoub[i];
      j++;
    }
    j -= tsize;
    /*  Read position error (in arc seconds)  */
    fits_read_col_dbl(fptr, 4, crow, 1, tsize, 0, tdoub, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].poserr = tdoub[i];
      j++;
    }
    j -= tsize;
    /*  Read magnitude  */
    fits_read_col_dbl(fptr, 5, crow, 1, tsize, 0, tdoub, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].mag = tdoub[i];
      j++;
    }
    j -= tsize;
    /*  Read magnitude error */
    fits_read_col_dbl(fptr, 6, crow, 1, tsize, 0, tdoub, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].magerr = tdoub[i];
      j++;
    }
    j -= tsize;
    /*  Read magband  */
    fits_read_col_lng(fptr, 7, crow, 1, tsize, 0, tint, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].magband = tint[i];
      j++;
    }
    j -= tsize;
    /*  Read classification  */
    fits_read_col_lng(fptr, 8, crow, 1, tsize, 0, tint, &any, &status);
    for (i = 0; i < tsize; i++) {
      (*star)[j].class = tint[i];
      j++;
    }
    crow += tsize;
    toread -= tsize;
    if (toread < tsize) tsize = toread;
  }
  if (status != 0) {
    return status;
  }
  *n = crow - 1;
  free(tint);
  free(tdoub);
  fits_close_file(fptr, &status);
  return status;
}
