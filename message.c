/*  message.c  */
/*  part of a fitsblink program  */
/*  Jure Skvarc, 1998  */
#include <stdio.h>
#include <forms.h>

#include "functs.h"

extern STATE state;

/*=======================================*/
/*  Create and display a message window  */
/*=======================================*/
void
open_message(char *a)

{
  /*  The window in not created yet, do it now  */
  if (state.message == NULL) {
    state.message = create_form_Message();
  }
  if (!fl_form_is_visible(state.message->Message)) {
    fl_show_form(state.message->Message,  FL_FIX_SIZE, FL_FULLBORDER | FL_PLACE_FREE_CENTER, a);
  }
  fl_deactivate_all_forms();
  fl_activate_form(state.message->Message);
}


/*=======================================*/
/*  Close a message window  */
/*=======================================*/
void
close_message(void)

{
  if (state.message != NULL)
    fl_hide_form(state.message->Message);
  fl_activate_all_forms();
}


/*=======================================*/
/*  Write a string into a message window */
/*=======================================*/
void
write_message(char *a)

{
  if (state.message != NULL) {
    fl_set_object_label(state.message->messagetextW, a);
    XFlush(state.disp);
  }
}
