/*  catalogs.c  */
/*  part of the fitsblink program  */
/*  routines for setting different star catalogs  */
/*  Jure Skvarc                                   */
#include <stdio.h>
#include <stdlib.h>
 
#include <forms.h>
#include <string.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern CATALOG catalog[];

void
checkusnoC(FL_OBJECT *obj, long val)
{
  catalog[CAT_USNO].use = fl_get_button(obj);
}

void
checkppmC(FL_OBJECT *obj, long val)
{
  catalog[CAT_PPM].use = fl_get_button(obj);
}

void
checkguideC(FL_OBJECT *obj, long val)
{
  if (val == 0) {
    catalog[CAT_GUIDE].use = fl_get_button(obj);
  }
  else {
    catalog[CAT_GUIDE_SOUTH].use = fl_get_button(obj);
  }
}

void
usnodirC(FL_OBJECT *obj, long val)
{
  int l = strlen(fl_get_input(obj));

  if (l > catalog[CAT_USNO].lpath) {
    if ((catalog[CAT_USNO].path = (char *) realloc(catalog[CAT_USNO].path, l)) == NULL) {
      fprintf(stderr, "Out of memory, reallocation of space for USNO catalog path,\n");
      exit(1);
    }
    catalog[CAT_USNO].lpath = l;
  }    
  strcpy(catalog[CAT_USNO].path, fl_get_input(obj));
}

void
ppmdirC(FL_OBJECT *obj, long val)
{
  int l = strlen(fl_get_input(obj));

  if (l > catalog[CAT_PPM].lpath) {
    if ((catalog[CAT_PPM].path = (char *) realloc(catalog[CAT_PPM].path, l)) == NULL) {
      fprintf(stderr, "Out of memory, reallocation of space for PPM catalog path,\n");
      exit(1);
    }
    catalog[CAT_PPM].lpath = l;
  }    
  strcpy(catalog[CAT_PPM].path, fl_get_input(obj));
}

void
guidedirC(FL_OBJECT *obj, long val)
{
  int l = strlen(fl_get_input(obj));

  if (l > catalog[CAT_GUIDE].lpath) {
    if ((catalog[CAT_GUIDE].path = (char *) realloc(catalog[CAT_GUIDE].path, l)) == NULL) {
      fprintf(stderr, "Out of memory, reallocation of space for GUIDE catalog path,\n");
      exit(1);
    }
    catalog[CAT_GUIDE].lpath = l;
  }    
  strcpy(catalog[CAT_GUIDE].path, fl_get_input(obj));
}


void
guidedirsouthC(FL_OBJECT *obj, long val)
{
  int l = strlen(fl_get_input(obj));

  if (l > catalog[CAT_GUIDE_SOUTH].lpath) {
    if ((catalog[CAT_GUIDE_SOUTH].path = (char *) realloc(catalog[CAT_GUIDE_SOUTH].path, l)) == NULL) {
      fprintf(stderr, "Out of memory, reallocation of space for GUIDE catalog path,\n");
      exit(1);
    }
    catalog[CAT_GUIDE_SOUTH].lpath = l;
  }    
  strcpy(catalog[CAT_GUIDE_SOUTH].path, fl_get_input(obj));
}


/*  Check if the catalogs are found at their default paths  */
void
check_catalogs(void)

{
  char *file;
  FILE *fp;

  /*  Check for USNO SA 1.0  */
  file = myalloc(sizeof(char) * (strlen(catalog[CAT_USNO].path) + 13), "check_catalogs", "file");
  sprintf(file, "%s/zone%04d.cat", catalog[CAT_USNO].path, 0);
  /*  Can I open catalog file? */
  if ((catalog[CAT_USNO].use = (((fp = fopen(file, "r")) != NULL)))) {
    fclose(fp);
  }
  free(file);
  /*  Check for GSC north */
  file = myalloc(sizeof(char) * (strlen(catalog[CAT_GUIDE].path) + 
				strlen("/gsc/n0000/0001.gsc") + 1),
		"check_catalogs", "file");
  sprintf(file, "%s/gsc/n0000/0001.gsc", catalog[CAT_GUIDE].path);
  /*  Can I open catalog file? */
  if ((catalog[CAT_GUIDE].use = (((fp = fopen(file, "r")) != NULL)))) {
    fclose(fp);
  }
  free(file);

  if (!catalog[CAT_GUIDE].use) {
    /*  Check for GSC north again  */
    file = myalloc(sizeof(char) * (strlen(catalog[CAT_GUIDE].path) + 
				  strlen("/gsc/n0000/0001.gsc.gz") + 1),
		  "check_catalogs", "file");
    sprintf(file, "%s/gsc/n0000/0001.gsc.gz", catalog[CAT_GUIDE].path);
    /*  Can I open catalog file? */
    if ((catalog[CAT_GUIDE].use = (((fp = fopen(file, "r")) != NULL)))) {
      fclose(fp);
    }
    free(file);
  }
  /*  Check for GSC south */
  file = myalloc(sizeof(char) * (strlen(catalog[CAT_GUIDE_SOUTH].path) + 
		  strlen("/gsc/s7500/9346.gsc") + 1), "check_catalogs", "file");
  sprintf(file, "%s/gsc/s7500/9346.gsc", catalog[CAT_GUIDE_SOUTH].path);
  /*  Can I open catalog file? */
  if ((catalog[CAT_GUIDE_SOUTH].use = (((fp = fopen(file, "r")) != NULL)))) {
    fclose(fp);
  }
  free(file);
  if (!catalog[CAT_GUIDE_SOUTH].use) {
    /*  Check for GSC south again*/
    file = myalloc(sizeof(char) * (strlen(catalog[CAT_GUIDE_SOUTH].path) + 
				   strlen("/gsc/s7500/9346.gsc.gz") + 1), 
		   "check_catalogs", "file");
    sprintf(file, "%s/gsc/s7500/9346.gsc.gz", catalog[CAT_GUIDE_SOUTH].path);
    /*  Can I open catalog file? */
    if ((catalog[CAT_GUIDE_SOUTH].use = (((fp = fopen(file, "r")) != NULL)))) {
      fclose(fp);
    }
    free(file);
  }
}


/*  Edit catalogs */
void
edit_catalogs(FL_OBJECT *obj, int m)

{
  int n;

  /*  The window in not created yet, do it now  */
  if (state.catalogs == NULL) {
    state.catalogs = create_form_Catalog();
    fl_set_form_minsize(state.catalogs->Catalog, 
		  state.catalogs->Catalog->w, state.catalogs->Catalog->h);
  }
  /*  Deactivate CATALOGS item in the menu */
  fl_set_menu_item_mode(obj, m, FL_PUP_GREY);
  fl_show_form(state.catalogs->Catalog,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Star catalogs");
  /*  Write current parameter values into the form */
  n = state.control[state.control_num];

  fl_set_button(state.catalogs->checkusnoW, catalog[CAT_USNO].use);
  fl_set_input(state.catalogs->usnodirW, catalog[CAT_USNO].path);
  fl_set_button(state.catalogs->checkppmW, catalog[CAT_PPM].use);
  fl_set_input(state.catalogs->ppmdirW, catalog[CAT_PPM].path);
  /*  Hide PPM for now  */
  fl_hide_object(state.catalogs->checkppmW);
  fl_hide_object(state.catalogs->ppmdirW);

  fl_set_button(state.catalogs->checkguideW, catalog[CAT_GUIDE].use);
  fl_set_button(state.catalogs->checkguidesouthW, catalog[CAT_GUIDE_SOUTH].use);
  fl_set_input(state.catalogs->guidedirW, catalog[CAT_GUIDE].path);
  fl_set_input(state.catalogs->guidedirsouthW, catalog[CAT_GUIDE_SOUTH].path);
}

