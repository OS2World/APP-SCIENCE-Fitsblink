/*  fits.c  for Fitsblink 1.2 */
#ifdef HPUX
#define _INCLUDE_HPUX_SOURCE
#define _INCLUDE_XOPEN_SOURCE
#define _INCLUDE_POSIX_SOURCE
#endif
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <fitsio.h>
#include "functs.h"
#include "consts.h"

extern OBSERVATORY observatory;
/* 
    The datatype parameter can have one of the following symbolic constant 
    values: TSTRING, TLOGICAL, TBYTE, TSHORT, TINT, TLONG, TFLOAT, TDOUBLE.
*/
int
read_fits(char *name, PIXEL **image1, int *xmax, int *ymax,  FITSFILE *f, 
	  char **errors)
     
{
  int pixelsize;
  int imagesize;
  
  int status = 0;
  int nfound, anyf;
  char error[81], comment[81], temp[81];
  int verbose = 0;
  int i;
  PIXEL *image;
  int errnum = 0;
  
  /*  open the file  */
  fits_open_file(&f->fptr, name, READONLY, &status);
  if (status != 0)  {
    do  {
      fits_read_errmsg(error);
      errors[errnum++] = strdup(error);
    } while(strlen(error) > 0 && errnum < ERROR_MAX - 1);
    if (errnum == ERROR_MAX - 1) {
      errors[errnum] = "And there may be other problems, too";
    }
    return errnum;
  }
  /*  Check if the file is simple fits file  */
  fits_read_key_log(f->fptr, "SIMPLE", &f->simple, comment, &status);
  if (verbose)  {
    printf("SIMPLE=%c\n", f->simple ? 'T' : 'F');
  }
  if (!f->simple) {
    errors[errnum] = "I am not supposed to handle nonsimple files";
    return errnum;
  }
  /*  Get depth of a pixel */
  fits_read_key_lng(f->fptr, "BITPIX", &f->bitpix, comment, &status);
  if (verbose)  {
    printf("BITPIX=%ld\n", f->bitpix);
  }
  /*  Read number of axis  */
  fits_read_key_lng(f->fptr, "NAXIS", &f->naxis, comment, &status);
  if (verbose)  {
    printf("NAXIS=%ld\n", f->naxis);
  }
  if (f->naxis != 2)  {
    errors[errnum++] = "At the moment, I can handle only files with 2 axis";
    sprintf(temp, "File %s has %ld axis\n", name, f->naxis);
    errors[errnum++] = strdup(temp);
    do  {
      fits_read_errmsg(error);
      errors[errnum++] = strdup(error);
    } while(strlen(error) > 0 && errnum < ERROR_MAX - 1);
    if (errnum == ERROR_MAX - 1) {
      errors[errnum] = "And there may be other problems, too.";
    }
    return errnum;
  }
  /*  Read dimensions at once  */
  fits_read_keys_lng(f->fptr, "NAXIS", 1, f->naxis, f->nax, &nfound, &status);
  if (verbose)  {
    for (i = 0; i < f->naxis; i++)  {
      printf("NAXIS%d=%ld\n", i + 1, f->nax[i]);
    }
  }
  /*  Size of pixel in bytes  */
  pixelsize = sizeof(PIXEL);
  /*  Determine number of pixels  */
  imagesize = 1;
  for (i = 0; i < f->naxis; i++) imagesize *= f->nax[i];
  if (status != 0) {
    do  {
      fits_read_errmsg(error);
      errors[errnum++] = strdup(error);
    } while(strlen(error) > 0 && errnum < ERROR_MAX - 1);
    if (errnum == ERROR_MAX - 1) {
      errors[errnum] = "And there may be other problems, too.";
    }
  }
  /*============================================================*/
  /*  Try to read WCS coordinates                               */
  /*============================================================*/
  status = 0;
  if (verbose)  {
    printf("Reading WCS\n");
  }
  fits_read_img_coord(f->fptr,  &f->crval1, &f->crval2, 
       &f->crpix1, &f->crpix2, &f->cdelt1, &f->cdelt2,
       &f->crot, f->ctype, &status);
  /*  Check if everything about WCS was read properly  */
  f->wcs = (status == 0);
  if (f->wcs) {
    /*  if ctype is empty, assume -TAN  */
    if (strlen(f->ctype) == 0) {
      strcpy(f->ctype, "-TAN");
    }
    /*  Read CTYPE1 keyword  */
    status = 0;
    fits_read_keyword(f->fptr, "CTYPE1", f->ctype1, comment, &status);
    /*  if CTYPE1 is DEC, than exchange the crval1 and crval2 values  */
    if (strncmp(f->ctype1 + 1, "DEC", 3) == 0) {
      double t;
      t = f->crval1;
      f->crval1 = f->crval2;
      f->crval2 = t;
    }
  }
  /*============================================================*/
  /*  Get date and time of observation and exposure time        */
  /*============================================================*/
  status = 0;
  fits_read_keyword(f->fptr, "DATE-OBS", f->date_obs, comment, &status);
  f->is_date_obs = (status == 0);   /* Flag success on reading date  */
  status = 0;
  fits_read_key(f->fptr, TFLOAT, "EXPOSURE", &(f->exposure), comment, &status);
  f->is_exposure = (status == 0);   /* Flag success on reading exposure time  */
  if (status != 0) {
    /*  Try again with a nonstandard keyword  */
    status = 0;
    fits_read_key(f->fptr, TFLOAT, "EXPTIME", &(f->exposure), comment, &status);
    f->is_exposure = (status == 0);   /* Flag success on reading exposure time  */
  }
  status = 0;
  fits_read_keyword(f->fptr, "UT-START", f->ut_start, comment, &status);
  f->is_ut_start = (status == 0);   /* Flag success on reading start-of-exposure time  */
  status = 0;
  fits_read_keyword(f->fptr, "UT-CENT", f->ut_cent, comment, &status);
  f->is_ut_cent = (status == 0);   /* Flag success on reading mid-exposure time  */
  status = 0;
  fits_read_keyword(f->fptr, "UT-END", f->ut_end, comment, &status);
  f->is_ut_end = (status == 0);   /* Flag success on reading end-of-exposure time  */
  status = 0;
  fits_read_keyword(f->fptr, "TIME-BEG", f->time_beg, comment, &status);
  f->is_time_beg = (status == 0);   /* Flag success on reading start-of-exposure time  */
  status = 0;
  fits_read_keyword(f->fptr, "TIME-END", f->time_end, comment, &status);
  f->is_time_end = (status == 0);   /* Flag success on reading start-of-exposure time  */
  /*============================================================*/
  /*  Get observer, instrument and telescope                    */
  /*  If they are not present, insert values from the settings  */
  /*  file                                                      */
  /*============================================================*/
  status = 0;
  fits_read_keyword(f->fptr, "OBSERVER", f->observer, comment, &status);
  if (status != 0) {
    strcpy(f->observer, observatory.observer);
    status = 0;
  }
  else {
    strcpy(observatory.observer, f->observer);
  }
  fits_read_keyword(f->fptr, "TELESCOP", f->telescope, comment, &status);
  if (status != 0) {
    strcpy(f->telescope, observatory.telescope);
    status = 0;
  }
  else {
    strcpy(observatory.telescope, f->telescope);
  }
  fits_read_keyword(f->fptr, "INSTRUME", f->instrument, comment, &status);
  if (status != 0) {
    strcpy(f->instrument, observatory.instrument);
    status = 0;
  }
  /*===================================================*/
  /*  Read the object name                             */
  /*===================================================*/
  fits_read_keyword(f->fptr, "OBJECT", f->object, comment, &status);
  /*  Erase object  */
  f->object[0] = '\0';
  if (status != 0) {
    f->object[0] = '\0';
    status = 0;
  }
  /*================================================================*/
  /*  Read in the image                                             */
  /*================================================================*/
  status = 0;
  /*  Allocate memory for the image  */
  if (verbose)  {
    printf("Allocating space for the image\n");
  }
  image = (PIXEL *) myalloc(pixelsize * imagesize, "read_fits", "image");
  /*  Transfer the image from disk to memory */
  if (verbose)  {
    printf("Reading the image\n");
  }
  fits_read_2d_sht(f->fptr, 0, 0, f->nax[0], f->nax[0], f->nax[1], image, &anyf, &status); 
  if (verbose)  {
    printf("Image read\n");
  }
  if (status == NUM_OVERFLOW && f->bitpix <= 16) {
    /*  Image reading did not succeed due to numerical overflow  */
    double bscale, bzero = 0;
    status = 0;
    /*  Find out bscale parameter  */
    fits_read_key(f->fptr, TDOUBLE, "BSCALE", &bscale, comment, &status);
    /*  Try again with a BZERO set to 0  */
    fits_set_bscale(f->fptr, bscale, bzero, &status); 
    fits_read_2d_sht(f->fptr, 0, 0, f->nax[0], f->nax[0], f->nax[1], image, &anyf, &status);
    for (i = 0; i < f->nax[0] * f->nax[1]; i++) {
      image[i] -= 32768;
    }
  }
  /*  Close the file  */
  fits_close_file(f->fptr, &status);
  /*	process();  */
  *xmax = f->nax[0];
  *ymax = f->nax[1];
  *image1 = image;
  if (status != 0) {
    do  {
      fits_read_errmsg(error);
      errors[errnum++] = strdup(error);
    } while(strlen(error) > 0 && errnum < ERROR_MAX - 1);
    if (errnum == ERROR_MAX - 1) {
      errors[errnum] = "And there may be other problems, too.";
    }
  }
  if (verbose)  {
    printf("End of read_fits\n");
  }
  return errnum;
}



