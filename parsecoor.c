#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "functs.h"


/*  Convert a string to lower case  */
void
strlwr(char *a) 

{
  int i;
  for (i = 0; a[i] != 0; i++) a[i] = tolower(a[i]);
}


/*  Parse a string and output number of hours, minutes and seconds as well as
    RA in decimal degrees   */
double
parse_ra(char *ra)

{
  int h, m;
  float s;

  strlwr(ra);
  while (!isdigit(*ra)) ra++;
  sscanf(ra, "%dh%dm%fs", &h, &m, &s);
  h %= 24;
  m %= 60;
  s = fmod(s, 60.0);
  return  15.0 * (h + m / 60.0 + s / 3600.0);
}


/*  Parse a string and output declination in decimal degrees   */
double
parse_dec(char *dec)

{
  char c;
  double dc;
  int h, m;
  float s;

  h = m = 0;
  s = 0.0;
  strlwr(dec);
  /*  Skip funny characters  */
  while (!(isdigit(*dec) || *dec == '+' || *dec == '-')) dec++;
  if (*dec != '+' && *dec != '-') {
    c = '+';
    sscanf(dec, "%dd%d'%f\"", &h, &m, &s);
  }
  else {
    sscanf(dec, "%c%dd%d'%fs\"", &c, &h, &m, &s);
  }
  if (h != 90) h %= 90;
  m %= 60;
  s = (s - floor(s)) + ((int) s) % 60;
  dc = (((char) c == '-') ? -1 : 1) * (h + m / 60.0 + s / 3600.0);
  if (dc < -90.0) dc = -90.0;
  if (dc > 90.0) dc = 90.0;
  return dc;
}

/*  Format RA for printout  */
char *
format_ra_g(double ra, char sep)

{
  static char a[20];
  int h, m;
  float s;

  ra /= 15.0;
  h = (int) ra;
  ra -= h;
  ra *= 60.0;
  m = (int) (ra + 1e-9);
  s = (ra - m + 1e-9) * 60;
  sprintf(a, "%02d%c%02d%c%05.2f", h, sep, m, sep, s);
  return a;
}

/*  Format DEC for printout  */
char *
format_dec_g(double dec, char sep)

{
  static char a[20];
  int d, m, c;
  float s;
  
  if (dec < 0) {
    c = '-';
    dec = -dec;
  }
  else {
    c = '+';
  }
  d = (int) dec;
  dec -= d;
  dec *= 60.0;
  m = (int) (dec + 1e-9);
  s = (dec - m + 1e-9) * 60;
  sprintf(a, "%c%02d%c%02d%c%04.1f", c, d, sep, m, sep, s);
  return a;
}


char *
format_ra(double ra)

{
  return format_ra_g(ra, ':');
}

char *
format_dec(double dec)

{
  return format_dec_g(dec, ':');
}

/*  Format ra for MPC report  */
char *
format_ra_mpc(double ra)

{
  return format_ra_g(ra, ' ');
}

/*  Format declination for MPC report  */
char *
format_dec_mpc(double dec)

{
  return format_dec_g(dec, ' ');
}



char *
format_az(double az)

{
  static char a[20];
  int d, m;
  float s;
  
  d = (int) az;
  az -= d;
  az *= 60.0;
  m = (int) (az + 1e-9);
  s = (az - m + 1e-9) * 60;
  sprintf(a, "%3d:%02d:%04.1f", d, m, s);
  return a;
}




/*  Parse a string in hh:mm:ss,ss format and output RA in decimal degrees   */
double
parse_ra_c(char *ra)

{
  int h, m;
  float s;

  strlwr(ra);
  while (!isdigit(*ra)) ra++;
  sscanf(ra, "%d:%d:%f", &h, &m, &s);
  h %= 24;
  m %= 60;
  s = fmod(s, 60.0);
  return  15.0 * (h + m / 60.0 + s / 3600.0);
}


/*  Parse a string in dd:mm:ss,ss format and output declination in decimal degrees   */
double
parse_dec_c(char *dec)

{
  char c;
  double dc;
  int h, m;
  float s;

  h = m = 0;
  s = 0.0;
  strlwr(dec);
  /*  Skip funny characters  */
  while (!(isdigit(*dec) || *dec == '+' || *dec == '-')) dec++;
  if (*dec != '+' && *dec != '-') {
    c = '+';
    sscanf(dec, "%d:%d:%f", &h, &m, &s);
  }
  else {
    sscanf(dec, "%c%d:%d:%f", &c, &h, &m, &s);
  }
  if (h != 90) h %= 90;
  m %= 60;
  s = (s - floor(s)) + ((int) s) % 60;
  dc = (((char) c == '-') ? -1 : 1) * (h + m / 60.0 + s / 3600.0);
  if (dc < -90.0) dc = -90.0;
  if (dc > 90.0) dc = 90.0;
  return dc;
}


