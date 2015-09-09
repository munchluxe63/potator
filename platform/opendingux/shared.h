#ifndef SHARED_H
#define SHARED_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#ifndef _TINSPIRE
#include <SDL/SDL.h>
#endif

// defines and macros
#define MAX__PATH 256
#define FILE_LIST_ROWS 19

#define SYSVID_WIDTH	160
#define SYSVID_HEIGHT	160

#define GF_GAMEINIT    1
#define GF_MAINUI      2
#define GF_GAMEQUIT    3
#define GF_GAMERUNNING 4

#ifndef O_BINARY
#define O_BINARY 0
#endif


#ifdef GCW
#ifndef HOME_SUPPORT
#define HOME_SUPPORT
#endif
#endif

#define true 1
#define false 0

#define PIX_TO_RGB(fmt, r, g, b) (((r>>fmt->Rloss)<<fmt->Rshift)| ((g>>fmt->Gloss)<<fmt->Gshift)|((b>>fmt->Bloss)<<fmt->Bshift))

// potator dependencies
#include "../../common/sound.h"
#include "../../common/memorymap.h"
#include "../../common/supervision.h"
#include "../../common/controls.h"
#include "../../common/types.h"

extern unsigned char controls_state;

#ifdef _TINSPIRE
#define PAD_UP		KEY_NSPIRE_UP
#define PAD_LEFT	KEY_NSPIRE_LEFT
#define PAD_RIGHT	KEY_NSPIRE_RIGHT
#define PAD_DOWN	KEY_NSPIRE_DOWN
#define PAD_XUP		KEY_NSPIRE_UP
#define PAD_XLEFT	KEY_NSPIRE_LEFT
#define PAD_XRIGHT	KEY_NSPIRE_RIGHT
#define PAD_XDOWN	KEY_NSPIRE_DOWN
#define PAD_YUP		KEY_NSPIRE_8
#define PAD_YLEFT	KEY_NSPIRE_4
#define PAD_YRIGHT	KEY_NSPIRE_6
#define PAD_YDOWN	KEY_NSPIRE_5
#define PAD_A		KEY_NSPIRE_CTRL
#define PAD_B		KEY_NSPIRE_SHIFT
#define PAD_X		KEY_NSPIRE_VAR
#define PAD_Y		KEY_NSPIRE_DEL
#define PAD_L		KEY_NSPIRE_L
#define PAD_R		KEY_NSPIRE_R
#define PAD_START	KEY_NSPIRE_TAB
#define PAD_SELECT	KEY_NSPIRE_MENU
#define PAD_SLIDER	KEY_NSPIRE_ENTER
#define PAD_QUIT	KEY_NSPIRE_ESC

#elif defined(GCW)

#define PAD_XUP		SDLK_UP
#define PAD_XLEFT	SDLK_LEFT
#define PAD_XRIGHT	SDLK_RIGHT
#define PAD_XDOWN	SDLK_DOWN
#define PAD_UP		SDLK_UP
#define PAD_LEFT	SDLK_LEFT
#define PAD_RIGHT	SDLK_RIGHT
#define PAD_DOWN	SDLK_DOWN
#define PAD_A		SDLK_LCTRL
#define PAD_B		SDLK_LALT
#define PAD_X		SDLK_LSHIFT
#define PAD_Y		SDLK_SPACE
#define PAD_L		SDLK_TAB
#define PAD_R		SDLK_BACKSPACE
#define PAD_START	SDLK_RETURN
#define PAD_SELECT	SDLK_ESCAPE
#define PAD_SLIDER	SDLK_HOME
#define PAD_QUIT	SDLK_ESCAPE


#else
#define PAD_XUP		SDLK_UP
#define PAD_XLEFT	SDLK_LEFT
#define PAD_XRIGHT	SDLK_RIGHT
#define PAD_XDOWN	SDLK_DOWN
#define PAD_UP		SDLK_UP
#define PAD_LEFT	SDLK_LEFT
#define PAD_RIGHT	SDLK_RIGHT
#define PAD_DOWN	SDLK_DOWN
#define PAD_A		SDLK_LCTRL
#define PAD_B		SDLK_LALT
#define PAD_X		SDLK_LSHIFT
#define PAD_Y		SDLK_SPACE
#define PAD_L		SDLK_TAB
#define PAD_R		SDLK_BACKSPACE
#define PAD_START	SDLK_RETURN
#define PAD_SELECT	SDLK_ESCAPE
#define PAD_SLIDER	SDLK_HOME
#define PAD_QUIT	SDLK_ESCAPE
#endif

#define cartridge_IsLoaded() (strlen(gameName) != 0)

typedef struct {
  unsigned int sndLevel;
  unsigned int m_ScreenRatio; // 0 = original show, 1 = full screen
  unsigned int OD_Joy[12]; // each key mapping
  unsigned int m_DisplayFPS;
  char current_dir_rom[MAX__PATH];
  unsigned int m_Color;
} gamecfg;

extern char gameName[MAX__PATH];
extern unsigned int gameCRC;

#if defined(GIZMONDO) || defined(KOLIBRI)
typedef unsigned int uint32_t;
#endif

//typedef unsigned char byte;
#if !defined(GIZMONDO) || !defined(KOLIBRI)
typedef unsigned short word;
#endif
typedef unsigned int uint;


#ifndef _TINSPIRE
extern SDL_Surface* actualScreen;						// Main program screen
extern SDL_Event event;
#endif

extern gamecfg GameConf;
extern unsigned int m_Flag;

extern char current_conf_app[MAX__PATH];

extern void system_loadcfg(const char *cfg_name);
extern void system_savecfg(const char *cfg_name);

extern unsigned long crc32 (unsigned int crc, const unsigned char *buf, unsigned int len);

extern void mainemuinit();

// menu
extern void screen_showtopmenu(void);
extern void print_string_video(short x, const short y, const char *s);
extern void gethomedir(char *dir, char* name);

// sound
extern void AWriteAudioData(unsigned int chan, unsigned int len, unsigned char *pWav);
/*extern void Ainit(void);*/
/*extern void Aclose(void);*/
extern void APlayVoice(unsigned int chan, unsigned char *pWav, unsigned int loop);
extern void AStopVoice(unsigned int chan, unsigned char *pWav);

extern short button_state[18];
extern unsigned char button_time[18];
extern void Buttons(void);


#endif
