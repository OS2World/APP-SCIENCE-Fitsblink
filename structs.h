/* structs.h for fitsblink 2,0   */
/* Data structures  */
#ifndef _blink_structs
#define _blink_structs
#include <forms.h>
#include <fitsio.h>
#include "consts.h"
#include "formblink.h"

typedef unsigned short int PIXEL;
typedef unsigned char BYTE;

typedef
struct {
  int simple;
  long bitpix;
  long naxis, nax[2];
  fitsfile *fptr;
  char ctype1[9], ctype2[9];
  double crval1, crval2;  /* Celestial coordinates of the reference pixel (in decimal degrees) */
  double crpix1, crpix2;  /* Image coordinates of the reference pixel  */
  double cdelt1, cdelt2;  /* Coordinate incremement at reference pixel  */
  double crot;            /* Rotation angle (should be the same for both axis */
  char ctype[5];          /* Projection type (like -TAN)  */
  short wcs;                /* Flag which is set if wcs coordinates are present  */
  float sizex, sizey;
  char telescope[FLEN_VALUE];
  char instrument[FLEN_VALUE];
  char observer[FLEN_VALUE];
  char date_obs[FLEN_VALUE];
  char ut_start[20];
  char ut_cent[20];
  char ut_end[20];
  char time_beg[20];
  char time_end[20];
  char object[FLEN_VALUE];     /*  Name of the object  */
  /*  Flags which tell whether specific keyword was read  */
  short is_date_obs;
  short is_exposure;
  short is_ut_start;
  short is_ut_cent;
  short is_ut_end;
  short is_time_beg;
  short is_time_end;
  char time[9];
  float exposure;
  float ccd_temp;
  float temperat, pressure, humidity;
  float equinox;
  char *name;
} FITSFILE;


typedef
struct slika {
  int width, height;  /* Width and height of the image */
  int  x1, y1, x2, y2;/* Coordinates of the active rectangle inside the image */
  PIXEL *image;       /* Pointer to image */
  PIXEL **pi;         /* Array of pointers to image rows  */
  float fitmeasure;   /*  Ratio between fitted and measured brightness 
			  (and vice versa) of star which is still accepted  */
  float minbright;      /*  Minimum still accepted brightness  */
  int minstar;
  int maxsize;      /*  Size of the window inside which the average background is calculated  */
  float sg_num;     /*  Sensitivity threshold expressed in standard deviations of the background */
  short border;       /* The width of the region on the image edge where no star is reported  */
  short aperture;   /* Flag which tells if we do aperture astrometry  */
  float inner;      /* radius of the inner circle for the aperture photometry  */
  float outer;      /* radius of the outer circle for the aperture photometry  */
  char *name;       /*  name od the image  */
} FITS_IMAGE;


/*  Structure with coordinates highlighted in the image */
typedef struct {
  float x, y;  /* Coordinates of the center  */
  float r;     /* radius of the circle  */
  char *s;     /* Name of the object  */
  short color; /* Object color  */
  short shape; /* Object shape  */
}  OBJECT;

/*  Data describing window showing stars  */
typedef
struct {
  double ra, dec;  /*  RA and rec in decimal degrees  */
  double width, height;  /*  width and height in decimal degrees */
  double fi;
  double w, h;        /* width and height in pixels  */
  float minmag, maxmag;  /*  Minimum and maximum acceptable magnitudes */
  int minsize, maxsize;  /* Minimum and maximum size of star  */
  int nm;       /*  Number of stars in memory */
} MAP_IMAGE;

/*  A structure which describes a single star from a catalog  */
typedef
struct {
  int catalog;  /*  Catalog number  */
  int n;     /*  Star number  */
  int m;     /*  Index of the matched star from the image  */
  double x, y;  /*  Star screen coordinates */
  double ra, dec;    /*  Star space coordinates  */
  float bluemag, redmag, mag;  /*  Star magnitudes  */
  int q;     /*  Indicator whether magnitudes can be trusted  */
  float fitmag;  /*  Fitted star intensity  */
  float err; /*  Distance between the imaged in catalogued star  */
}  STAR;

typedef
struct {
  int n;
  STAR *s;
  float sp;
} STARS;

/*  Structure which hold values needed for matching subroutine  */
typedef
struct {
  float min;        /* Minimum acceptable value of constellation to accepted as a match  */
  int nstar;        /* Number of neighbouring stars in constellation  */
  int firstbright;  /* Number of bright stars used for initial matching  */
  float poserrmax;     /* Maximum positional error allowed for acceptance of a star in costellation  */
  float magerrmax;     /* Maximum magnitude error (factor) allowed for acceptance a a star in constellation  */
  float maxres;        /* Maximal allowed residual  */
} MATCH_OPTIONS;


/*  Structure carrying information about individual images  */
typedef struct {
  float dx, dy;  /*  Translation of the image relative to a reference image */
  float alfa;    /*  Angle of rotation */
  float a, b, d, e;   /*  Koeficients relating to angle od rotation and extension */
  double ra, dec; /* right ascension and declination of the image  */
  double fi;     /* Angle of rotation from north to east  */
  double xsize, ysize;  /* Sizes of pixel in x and y direction  */
  int year, month, day; /* Time when the image was taken  */
  int hour, minute;
  float second;
  float *gamx, *gamy;  /*  Lists of values for grey level correction */
  float *kg;    /* list of coefficients for grey level correction */
  int ngam;     /*  Number of points for grey level correction */
  int low, high;  /*  The lowest and the highest value in the image  */
  float *histx, *histy;  /*  Lists of values for the histogram  */
  float xmin, xmax;   /*  Histogram range  */
  int nhisto;   /*  Number of values in the histogram */
  char *name;   /*  The name of the image  */
  Pixmap  pixmap;  /*  Image as shown in X window  */
  FITS_IMAGE fits;      /*  Structure with information about FITS image */
  FITS_IMAGE astrwin;   /*  Small image inside which an object is located */
  FITSFILE fitsfile;
  OBJECT *obj;
  int objnum;       /*  Number of markers loaded to memory  */
  int objmax;       /*  maximum number of markers  */
  short objshow;    /*  A flag which tells whether the markers are shown  */
  int grey_max;     /*  Maximum grey level value for the screen  */
  short detect;     /*  Set to 1 if any stars were detected  */
  short matched;      /*  Set to 1 if star matching was successful  */
  MATCH_OPTIONS match;
  MAP_IMAGE map;
  STARS imagelist;
  STARS cataloglist;   /*  List of stars from the catalog  */
}  BLINK_FRAME;

/*=========================================================================*/
/*  Structure with the information about the current state of the program  */
/*=========================================================================*/

typedef struct {
  int control_num;  /*  Number of the currently selected control (0 or 1)  */
  int control[2];  /*  Numbers of images belonging to the first and second control */
  int frame_max;   /*  Number reserved spaces for frames  */
  int frame_used;  /*  Number of actually used frames  */
  Display *disp;   /*  Pointer to X11 display structure */
  int bw, bh;      /*  Dimensions of the blinking area in pixels  */
  FD_setting *gui; /*  Pointer to structure of control window  */
  FD_blinker *blinker;  /*  Pointer to blinker structure  */
  FL_COORD blinkwidth;  /*  Width  */
  FL_COORD blinkheight; /*  and height of the blinker window  */
  FD_Edit *browser[2];  /* Pair of pointers to browsers  */
  FD_Error *error;  /*  Browser for the output of errors */
  FD_help *readme;  /*  Window with the README file  */
  FD_Parameters *parameters;  /*  Image parameters  */
  FD_Report *report;/*  Report window  */
  FD_Options *options; /* astrometry options  */
  FD_Catalog *catalogs; /* catalog options  */
  FD_Mail *mail;
  FD_Wcs *newwcs; 
  FD_Settings *settings;
  FD_Set_catalogs *set_catalogs;
  FD_Set_observatory *set_observatory;
  FD_Set_display *set_display;
  FD_Set_mail *set_mail;
  FD_Set_help *set_help;
  FD_Commands *set_commands;
  FD_Set_ccd *set_ccd;
  FD_Set_count *set_count;
  FD_Set_match *set_match;
  FD_Star *showstar;
  FD_Comet *showcomet;
  FD_Telescope *telescope;
  FD_Message *message;
  int b[2];         /* */
  short zoom_width;   /*  Number of magnification (zoom) */
  short freeze;       /*  Flag which is set if the zoom windows are locked  */
  int xf, yf;       /*  Coordinates of the zoom window when the zoom is locked */
  short blink;        /*  Am I in a blink mode ?  */
  int blinknum;     /*  Number of the image shown in the blink mode  */
  int grey_max;
  short color;      /* Is display in color mode (1) or grey level (0) ? */
  short inverse;    /* Is display inversed after autoleveling?  */
  unsigned long pixel[GREY_WANTED];  /*  Index for X colors  */
  char *logfile;
  FILE *logfp;
  short interactive;
  short verbose;
  short doastr;
  float delay;
  float autolevel_high;
  int timeout;
  char *wwwbrowser;
  char *helpfile;
} STATE;


/*======================================================*/
/* Structure with the information about the observatory */
/*======================================================*/
typedef
struct {
  int code;
  char observer[CARDLENGTH];
  char telescope[CARDLENGTH];
  char instrument[CARDLENGTH];
  double longitude;
  double latitude;
} OBSERVATORY;


/*======================================================*/
/* Current telescope and camera settings                */
/*======================================================*/
typedef
struct {
  double ra, dec; /*  right ascension and declination  */
  double alt, az; /*  altitude and azimuth  */
  double jd, sr;  /*  Julian day and sidereal time  */
  double exptime; /*  exposure time  */
  double temp;    /*  Camera temperature  */
  char *filename; /*  output file name  */
  short dark;     /*  dark image indicator  */
  char start[256];
  char point[256];
  char park[256];
  char where[256];
  char zero[256];
  char center[256];
  char gettemp[256];
  char settemp[256];
  char expose[256];
} TELESCOPE;


/*================================================*/
/* Structure with information about email report  */
/*================================================*/
typedef
struct {
  char *to;    /* Address to which we send  */
  char *cc;    /* A list of cc addresses    */
  char *title; /* Title of the message      */
  char *file;  /* Name of the file which contains the body of message  */
} MAIL;


/*  Structures for catalogs  */
/*===========================*/

/*  Description of catalogs  */
typedef
struct {
  int use;      /* Set to TRUE if the catalog is used  */
  int lpath;    /* Length of the string, reserved for path to the catalog  */
  char *name;   /* Catalogs name  */
  char *path;   /* full path to the catalog  */
} CATALOG;

/**********/
/*  USNO  */
/**********/
/*  A list of bytes definining one record in USNO catalog  */
typedef
struct {
  unsigned char a[USNO_RECORDLENGTH];
} USNO_STAR;

/*  One entry from an accelerator file  */
typedef
struct {
  float ra;
  int index, length;
} ACCELERATOR;

/*********  USNO end  ***********/


/**********************/
/*     GSC            */
/**********************/
typedef
struct {
  int id, plate;
  double ra, dec, poserr;
  float mag, magerr;
  short magband;
  short class;
} GUIDE_STAR;

typedef
struct {
  double center;  /* center of declination band  */
  short numlarge;   /* Number of large regions in declination band  */
  short firstlarge; /* First large region in declination band */
} LARGE_REGION;

typedef
struct {
  short largeid;    /*  Id of the large region  */
  short firstsmall; /* id of first small region in large region  */
  short depth;      /*  Depth of subdivision of large region  */
} SMALL_REGION;

/****** GSC end  *********/

/*======================================*/
/*  Structures for starcount functions  */
/*======================================*/

typedef 
struct {
  int x, y;     /*      Koordinati in vrednost tocke  */
  PIXEL v;
} POINT;

typedef 
struct {
  int xf, yf;	        /* First point in object found by the program  */
  double xt, yt;	/* Centroid coordinates */
  int x1, y1, x2, y2;	/* Limiting coordinates  */
  long area;	        /* Number of pixels belonging to the object  */
  long perim;	        /* Number of pixels in object which lie on the edge */
  long p_out;	        /* Number of points touching the object from outside  */
  long grey;            /* Sum of grey level values inside the object  */
  float  bg;            /* Average background value inside the object  */
  POINT *outer;	        /* Pointer to the list of outside edge points  */
  POINT *inner;	        /* Pointer to the list of inside points    */ 
  int xu;               /* Coordinate of a point on the upper edge of the object */
  int n;
} OBJEKT;

typedef struct {
  BYTE *s, **p;	/*	Kazalec na sliko  */
  int width;  /*  Sirina slike v BYTE-ih  * 8  (torej v pixlih) */
  long size;  /*	Velikost  slike  v bytih  */
  int x0, y0;  /*	Offset glede na koordinate objekta na sliki  */
} SHADOW;

/*  Structure with data about Gaussian filter */
typedef struct {
  float sigma;   /*  Width of the gaussian in pixels  */
  int n;         /*  linear size of the array  */
  float r;       /*  radius of the circle taken into account  */
  float **g;     /*  Pointer to the coefficient array  */
} GAUSSF;


/*==========================================*/
/*  Structures for field_compare functions  */
/*==========================================*/
/*  Pair of star coordinates  */
typedef
struct {
  float x1, y1, x2, y2;
  float mag1, mag2;
  int n1, n2;
  float err;
} STAR_PAIR;


/*  Parameters of the coordinate rotation and translation and magnitude transfor
mation  */
typedef
struct {
  double a, b, c, d, e, f, hi;  /*  Translation and rotation  */
  double ma, mb, sma, smb;      /*  Magnitude transformation  */
} TRANSROT;
  

#endif



