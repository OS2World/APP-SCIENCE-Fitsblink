/*  shadow.c  */
/*  part of the fitsblink program  */
/*  routines which access "shadow" image */
/*  Jure Skvarc,  May 1998                   */
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <forms.h>

#include "../functs.h"

int set_mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
int clear_mask[] = {127, 191, 223, 239, 247, 251, 253, 254};

/*  Reserve space for the shadow image and set pointers for each line  */
/*=====================================================================*/
void
reserve_shadow(SHADOW *shadow, int xmax, int ymax, int x0, int y0)

{
  int i, w;
  
  w = xmax / 8 + ((xmax % 8 == 0) ? 0 : 1);
  /*  Shadow size in bytes  */
  shadow->size = ymax * w;
  /*  Coordinates relative to the sky image  */
  shadow->x0 = x0;
  shadow->y0 = y0;
  /*  Space for shadow image */
  shadow->s = (BYTE *) myalloc(sizeof(BYTE) * shadow->size, 
			      "reserve_shadow", "shadow->s");
  /*  Array of pointers to rows of shadow image  */
  shadow->p = (BYTE **) myalloc(sizeof(BYTE *) * ymax, 
			      "reserve_shadow", "shadow->p");
  /*  Set pointers to rows of shadow image  */
  for (i = 0; i < ymax; i++) {
    shadow->p[i] = shadow->s + i * w;
  }
}

/*  Free space used for the shadow image  */
/*========================================*/
void
free_shadow(SHADOW *shadow)

{
  free(shadow->p);
  free(shadow->s);
}

/*  Test a point x, y in the image shadow */
/*========================================*/
int
test(SHADOW *shadow, int x, int y)
{
  int x1 = x - shadow->x0;
  return shadow->p[y - shadow->y0][x1 >> 3] & set_mask[x1 & 7];
}


/*  Set a point x, y in the image shadow  */
/*========================================*/
void
set(SHADOW *shadow, int x, int y)
{
  int x1 = x - shadow->x0;
  shadow->p[y - shadow->y0][x1 >> 3] |= set_mask[x1 & 7];
}

/*  Clear a point x, y in the image shadow  */
/*==========================================*/
void
clear(SHADOW *shadow, int x, int y)
{
  int x1 = x - shadow->x0;
  shadow->p[y - shadow->y0][x1 >> 3] &= clear_mask[x1 & 7];
}


/*  Reverse a point x, y in the image shadow  */
/*==========================================*/
void
reverse(SHADOW *shadow, int x, int y)
{
  int x1 = x - shadow->x0;
  shadow->p[y - shadow->y0][x1 >> 3] ^= set_mask[x1 & 7];
}


/*  Clear all of the image shadow  */
void
clear_shadow(SHADOW *shadow)
{
  long i;
  BYTE *p;
  
  p = shadow->s;
  for (i = 0; i < shadow->size; i++) {
    *(p++) = 0;
  }
}
