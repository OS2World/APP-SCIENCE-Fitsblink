/*  time.c      */
/*  Time routines for the orbits program   */
/*  (c) Jure Skvarc                        */
/*  Equations are taken from J. Meeus: Astronomical Algorithms  */
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "functs.h"
char error[256];

/*============================================================*/
/*  Calculate a Julian day for a given year, month and day  */
/*  Fractional part of the day can be given    */
/*============================================================*/
double
julian_day(int year, int month, double day)

{
  int a, b;
  static int dm[] = {0, 32, 30, 32, 31, 32, 31, 32, 32, 31, 32, 31, 32};
  if (year < -4712 || month < 1 || month > 12 || day < 0 || day >= dm[month]) {
    return -1;
  }
  if (year == 1582 && month == 10 && day >=5 && day < 15) {
   return -1;
  }
  if (month < 3) {
    year--;
    month += 12;
  }
  a = year / 100;
  if (year > 1582 || (year == 1582 && (month > 10 || 
				       (month == 10 && day >= 15.0)))) {
    b = 2 - a + a / 4;
  }
  else {
    b = 0;
  }
  return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

/*=================================================================*/
/*  Calculate a fractional day from day number, hours, minutes and */
/*  seconds (which may have a fractional part)                     */
/*=================================================================*/
double
make_a_day(int day, int hours, int minutes, double seconds)

{
  if (day < 1 || day > 31 
      || hours < 0 || hours > 23 
      || minutes < 0 || minutes > 59
      || seconds < 0 || seconds >= 60) {
    return -1;
  }
  return day + (hours + (minutes + seconds / 60.0) / 60.0) / 24.0;
}


/*=================================================================*/
/*  Calculate a local sidereal day from a Julian day and           */
/*  longitude (in decimal degrees                                  */
/*=================================================================*/
double
sidereal(double jd, double longitude)

{
  double sidereal, t;

  t = (jd - 2451545.0) / 36525.0;
  /*  sidereal time at ut in Greenwich  */
  sidereal = 280.46061837 + 360.98564736629 * (jd - 2451545.0) +
    (0.000387933 - t / 38710000.0) * t * t;
  sidereal -= longitude;
  return fmod(sidereal, 360.0);
}




