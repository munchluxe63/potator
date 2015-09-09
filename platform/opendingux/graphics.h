#ifndef _TINSPIRE
#include <SDL/SDL.h>
#endif
#include "shared.h"

extern unsigned short XBuf[(SYSVID_WIDTH+1)*(SYSVID_HEIGHT+1)];
extern unsigned long nextTick, lastTick, newTick, currentTick, wait;
extern unsigned short FPS; 
extern unsigned short pastFPS; 


extern unsigned long SDL_UXTimerRead(void);
void graphics_paint(void);
