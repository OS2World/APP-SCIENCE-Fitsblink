/*  mail.c  */
/*  part of the fitsblink program  */
/*  routines for sending report by email */
/*  Jure Skvarc,  May 1998                   */
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <forms.h>
#include <time.h>

#include "formblink.h"
#include "functs.h"

extern STATE state;
extern BLINK_FRAME *frame;
extern MAIL mail;
extern OBSERVATORY observatory;
extern CATALOG catalog[];

void 
toaddressC(FL_OBJECT *obj, long val) 
{
  if (mail.to) {
    free(mail.to);
  }
  mail.to = strdup((char *) fl_get_input(obj));
}

void 
ccaddressC(FL_OBJECT *obj, long val) 
{
  if (mail.cc) {
    free(mail.cc);
  }
  mail.cc = strdup((char *) fl_get_input(obj));
}

void 
mailtitleC(FL_OBJECT *obj, long val) 
{
  if (mail.title) {
    free(mail.title);
  }
  mail.title = strdup((char *) fl_get_input(obj));
}

void
mailfileC(FL_OBJECT *obj, long val)

{
  if (mail.file) free(mail.file);
  mail.file = strdup((char *) fl_get_input(obj));
  if (!fl_load_browser(state.mail->messageW, mail.file)) {
    fl_show_alert("Can't open file to mail", "", "", 1);
  }
}


void 
messageC(FL_OBJECT *obj, long val) 
{
}


/*  Get a file name and tranfer it into file input field  */
/*  Also, read a file into a browser window  */
void
findfileC(FL_OBJECT *obj, long val)

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.mail";
  static char *name = "";

  fl_use_fselector(2);
  fl_invalidate_fselector_cache();
  fn = fl_show_fselector("Choose file to mail", curdir, pattern, name);
  if (fn == NULL) return;
  /*  Save the current directory  */
  curdir = (char *) fl_get_directory();
  /*  Save the current pattern */
  pattern = (char *) fl_get_pattern();
  /*  Set the file name  */
  fl_set_input(state.mail->mailfileW, fn);
  mailfileC(state.mail->mailfileW, 0);
}


/*  Edit a line from a browser  */
/*==============================*/
void 
editorC(FL_OBJECT *obj, long val) 
{
  int i, n;
  FL_OBJECT *br = state.mail->messageW;

  /*  Check if any line in a browser is selected  */
  n = fl_get_browser_maxline(br);
  for (i = 1; i <= n; i++) {
    if (fl_isselected_browser_line(br, i)) break;
  }
  if (i <= n) {
    fl_replace_browser_line(br, i, fl_get_input(obj));
  }
  else {
    fl_addto_browser(br, fl_get_input(obj));
  }
}

void 
editlineC(FL_OBJECT *obj, long val) 
{
  int i, n;
  FL_OBJECT *br = state.mail->messageW;

  /*  Check if any line in a browser is selected  */
  n = fl_get_browser_maxline(br);
  for (i = 1; i <= n; i++) {
    if (fl_isselected_browser_line(br, i)) break;
  }
  if (i <= n) {
    fl_set_input(state.mail->editorW, fl_get_browser_line(br, fl_get_browser(br)));
  }
}


/*  Insert a line before the currently selected line  */
void 
insertlineC(FL_OBJECT *obj, long val) 
{
  int i, n;
  FL_OBJECT *br = state.mail->messageW;

  /*  Check if any line in a browser is selected  */
  n = fl_get_browser_maxline(br);
  for (i = 1; i <= n; i++) {
    if (fl_isselected_browser_line(br, i)) break;
  }
  if (i <= n) {
    fl_insert_browser_line(br, i, fl_get_input(state.mail->editorW));
  }
}

void 
deletelineC(FL_OBJECT *obj, long val) 
{
  int i, n;
  FL_OBJECT *br = state.mail->messageW;

  /*  Check if any line in a browser is selected  */
  n = fl_get_browser_maxline(br);
  for (i = 1; i <= n; i++) {
    if (fl_isselected_browser_line(br, i)) break;
  }
  if (i <= n) {
    fl_delete_browser_line(br, i);
  }
}


/*   Include a header in a browser   */
void 
headerC(FL_OBJECT *obj, long val) 

{
  char a[81];
  FL_OBJECT *br = state.mail->messageW;
  time_t t;
  int i;

  t = time(NULL);
  sprintf(a, "COD %d", observatory.code);
  fl_insert_browser_line(br, 1, a);
  sprintf(a, "OBS %s", observatory.observer);
  fl_insert_browser_line(br, 2, a);
  sprintf(a, "TEL %s", observatory.telescope);
  fl_insert_browser_line(br, 3, a);
  for (i = 0; i < NUMBER_OF_CATALOGS; i++) {
    if (catalog[i].use) {
      sprintf(a, "NET %s", catalog[i].name);
    }
  }
  fl_insert_browser_line(br, 4, a);
  sprintf(a, "ACK Fitsblink %s report generated on %s", VERSION,
	  asctime(gmtime(&t)));
  fl_insert_browser_line(br, 5, a);
}

int
save_browser(FILE *fp)

{
  int i, n;

  n = fl_get_browser_maxline(state.mail->messageW);
  for (i = 1; i <= n; i++) {
    fprintf(fp, "%s\n", fl_get_browser_line(state.mail->messageW, i));
  }
  return 0;
}


void 
sendmailC(FL_OBJECT *obj, long val) 

{
  char a[1024], *name;
  FILE *fp;
  
  /*  Generate a temporary file  */
  name = tmpnam(NULL);
  if (name == NULL || (fp = fopen(name, "w")) == NULL) {
    fl_show_alert("We have a problem: Can't generate a temporary file!", "", "", 1);
    return;
  }
  save_browser(fp);
  fclose(fp);
  sprintf(a, "Do you really want to send report to %s?", mail.to);
  if (fl_show_question(a, 0)) {
    sprintf(a, "cat %s | mail %s %s %s %s %s", name, 
	  (strlen(mail.title) > 0) ? "-s" : "", 
	  (strlen(mail.title) > 0) ? mail.title : "", 
	  (strlen(mail.cc) > 0) ? "-c" : "",
	  (strlen(mail.cc) > 0) ? mail.cc : "", mail.to);
    system(a);
  }
  remove(name);
}


void 
savemailC(FL_OBJECT *obj, long val) 

{
  const char *fn;
  static char *curdir = "";
  static char *pattern = "*.mail";
  static char *name = "";
  FILE *fp;

  fl_use_fselector(5);
  fl_invalidate_fselector_cache();
  fn = fl_show_fselector("Choose output file", curdir, pattern, name);
  if (fn == NULL) return;
  /*  Save the current directory  */
  curdir = (char *) fl_get_directory();
  /*  Save the current pattern */
  pattern = (char *) fl_get_pattern();
  name = (char *) fl_get_filename();
  if (!(fp = fopen(fn, "w"))) {
    fl_show_alert("Can not open file", fn, "for writing!", 0);
    return;
  }
  save_browser(fp);
  fclose(fp);
}


/*  Open and show the mail window  */
/*=================================*/
void
edit_mail(FL_OBJECT *obj, int m)
{

  /*  The window in not created yet, do it now  */
  if (state.mail == NULL) {
    state.mail = create_form_Mail();
    fl_set_form_minsize(state.mail->Mail, 
		  state.mail->Mail->w, state.mail->Mail->h);
    fl_set_browser_fontsize(state.mail->messageW, 12);
    fl_set_browser_fontstyle(state.mail->messageW, FL_FIXED_STYLE);
    if (!fl_load_browser(state.mail->messageW, mail.file)) {
      fl_show_alert("Can't open file to mail", "", "", 1);
    }
  }
  /*  Deactivate OPTIONS item in the menu */
  fl_set_menu_item_mode(obj, m, FL_PUP_GREY);
  fl_show_form(state.mail->Mail,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, "Image options");
  fl_set_input(state.mail->toaddressW, mail.to);
  fl_set_input(state.mail->ccaddressW, mail.cc);
  fl_set_input(state.mail->mailtitleW, mail.title);
  fl_set_input(state.mail->mailfileW, mail.file);
}

