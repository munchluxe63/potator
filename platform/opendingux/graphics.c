#include "graphics.h"
#ifdef _TINSPIRE
#include "n2DLib.h"
#endif

void graphics_paint(void) 
{
#ifdef _TINSPIRE
	unsigned short *buffer_scr = (unsigned short *)BUFF_BASE_ADDRESS;
#else
	unsigned short *buffer_scr = (unsigned short *) actualScreen->pixels;
#endif
	unsigned short *buffer_flip = (unsigned short *) XBuf;
	unsigned int W,H,ix,iy,x,y;

#ifndef _TINSPIRE
	SDL_LockSurface(actualScreen);
#endif
	
	switch(GameConf.m_ScreenRatio)
	{
		case 2:
			x=((320 - 240)/2);
			y=((240 - 240)/2); 
			W=240;
			H=240;
			ix=(SYSVID_WIDTH<<16)/W;
			iy=(SYSVID_HEIGHT<<16)/H;
			
			buffer_scr += (y)*240;
			buffer_scr += (x);
			do   
			{
				unsigned short *buffer_mem=(buffer_flip+((y>>16)*SYSVID_WIDTH));
				W=240; 
				x=((320 - 240)/2);
				do {
					unsigned short value = (((buffer_mem[x>>16]) & 0x7FE0) << 1) | ((buffer_mem[x>>16]) & 0x001F);
					*buffer_scr++=value;
					x+=ix;
				} while (--W);
				y+=iy;
				buffer_scr += 320 - 240;
			} while (--H);
		break;
		case 1:
			x=0;
			y=0; 
			W=320;
			H=240;
			ix=(SYSVID_WIDTH<<16)/W;
			iy=(SYSVID_HEIGHT<<16)/H;

			do   
			{
				unsigned short *buffer_mem=(buffer_flip+((y>>16)*SYSVID_WIDTH));
				W=320; 
				x=0;
				do {
					unsigned short value = (((buffer_mem[x>>16]) & 0x7FE0) << 1) | ((buffer_mem[x>>16]) & 0x001F);
					*buffer_scr++=value;
					x+=ix;
				} while (--W);
				y+=iy;
			} while (--H);
		break;
		case 0:
			x=((320 - SYSVID_WIDTH)/2);
			y=((240 - SYSVID_HEIGHT)/2); 
			W=SYSVID_WIDTH;
			H=SYSVID_HEIGHT;
			ix=(SYSVID_WIDTH<<16)/W;
			iy=(SYSVID_HEIGHT<<16)/H;
			
			buffer_scr += (y)*320;
			buffer_scr += (x);
			do   
			{
				unsigned short *buffer_mem=(buffer_flip+((y>>16)*SYSVID_WIDTH));
				W=SYSVID_WIDTH; 
				x=((320 - SYSVID_WIDTH)/2);
				do {
					unsigned short value = (((buffer_mem[x>>16]) & 0x7FE0) << 1) | ((buffer_mem[x>>16]) & 0x001F);
					*buffer_scr++=value;
					x+=ix;
				} while (--W);
				y+=iy;
				buffer_scr += 320 - SYSVID_WIDTH;
			} while (--H);
		break;
	}
	
	if (GameConf.m_DisplayFPS) 
	{
		static char buffer[5];
		pastFPS++;
		newTick = SDL_UXTimerRead();
		if ((newTick-lastTick)>1000000) 
		{
			FPS = pastFPS;
			pastFPS = 0;
			lastTick = newTick;
		}
		sprintf(buffer,"%d",FPS);
		print_string_video(0,0,buffer);
	}
	
	
#ifdef _TINSPIRE	
	 updateScreen();
#else
	SDL_UnlockSurface(actualScreen);
	SDL_Flip(actualScreen);
#endif
	
}
