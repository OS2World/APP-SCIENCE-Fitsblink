/*  konstelacije.c  */
/*  part of the fitsblink program  */
/*  Jure Skvarc, May 1998  */
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <forms.h>

#include "../functs.h"
#define PI 3.1415927


int kmax;
char str[200];

extern STATE state;

/*  Compare two stars first on their y - coordinates,   */
/*  then x coordinates and then on their consecutive number in the file  */
int
compare_star(const void *a1, const void *b1)
     
{
  double t;
  STAR *a, *b;
  
  a = (STAR *)a1;
  b = (STAR *)b1;
  t = a->y - b->y;
  if (t < 0) t = -1;
  else if (t > 0) t = 1;
  else if (t == 0) {
    t = a->x - b->x;
    if (t == 0) {
      t = a->n - b->n;
    }
  }
  return (int) t;
}

/*  Compare stars on their intensity   */
int
compare_starmag(const void *a1, const void *b1)
     
{
  float t;
  STAR *a, *b;
  
  a = (STAR *)a1;
  b = (STAR *)b1;
  t = b->mag - a->mag;
  if (t < 0) return -1;
  else if (t > 0) return 1;
  else return 0;
}


int
increment_constellation(int *nk, int *nkt, int nsled, int **kandidat, double **pot,
			STARS *s2, float poserrmax)
{
  int i, j, k;
  int kind, kl;
  double x, y;
  double r;
  
  k = 1;
  do {
    /*  Izberem naslednjo kombinacijo sledi  */
    nkt[k]++;
    /*  Ce sem pri eni razdalji prisel do konca, povecam stevec pri naslednji */
    if (nkt[k] > nk[k]) {
      nkt[k] = 0;
      k++;
      if (k > kmax) kmax = k;
    }
    else {
      /*  Pregledam, ali se novo izbrana sled ujema z razdaljami v */
      /*  konstelaciji    */
      kind = kandidat[k][nkt[k]];
      x = s2->s[kind].x;
      y = s2->s[kind].y;
      /*	  printf("[%d ", kind);       */
      for (i = k + 1; i <= kmax; i++) {
	if (nkt[i] != 0) {
	  kl = kandidat[i][nkt[i]];
	  /*	      printf("%d ", kl);      */
	  r = sqrt(fsqr(s2->s[kl].x - x) + fsqr(s2->s[kl].y - y));
	  /*	      printf("r=%f, pot[%d][%d]=%f\n", r, k, l, pot[k][i]);      */
	  if (fabs(r - pot[k][i]) > poserrmax) {
	    break;
	  }
	}
      }
      if (i <= kmax) {
	/*  Kandidat se ne ujema z obstojeco konstelacijo, zato */
	/*  resetiram stevce pri razdaljah, ki so manjse od k -te */
	/*  razdalje.  */
	for (j = 1; j < k; j++) {
	  nkt[j] = 0;
	}
      }
      else {
	/*  Ce pridem do te tocke, je bilo vse v redu (konstelacija je */
	/*  pravilna in lahko skocim iz zanke  */
	break;
      }
    }
  } while (k <= nsled);
  
#if 0
  {
    int i;
    printf("Kombinacija:");   
    for (i = 1; i <= nsled; i++) printf("%d ", nkt[i]);  
    printf("\n"); 
  }
#endif
  return (k <= nsled);
}



void
constellations(STARS *s1, STARS *s2, int gottrans, TRANSROT *tr, 
	       MATCH_OPTIONS match_options)
     
{
  double x, y, xj, yj, xref, yref, delta, deltamin = 0, dmag;
  double r, rmax;
  float loto, lotomax, g;
  int nstar1;
  int i, j, k, l;
  int tabzac, tabkon;
  int *indeks;
  int kind, kl;
  double *razdalja;
  double **pot, *kot;
  int **kandidat;
  int *nk, *nkt;
  STAR match;
  int jmatch = 0;
  int nktrot = 0;
  STAR_PAIR *gs;
  float xex, yex;
  double dx, dy;
  float alfa1 = 0.0, alfa2 = 0.0, dalfa = PI - 0.001;
  float dkot1, dkot2, ang;
  double yfind;
  int low, high, j1, j2;
  FL_OBJECT *o;
  XEvent event;
  
  gs = (STAR_PAIR *) myalloc(sizeof(STAR_PAIR) * match_options.firstbright, 
			     "constellations", "gs");
  razdalja = (double *) myalloc(sizeof(double) * (match_options.nstar + 1),
				"constellations", "razdalja");
  /*  Indeksi sledi, ki so najblizje sosede trenutne sledi  */
  indeks = (int *) myalloc(sizeof(int) * (match_options.nstar + 1),
				"constellations", "indeks");
  /*  Tabela kandidatov za sledi v konstelaciji  */
  kandidat = (int **) myalloc(sizeof(int *) * (match_options.nstar + 1),
			     "constellations", "kandidat");
  for (i = 0; i <= match_options.nstar; i++) {
    kandidat[i] = (int *) myalloc(sizeof(int) * CANDIDATE_MAX,
				  "constellations", "kandidat[i]");
  }
  /*  Stevilo kandidatov za posamezne razdalje od opazovane sledi  */
  nk = (int *) myalloc(sizeof(int) * (match_options.nstar + 1),
		       "constellations", "nk");
  
  /*  Trenutni indeksi kandidatov za posamezne razdalje od opazovane sledi  */
  nkt = (int *) myalloc(sizeof(int) * (match_options.nstar + 1),
			"constellations", "nkt");
  /*   Rezerviram tabelo razdalj med sledmi, ki nastopajo v konstelaciji  */
  pot = (double **) myalloc(sizeof(double *) * (match_options.nstar + 1),
			    "constellations", "pot");
  for (k = 0; k <= match_options.nstar; k++) {
    pot[k] = (double *) myalloc(sizeof(double) * (match_options.nstar + 1),
				"constellations", "pot[k]");
  }
  /********KOT********/
  /*   Rezerviram tabelo kotov med sledmi, ki nastopajo v konstelaciji  */
  kot = (double *) myalloc(sizeof(double) * (match_options.nstar + 1),
				"constellations", "kot");
  /********KOT********/
  
  if (state.interactive) open_message("Star matching");
  /*  For all stars from the star map  */
  for (i = 0; i < s1->n; i++) {
    if (state.interactive) {
      char a[50];
      sprintf(a, "Matching star %d of %d", i + 1, s1->n);
      write_message(a);
    }
    x = s1->s[i].x;
    y = s1->s[i].y;
    if (gottrans) {
      xref = tr->a * x + tr->b * y + tr->c;
      yref = tr->d * x + tr->e * y + tr->f;
    }
    else {
      xref = x;
      yref = y;
    }
    nstar1 = match_options.nstar;
#ifdef DEBUG
    if (gottrans)
      printf("i=%d, x=%f, y=%f, mag=%f, n=%d\n", i, xref, yref, s1->s[i].mag, s1->s[i].n); 
#endif
    /*  Initialize distance vector  */
    for (k = 0; k <= nstar1; k++) {
      razdalja[k] = 1e30;
      indeks[k] = -1;
    }
    /*  Find nstar1 closest neighbours */
    for (j = 0; j < s1->n; j++) {
      if (i != j /* && s1->s[j].mag > s1->sp */) {
	r = sqrt(fsqr(x - s1->s[j].x) + fsqr(y - s1->s[j].y));
	/*	printf("r(%d)=%f\n", j, r);   */
	k = nstar1 - 1;
	/*  Is the distance among nstar1 closest distances ?  */
	while (r < razdalja[k] && k >= 0) {
	  razdalja[k + 1] = razdalja[k];
	  razdalja[k] = r;
	  indeks[k + 1] = indeks[k];
	  indeks[k] = j;
	  k--;
	}
      }
    }
    k = nstar1 - 1;
    while (razdalja[k] > 1e29 && k >= 0) k--;
    if (k < 2) continue;    /*  Continue belongs to "for i" loop  */
    nstar1 = k + 1;
    rmax = razdalja[k] + match_options.poserrmax;;
#ifdef DEBUG1
    printf("rmax= %f\n", rmax);
    printf("k=%d\n", k); 
#endif
    for (j = nstar1 - 1; j >= 0; j--) {
      indeks[j + 1] = indeks[j];
      /*      printf("s[%d]=%d\n", j, indeks[j]); */
    }
    indeks[0] = i;
#ifdef DEBUG1
    for (j = 0; j < nstar1; j++) 
      printf("raz(%d)=%.4f, indeks=%d, n=%d\n", 
	     j, razdalja[j], indeks[j], s1->s[indeks[j]].n);
#endif
    /*  Zdaj imam razdalje do prvih nstar1 najblizjih sledi  */
    for (j = 0; j <= nstar1; j++) {
      for (k = j; k <= nstar1; k++) {
	dx = s1->s[indeks[j]].x - s1->s[indeks[k]].x;
	dy = s1->s[indeks[j]].y - s1->s[indeks[k]].y;
	pot[j][k] = sqrt(dx * dx + dy * dy);
	if (pot[j][k] == 0.0) pot[j][k] = 1e-6;
	pot[k][j] = pot[j][k];
	/*	    printf("%.4f  ", pot[j][k]);  */
      }
      /*	  printf("\n");   */
    }
    /*****KOT*******/
    /*  Calculate angles between lines connecting reference star and 
	surrounding star and the horizontal axis  */
    for (k = 1; k <= nstar1; k++) {
      dx = s1->s[indeks[k]].x - s1->s[indeks[0]].x;
      dy = s1->s[indeks[k]].y - s1->s[indeks[0]].y;
      if (dx != 0.0 && dy != 0.0) {
	kot[k] = atan2(dy, dx) - alfa1 + alfa2;
	if (kot[k] > PI) kot[k] -= 2 * PI;
	if (kot[k] < -PI) kot[k] += 2 * PI;
      }
      else {
	kot[k] = 0;
      }
    }
    /******KOT******/
    lotomax = 0;
    
    if (gottrans) {
      /*  Find range of indexes in list s2. where possible candidates for a match */
      /* are located.  Take advantage of that s2 is sorted on y coordinate.  */
      j1 = 0; /*  Lower limit of index */
      j2 = s2->n;  /* Upper limit + 1 */
      /*  Find lower limit first  */
      yfind = yref - match_options.poserrmax;
      do {
	j = (j1 + j2)  / 2;
	if (s2->s[j].y < yfind) {
	  j1 = j + 1;
	}
	else {
	  j2 = j;
	}
      } while (j2 - j1 > 1);
      low = j1;
      /*  Now find higher limit  */
      /* j1 is unchanged from previous loop  */
      j2 = s2->n;
      yfind = yref + match_options.poserrmax;
      do {
	j = (j1 + j2)  / 2;
	if (s2->s[j].y < yfind) {
	  j1 = j + 1;
	}
	else {
	  j2 = j;
	}
      } while (j2 - j1 > 1);
      high = j2;
      deltamin = 1e30;
      jmatch = -1;
    }
    else {
      low = 0;
      high = s2->n;
    }
    /*  For every star calculate the distances to other stars. */
    /*  Search is limited to area where distance in y coordinate from the */
    /*  examined star is not more than maximum distance from the first */
    /*  star in the referential constellation    */
    for (j = low; j < high; j++) {
      /*  Koordinate sledi, za katero preverjam, ce se ujema z i-to sledjo z
	  detektorja 1  */
      xj = s2->s[j].x;
      yj = s2->s[j].y;
#if 1
      if (gottrans) {
	/*  Preverim, ali je sled v okolici mesta, kjer jo pricakujem  */
	xex = xref - xj;
	yex = yref - yj;
	delta = sqrt(xex * xex + yex * yex);
	dmag =  (tr->ma + tr->mb * s1->s[i].mag) / s2->s[j].mag;
	if (delta > match_options.poserrmax /*|| 
					      dmag > match_options.magerrmax || (1.0 / dmag) > match_options.magerrmax*/) {
	  continue;
	}
	if (delta < deltamin) {
	  /*  Found a match  */
	  jmatch = j;
	  match = s2->s[j];
	  deltamin = delta;
	}
      }
      else {
#endif
#if 0
	if (gottrans) {
	  xex = xref - xj;
	  yex = yref - yj;
	  delta = sqrt(xex * xex + yex * yex);
	  if (delta > match_options.poserrmax) {
	    continue;
	  }
	}
#endif

	/*	      printf("s2->s[j].n=%d\n", s2->s[j].n); */
	/*  Ugotovim obmocje v tabeli, kjer so kandidatske sledi */ 
	k = j - 1;
	while (k >= 0 && fabs(yj - s2->s[k].y) <= rmax) {
	  k--;
	}
	if (k < 0) {
	  if (j == 0) {
	    tabzac = 1;
	  }
	  else {
	    tabzac = 0;
	  }
	}
	else tabzac = k + 1;
	
	k = j + 1;
	while (k < s2->n && fabs(yj - s2->s[k].y) <= rmax) {
	  k++;
	}
	if (k == s2->n) {
	  if (j == s2->n - 1) {
	    tabkon = j - 1;
	  }
	  else {
	    tabkon = s2->n - 1;
	  }
	}
	else {
	  tabkon = k - 1;
	}
#ifdef DEBUG1
	printf("tabzac=%d, tabkon=%d\n", tabzac, tabkon);
#endif
	/*  Naredim tabele vseh sledi, ki so priblizno na razdaljah,  */
	/*  shranjenih v vrstici pot[0]                               */
	for (k = 1; k <= nstar1; k++) {
	  /*		printf("%d. %.4f::", k, pot[0][k]);  */
	  nk[k] = 0;
	  for (l = tabzac; l <= tabkon; l++) {
	    if (l != j) {
	      dx = s2->s[l].x - xj;
	      dy = s2->s[l].y - yj;
	      r = sqrt(dx * dx + dy * dy);
#ifdef DEBUG1
	      printf("r=%f, pot[0][%d]=%f\n", r, k, pot[0][k]);
#endif
	      g = fabs(r - pot[0][k]);
	      if (g <= match_options.poserrmax && r > 0) {
		/*****KOT********/
		ang = atan2(dy, dx);
		dkot1 = ang - dalfa;
		if (dkot1 < -PI) dkot1 += 2.0 * PI;
		dkot2 = ang + dalfa;
		if (dkot2 > PI) dkot2 -= 2.0 * PI;
		/*****KOT********/
		if ((dkot1 < dkot2 && kot[k] > dkot1 && kot[k] < dkot2) || (dkot1 > dkot2 && (kot[k] > dkot1 || kot[k] < dkot2))) {
		  nk[k]++;
		  if (nk[k] == CANDIDATE_MAX) {
		    nk[k]--;
		  }
#ifdef DEBUG1
		  printf("nk[%d]=%d, l=%d\n", k, nk[k], l);
#endif
		  kandidat[k][nk[k]] = l;
		}
	      }
	    }
	  }
	  /*		printf("\n");  */
	}
#ifdef DEBUG
	printf("nstar1=%d\n", nstar1);
	for (k = 1; k <= nstar1; k++) {
	  printf("nk[%d]=%d ", k, nk[k]);
	  for (l = 1; l <= nk[k]; l++) {
	    printf("%d ", kandidat[k][l]);
	  }
	  printf("\n");
	}
#endif
	for (k = 1; k <= nstar1; k++) {
	  nkt[k] = 0;
	}
	/*  Generiram vse mogoce konstelacije s sledmi ali brez sledi v */
	/*  seznamu in dolocim, v kateri je najvecje ujemanje  */
	kmax = 1;
	while (increment_constellation(nk, nkt, nstar1, kandidat, pot, s2,
				       match_options.poserrmax)) {
	  loto = 0;  /* Ocena pravilnosti konstelacije  */
	  for (k = 1; k <= kmax; k++) {
	    /*  Indeks kandidata, ki ga preizkusam  */
	    if (nkt[k] == 0) continue;
	    kind = kandidat[k][nkt[k]];
	    x = s2->s[kind].x;
	    y = s2->s[kind].y;
	    r = sqrt(fsqr(xj - x) + fsqr(yj - y));
	    g = fabs(r - pot[0][k]);
	    if (g > match_options.poserrmax) {
	      printf("Napaka: r = %f, g=%f\n", r, g);
	    }
	    loto += 1 - g / match_options.poserrmax;
	    /*  Za vse druge sledi, ki so v konstelaciji, preverim ujemanje */
	    for (l = k + 1; l <= kmax; l++) {
	      if (nkt[l] != 0) {
		kl = kandidat[l][nkt[l]];
		r = sqrt(fsqr(s2->s[kl].x - x) + fsqr(s2->s[kl].y - y));
		if (pot[k][l] > 0) {
		  g = fabs(r - pot[k][l]);
		  if (g <= match_options.poserrmax)  {
		    loto += 1 - g / match_options.poserrmax;
		  }
		  else {
		    int i, ji;   
		    
		    loto = 0;
		    printf("Napaka : kl=%d, l =%d\n", kl, l);
		    printf("nkt[%d]=%d\n", l, nkt[l]);
		    printf("g=%f, r=%f\n", g, r);
		    printf("j=%d, j.n=%d\n", j, s2->s[j].n);  
		    for (i = 1; i <= nstar1; i++) {
		      printf("kan[%d]=%d, nkt[%d]=%d, indeks[%d]=%d\n", i, 
			     kandidat[i][nkt[i]], i, nkt[i], i, indeks[i]);
		      if (nkt[i] > 0) {
			ji = indeks[i];
			printf("n=%d, x=%f, y=%f\n", 
			       s1->s[ji].n, s1->s[ji].x, s1->s[ji].y);
		      }
		    } 
		    for (i = 0; i <= nstar1; i++) {
		      for (ji = 0; ji <= nstar1; ji++) {
			printf("%.4f  ", pot[i][ji]);
		      }
		      printf("\n");
		    }
		    exit(1);
		    break;
		  }
		}
	      }
	    }
	    if (loto == 0) break;
	  }
	  if (loto > lotomax) {
	    lotomax = loto;
	    match = s2->s[j];
	    jmatch = j;
	  }
	}
#if 1
      }
#endif
    }
#ifdef DEBUG
    if (gottrans)
      printf("%d %.2f %.2f %.2f %d %.2f %.2f %.2f %.4f\n", 
	   match.n,  match.x,  match.y, match.mag, 
	   s1->s[i].n, s1->s[i].x, s1->s[i].y, s1->s[i].mag, lotomax);
#endif
    if (!gottrans) {
      if (lotomax > match_options.min) {
	gs[nktrot].x1 = s1->s[i].x;
	gs[nktrot].y1 = s1->s[i].y;
	gs[nktrot].mag1 = s1->s[i].mag;
	gs[nktrot].x2 = match.x;
	gs[nktrot].y2 = match.y;
	gs[nktrot].mag2 = match.mag;
	nktrot++;
      }
    }
    else if (jmatch != -1) {
      int k;

      k = s2->s[jmatch].m;
      /*  Before assigning a match check if a better match already exists for this star  */
      if (k != -1) {
	double xmatch, ymatch, dmatch;
	xmatch = tr->a * s1->s[k].x + tr->b * s1->s[k].y + tr->c;
	ymatch = tr->d * s1->s[k].x + tr->e * s1->s[k].y + tr->f;
	dmatch = sqrt(fsqr(xmatch - s2->s[jmatch].x) + fsqr(ymatch - s2->s[jmatch].y));
	if (deltamin < dmatch) {
	  /*  Remove the previously matched object, because it is not good enough  */
	  s1->s[s2->s[jmatch].m].m = -1;
	  s1->s[i].m = jmatch;
	  s2->s[jmatch].m = i;
	}
      }
      else {
	s1->s[i].m = jmatch;
	s2->s[jmatch].m = i;
      }
    }
    if (state.interactive) {
      if (!gottrans || i % 100 == 0) {
	o = fl_check_forms();
	if (o == FL_EVENT) {
	  fl_XNextEvent(&event);
	}
	else if (o == state.message->messagecancelW) {
	  break;
	}
      }
    }
  }
  if (!gottrans) {
    if (nktrot > 2) {
      tr->hi = transrot(gs, &nktrot, tr, &match_options);
#ifdef DEBUG
      printf("a=%f, b=%f, c=%f, d=%f, e=%f, f=%f, tr.hi=%.2e\n", tr->a, tr->b, tr->c, 
	   tr->d, tr->e, tr->f, tr->hi);
#endif
      tr->hi *= 100.0;
      alfa1 = atan2((double) (gs[1].x1 - gs[0].x1), (double) (gs[1].y1 - gs[0].y1)); 
      alfa2 = atan2((double) (gs[1].x2 - gs[0].x2), (double) (gs[1].y2 - gs[0].y2));
      dalfa = PI / 4;  
    }
    else {
      tr->hi = -1;
    }
  }

  close_message();
  free(kot);
  for (k = match_options.nstar; k >= 0; k--) {
    free(pot[k]);
  }
  free(pot);
  free(nkt);
  free(nk);
  for (i = match_options.nstar; i >= 0; i--) {
    free(kandidat[i]);
  }
  free(kandidat);
  free(indeks);
  free(razdalja);
  free(gs);
}

