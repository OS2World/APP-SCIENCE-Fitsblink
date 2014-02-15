#include <stdio.h>
#include <malloc.h>
#include "functs.h"


/*   Determination of the objects' contour  */
/*   From this we can get real area and perimeter  */
int
  obris(OBJEKT *obj, FITS_IMAGE slika)
{
  static int disx[] = {1, 0, -1, 0};
  static int sosx[] = {0, -1, 0, 1};
  static int disy[] = {0, 1, 0, -1};
  static int sosy[] = {1, 0, -1, 0};
  int x, y, xp, yp, xt, yt, xc, yc;
  int smer;
  int tsize;
  POINT *t;
  SENCA senca1, *s;
  int i = 0;

  /*	back = find_class(razred.back);  */
  /*	Kazalec na seznam zunanjih tock  */
  tsize = 4 * obj->perim;
  t = (POINT *) myalloc(tsize * sizeof(POINT), "obris", "t");
  /*******************************/
  
  s = &senca1;
  /*	Sirina slike je za dve vecja od originalne sirine  */
  senca1.width = (obj->x2 - obj->x1 + 1 + 2);
  /*	Rezerviram prostor za pomozno sliko  */
  senca1.size = senca1.width * (obj->y2 - obj->y1 + 1 + 2);
  senca1.p = (PIXEL *) myalloc(sizeof(PIXEL) * senca1.size, "obris", "senca1.p");
  /*	Postavim offset relativno na zacetek objekta  */
  senca1.x0 = obj->x1 - 1;
  senca1.y0 = obj->y1 - 1;
  /*	Ocistim pomozno sliko  */
  clear_image(s);
  /*  Naredim enobitno kopijo objekta v senca1 (*s) */
  for (i = 0; i < obj->area; i++) {
    set(s, (obj->inner)[i].x, (obj->inner)[i].y);
  }
  /*******************************/
  obj->outer = t;
  /*	Zdaj se lahko sprehodim po obrisu in naredim seznam zunanjih tock */
  /*	Zacnem eno tocko nad prvo tocko v objektu, na katero sem bil naletel  */
  /*	Objekt obrisujem v smeri urinega kazalca in zacnem zgoraj  */
  xp = x = obj->xu;
  yp = y = obj->y1 - 1;
  xc = 0;
  yc = 0;
  /*	Najprej je smer gibanja v desno  */
  smer = 0;
  /*	Stevec zunanjih tock  */
  obj->p_out = 0;
  do {
    /*	Premaknem se za eno mesto naprej  */
    x += disx[smer];
    y += disy[smer];
    
    /*	Ce sem naletel na del objekta, se moram obrniti v obratni smeri
       urinega kazalca  */
    if (test(s, x, y)) {
      x -= disx[smer];	/*	Popravim se spet na stare koordinate  */
      y -= disy[smer];
      smer--;         	/*	Obrnem se v nasprotni smeri urinega kazalca  */
      if (smer < 0) smer = 3;
      continue;  /*	Poskusim znova  */
    }
    /*	Ce sem se prevec oddaljil od objekta, se popravim  */
    if (!test(s, xt = x + sosx[smer], yt = y + sosy[smer])) {
      x = xt;
      y = yt;
      /*	Spremenim smer   */
      smer++;
      if (smer == 4) smer = 0;
    }
    /*	Spravim koordinate  */
    t->x = x;
    xc += x;
    t->y = y;
    yc += y;
    /*	Vrednosti zaenkrat ne potrebujem  */
    /*	t->v = video_point(slika, x + obj->x1 + 1, y + obj->y1 + 1);  */
    t++;
    obj->p_out++;
    if (obj->p_out == tsize) {
      tsize = 2 * tsize;
      obj->outer = (POINT *)
	realloc(obj->outer, tsize * sizeof(POINT));
      if (obj->outer == NULL) {
	printf("Premalo pomnilnika, funkcija obris, spremenljivka obj->outer\n");
	printf("tsize=%d\n", tsize);
	exit(1);
      }
      t = obj->outer + obj->p_out;
    }
    i++;
  } while(x != xp || y != yp || obj->p_out == 0);
  obj->xt = (double) xc / obj->p_out;
  obj->yt = (double) yc / obj->p_out;
  free(s->p);
  /*	Vrnem odvecni prostor  */
  obj->outer = (POINT *) realloc(obj->outer, obj->p_out * sizeof(POINT));
  if (obj->outer == NULL) {
    printf("Premalo pomnilnika, funkcija obris, spremenljivka obj->outer.2\n");
    exit(1);
  }
  return(0);
}

