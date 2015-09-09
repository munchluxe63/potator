#include "shared.h"

extern void Buttons(void);

void clear_screen_menu(void);
void draw_bluerect_menu(unsigned char i);
void draw_bluerect_file(unsigned char i);

void menuReset(void);
void menuQuit(void);
void menuContinue(void);
void menuFileBrowse(void);
void menuSaveState(void);
void menuLoadState(void);
void screen_showkeymenu(void);
unsigned char ifactive(void);
void flip_menu(void);

//---------------------------------------------------------------------------------------
typedef struct {
	const char itemName[16];
	int *itemPar;
	int itemParMaxValue;
	const char *itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int itemNum; // number of items
	int itemCur; // current item
	MENUITEM *m; // array of items
} MENU;


const char mnuYesNo[2][16] = {"no", "yes"};
const char mnuRatio[3][16] = { "1x size","Full screen", "Keep Aspect"};
const char mnuColor[4][16] = { "White","Amber","Green","Blue"};


MENUITEM MainMenuItems[] = {
	{"Load rom", NULL, 0, NULL, &menuFileBrowse},
	{"Continue", NULL, 0, NULL, &menuContinue},
	{"Reset", NULL, 0, NULL, &menuReset},
	{"Ratio: ", (int *) &GameConf.m_ScreenRatio, 2, (const char *) &mnuRatio, NULL},
	{"Color scheme: ",(int *) &GameConf.m_Color, 3, (const char *) &mnuColor, NULL},
	{"Show FPS: ", (int *) &GameConf.m_DisplayFPS, 1,(const char *) &mnuYesNo, NULL},
	/*{"Take Screenshot", NULL, 0, NULL, &menuSaveBmp},*/
	{"Exit", NULL, 0, NULL, &menuQuit}
};


MENU mnuMainMenu = { 7, 0, (MENUITEM *) &MainMenuItems };


const char *file_ext[] = { (const char *) ".sv", (const char *) ".bin",  (const char *) ".tns", NULL };


