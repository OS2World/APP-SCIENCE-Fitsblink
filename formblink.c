/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "formblink.h"

FD_setting *create_form_setting(void)
{
  FL_OBJECT *obj;
  FD_setting *fdui = (FD_setting *) fl_calloc(1, sizeof(*fdui));

  fdui->setting = fl_bgn_form(FL_NO_BOX, 390, 480);
  obj = fl_add_box(FL_UP_BOX,0,0,390,480,"");
    fl_set_object_resize(obj, FL_RESIZE_NONE);
  fdui->secondleraseW = obj = fl_add_button(FL_NORMAL_BUTTON,285,40,45,25,"Erase");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,eraseC,1);
  fdui->secondgreyW = obj = fl_add_xyplot(FL_NORMAL_XYPLOT,205,120,140,130,"Grey level histogram");
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,greyC,1);
  fdui->firstgreyW = obj = fl_add_xyplot(FL_NORMAL_XYPLOT,15,120,140,130,"Grey level histogram");
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,greyC,0);
  fdui->firstgammaW = obj = fl_add_xyplot(FL_ACTIVE_XYPLOT,15,265,140,140,"Grey level correction");
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,gammaC,0);
  fdui->secondgammaW = obj = fl_add_xyplot(FL_ACTIVE_XYPLOT,205,265,140,140,"Grey level correction");
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
    fl_set_object_lsize(obj,FL_DEFAULT_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,gammaC,1);

  fdui->firstsecond = fl_bgn_group();
  fdui->firstimageW = obj = fl_add_button(FL_RADIO_BUTTON,50,5,80,30,"1. image");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,turnimageC,0);
  fdui->secondimageW = obj = fl_add_button(FL_RADIO_BUTTON,240,5,80,30,"2. image");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,turnimageC,1);
  fl_end_group();

  fdui->firstdeltaxW = obj = fl_add_input(FL_FLOAT_INPUT,35,450,50,25,"dx");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,deltaxC,0);
  fdui->firstdeltayW = obj = fl_add_input(FL_FLOAT_INPUT,105,450,50,25,"dy");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,deltayC,0);
  fdui->seconddeltaxW = obj = fl_add_input(FL_FLOAT_INPUT,220,450,55,25,"dx");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,deltaxC,1);
  fdui->seconddeltayW = obj = fl_add_input(FL_FLOAT_INPUT,295,450,50,25,"dy");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,deltayC,1);
  fdui->firstloadW = obj = fl_add_button(FL_NORMAL_BUTTON,40,40,45,25,"Load");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,loadC,0);
  fdui->firstleraseW = obj = fl_add_button(FL_NORMAL_BUTTON,95,40,45,25,"Erase");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,eraseC,0);
  fdui->secondloadW = obj = fl_add_button(FL_NORMAL_BUTTON,230,40,45,25,"Load");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,loadC,1);
  fdui->quitW = obj = fl_add_button(FL_NORMAL_BUTTON,160,40,45,30,"Quit");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,quitC,0);
  fdui->firstinvertW = obj = fl_add_button(FL_NORMAL_BUTTON,160,310,35,25,"Invert");
    fl_set_object_callback(obj,invertC,0);
  fdui->firstautoW = obj = fl_add_button(FL_NORMAL_BUTTON,160,280,35,25,"Auto");
    fl_set_object_callback(obj,autoC,0);
  fdui->firstnameW = obj = fl_add_choice(FL_NORMAL_CHOICE,50,70,90,30,"File");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,nameC,0);
  fdui->secondnameW = obj = fl_add_choice(FL_NORMAL_CHOICE,240,70,90,30,"File");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,nameC,1);
  fdui->firstfitsW = obj = fl_add_button(FL_NORMAL_BUTTON,160,130,35,25,"FITS");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,fitsC,0);
  fdui->secondfitsW = obj = fl_add_button(FL_NORMAL_BUTTON,350,130,35,25,"FITS");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,fitsC,1);
  fdui->secondinvertW = obj = fl_add_button(FL_NORMAL_BUTTON,350,310,35,25,"Invert");
    fl_set_object_callback(obj,invertC,1);
  fdui->secondautoW = obj = fl_add_button(FL_NORMAL_BUTTON,350,280,35,25,"Auto");
    fl_set_object_callback(obj,autoC,1);
  fdui->helpW = obj = fl_add_menu(FL_PUSH_MENU,160,80,45,35,"Help");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE);
    fl_set_object_callback(obj,helpC,0);
  fdui->leftlowdecW = obj = fl_add_button(FL_NORMAL_BUTTON,10,410,35,35,"@<");
    fl_set_object_callback(obj,histadjC,1);
  obj = fl_add_button(FL_NORMAL_BUTTON,45,410,35,35,"@>");
    fl_set_object_callback(obj,histadjC,2);
  obj = fl_add_button(FL_NORMAL_BUTTON,90,410,35,35,"@<");
    fl_set_object_callback(obj,histadjC,3);
  obj = fl_add_button(FL_NORMAL_BUTTON,125,410,35,35,"@>");
    fl_set_object_callback(obj,histadjC,4);
  obj = fl_add_button(FL_NORMAL_BUTTON,200,410,35,35,"@<");
    fl_set_object_callback(obj,histadjC,5);
  obj = fl_add_button(FL_NORMAL_BUTTON,235,410,35,35,"@>");
    fl_set_object_callback(obj,histadjC,6);
  obj = fl_add_button(FL_NORMAL_BUTTON,280,410,35,35,"@<");
    fl_set_object_callback(obj,histadjC,7);
  obj = fl_add_button(FL_NORMAL_BUTTON,315,410,35,35,"@>");
    fl_set_object_callback(obj,histadjC,8);
  fl_end_form();

  fdui->setting->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_blinker *create_form_blinker(void)
{
  FL_OBJECT *obj;
  FD_blinker *fdui = (FD_blinker *) fl_calloc(1, sizeof(*fdui));

  fdui->blinker = fl_bgn_form(FL_NO_BOX, 1000, 910);
  obj = fl_add_box(FL_UP_BOX,0,0,1000,910,"");
  fdui->ycoorW = obj = fl_add_input(FL_NORMAL_INPUT,255,10,90,40,"y=");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_callback(obj,emptyC,0);
  fdui->zoom1W = obj = fl_add_box(FL_FRAME_BOX,10,716,251,141,"");
    fl_set_object_callback(obj,zoom1C,0);
  fdui->zoom2W = obj = fl_add_box(FL_FRAME_BOX,271,716,251,141,"");
    fl_set_object_callback(obj,zoom2C,0);
  fdui->lockedW = obj = fl_add_text(FL_NORMAL_TEXT,195,871,152,29,"LOCKED !");
    fl_set_object_color(obj,FL_RED,FL_BOTTOM_BCOL);
    fl_set_object_lcolor(obj,FL_WHITE);
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,emptyC,0);
  fdui->valueW = obj = fl_add_input(FL_NORMAL_INPUT,385,10,85,40,"v=");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_callback(obj,emptyC,0);
  fdui->racoorW = obj = fl_add_input(FL_NORMAL_INPUT,120,60,125,40,"ra=");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_callback(obj,emptyC,0);
  fdui->deccoorW = obj = fl_add_input(FL_NORMAL_INPUT,310,60,120,40,"dec=");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_callback(obj,emptyC,0);
  fdui->xcoorW = obj = fl_add_input(FL_NORMAL_INPUT,120,10,95,40,"x=");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_callback(obj,emptyC,0);
  fdui->astrometryW = obj = fl_add_menu(FL_PULLDOWN_MENU,510,15,105,35,"Astrometry");
    fl_set_object_boxtype(obj,FL_UP_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,astrometryC,0);

  fdui->bgroupW = fl_bgn_group();
  fdui->playW = obj = fl_add_lightbutton(FL_PUSH_BUTTON,890,720,85,40,"Blink");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_gravity(obj, FL_East, FL_East);
  fdui->leftW = obj = fl_add_button(FL_NORMAL_BUTTON,530,770,35,35,"@4>");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,leftC,0);
  fdui->upW = obj = fl_add_button(FL_NORMAL_BUTTON,565,735,35,35,"@8>");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,upC,0);
  fdui->rightW = obj = fl_add_button(FL_NORMAL_BUTTON,600,770,35,35,"@6>");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,rightC,0);
  fdui->downW = obj = fl_add_button(FL_NORMAL_BUTTON,565,805,35,35,"@2>");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,downC,0);
  fdui->circleW = obj = fl_add_button(FL_NORMAL_BUTTON,565,770,35,35,"@circle");
    fl_set_object_lcolor(obj,FL_BLUE);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,circleC,0);
  fl_end_group();

  fdui->blinkW = obj = fl_add_free(FL_NORMAL_FREE,10,105,980,605,"",
			freeobj_blinkW_handle);
    fl_set_object_boxtype(obj,FL_EMBOSSED_BOX);
  fdui->deltatW = obj = fl_add_counter(FL_SIMPLE_COUNTER,890,785,85,40,"Delay");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_gravity(obj, FL_East, FL_East);
    fl_set_object_callback(obj,deltatC,0);
  fdui->telescope_controlW = obj = fl_add_button(FL_NORMAL_BUTTON,620,15,140,35,"Telescope control");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telescope_controlC,0);
  fdui->markersW = obj = fl_add_lightbutton(FL_RADIO_BUTTON,840,50,75,35,"Mark.");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,markersC,0);
    fl_set_button(obj, 1);

  fdui->colorsW = fl_bgn_group();
  fdui->greybutW = obj = fl_add_lightbutton(FL_RADIO_BUTTON,840,15,75,35,"Grey");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,colormapC,1);
    fl_set_button(obj, 1);
  fdui->colorbutW = obj = fl_add_lightbutton(FL_RADIO_BUTTON,915,15,75,35,"Color");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,colormapC,2);
  fl_end_group();

  fl_end_form();

  fdui->blinker->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Edit *create_form_Edit(void)
{
  FL_OBJECT *obj;
  FD_Edit *fdui = (FD_Edit *) fl_calloc(1, sizeof(*fdui));

  fdui->Edit = fl_bgn_form(FL_NO_BOX, 440, 470);
  fdui->helpW = obj = fl_add_box(FL_UP_BOX,0,0,440,470,"");
  fdui->browseW = obj = fl_add_browser(FL_NORMAL_BROWSER,15,15,405,395,"");
  fdui->closeeditW = obj = fl_add_button(FL_NORMAL_BUTTON,15,425,80,35,"Close");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->Edit->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_info *create_form_info(void)
{
  FL_OBJECT *obj;
  FD_info *fdui = (FD_info *) fl_calloc(1, sizeof(*fdui));

  fdui->info = fl_bgn_form(FL_NO_BOX, 450, 240);
  obj = fl_add_box(FL_UP_BOX,0,0,450,240,"");
  obj = fl_add_text(FL_NORMAL_TEXT,125,95,225,30,"by Jure Skvarc, December 1998");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,60,140,370,45,"Please send remarks, questions and bug reports to:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,160,170,125,25,"jure.skvarc@ijs.si");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,15,65,415,30,"A program for display and visual comparison of FITS images");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->infookW = obj = fl_add_button(FL_NORMAL_BUTTON,10,195,430,35,"OK");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->versionW = obj = fl_add_text(FL_NORMAL_TEXT,100,20,230,35,"Fitsblink 2.21");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->info->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Error *create_form_Error(void)
{
  FL_OBJECT *obj;
  FD_Error *fdui = (FD_Error *) fl_calloc(1, sizeof(*fdui));

  fdui->Error = fl_bgn_form(FL_NO_BOX, 440, 470);
  obj = fl_add_box(FL_UP_BOX,0,0,440,470,"");
  fdui->closeW = obj = fl_add_button(FL_NORMAL_BUTTON,15,420,80,40,"Close");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  fdui->errorW = obj = fl_add_browser(FL_NORMAL_BROWSER,15,10,415,405,"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->Error->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_help *create_form_help(void)
{
  FL_OBJECT *obj;
  FD_help *fdui = (FD_help *) fl_calloc(1, sizeof(*fdui));

  fdui->help = fl_bgn_form(FL_NO_BOX, 400, 470);
  fdui->helpW = obj = fl_add_box(FL_UP_BOX,0,0,400,470,"");
  fdui->readmeW = obj = fl_add_browser(FL_NORMAL_BROWSER,10,10,385,410,"");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,emptyC,0);
  fdui->closehelpW = obj = fl_add_button(FL_NORMAL_BUTTON,10,430,70,30,"Close");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  fl_end_form();

  fdui->help->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Parameters *create_form_Parameters(void)
{
  FL_OBJECT *obj;
  FD_Parameters *fdui = (FD_Parameters *) fl_calloc(1, sizeof(*fdui));

  fdui->Parameters = fl_bgn_form(FL_NO_BOX, 430, 680);
  obj = fl_add_box(FL_UP_BOX,0,0,430,680,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,20,400,395,60,"Object");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,20,60,395,60,"Center coordinates");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,20,270,395,110,"Time");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,20,200,395,60,"Rotation angle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,20,130,395,60,"Pixel size");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->rainputW = obj = fl_add_input(FL_NORMAL_INPUT,60,80,115,30,"RA:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,rainputC,0);
  fdui->decinputW = obj = fl_add_input(FL_NORMAL_INPUT,275,80,115,30,"Dec:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,decinputC,0);
  fdui->xpixelW = obj = fl_add_input(FL_FLOAT_INPUT,75,145,100,30,"x:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,xpixelC,0);
  fdui->ypixelW = obj = fl_add_input(FL_FLOAT_INPUT,290,145,100,30,"y:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,ypixelC,0);
  fdui->rotationW = obj = fl_add_input(FL_NORMAL_INPUT,80,215,95,30,"decimal degrees");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,rotationC,0);
  fdui->timeyearW = obj = fl_add_input(FL_INT_INPUT,65,300,55,30,"Year");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timeyearC,0);
  fdui->timemonthW = obj = fl_add_input(FL_INT_INPUT,125,300,35,30,"Month");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timemonthC,0);
  fdui->timehourW = obj = fl_add_input(FL_INT_INPUT,245,300,35,30,"Hours");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timehourC,0);
  fdui->timeminuteW = obj = fl_add_input(FL_INT_INPUT,285,300,35,30,"Min");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timeminuteC,0);
  fdui->timesecondW = obj = fl_add_input(FL_FLOAT_INPUT,325,300,65,30,"Sec");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timesecondC,0);
  fdui->timedecdayW = obj = fl_add_input(FL_FLOAT_INPUT,265,340,130,30,"Decimal day");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timedecdayC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,175,145,20,35,"\"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,390,145,20,35,"\"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->parokW = obj = fl_add_button(FL_NORMAL_BUTTON,175,625,100,45,"OK");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->timedayW = obj = fl_add_input(FL_INT_INPUT,165,300,35,30,"Day");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,timedayC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,135,10,155,40,"Image parameters");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->objectW = obj = fl_add_input(FL_NORMAL_INPUT,30,420,375,30,"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,objectC,0);
  fdui->observerW = obj = fl_add_input(FL_NORMAL_INPUT,110,475,295,30,"Observer");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,observerC,0);
  fdui->telescopeW = obj = fl_add_input(FL_NORMAL_INPUT,110,510,295,30,"Telescope");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telescopeC,0);
  fdui->instrumentW = obj = fl_add_input(FL_NORMAL_INPUT,110,545,295,30,"Instrument");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,instrumentC,0);
  fl_end_form();

  fdui->Parameters->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Report *create_form_Report(void)
{
  FL_OBJECT *obj;
  FD_Report *fdui = (FD_Report *) fl_calloc(1, sizeof(*fdui));

  fdui->Report = fl_bgn_form(FL_NO_BOX, 885, 650);
  obj = fl_add_box(FL_UP_BOX,0,0,885,650,"");
  fdui->reportW = obj = fl_add_browser(FL_SELECT_BROWSER,15,15,855,585,"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,reportC,0);
  fdui->closereportW = obj = fl_add_button(FL_NORMAL_BUTTON,740,610,115,35,"Close");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
  fdui->savereportW = obj = fl_add_button(FL_NORMAL_BUTTON,20,610,115,35,"Save");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,savereportC,0);
  fl_end_form();

  fdui->Report->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Options *create_form_Options(void)
{
  FL_OBJECT *obj;
  FD_Options *fdui = (FD_Options *) fl_calloc(1, sizeof(*fdui));

  fdui->Options = fl_bgn_form(FL_NO_BOX, 850, 515);
  obj = fl_add_box(FL_UP_BOX,0,0,850,515,"");
  obj = fl_add_frame(FL_ENGRAVED_FRAME,445,365,380,85,"");
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,430,140,405,315,"Star counting");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,15,140,405,315,"Star matching");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->logfileW = obj = fl_add_input(FL_NORMAL_INPUT,225,70,175,35,"Name of the log file");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,logfileC,0);
  fdui->minconstW = obj = fl_add_input(FL_FLOAT_INPUT,335,160,65,35,"Minimal value to accept constellation");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,minconstC,0);
  fdui->initconstW = obj = fl_add_input(FL_INT_INPUT,335,200,65,35,"Number of bright stars in initial  matching");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,initconstC,0);
  fdui->starconstW = obj = fl_add_input(FL_INT_INPUT,335,240,65,35,"Number of stars in constellation");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starconstC,0);
  fdui->countsigmaW = obj = fl_add_input(FL_FLOAT_INPUT,755,160,65,35,"Sigma above background");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,countsigmaC,0);
  fdui->countminvalW = obj = fl_add_input(FL_INT_INPUT,755,200,65,35,"Minimal accepted intensity");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,countminvalC,0);
  fdui->starerrorW = obj = fl_add_input(FL_FLOAT_INPUT,335,280,65,35,"Maximal positional error in pixels");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starerrorC,0);
  fdui->starminsizeW = obj = fl_add_input(FL_INT_INPUT,755,240,65,35,"Minimal star size in pixels");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starminsizelC,0);
  fdui->acceptoptionsW = obj = fl_add_button(FL_NORMAL_BUTTON,20,460,95,45,"Accept");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->defaultoptionsW = obj = fl_add_button(FL_NORMAL_BUTTON,740,460,95,45,"Defaults");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,defaultoptionsC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,140,15,175,35,"Astrometry options");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->starminmagW = obj = fl_add_input(FL_FLOAT_INPUT,335,320,65,35,"Minimal magnitude (brightest star)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starminmagC,0);
  fdui->starmaxmagW = obj = fl_add_input(FL_FLOAT_INPUT,335,360,65,35,"Maximal magnitude (faintest star)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starmaxmagC,0);
  fdui->maxresW = obj = fl_add_input(FL_FLOAT_INPUT,335,400,65,35,"Maximal residual (arcseconds)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,maxresC,0);
  fdui->stargridW = obj = fl_add_input(FL_INT_INPUT,755,280,65,35,"Background grid size");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,stargridC,0);
  fdui->staredgeW = obj = fl_add_input(FL_INT_INPUT,755,325,65,35,"Insensitive edge");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,staredgeC,0);
  fdui->starinnerW = obj = fl_add_input(FL_FLOAT_INPUT,755,365,65,35,"Radius of inner circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starinnerC,0);
  fdui->starouterW = obj = fl_add_input(FL_FLOAT_INPUT,755,405,65,35,"Radius of outer circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starouterC,0);
  fdui->starapertureW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,455,380,115,45,"Aperture");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,starapertureC,0);
  fl_end_form();

  fdui->Options->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Catalog *create_form_Catalog(void)
{
  FL_OBJECT *obj;
  FD_Catalog *fdui = (FD_Catalog *) fl_calloc(1, sizeof(*fdui));

  fdui->Catalog = fl_bgn_form(FL_NO_BOX, 625, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,625,330,"");
  obj = fl_add_labelframe(FL_EMBOSSED_FRAME,20,30,590,230,"Star catalogs");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->checkusnoW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,30,55,145,40,"USNO SA 1.0");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,checkusnoC,0);
  fdui->checkppmW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,30,105,145,40,"PPM");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,checkppmC,0);
  fdui->checkguidesouthW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,30,205,145,40,"Guide south");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,checkguideC,1);
  fdui->usnodirW = obj = fl_add_input(FL_NORMAL_INPUT,220,55,370,40,"Path:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,usnodirC,0);
  fdui->ppmdirW = obj = fl_add_input(FL_NORMAL_INPUT,220,105,370,40,"Path:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,ppmdirC,0);
  fdui->guidedirsouthW = obj = fl_add_input(FL_NORMAL_INPUT,220,205,370,40,"Path:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,guidedirsouthC,0);
  fdui->okcatalogsW = obj = fl_add_button(FL_NORMAL_BUTTON,260,275,105,40,"OK");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->checkguideW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,30,155,145,40,"Guide north");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,checkguideC,0);
  fdui->guidedirW = obj = fl_add_input(FL_NORMAL_INPUT,220,155,370,40,"Path:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,guidedirC,0);
  fl_end_form();

  fdui->Catalog->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Mail *create_form_Mail(void)
{
  FL_OBJECT *obj;
  FD_Mail *fdui = (FD_Mail *) fl_calloc(1, sizeof(*fdui));

  fdui->Mail = fl_bgn_form(FL_NO_BOX, 635, 745);
  obj = fl_add_box(FL_UP_BOX,0,0,635,745,"");
  fdui->toaddressW = obj = fl_add_input(FL_NORMAL_INPUT,45,55,575,25,"To:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,toaddressC,0);
  fdui->ccaddressW = obj = fl_add_input(FL_NORMAL_INPUT,45,85,575,25,"CC:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,ccaddressC,0);
  fdui->messageW = obj = fl_add_browser(FL_HOLD_BROWSER,45,265,575,410,"");
    fl_set_object_callback(obj,messageC,0);
  fdui->sendmailW = obj = fl_add_button(FL_NORMAL_BUTTON,50,685,100,40,"Send");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,sendmailC,0);
  fdui->closemailW = obj = fl_add_button(FL_NORMAL_BUTTON,510,685,100,40,"Close");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
  obj = fl_add_text(FL_NORMAL_TEXT,110,20,350,25,"Send astrometry report");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->editorW = obj = fl_add_input(FL_NORMAL_INPUT,45,195,575,25,"Edit:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,editorC,0);
  fdui->editlineW = obj = fl_add_button(FL_NORMAL_BUTTON,45,230,85,30,"Edit");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,editlineC,0);
  fdui->insertlineW = obj = fl_add_button(FL_NORMAL_BUTTON,135,230,85,30,"Insert");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,insertlineC,0);
  fdui->deletelineW = obj = fl_add_button(FL_NORMAL_BUTTON,225,230,85,30,"Delete");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,deletelineC,0);
  fdui->mailtitleW = obj = fl_add_input(FL_NORMAL_INPUT,45,115,575,25,"Subj:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,mailtitleC,0);
  fdui->mailfileW = obj = fl_add_input(FL_NORMAL_INPUT,45,145,480,25,"File:");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,mailfileC,0);
  fdui->findfileW = obj = fl_add_button(FL_NORMAL_BUTTON,530,145,90,25,"Find");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,findfileC,0);
  fdui->savemailW = obj = fl_add_button(FL_NORMAL_BUTTON,155,685,100,40,"Save");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,savemailC,0);
  fdui->headerW = obj = fl_add_button(FL_NORMAL_BUTTON,495,230,125,30,"Insert header");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_gravity(obj, FL_West, FL_West);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
    fl_set_object_callback(obj,headerC,0);
  fl_end_form();

  fdui->Mail->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Wcs *create_form_Wcs(void)
{
  FL_OBJECT *obj;
  FD_Wcs *fdui = (FD_Wcs *) fl_calloc(1, sizeof(*fdui));

  fdui->Wcs = fl_bgn_form(FL_NO_BOX, 375, 475);
  obj = fl_add_box(FL_UP_BOX,0,0,375,475,"");
  obj = fl_add_text(FL_NORMAL_TEXT,30,20,320,45,"Do you want to accept the new WCS values?");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,120,70,70,30,"OLD");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,230,70,70,30,"NEW");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->crval1oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,105,105,30,"CRVAL1");
  fdui->crval1newW = obj = fl_add_input(FL_NORMAL_INPUT,210,105,105,30,"");
  fdui->crval2oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,145,105,30,"CRVAL2");
  fdui->crval2newW = obj = fl_add_input(FL_NORMAL_INPUT,210,145,105,30,"");
  fdui->crpix1oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,185,105,30,"CRPIX1");
  fdui->crpix1newW = obj = fl_add_input(FL_NORMAL_INPUT,210,185,105,30,"");
  fdui->crpix2oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,225,105,30,"CRPIX2");
  fdui->crpix2newW = obj = fl_add_input(FL_NORMAL_INPUT,210,225,105,30,"");
  fdui->cdelt1oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,265,105,30,"CDELT1");
  fdui->cdelt1newW = obj = fl_add_input(FL_NORMAL_INPUT,210,265,105,30,"");
  fdui->cdelt2oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,305,105,30,"CDELT2");
  fdui->cdelt2newW = obj = fl_add_input(FL_NORMAL_INPUT,210,305,105,30,"");
  fdui->crota2oldW = obj = fl_add_input(FL_NORMAL_INPUT,90,345,105,30,"CROTA2");
  fdui->crota2newW = obj = fl_add_input(FL_NORMAL_INPUT,210,345,105,30,"");
  fdui->wcsyesW = obj = fl_add_button(FL_NORMAL_BUTTON,45,405,85,45,"Yes");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->wcsnoW = obj = fl_add_button(FL_NORMAL_BUTTON,235,405,85,45,"No");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->writetofileW = obj = fl_add_button(FL_NORMAL_BUTTON,140,405,85,45,"Write to file");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->Wcs->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Settings *create_form_Settings(void)
{
  FL_OBJECT *obj;
  FD_Settings *fdui = (FD_Settings *) fl_calloc(1, sizeof(*fdui));

  fdui->Settings = fl_bgn_form(FL_NO_BOX, 755, 445);
  obj = fl_add_box(FL_UP_BOX,0,0,755,445,"");
  fdui->savesettingsW = obj = fl_add_button(FL_NORMAL_BUTTON,25,390,105,45,"Save");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->closesettingsW = obj = fl_add_button(FL_NORMAL_BUTTON,630,390,105,45,"Close");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  obj = fl_add_text(FL_NORMAL_TEXT,195,15,320,30,"Fitsblink default settings");
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->setfolderW = obj = fl_add_tabfolder(FL_TOP_TABFOLDER,15,95,730,290,"");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->Settings->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_catalogs *create_form_Set_catalogs(void)
{
  FL_OBJECT *obj;
  FD_Set_catalogs *fdui = (FD_Set_catalogs *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_catalogs = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->usno_pathW = obj = fl_add_input(FL_NORMAL_INPUT,135,55,480,30,"USNO SA");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,usno_path_setC,0);
  fdui->ppm_pathW = obj = fl_add_input(FL_NORMAL_INPUT,135,90,480,30,"PPM");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,ppm_path_setC,0);
  fdui->gsc_north_pathW = obj = fl_add_input(FL_NORMAL_INPUT,135,125,480,30,"GSC_NORTH");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,gsc_north_path_setC,0);
  fdui->gsc_south_pathW = obj = fl_add_input(FL_NORMAL_INPUT,135,160,480,30,"GSC_SOUTH");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,gsc_south_path_setC,0);
  fl_end_form();

  fdui->Set_catalogs->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_observatory *create_form_Set_observatory(void)
{
  FL_OBJECT *obj;
  FD_Set_observatory *fdui = (FD_Set_observatory *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_observatory = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->instrumentW = obj = fl_add_input(FL_NORMAL_INPUT,125,130,480,30,"Instrument");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,instrument_setC,0);
  fdui->telescopeW = obj = fl_add_input(FL_NORMAL_INPUT,125,95,480,30,"Telescope");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,telescope_setC,0);
  fdui->observatoryW = obj = fl_add_input(FL_INT_INPUT,125,25,50,30,"Observatory");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,observatory_setC,0);
  fdui->observerW = obj = fl_add_input(FL_NORMAL_INPUT,125,60,480,30,"Observer");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,observer_setC,0);
  fdui->observatory_longitudeW = obj = fl_add_input(FL_FLOAT_INPUT,125,165,85,30,"Longitude");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,observatory_longitudeC,0);
  fdui->observatory_latitudeW = obj = fl_add_input(FL_FLOAT_INPUT,125,200,85,30,"Latitude");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,observatory_latitudeC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,220,165,185,65,"In decimal degrees\npositive for north and west");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->Set_observatory->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_display *create_form_Set_display(void)
{
  FL_OBJECT *obj;
  FD_Set_display *fdui = (FD_Set_display *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_display = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->color_setW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,95,30,30,30,"Color");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,color_setC,0);
  fdui->inverse_setW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,95,65,30,30,"Inverse");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,inverse_setC,0);
  fdui->set_deltatW = obj = fl_add_counter(FL_SIMPLE_COUNTER,95,115,85,40,"Delay");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_gravity(obj, FL_East, FL_East);
    fl_set_object_resize(obj, FL_RESIZE_NONE);
  fdui->blinkerwidthW = obj = fl_add_input(FL_INT_INPUT,530,30,75,30,"Blinker window width");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,blinkerwidthC,0);
  fdui->blinkerheightW = obj = fl_add_input(FL_INT_INPUT,530,65,75,30,"Blinker window height");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,blinkerheightC,0);
  fdui->set_autolevel_highW = obj = fl_add_input(FL_FLOAT_INPUT,140,180,70,30,"Autolevel parameter");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_autolevel_highC,0);
  fl_end_form();

  fdui->Set_display->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_mail *create_form_Set_mail(void)
{
  FL_OBJECT *obj;
  FD_Set_mail *fdui = (FD_Set_mail *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_mail = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->toW = obj = fl_add_input(FL_NORMAL_INPUT,120,60,480,30,"To");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,to_setC,0);
  fdui->ccW = obj = fl_add_input(FL_NORMAL_INPUT,120,95,480,30,"Cc");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,cc_setC,0);
  fdui->subjectW = obj = fl_add_input(FL_NORMAL_INPUT,120,130,480,30,"Subject");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,subject_setC,0);
  fdui->fileW = obj = fl_add_input(FL_NORMAL_INPUT,120,165,480,30,"File");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,file_setC,0);
  fl_end_form();

  fdui->Set_mail->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Star *create_form_Star(void)
{
  FL_OBJECT *obj;
  FD_Star *fdui = (FD_Star *) fl_calloc(1, sizeof(*fdui));

  fdui->Star = fl_bgn_form(FL_NO_BOX, 390, 515);
  obj = fl_add_box(FL_UP_BOX,0,0,390,515,"");
  fdui->windstarW = obj = fl_add_frame(FL_ENGRAVED_FRAME,10,10,370,320,"");
  fdui->acceptstarW = obj = fl_add_button(FL_NORMAL_BUTTON,10,460,110,40,"Accept");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->rejectstarW = obj = fl_add_button(FL_NORMAL_BUTTON,270,460,110,40,"Reject");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->xstarW = obj = fl_add_text(FL_NORMAL_TEXT,15,345,90,35,"x=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ystarW = obj = fl_add_text(FL_NORMAL_TEXT,165,345,90,35,"y=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,15,385,90,35,"x=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->rastarW = obj = fl_add_text(FL_NORMAL_TEXT,15,380,145,35,"RA=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->decstarW = obj = fl_add_text(FL_NORMAL_TEXT,165,380,140,35,"DEC=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->okstarW = obj = fl_add_button(FL_NORMAL_BUTTON,140,460,110,40,"OK");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->objstarW = obj = fl_add_input(FL_NORMAL_INPUT,60,415,320,35,"Object");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,objstarC,0);
  fdui->vstarW = obj = fl_add_text(FL_NORMAL_TEXT,285,345,90,35,"v=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->Star->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Telescope *create_form_Telescope(void)
{
  FL_OBJECT *obj;
  FD_Telescope *fdui = (FD_Telescope *) fl_calloc(1, sizeof(*fdui));

  fdui->Telescope = fl_bgn_form(FL_NO_BOX, 765, 590);
  obj = fl_add_box(FL_UP_BOX,0,0,765,590,"");
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,460,245,295,290,"Camera");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,460,70,295,160,"Time");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_labelframe(FL_ENGRAVED_FRAME,15,70,410,465,"Telescope");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->teldecW = obj = fl_add_input(FL_NORMAL_INPUT,260,90,130,40,"DEC:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,teldecC,0);
  fdui->telraW = obj = fl_add_input(FL_NORMAL_INPUT,65,90,130,40,"RA:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,telraC,0);
  fdui->telaziW = obj = fl_add_input(FL_NORMAL_INPUT,260,135,130,40,"AZ:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,telaziC,0);
  fdui->telaltW = obj = fl_add_input(FL_NORMAL_INPUT,65,135,130,40,"ALT:");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,telaltC,0);
  fdui->telbrowsecatW = obj = fl_add_browser(FL_SELECT_BROWSER,65,255,325,260,"");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_callback(obj,telbrowsecatC,0);
  fdui->telutW = obj = fl_add_clock(FL_DIGITAL_CLOCK,580,95,140,30,"UTC");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BOTTOM_BCOL);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
  fdui->tellocalW = obj = fl_add_clock(FL_DIGITAL_CLOCK,580,125,140,30,"Local");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_COL1,FL_BOTTOM_BCOL);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
  fdui->teltempW = obj = fl_add_input(FL_NORMAL_INPUT,515,260,55,30,"Temp");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,teltempC,0);
  fdui->telreadtempW = obj = fl_add_button(FL_NORMAL_BUTTON,610,260,50,30,"Read");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,telreadtempC,0);
  fdui->telsettempW = obj = fl_add_button(FL_NORMAL_BUTTON,660,260,50,30,"Set");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telsettempC,0);
  fdui->telexposureW = obj = fl_add_input(FL_NORMAL_INPUT,540,300,75,30,"Exposure");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,telexposureC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,615,300,60,30,"seconds");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->telfileW = obj = fl_add_input(FL_NORMAL_INPUT,505,340,145,30,"File");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,telfileC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,575,260,20,30,"C");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->teldarkW = obj = fl_add_lightbutton(FL_PUSH_BUTTON,480,380,65,35,"Dark");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,teldarkC,0);
  fdui->telstartexposureW = obj = fl_add_button(FL_NORMAL_BUTTON,550,380,75,35,"Expose");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telstartexposureC,0);
  fdui->telcloseW = obj = fl_add_button(FL_NORMAL_BUTTON,10,540,85,40,"Close");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->telstartW = obj = fl_add_button(FL_NORMAL_BUTTON,65,180,60,35,"Start");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telstartC,0);
  fdui->telparkW = obj = fl_add_button(FL_NORMAL_BUTTON,135,180,60,35,"Park");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telparkC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,235,10,335,50,"Telescope and camera control");
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->telreadposW = obj = fl_add_button(FL_NORMAL_BUTTON,260,180,130,35,"Read position");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,telreadposC,0);
  fdui->telgoW = obj = fl_add_button(FL_NORMAL_BUTTON,260,215,130,35,"Go!");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_callback(obj,telgoC,0);
  fdui->telchoosecatW = obj = fl_add_button(FL_NORMAL_BUTTON,65,215,130,35,"Choose catalog");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telchoosecatC,0);
  fdui->teljulianW = obj = fl_add_input(FL_NORMAL_INPUT,580,155,140,30,"Julian");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
  fdui->telsiderealW = obj = fl_add_input(FL_NORMAL_INPUT,580,185,140,30,"Sidereal");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
  fdui->telzeroW = obj = fl_add_button(FL_NORMAL_BUTTON,195,180,65,35,"Zero");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telzeroC,0);
  fdui->telcenterW = obj = fl_add_button(FL_NORMAL_BUTTON,630,380,65,35,"Center");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,telcenterC,0);
  fl_end_form();

  fdui->Telescope->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Commands *create_form_Commands(void)
{
  FL_OBJECT *obj;
  FD_Commands *fdui = (FD_Commands *) fl_calloc(1, sizeof(*fdui));

  fdui->Commands = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->comstartW = obj = fl_add_input(FL_NORMAL_INPUT,80,15,565,25,"Start");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comstartC,0);
  fdui->compointW = obj = fl_add_input(FL_NORMAL_INPUT,80,45,565,25,"Point");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,compointC,0);
  fdui->comparkW = obj = fl_add_input(FL_NORMAL_INPUT,80,75,565,25,"Park");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comparkC,0);
  fdui->comwhereW = obj = fl_add_input(FL_NORMAL_INPUT,80,105,565,25,"Where");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comwhereC,0);
  fdui->comzeroW = obj = fl_add_input(FL_NORMAL_INPUT,80,135,565,25,"Zero");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comzeroC,0);
  fdui->comcenterW = obj = fl_add_input(FL_NORMAL_INPUT,80,165,565,25,"Center");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comcenterC,0);
  fl_end_form();

  fdui->Commands->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Message *create_form_Message(void)
{
  FL_OBJECT *obj;
  FD_Message *fdui = (FD_Message *) fl_calloc(1, sizeof(*fdui));

  fdui->Message = fl_bgn_form(FL_NO_BOX, 375, 140);
  obj = fl_add_box(FL_UP_BOX,0,0,375,140,"");
  fdui->messagetextW = obj = fl_add_text(FL_NORMAL_TEXT,10,20,355,60,"");
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->messagecancelW = obj = fl_add_button(FL_NORMAL_BUTTON,135,90,115,40,"Cancel");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
  fl_end_form();

  fdui->Message->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_help *create_form_Set_help(void)
{
  FL_OBJECT *obj;
  FD_Set_help *fdui = (FD_Set_help *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_help = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->setbrowserW = obj = fl_add_input(FL_NORMAL_INPUT,145,45,495,35,"WWW browser path");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,setbrowserC,0);
  fdui->sethelpfileW = obj = fl_add_input(FL_NORMAL_INPUT,145,85,495,35,"Help file path");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,sethelpfileC,0);
  fl_end_form();

  fdui->Set_help->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_ccd *create_form_Set_ccd(void)
{
  FL_OBJECT *obj;
  FD_Set_ccd *fdui = (FD_Set_ccd *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_ccd = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->comgettempW = obj = fl_add_input(FL_NORMAL_INPUT,80,30,565,25,"Get temp.");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comgettempC,0);
  fdui->comsettempW = obj = fl_add_input(FL_NORMAL_INPUT,80,60,565,25,"Set temp");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comsettempC,0);
  fdui->comexposeW = obj = fl_add_input(FL_NORMAL_INPUT,80,90,565,25,"Expose");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,comexposeC,0);
  fl_end_form();

  fdui->Set_ccd->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Comet *create_form_Comet(void)
{
  FL_OBJECT *obj;
  FD_Comet *fdui = (FD_Comet *) fl_calloc(1, sizeof(*fdui));

  fdui->Comet = fl_bgn_form(FL_NO_BOX, 392, 575);
  obj = fl_add_box(FL_UP_BOX,0,0,392,575,"");
  fdui->windcometW = obj = fl_add_frame(FL_ENGRAVED_FRAME,10,10,370,320,"");
  fdui->xcometW = obj = fl_add_text(FL_NORMAL_TEXT,15,345,90,35,"x=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ycometW = obj = fl_add_text(FL_NORMAL_TEXT,165,345,90,35,"y=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->racometW = obj = fl_add_text(FL_NORMAL_TEXT,15,380,145,35,"RA=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->deccometW = obj = fl_add_text(FL_NORMAL_TEXT,165,380,140,35,"DEC=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->objcometW = obj = fl_add_input(FL_NORMAL_INPUT,60,415,320,35,"Object");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,objstarC,0);
  fdui->acceptcometW = obj = fl_add_button(FL_NORMAL_BUTTON,15,525,110,40,"Accept");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->okcometW = obj = fl_add_button(FL_NORMAL_BUTTON,145,525,110,40,"OK");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->rejectcometW = obj = fl_add_button(FL_NORMAL_BUTTON,270,525,110,40,"Reject");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->innercircleW = obj = fl_add_counter(FL_NORMAL_COUNTER,40,475,135,35,"Inner circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,innercircleC,0);
  fdui->outercircleW = obj = fl_add_counter(FL_NORMAL_COUNTER,220,475,135,35,"Outer circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,outercircleC,0);
  fdui->vcometW = obj = fl_add_text(FL_NORMAL_TEXT,285,345,90,35,"v=");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->Comet->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_count *create_form_Set_count(void)
{
  FL_OBJECT *obj;
  FD_Set_count *fdui = (FD_Set_count *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_count = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->set_sg_numW = obj = fl_add_input(FL_FLOAT_INPUT,210,45,70,30,"Threshold");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_sg_numC,0);
  fdui->set_minbrightW = obj = fl_add_input(FL_FLOAT_INPUT,210,80,70,30,"Minimal brightness");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_minbrightC,0);
  fdui->set_minstarW = obj = fl_add_input(FL_INT_INPUT,210,115,70,30,"Minimal star size");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_minstarC,0);
  fdui->set_gridsizeW = obj = fl_add_input(FL_INT_INPUT,210,150,70,30,"Backgrund grid size");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_gridsizeC,0);
  fdui->set_borderW = obj = fl_add_input(FL_INT_INPUT,210,185,70,30,"Insensitive edge size");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_borderC,0);
  fdui->set_innerW = obj = fl_add_input(FL_FLOAT_INPUT,485,80,70,30,"Size of the inner circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_innerC,0);
  fdui->set_outerW = obj = fl_add_input(FL_FLOAT_INPUT,485,115,70,30,"Size of the outer circle");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_outerC,0);
  fdui->set_apertureW = obj = fl_add_checkbutton(FL_PUSH_BUTTON,485,50,70,25,"Aperture photometry");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT);
    fl_set_object_callback(obj,set_apertureC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,190,5,390,25,"Default settings for star detection routines");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->Set_count->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

FD_Set_match *create_form_Set_match(void)
{
  FL_OBJECT *obj;
  FD_Set_match *fdui = (FD_Set_match *) fl_calloc(1, sizeof(*fdui));

  fdui->Set_match = fl_bgn_form(FL_NO_BOX, 665, 250);
  obj = fl_add_box(FL_FLAT_BOX,0,0,665,250,"");
  fdui->set_starerrorW = obj = fl_add_input(FL_FLOAT_INPUT,205,150,55,25,"Max. pos. error (pixels)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_starerrorC,0);
  obj = fl_add_text(FL_NORMAL_TEXT,130,10,420,30,"Default settings for catalog matching routines");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->set_minconstW = obj = fl_add_input(FL_FLOAT_INPUT,205,60,55,25,"Min. value to accept const.");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_minconstC,0);
  fdui->set_initconstW = obj = fl_add_input(FL_INT_INPUT,205,90,55,25,"No. of stars in initial matching.");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_initconstC,0);
  fdui->set_starconstW = obj = fl_add_input(FL_INT_INPUT,205,120,55,25,"No. of stars in constellation");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_starconstC,0);
  fdui->set_maxresW = obj = fl_add_input(FL_FLOAT_INPUT,205,180,55,25,"Max. residual (arcseconds)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_maxresC,0);
  fdui->set_minmagW = obj = fl_add_input(FL_FLOAT_INPUT,510,60,55,25,"Minimal magnitude");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_minmagC,0);
  fdui->set_maxmagW = obj = fl_add_input(FL_FLOAT_INPUT,510,90,55,25,"Maximal magnitude");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_callback(obj,set_maxmagC,0);
  fl_end_form();

  fdui->Set_match->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

