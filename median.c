/*  median.c  */
/*  Finds the median of values given in an table without sorting */
/*  the entire table   */
/*  Jure Skvarc, november 1998  */
/*  Parameters: */
/*  a         -- a pointer to the table  */
/*  n         -- number of elements in a table  */
/*  width     -- size of elements in bytes      */
/*  compare   -- the compare function           */
/*  The algorithm:  pick up a random element from the table and */
/*  put all of the elements smaller than it before the elements */
/*  that are bigger.  Than repeat the procedure on the half that contains */
/*  middle element and so on until only the middle element remains  */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void
median(void *a, int n, int width, int (*compare)(const void *, const void *))

{
  int p, z, i, j, middle, c, q, d;
  void *cel, *ti, *tj, *tempel, *tp, *td;

  p = 0;     /*  Lowest index of the subarray that has to be examined  */
  z = n - 1; /*  Highest index of the subarray that has to be examined  */
  if (!(cel = malloc(width))) {
    fprintf(stderr, "Out of memory, function median!\n");
    exit(1);
  }
  if (!(tempel = malloc(width))) {
    fprintf(stderr, "Out of memory, function median!\n");
    exit(1);
  }
  middle = (p + z) / 2; /*  Index of the middle element  */
  do {
    tp = a + width * p;
    /*  Select a random element of the table  */
    /*  and exchange its position with the first element */
    td = a + (d = (p + rand() % (z - p) + 1)) * width;
    memcpy(cel, td, width);
    memcpy(td, tp, width);
    memcpy(tp, cel, width);
    i = p + 1;
    j = z + 1;
    q = 1;
    do {
      ti = a + i * width;
      /*  Find an element in the upper part (lower index) of the array */
      /*  which is bigger or equal than the comparison element  */
      while (i < j && (c = compare(cel, ti)) >= 0) {
	q = q && !c;
	i++;
	ti += width;
      }
      j--;
      tj = a + j * width;
      /*  Find an element in the upper part (higher index) of the array */
      /*  which is smaller than the above found element  */
      while (i < j && compare(cel, tj) < 0) {
	q = 0;
	j--;
	tj -= width;
      }
      if (i < j) {
	/* swap the element positions  */
	memcpy(tempel, ti, width);
	memcpy(ti, tj, width);
	memcpy(tj, tempel, width);
	q = 0;
 	i++;
      }
    } while (i < j);  
    /*  Now determine which part of the partially sorted table */
    /*  contains the middle index and set the new lower and upper */
    /*  indices accordingly   */
    i--;
    ti = a + i * width;
    if (i != p) {
      memcpy(tempel, tp, width);
      memcpy(tp, ti, width);
      memcpy(ti, tempel, width);
    }
    if (i > middle) {
      if (i == z) z--;
      else  z = i;
    }
    else p = i + 1;
  } while (p < z && !q);
  free(tempel);
  free(cel);
}


