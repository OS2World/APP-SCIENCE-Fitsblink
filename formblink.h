#ifndef FD_setting_h_
#define FD_setting_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void eraseC(FL_OBJECT *, long);
extern void greyC(FL_OBJECT *, long);
extern void gammaC(FL_OBJECT *, long);
extern void turnimageC(FL_OBJECT *, long);
extern void deltaxC(FL_OBJECT *, long);
extern void deltayC(FL_OBJECT *, long);
extern void loadC(FL_OBJECT *, long);
extern void quitC(FL_OBJECT *, long);
extern void invertC(FL_OBJECT *, long);
extern void autoC(FL_OBJECT *, long);
extern void nameC(FL_OBJECT *, long);
extern void fitsC(FL_OBJECT *, long);
extern void helpC(FL_OBJECT *, long);
extern void histadjC(FL_OBJECT *, long);

extern void emptyC(FL_OBJECT *, long);
extern void zoom1C(FL_OBJECT *, long);
extern void zoom2C(FL_OBJECT *, long);
extern void astrometryC(FL_OBJECT *, long);
extern void leftC(FL_OBJECT *, long);
extern void upC(FL_OBJECT *, long);
extern void rightC(FL_OBJECT *, long);
extern void downC(FL_OBJECT *, long);
extern void circleC(FL_OBJECT *, long);
extern int freeobj_blinkW_handle(FL_OBJECT *, int, FL_Coord, FL_Coord,
			int, void *);
extern void deltatC(FL_OBJECT *, long);
extern void telescope_controlC(FL_OBJECT *, long);
extern void markersC(FL_OBJECT *, long);
extern void colormapC(FL_OBJECT *, long);




extern void emptyC(FL_OBJECT *, long);

extern void rainputC(FL_OBJECT *, long);
extern void decinputC(FL_OBJECT *, long);
extern void xpixelC(FL_OBJECT *, long);
extern void ypixelC(FL_OBJECT *, long);
extern void rotationC(FL_OBJECT *, long);
extern void timeyearC(FL_OBJECT *, long);
extern void timemonthC(FL_OBJECT *, long);
extern void timehourC(FL_OBJECT *, long);
extern void timeminuteC(FL_OBJECT *, long);
extern void timesecondC(FL_OBJECT *, long);
extern void timedecdayC(FL_OBJECT *, long);
extern void timedayC(FL_OBJECT *, long);
extern void objectC(FL_OBJECT *, long);
extern void observerC(FL_OBJECT *, long);
extern void telescopeC(FL_OBJECT *, long);
extern void instrumentC(FL_OBJECT *, long);

extern void reportC(FL_OBJECT *, long);
extern void savereportC(FL_OBJECT *, long);

extern void logfileC(FL_OBJECT *, long);
extern void minconstC(FL_OBJECT *, long);
extern void initconstC(FL_OBJECT *, long);
extern void starconstC(FL_OBJECT *, long);
extern void countsigmaC(FL_OBJECT *, long);
extern void countminvalC(FL_OBJECT *, long);
extern void starerrorC(FL_OBJECT *, long);
extern void starminsizelC(FL_OBJECT *, long);
extern void defaultoptionsC(FL_OBJECT *, long);
extern void starminmagC(FL_OBJECT *, long);
extern void starmaxmagC(FL_OBJECT *, long);
extern void maxresC(FL_OBJECT *, long);
extern void stargridC(FL_OBJECT *, long);
extern void staredgeC(FL_OBJECT *, long);
extern void starinnerC(FL_OBJECT *, long);
extern void starouterC(FL_OBJECT *, long);
extern void starapertureC(FL_OBJECT *, long);

extern void checkusnoC(FL_OBJECT *, long);
extern void checkppmC(FL_OBJECT *, long);
extern void checkguideC(FL_OBJECT *, long);
extern void usnodirC(FL_OBJECT *, long);
extern void ppmdirC(FL_OBJECT *, long);
extern void guidedirsouthC(FL_OBJECT *, long);
extern void guidedirC(FL_OBJECT *, long);

extern void toaddressC(FL_OBJECT *, long);
extern void ccaddressC(FL_OBJECT *, long);
extern void messageC(FL_OBJECT *, long);
extern void sendmailC(FL_OBJECT *, long);
extern void editorC(FL_OBJECT *, long);
extern void editlineC(FL_OBJECT *, long);
extern void insertlineC(FL_OBJECT *, long);
extern void deletelineC(FL_OBJECT *, long);
extern void mailtitleC(FL_OBJECT *, long);
extern void mailfileC(FL_OBJECT *, long);
extern void findfileC(FL_OBJECT *, long);
extern void savemailC(FL_OBJECT *, long);
extern void headerC(FL_OBJECT *, long);



extern void usno_path_setC(FL_OBJECT *, long);
extern void ppm_path_setC(FL_OBJECT *, long);
extern void gsc_north_path_setC(FL_OBJECT *, long);
extern void gsc_south_path_setC(FL_OBJECT *, long);

extern void instrument_setC(FL_OBJECT *, long);
extern void telescope_setC(FL_OBJECT *, long);
extern void observatory_setC(FL_OBJECT *, long);
extern void observer_setC(FL_OBJECT *, long);
extern void observatory_longitudeC(FL_OBJECT *, long);
extern void observatory_latitudeC(FL_OBJECT *, long);

extern void color_setC(FL_OBJECT *, long);
extern void inverse_setC(FL_OBJECT *, long);
extern void blinkerwidthC(FL_OBJECT *, long);
extern void blinkerheightC(FL_OBJECT *, long);
extern void set_autolevel_highC(FL_OBJECT *, long);

extern void to_setC(FL_OBJECT *, long);
extern void cc_setC(FL_OBJECT *, long);
extern void subject_setC(FL_OBJECT *, long);
extern void file_setC(FL_OBJECT *, long);

extern void objstarC(FL_OBJECT *, long);

extern void teldecC(FL_OBJECT *, long);
extern void telraC(FL_OBJECT *, long);
extern void telaziC(FL_OBJECT *, long);
extern void telaltC(FL_OBJECT *, long);
extern void telbrowsecatC(FL_OBJECT *, long);
extern void teltempC(FL_OBJECT *, long);
extern void telreadtempC(FL_OBJECT *, long);
extern void telsettempC(FL_OBJECT *, long);
extern void telexposureC(FL_OBJECT *, long);
extern void telfileC(FL_OBJECT *, long);
extern void teldarkC(FL_OBJECT *, long);
extern void telstartexposureC(FL_OBJECT *, long);
extern void telstartC(FL_OBJECT *, long);
extern void telparkC(FL_OBJECT *, long);
extern void telreadposC(FL_OBJECT *, long);
extern void telgoC(FL_OBJECT *, long);
extern void telchoosecatC(FL_OBJECT *, long);
extern void telzeroC(FL_OBJECT *, long);
extern void telcenterC(FL_OBJECT *, long);

extern void comstartC(FL_OBJECT *, long);
extern void compointC(FL_OBJECT *, long);
extern void comparkC(FL_OBJECT *, long);
extern void comwhereC(FL_OBJECT *, long);
extern void comzeroC(FL_OBJECT *, long);
extern void comcenterC(FL_OBJECT *, long);


extern void setbrowserC(FL_OBJECT *, long);
extern void sethelpfileC(FL_OBJECT *, long);

extern void comgettempC(FL_OBJECT *, long);
extern void comsettempC(FL_OBJECT *, long);
extern void comexposeC(FL_OBJECT *, long);

extern void objstarC(FL_OBJECT *, long);
extern void innercircleC(FL_OBJECT *, long);
extern void outercircleC(FL_OBJECT *, long);

extern void set_sg_numC(FL_OBJECT *, long);
extern void set_minbrightC(FL_OBJECT *, long);
extern void set_minstarC(FL_OBJECT *, long);
extern void set_gridsizeC(FL_OBJECT *, long);
extern void set_borderC(FL_OBJECT *, long);
extern void set_innerC(FL_OBJECT *, long);
extern void set_outerC(FL_OBJECT *, long);
extern void set_apertureC(FL_OBJECT *, long);

extern void set_starerrorC(FL_OBJECT *, long);
extern void set_minconstC(FL_OBJECT *, long);
extern void set_initconstC(FL_OBJECT *, long);
extern void set_starconstC(FL_OBJECT *, long);
extern void set_maxresC(FL_OBJECT *, long);
extern void set_minmagC(FL_OBJECT *, long);
extern void set_maxmagC(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *setting;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *secondleraseW;
	FL_OBJECT *secondgreyW;
	FL_OBJECT *firstgreyW;
	FL_OBJECT *firstgammaW;
	FL_OBJECT *secondgammaW;
	FL_OBJECT *firstsecond;
	FL_OBJECT *firstimageW;
	FL_OBJECT *secondimageW;
	FL_OBJECT *firstdeltaxW;
	FL_OBJECT *firstdeltayW;
	FL_OBJECT *seconddeltaxW;
	FL_OBJECT *seconddeltayW;
	FL_OBJECT *firstloadW;
	FL_OBJECT *firstleraseW;
	FL_OBJECT *secondloadW;
	FL_OBJECT *quitW;
	FL_OBJECT *firstinvertW;
	FL_OBJECT *firstautoW;
	FL_OBJECT *firstnameW;
	FL_OBJECT *secondnameW;
	FL_OBJECT *firstfitsW;
	FL_OBJECT *secondfitsW;
	FL_OBJECT *secondinvertW;
	FL_OBJECT *secondautoW;
	FL_OBJECT *helpW;
	FL_OBJECT *leftlowdecW;
} FD_setting;

extern FD_setting * create_form_setting(void);
typedef struct {
	FL_FORM *blinker;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ycoorW;
	FL_OBJECT *zoom1W;
	FL_OBJECT *zoom2W;
	FL_OBJECT *lockedW;
	FL_OBJECT *valueW;
	FL_OBJECT *racoorW;
	FL_OBJECT *deccoorW;
	FL_OBJECT *xcoorW;
	FL_OBJECT *astrometryW;
	FL_OBJECT *bgroupW;
	FL_OBJECT *playW;
	FL_OBJECT *leftW;
	FL_OBJECT *upW;
	FL_OBJECT *rightW;
	FL_OBJECT *downW;
	FL_OBJECT *circleW;
	FL_OBJECT *blinkW;
	FL_OBJECT *deltatW;
	FL_OBJECT *telescope_controlW;
	FL_OBJECT *markersW;
	FL_OBJECT *colorsW;
	FL_OBJECT *greybutW;
	FL_OBJECT *colorbutW;
} FD_blinker;

extern FD_blinker * create_form_blinker(void);
typedef struct {
	FL_FORM *Edit;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *helpW;
	FL_OBJECT *browseW;
	FL_OBJECT *closeeditW;
} FD_Edit;

extern FD_Edit * create_form_Edit(void);
typedef struct {
	FL_FORM *info;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *infookW;
	FL_OBJECT *versionW;
} FD_info;

extern FD_info * create_form_info(void);
typedef struct {
	FL_FORM *Error;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *closeW;
	FL_OBJECT *errorW;
} FD_Error;

extern FD_Error * create_form_Error(void);
typedef struct {
	FL_FORM *help;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *helpW;
	FL_OBJECT *readmeW;
	FL_OBJECT *closehelpW;
} FD_help;

extern FD_help * create_form_help(void);
typedef struct {
	FL_FORM *Parameters;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *rainputW;
	FL_OBJECT *decinputW;
	FL_OBJECT *xpixelW;
	FL_OBJECT *ypixelW;
	FL_OBJECT *rotationW;
	FL_OBJECT *timeyearW;
	FL_OBJECT *timemonthW;
	FL_OBJECT *timehourW;
	FL_OBJECT *timeminuteW;
	FL_OBJECT *timesecondW;
	FL_OBJECT *timedecdayW;
	FL_OBJECT *parokW;
	FL_OBJECT *timedayW;
	FL_OBJECT *objectW;
	FL_OBJECT *observerW;
	FL_OBJECT *telescopeW;
	FL_OBJECT *instrumentW;
} FD_Parameters;

extern FD_Parameters * create_form_Parameters(void);
typedef struct {
	FL_FORM *Report;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *reportW;
	FL_OBJECT *closereportW;
	FL_OBJECT *savereportW;
} FD_Report;

extern FD_Report * create_form_Report(void);
typedef struct {
	FL_FORM *Options;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *logfileW;
	FL_OBJECT *minconstW;
	FL_OBJECT *initconstW;
	FL_OBJECT *starconstW;
	FL_OBJECT *countsigmaW;
	FL_OBJECT *countminvalW;
	FL_OBJECT *starerrorW;
	FL_OBJECT *starminsizeW;
	FL_OBJECT *acceptoptionsW;
	FL_OBJECT *defaultoptionsW;
	FL_OBJECT *starminmagW;
	FL_OBJECT *starmaxmagW;
	FL_OBJECT *maxresW;
	FL_OBJECT *stargridW;
	FL_OBJECT *staredgeW;
	FL_OBJECT *starinnerW;
	FL_OBJECT *starouterW;
	FL_OBJECT *starapertureW;
} FD_Options;

extern FD_Options * create_form_Options(void);
typedef struct {
	FL_FORM *Catalog;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *checkusnoW;
	FL_OBJECT *checkppmW;
	FL_OBJECT *checkguidesouthW;
	FL_OBJECT *usnodirW;
	FL_OBJECT *ppmdirW;
	FL_OBJECT *guidedirsouthW;
	FL_OBJECT *okcatalogsW;
	FL_OBJECT *checkguideW;
	FL_OBJECT *guidedirW;
} FD_Catalog;

extern FD_Catalog * create_form_Catalog(void);
typedef struct {
	FL_FORM *Mail;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *toaddressW;
	FL_OBJECT *ccaddressW;
	FL_OBJECT *messageW;
	FL_OBJECT *sendmailW;
	FL_OBJECT *closemailW;
	FL_OBJECT *editorW;
	FL_OBJECT *editlineW;
	FL_OBJECT *insertlineW;
	FL_OBJECT *deletelineW;
	FL_OBJECT *mailtitleW;
	FL_OBJECT *mailfileW;
	FL_OBJECT *findfileW;
	FL_OBJECT *savemailW;
	FL_OBJECT *headerW;
} FD_Mail;

extern FD_Mail * create_form_Mail(void);
typedef struct {
	FL_FORM *Wcs;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *crval1oldW;
	FL_OBJECT *crval1newW;
	FL_OBJECT *crval2oldW;
	FL_OBJECT *crval2newW;
	FL_OBJECT *crpix1oldW;
	FL_OBJECT *crpix1newW;
	FL_OBJECT *crpix2oldW;
	FL_OBJECT *crpix2newW;
	FL_OBJECT *cdelt1oldW;
	FL_OBJECT *cdelt1newW;
	FL_OBJECT *cdelt2oldW;
	FL_OBJECT *cdelt2newW;
	FL_OBJECT *crota2oldW;
	FL_OBJECT *crota2newW;
	FL_OBJECT *wcsyesW;
	FL_OBJECT *wcsnoW;
	FL_OBJECT *writetofileW;
} FD_Wcs;

extern FD_Wcs * create_form_Wcs(void);
typedef struct {
	FL_FORM *Settings;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *savesettingsW;
	FL_OBJECT *closesettingsW;
	FL_OBJECT *setfolderW;
} FD_Settings;

extern FD_Settings * create_form_Settings(void);
typedef struct {
	FL_FORM *Set_catalogs;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *usno_pathW;
	FL_OBJECT *ppm_pathW;
	FL_OBJECT *gsc_north_pathW;
	FL_OBJECT *gsc_south_pathW;
} FD_Set_catalogs;

extern FD_Set_catalogs * create_form_Set_catalogs(void);
typedef struct {
	FL_FORM *Set_observatory;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *instrumentW;
	FL_OBJECT *telescopeW;
	FL_OBJECT *observatoryW;
	FL_OBJECT *observerW;
	FL_OBJECT *observatory_longitudeW;
	FL_OBJECT *observatory_latitudeW;
} FD_Set_observatory;

extern FD_Set_observatory * create_form_Set_observatory(void);
typedef struct {
	FL_FORM *Set_display;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *color_setW;
	FL_OBJECT *inverse_setW;
	FL_OBJECT *set_deltatW;
	FL_OBJECT *blinkerwidthW;
	FL_OBJECT *blinkerheightW;
	FL_OBJECT *set_autolevel_highW;
} FD_Set_display;

extern FD_Set_display * create_form_Set_display(void);
typedef struct {
	FL_FORM *Set_mail;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *toW;
	FL_OBJECT *ccW;
	FL_OBJECT *subjectW;
	FL_OBJECT *fileW;
} FD_Set_mail;

extern FD_Set_mail * create_form_Set_mail(void);
typedef struct {
	FL_FORM *Star;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *windstarW;
	FL_OBJECT *acceptstarW;
	FL_OBJECT *rejectstarW;
	FL_OBJECT *xstarW;
	FL_OBJECT *ystarW;
	FL_OBJECT *rastarW;
	FL_OBJECT *decstarW;
	FL_OBJECT *okstarW;
	FL_OBJECT *objstarW;
	FL_OBJECT *vstarW;
} FD_Star;

extern FD_Star * create_form_Star(void);
typedef struct {
	FL_FORM *Telescope;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *teldecW;
	FL_OBJECT *telraW;
	FL_OBJECT *telaziW;
	FL_OBJECT *telaltW;
	FL_OBJECT *telbrowsecatW;
	FL_OBJECT *telutW;
	FL_OBJECT *tellocalW;
	FL_OBJECT *teltempW;
	FL_OBJECT *telreadtempW;
	FL_OBJECT *telsettempW;
	FL_OBJECT *telexposureW;
	FL_OBJECT *telfileW;
	FL_OBJECT *teldarkW;
	FL_OBJECT *telstartexposureW;
	FL_OBJECT *telcloseW;
	FL_OBJECT *telstartW;
	FL_OBJECT *telparkW;
	FL_OBJECT *telreadposW;
	FL_OBJECT *telgoW;
	FL_OBJECT *telchoosecatW;
	FL_OBJECT *teljulianW;
	FL_OBJECT *telsiderealW;
	FL_OBJECT *telzeroW;
	FL_OBJECT *telcenterW;
} FD_Telescope;

extern FD_Telescope * create_form_Telescope(void);
typedef struct {
	FL_FORM *Commands;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *comstartW;
	FL_OBJECT *compointW;
	FL_OBJECT *comparkW;
	FL_OBJECT *comwhereW;
	FL_OBJECT *comzeroW;
	FL_OBJECT *comcenterW;
} FD_Commands;

extern FD_Commands * create_form_Commands(void);
typedef struct {
	FL_FORM *Message;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *messagetextW;
	FL_OBJECT *messagecancelW;
} FD_Message;

extern FD_Message * create_form_Message(void);
typedef struct {
	FL_FORM *Set_help;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *setbrowserW;
	FL_OBJECT *sethelpfileW;
} FD_Set_help;

extern FD_Set_help * create_form_Set_help(void);
typedef struct {
	FL_FORM *Set_ccd;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *comgettempW;
	FL_OBJECT *comsettempW;
	FL_OBJECT *comexposeW;
} FD_Set_ccd;

extern FD_Set_ccd * create_form_Set_ccd(void);
typedef struct {
	FL_FORM *Comet;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *windcometW;
	FL_OBJECT *xcometW;
	FL_OBJECT *ycometW;
	FL_OBJECT *racometW;
	FL_OBJECT *deccometW;
	FL_OBJECT *objcometW;
	FL_OBJECT *acceptcometW;
	FL_OBJECT *okcometW;
	FL_OBJECT *rejectcometW;
	FL_OBJECT *innercircleW;
	FL_OBJECT *outercircleW;
	FL_OBJECT *vcometW;
} FD_Comet;

extern FD_Comet * create_form_Comet(void);
typedef struct {
	FL_FORM *Set_count;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *set_sg_numW;
	FL_OBJECT *set_minbrightW;
	FL_OBJECT *set_minstarW;
	FL_OBJECT *set_gridsizeW;
	FL_OBJECT *set_borderW;
	FL_OBJECT *set_innerW;
	FL_OBJECT *set_outerW;
	FL_OBJECT *set_apertureW;
} FD_Set_count;

extern FD_Set_count * create_form_Set_count(void);
typedef struct {
	FL_FORM *Set_match;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *set_starerrorW;
	FL_OBJECT *set_minconstW;
	FL_OBJECT *set_initconstW;
	FL_OBJECT *set_starconstW;
	FL_OBJECT *set_maxresW;
	FL_OBJECT *set_minmagW;
	FL_OBJECT *set_maxmagW;
} FD_Set_match;

extern FD_Set_match * create_form_Set_match(void);

#endif /* FD_setting_h_ */
