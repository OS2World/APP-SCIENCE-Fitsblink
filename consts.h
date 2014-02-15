/*  consts.h  for Fitsblink 2.0 */
#ifndef _blink_consts
#define _blink_consts
#include "version.h"
#define NGAMMA 5
#define FALSE 0
#define TRUE 1

/*  Conversion factor from degrees to radians  */
#define DEGRAD 1.74532925199e-2
#define RADDEG 57.2957795132
/*  Initial number of frames  */
#define FRAME_MAX 8
/*  Number of bins in a grey level histogram  */
#define HISTOGRAM_WIDTH 64
#define ERROR_MAX 6
/*  A maximal number of objects attached to an image  */
#define OBJECT_NUM 1000
/*  SHAPE indexes  */
#define OBJ_CIRCLE 0
#define OBJ_BOX 1
#define OBJ_DIAMOND 2
#define OBJ_UPTRIANGLE 3
#define OBJ_DOWNTRIANGLE 4

/*  Wanted number of grey levels  */
#define GREY_WANTED 128
#ifndef min
#define min(a,b) ((a) > (b)) ? (b) : (a)
#endif
#ifndef max
#define max(a,b) ((a) > (b)) ? (a) : (b) 
#endif
#define DEFAULT_DELAY 0,2
#define DEFAULT_AUTOLEVEL_HIGH 0.01

/*  Menu item numbers from the Astrometry menu  */
/*==============================================*/
#define EDIT_PARAMETERS 1
#define DETECT_STARS 2
#define MATCH_STARS 3
#define SAVE_STAR_LIST 4
#define LOAD_STAR_LIST 5
#define DO_ASTROMETRY 6
#define EMPTY_ITEM1 7
#define OPTIONS 8
#define CATALOGS 9
#define SAVE_SETTINGS 10
#define SEND_MAIL 11
#define SHOW_REPORT 12


/*  Constant definitions for functions which access USNO catalog */
/*===============================================================*/
/*  Width of the declination zone for USNO catalog  */
#define USNO_DECWIDTH 7.5
/*  Distance between two accelerator lines in decimal hours  */
#define USNO_RADIST 0.25
/*  Record length in bytes  */
#define USNO_RECORDLENGTH 12
/*  Maximal allowed number od stars */
#define MAXSTARNUM 33546
/*  buffer size  */
#define BUFSIZE 1000

/*  Constant definitions for functions which access GSC  catalog */
/*===============================================================*/
/*  Width of the declination zone for GUIDE catalog  */
#define GUIDE_DECWIDTH 7.5


/*  Default values  */
#define DEFAULT_RAWIDTH   1.92
#define DEFAULT_DECHEIGHT 1.92
#define DEFAULT_WIDTH     512.0
#define DEFAULT_HEIGHT    512.0
#define DEFAULT_ROTATION  0.0
#define DEFAULT_MINMAG    -2.0
#define DEFAULT_MAXMAG    18.0

/*  FITSIO related constants  */
/*============================*/
#define CARDLENGTH 81

/*  General constants about catalogs  */
/*====================================*/
/*  Total number of catalogs  */
#define NUMBER_OF_CATALOGS 4

/*  Catalog numbers  */
#define PATHLENGTH 256
#define CAT_USNO 0
#define CAT_PPM 1
#define CAT_GUIDE 2
#define CAT_GUIDE_SOUTH 3


/*  Constants for the starcount functions  */
/*=========================================*/

#define PI 3.1415927 
/* stack size for star counting */
#define STACKMAX 40000
/* initial object array size */
#define NSTAR 2500;
/* assumed maximum star area  */
#define DEFAULT_STAR_AREA 50
/* Default size of a subimage where average background is calculated  */
#define DEFAULT_MAXSIZE 30
/* Default sensitivity threshold in background variation (sigma) units  */
#define DEFAULT_SGNUM 5.0
/*  Minimum object brightness above the background  */
#define DEFAULT_MINBRIGHT 50
/*  Minimum star size in pixels  */
#define DEFAULT_MINSTAR 2
/*  Size of the exclusion zone on the image edge where no stars are detected */
#define DEFAULT_BORDER 0
/*  Do we use aperture photometry? */
#define DEFAULT_APERTURE 1
/*  Radius of the inner circle for the aperture photometry */
#define DEFAULT_INNER 3.0
/*  Radius of the outer circle for the aperture photometry */
#define DEFAULT_OUTER 10.0

/*  Constants for the star matching functions  */
/*=============================================*/
/*  Default number of bright stars taken into account in initial matching  */
#define FIRSTBRIGHT 40
/*  Minimum acceptable number of stars for the initial matching */
#define MINFIRSTBRIGHT 6
/*  Maximum number of candidates for a constellation star  */
#define CANDIDATE_MAX 100
/*  Maximum allowed positional error for stars in constellation  */
#define POSERRMAX 6.0
/*  Maximum allowed magnitude error for stars in constellation  */
#define MAGERRMAX 100
/*  Minimum value of constellation to be accepted  */
#define CONSTMIN 15.0
/*  Number of stars in constellation  */
#define CONSTSTAR 12
/*  Maximal allowed residual (in arcseconds)  */
#define MAXRES 1.0

#endif
