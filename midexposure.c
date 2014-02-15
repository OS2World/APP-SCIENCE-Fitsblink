/*  midexposure.c  */
/*  part of the fitsblink program  */
/*  routines which calculate the time and date of mid-exposure */
#include <string.h>
#include <stdlib.h>

#include "functs.h"

int
days_per_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/*  Increment date for one day  */
/*==============================*/
void
incr_date(int *year, int *month, int *day)

{
  int leap = 0;

  if (*month == 2 && *year % 4 == 0 && (*year % 100 != 0 || *year % 400 == 0)) {
    leap = 1;
  }
  (*day)++;
  if (*day > days_per_month[*month] + leap) {
    *day = 1;  /* First day of month  */
    (*month)++;  /* Next month  */
    if (*month == 13) {
      *month = 1;
      (*year)++;
    }
  }
}

/*  convert time in decimal hours to hour, minute second format  */
/*===============================================================*/
void
dechour_to_hms(double dechour, int *h, int *m, float *s)

{
  *h = (int) dechour;
  dechour -= *h;
  dechour *= 60.0;
  dechour += 1e-9;
  *m = (int) dechour;
  dechour -= *m;
  *s = dechour * 60.0;
}


/*  Parse date which is either in format dd/mm/yy or dd/mm/yyyy or yyyy-mm-dd */
void
parse_date(char *date, int *year, int *month, int *day)

{
  int i;
  char sep;
  char *p;
  char *tdate;

  tdate = strdup(date);
  /*  Check for separator  */
  i = 0;
  p = tdate;
  sep = 0;
  while (p[i] != '\0') {
    if (p[i] == '/') {
      sep = '/';
      break;
    }
    else if (p[i] == '-') {
      sep = '-';
      break;
    }
    i++;
  }
  p = tdate;
  /*  Skip ' sign if present */
  if (*p == '\'') p++;
  switch (sep) {
  case '/':
    p = strtok(p, "/");
    *day = strtol(p, NULL, 10);
    p = strtok(NULL, "/");
    *month = strtol(p, NULL, 10);
    p = strtok(NULL, "/");
    *year = strtol(p, NULL, 10);
    if (*year < 50) *year += 2000;
    else if (*year < 100) *year += 1900;
    break;
  case '-':
    p = strtok(p, "-");
    *year = strtol(p, NULL, 10);
    p = strtok(NULL, "-");
    *month = strtol(p, NULL, 10);
    p = strtok(NULL, "-");
    *day = strtol(p, NULL, 10);
    if (*day > 31) {
      int t = *day;
      *day = *year;
      *year = t;
    }
    break;
  default:
    *year = 0;
    *month = 0;
    *day = 0;
  }
  free(tdate);
}


/*  Parse time writen in format hh:mm:ss.sss */
void
parse_time(char *time, int *hour, int *minute, float *second)

{
  char *p, *ttime;

  ttime = strdup(time);
  p = ttime;
  /*  Skip ' sign if present */
  if (*p == '\'') p++;
  p = strtok(p, ":");
  *hour = strtol(p, NULL, 10);
  p = strtok(NULL, ":.");
  *minute = strtol(p, NULL, 10);
  p = strtok(NULL, "':");
  *second = strtod(p, NULL);
  free(ttime);
}

/*  Calculate the time of midexposure  */
/*=====================================*/
void
calculate_midexposure(FITSFILE *f, int *year, int *month, int *day,
		      int *hour, int *minute, float *second)

{
  int h, m, h2, m2;
  float s, s2;
  double d1 = 0.0, d2 = 0.0;

  /*  If the DATE-OBS keyword is present, set the date  */
  if (f->is_date_obs) {
    parse_date(f->date_obs, year, month, day);
  }
  else {
    /*  Set date to a default  */
    *year = 2000;
    *month = 1;
    *day = 1;
  }

  /*  For the time od midexposure, first try the easy way: check if */
  /*  UT-CENT keyword is present  */
  if (f->is_ut_cent) {
    parse_time(f->ut_cent, hour, minute, second);
    /* Now, check if a correction of date is needed  */
    if (f->is_ut_start) {
      /*  Read start of exposure  */
      parse_time(f->ut_start, &h, &m, &s);
      if (h > *hour) {
	/* exposure is over midnight, adjust the date */
	incr_date(year, month, day);
      }
    }
    else {
      if (f->is_exposure) {  /*  This situation is already weird:  */
				/*  there is UT-CENT but no UT-START  */
	/*  Nevertheless, we happen to have the EXPOSURE keyword  */
	/*  Convert UT-CENT to seconds and subtract half of exposure  */
	d1 = (double) *hour + (double) *minute / 60.0 + *second / 3600.0 - 
	  f->exposure / 7200.0;
	if (d1 > 0) {  /* Increment date for one day  */
	  incr_date(year, month, day);
	}
      }
    }
  }
  else if (f->is_ut_start) {
    /*  There is no UT-CENT keyword, so we have to calculate it by our own  */
    /*  First, check for UT_START  */
    parse_time(f->ut_start, &h, &m, &s);
    d1 = (double) h + (double) m / 60.0 + s / 3600.0;
    if (f->is_exposure) {
      /*  Add half of the exposure time to the start of exposure  */
      d1 += f->exposure / 7200.0;
      if (d1 > 24.0) {
	incr_date(year, month, day);
	d1 -= 24;
      }
      dechour_to_hms(d1, hour, minute, second);
    } 
    else if (f->is_ut_end) {
      /* There is no EXPOSURE keyword but there is UT_END  */
      parse_time(f->ut_end, &h2, &m2, &s2);
      d2 = (double) h2 + (double) m2 / 60.0 + s2 / 3600.0;
      printf("d1=%f, d2=%f\n", d1, d2);
      d1 = 0.5 * (d1 + d2);
      if (d1 > 24.0) {
	incr_date(year, month, day);
	d1 -= 24;
      }
      printf("d1=%f, d2=%f\n", d1, d2);
      dechour_to_hms(d1, hour, minute, second);
    }
  }
  else  if (f->is_time_beg) {     /*  Last hope: check for TIME-BEG */
    parse_time(f->time_beg, &h, &m, &s);
    d1 = (double) h + (double) m / 60.0 + s / 3600.0;
    if (f->is_exposure) {
      /*  Add half of the exposure time to the start of exposure  */
      d1 += f->exposure / 7200.0;
      if (d1 > 24.0) {
	incr_date(year, month, day);
	d1 -= 24;
      }
      dechour_to_hms(d1, hour, minute, second);
    } 
    else if (f->is_time_end) {
      /* There is no EXPOSURE keyword but there is TIME-END  */
      parse_time(f->time_end, &h2, &m2, &s2);
      d2 = (double) h2 + (double) m2 / 60.0 + s2 / 3600.0;
      d1 = 0.5 * (d1 + d2);
      if (d1 > 24.0) {
	incr_date(year, month, day);
	d1 -= 24;
      }
      dechour_to_hms(d1, hour, minute, second);
    }
  }
}
