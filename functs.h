/*  functs.h for Fitsblink 2.x  */
#ifndef _blink_functs
#define _blink_functs
#include <stdio.h>
#include <forms.h>
#include "formblink.h"
#include "structs.h"

char *strdup(const char *);

/*****************/
/*  BLINK.C      */
/*****************/
void *myalloc(unsigned int, char *, char *);
void delay(int);
int correct(BLINK_FRAME *, int, PIXEL);
void draw_objects(BLINK_FRAME *, int);
void set_image(BLINK_FRAME *, int);
void set_lin(BLINK_FRAME *, int);
int load_frame(BLINK_FRAME *, int);
int remove_frame(BLINK_FRAME *, int);
int load_file(char *);


/*****************/
/*  BLINKGUI.C   */
/*****************/
void draw_image(BLINK_FRAME *, int);
void draw_zoom(BLINK_FRAME *, int, FL_OBJECT *, int, int);
void show_left_offsets(BLINK_FRAME *, int);
void show_right_offsets(BLINK_FRAME *, int);
void show_frame_info(BLINK_FRAME *, int, int);
void turnimageC(FL_OBJECT *, long);
void loadC(FL_OBJECT *, long);
void eraseC(FL_OBJECT *, long);
void nameC(FL_OBJECT *, long);
void fitsC(FL_OBJECT *, long);
void greyC(FL_OBJECT *, long);
void emptyC(FL_OBJECT *, long);
void gammaC(FL_OBJECT *, long);
int cmppixel(const void *, const void *);
void autolevels(int n);
void autoC(FL_OBJECT *, long);
void invertC(FL_OBJECT *, long);
void deltaxC(FL_OBJECT *, long);
void deltayC(FL_OBJECT *, long);
void quitC(FL_OBJECT *, long);
void helpC(FL_OBJECT *, long);
void info(void);
void readme(FL_OBJECT *, int);
void zoom1C(FL_OBJECT *, long);
void zoom2C(FL_OBJECT *, long);
void do_blink(void);
void deltatC(FL_OBJECT *, long);
void blinkC(FL_OBJECT *, long);
void upC(FL_OBJECT *, long);
void downC(FL_OBJECT *, long);
void rightC(FL_OBJECT *, long);
void leftC(FL_OBJECT *, long);
void circleC(FL_OBJECT *, long);
int define_colormap(void);
int define_greymap(void);
void tgui(int, char * []);


/****************/
/*  FITS.C      */
/****************/
int read_fits(char *, PIXEL **, int *, int *, FITSFILE *, char **);


/******************/
/*  FORMBLINK.C   */
/******************/
FD_setting *create_form_setting(void);
FD_blinker *create_form_blinker(void);
FD_Edit *create_form_Edit(void);
FD_info *create_form_info(void);

/********************/
/*  IMAGEACC.C      */
/********************/
void set_image_parameters(FITS_IMAGE *, int, int, PIXEL *);
PIXEL video_point(FITS_IMAGE *, int, int);
void set_point(FITS_IMAGE *, int, int, PIXEL);
void xor_point(FITS_IMAGE *, int, int);
float convolve(GAUSSF *, FITS_IMAGE *, int, int);
void blur(FITS_IMAGE *);
void set_gaussian_filter(GAUSSF *, float, float);

/********************/
/*  SHADOW.C        */
/********************/
void reserve_shadow(SHADOW *, int, int, int, int);
void free_shadow(SHADOW *shadow);
int test(SHADOW *, int, int);
void set(SHADOW *, int, int);
void reverse(SHADOW *, int, int);
void clear(SHADOW *, int, int);
void clear_shadow(SHADOW *);



/*********************/
/*  ERROR.C          */
/*********************/
void show_errors(int, char **); 

/*********************/
/*  WCS.C            */
/*********************/
int worldpos(double, double, double, double, double, double, double, 
		    double, double, char *, double *, double *);

int xypix(double, double, double, double, double, double, double, 
		 double, double, char *, double *, double *);


/********************/
/* PARSECOOR.C      */
/********************/
void strlwr(char *);
double parse_ra(char *);
double parse_dec(char *);
double parse_ra_c(char *);
double parse_dec_c(char *);
char *format_ra(double);
char *format_dec(double);
char *format_ra_mpc(double);
char *format_dec_mpc(double);
char *format_az(double);

/********************/
/* midexposure.c    */
/********************/
void calculate_midexposure(FITSFILE *, int *, int *, int *, int *, int *, float *);

/********************/
/* PARAMETERS.C      */
/********************/
void set_rainput(FL_OBJECT *, double);
void set_decinput(FL_OBJECT *, double);
void set_doubleval(FL_OBJECT *, char *, double);
void edit_parameters(FL_OBJECT *, int);

/********************/
/* REPORT.C         */
/********************/
void show_report(void);
void hide_report(void);

/********************/
/* MESSAGE.C        */
/********************/
void open_message(char *);
void close_message(void);
void write_message(char *);


/*************************/
/*  OPTIONS.C            */
/*************************/
void logfileC(FL_OBJECT *, long);
void minconstC(FL_OBJECT *, long); 
void initconstC(FL_OBJECT *, long); 
void starconstC(FL_OBJECT *, long);
void countsigmaC(FL_OBJECT *, long);
void countminvalC(FL_OBJECT *, long);
void starerrorC(FL_OBJECT *, long);
void starminsizelC(FL_OBJECT *, long);
void acceptoptionsC(FL_OBJECT *, long);
void defaultoptionsC(FL_OBJECT *, long);
void set_match_defaults(BLINK_FRAME *, int);
void set_defvals(void);
void edit_options(FL_OBJECT *, int);

float proc_sg_num(FL_OBJECT *, float);
int proc_minbright(FL_OBJECT *, int);
int proc_minstar(FL_OBJECT *, int);
int proc_gridsize(FL_OBJECT *, int);
int proc_border(FL_OBJECT *, int);
float proc_inner(FL_OBJECT *, float, float);
float proc_outer(FL_OBJECT *, float, float);

float proc_min(FL_OBJECT *, float, int);
int proc_firstbright(FL_OBJECT *, int);
int proc_nstar(FL_OBJECT *, int);
float proc_poserrmax(FL_OBJECT *, float);
float proc_maxres(FL_OBJECT *, float);
float proc_minmag(FL_OBJECT *, float, float);
float proc_maxmag(FL_OBJECT *, float, float);


/********************/
/* CATALOGS.C       */
/********************/
void check_catalogs(void);
void edit_catalogs(FL_OBJECT *, int);

/********************/
/* MAIL.C           */
/********************/
void edit_mail(FL_OBJECT *, int);


/********************/
/* MATCH_STARS.C    */
/********************/
int detect_stars(BLINK_FRAME *, int);
int match_stars(BLINK_FRAME *, int);
int star_list(BLINK_FRAME *, int);
int load_star_list(BLINK_FRAME *, int);

/********************/
/*  NEWCS.C         */
/********************/
void cal_new_wcs(TRANSROT, double, double, double, double, double, double, double, 
	    double *, double *, double *, double *, double *);
void newwcs(BLINK_FRAME *, int, TRANSROT);

/********************/
/*  SHOWSTAR.C      */
/********************/
int showstar(BLINK_FRAME *, int, STAR *, int, char *);
int showcomet(BLINK_FRAME *, int, STAR *, int, char *);

/********************/
/*  TELESCOPE.C     */
/********************/
void show_telescope_control(void);


/********************/
/*  TIME.C          */
/********************/
double julian_day(int, int, double);
double make_a_day(int, int, int, double);
double sidereal(double, double);

/********************/
/*  MEDIAN.C        */
/********************/
void median(void *, int, int, int (*)(const void *, const void *));

/********************/
/* SETTINGS.C       */
/********************/
void show_settings(FL_OBJECT *, int);
int save_settings(void);
int load_settings(void);

/**********************/
/*  NONINTERACTIVE.C  */
/**********************/
int update_wcs(BLINK_FRAME *, int, TRANSROT);
void starcount(int, char *[]);
void make_catalog(int, char *[]);

/*************************/
/*  READUSNO.C           */
/*************************/
int calculate_corners(MAP_IMAGE *, STAR *);
int get_usno_stars(MAP_IMAGE *, STAR *, char *);


/*************************/
/*  READGUIDE.C           */
/*************************/
int get_guide_stars(MAP_IMAGE *, STAR *, char *, char *);

/*************/
/*  GSCFITS  */
/*************/
int read_large_regions(char *, LARGE_REGION *);
int read_small_regions(char *, SMALL_REGION *);
int read_sky_region(char *, GUIDE_STAR **, int *, int);



/*************************/
/*  STETJE.C             */
/*************************/
int background(FITS_IMAGE *, int, int);
int above_threshold(FITS_IMAGE *, int, int, float *);
int vrh1(FITS_IMAGE *, int, int); 
int vrh2(FITS_IMAGE *, int, int, int); 
int find_top(FITS_IMAGE *, SHADOW *, POINT *, int, int, int, OBJEKT *);
int obhod(FITS_IMAGE *, SHADOW *, POINT *, int, int, int, OBJEKT *);
int stetje(FITS_IMAGE, int *, STAR **);
void rob(FITS_IMAGE *, int);

/**************************/
/*  UTILITIES.C           */
/**************************/
void make_output_filename(char *, char **, char *);
void trim_spaces(char *p);
float **float_array(int, int);
void free_float_array(float **, int);
double fsqr(double);

/**************************/
/*  POVPRECJA.C           */
/**************************/
int povprecja(FITS_IMAGE *, float **, float **, float *, float *);

/*************************/
/*  APERTURE.C           */
/*************************/
double aperture(FITS_IMAGE *, double, double, double, double, 
		double *, double *);


/*  This should go out  */
void nrerror(char []);
float *vector(int, int);
int *ivector(int, int);
double *dvector(int, int);
float **matrix(int, int, int, int);
double **dmatrix(int, int, int, int);
int **imatrix(int, int, int, int);
float **submatrix(float **, int, int, int, int, int, int);
void free_vector(float *, int, int);
void free_ivector(int *, int, int);
void free_dvector(double *, int, int);
void free_matrix(float **, int, int, int, int);
void free_dmatrix(double **, int, int, int, int);
void free_imatrix(int **, int, int, int, int);
void free_submatrix(float *, int, int, int, int);
float **convert_matrix(float *, int, int, int, int);
void free_convert_matrix(float **, int, int, int, int);

/******************/
/* FIELD_COMPARE  */
/******************/
int field_compare(STARS *, STARS *, MATCH_OPTIONS, BLINK_FRAME *, int);

/********************/
/*  KONSTELACIJE.C  */
/********************/
void constellations(STARS *, STARS *, int, TRANSROT *, MATCH_OPTIONS);
int compare_star(const void *, const void *);
int compare_starmag(const void *, const void *);

/****************/
/*  TRANSROT.C  */
/****************/
float transrot(STAR_PAIR *, int *, TRANSROT *, MATCH_OPTIONS *match_options); 
float transrot1(STAR_PAIR *, int *, TRANSROT *, MATCH_OPTIONS *match_options); 


#endif


