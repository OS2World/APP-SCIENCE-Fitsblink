/*  utilities.c   */
/*  part of the fitsblink program  */
/*  general utility routines   */
/*  Jure Skvarc, May 1998    */
/***********************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>


#include "../functs.h"



/*  Minimum and maximum size of stars taken into account  */
int minsize = 1;

/*  Prikazovanje obdelave slike  */
#ifdef NODEMO
int demo = 0;
#else
int demo = 1;
#endif

/*============================================================*/
/*  make_output_filename   */
/*============================================================*/
void
make_output_filename(char *name, char **output, char *end)

{
  char *p;

  p = strrchr(name, '.');  /*  Find last '.'  */
  if (p && p != name) {
    *output = (char *) myalloc(sizeof(char) * (p - name + 5), "make_output_filename", "output 1");
    strcpy(*output, name);  /*  Copy name  */
    (*output)[p - name + 1] = '\0';  /*  Remove string after .  */
    strcat(*output, end);
  }
  else {
    *output = (char *) myalloc(sizeof(char) * (strlen(name) + 5), "make_output_filename", "output 2");
    strcpy(*output, name);  /*  Copy name  */
    strcat(*output, ".");
    strcat(*output, end);
  }
}

/*  Reads a string from the end and finds first nonspace or  */
/*  non-newline character  */
void
trim_spaces(char *p)

{
  char *r;

  r = p + strlen(p) - 1;
  while (r > p && (*r == ' ' || *r == '\n')) {
    r--;
  }
  ++r; 
  *r = '\0';
}


/*  Reserve space and pointers for two-dimensional array of floats  */
/*==================================================================*/
float **
float_array(int w, int h)

{
  float **p;
  int i;

  p = (float **) myalloc(sizeof(float *) * w, "float_array", "p");
  for (i = 0; i < w; i++) {
    p[i] = (float *) myalloc(sizeof(float) * h, "float_array", "p[i]");
  }
  return p;
}


/*  Free two-dimensional array of floats  */
/*==================================================================*/
void
free_float_array(float **p, int w)
{
  int i;
  for (i = w - 1; i >= 0; i--) {
    free((void *) p[i]);
  }
  free((void *) p);
}


/*  Calculate a square of a double  */
/*==================================*/
double
fsqr(double x)

{
  return x * x;
}





