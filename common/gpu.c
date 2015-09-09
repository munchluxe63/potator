////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef GP2X
#include "minimal.h"
#endif
#ifdef NDS
#include <nds.h>
#endif

#include "../platform/opendingux/shared.h"

static uint16	*supervision_palette;
static uint8    gpu_regs[4];
#ifdef NDS
#define RGB555(R,G,B) ((((int)(B))<<10)|(((int)(G))<<5)|(((int)(R)))|BIT(15))
#else
#define RGB555(R,G,B) ((((int)(B))<<10)|(((int)(G))<<5)|(((int)(R))))
#endif

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void gpu_init(void)
{
	#ifdef DEBUG
	printf("Gpu Init\n");
	#endif
	//fprintf(log_get(), "gpu: init\n");
	memory_malloc_secure((void**)&supervision_palette,  4*sizeof(int16), "Palette");
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void gpu_reset(void)
{
	#ifdef DEBUG
	printf("Gpu Reset\n");
	#endif


#ifdef _TINSPIRE
    supervision_palette[3] = (0 >> 3) << 11 | (0 >> 2) << 5 | (0 >> 3);
	supervision_palette[2] = (85 >> 3) << 11 | (85 >> 2) << 5 | (85 >> 3);
	supervision_palette[1] = (170 >> 3) << 11 | (170 >> 2) << 5 | (170 >> 3);
	supervision_palette[0] = (240 >> 3) << 11 | (240 >> 2) << 5 | (240 >> 3);
#else
    supervision_palette[3] = PIX_TO_RGB(actualScreen->format,0,0,0);
	supervision_palette[2] = PIX_TO_RGB(actualScreen->format,85,85,85);
	supervision_palette[1] = PIX_TO_RGB(actualScreen->format,170,170,170);
	supervision_palette[0] = PIX_TO_RGB(actualScreen->format,240,240,240);
#endif

	memset(gpu_regs, 0, 4);
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void gpu_set_colour_scheme(int colourScheme)
{
	#ifdef DEBUG
	printf("Gpu Set Color Scheme\n");
	#endif

	float greenf=1;
	float bluef=1;
	float redf=1;

	switch (colourScheme)
	{
	case COLOUR_SCHEME_DEFAULT:
		break;
	case COLOUR_SCHEME_AMBER:
		greenf=0.61f;
		bluef=0.00f;
		redf=1.00f;
		break;
	case COLOUR_SCHEME_GREEN:
		greenf=0.90f;
		bluef=0.20f;
		redf=0.20f;
		break;
	case COLOUR_SCHEME_BLUE:
		greenf=0.30f;
		bluef=0.75f;
		redf=0.30f;
		break;
	default: 
		colourScheme=0; 
		break;
	}

	int p11 = (int) 85*redf; int p12 = (int) 85*greenf; int p13 = (int) 85*bluef;
	int p21 = (int) 170*redf; int p22 = (int) 170*greenf; int p23 = (int) 170*bluef;
	int p31 = (int) 255*redf; int p32 = (int) 255*greenf; int p33 = (int) 255*bluef;
#ifdef _TINSPIRE
    supervision_palette[3] = (0 >> 3) << 11 | (0 >> 2) << 5 | (0 >> 3);
	supervision_palette[2] = (p11 >> 3) << 11 | (p12 >> 2) << 5 | (p13 >> 3);
	supervision_palette[1] = (p21 >> 3) << 11 | (p22 >> 2) << 5 | (p23 >> 3);
	supervision_palette[0] = (p31 >> 3) << 11 | (p32 >> 2) << 5 | (p33 >> 3);
#else
    supervision_palette[3] = PIX_TO_RGB(actualScreen->format,0,0,0);
	supervision_palette[2] = PIX_TO_RGB(actualScreen->format,p11,p12,p13);
	supervision_palette[1] = PIX_TO_RGB(actualScreen->format,p21,p22,p23);
	supervision_palette[0] = PIX_TO_RGB(actualScreen->format,p31,p32,p33);
#endif

}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void gpu_write(uint32 addr, uint8 data)
{
	gpu_regs[(addr&0x03)] = data;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
uint8 gpu_read(uint32 addr)
{
	return(gpu_regs[(addr&0x03)]);
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void gpu_render_scanline(uint32 scanline, int16 *backbuffer)
{
	uint8 *vram_line = &(memorymap_getUpperRamPointer())[(gpu_regs[2] >> 2) + (scanline*0x30)];
	uint8 x;

	for (x =0; x < 160; x += 4)
	{
		uint8 b = *(vram_line++);
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
	}
}


/* This version is faster than the one below */
/*
void gpu_render_scanline_fast(uint32 scanline, int16 *backbuffer)
{
	uint8 *vram_line = &(memorymap_getUpperRamPointer())[(gpu_regs[2] >> 2) + (scanline)];
	uint8 b = *(vram_line++);
  	
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;
  	
  	    b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
	    b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;
		
		b = *(vram_line++);
        backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
		b = *(vram_line++);
  	
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;
  	
  	    b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
	    b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;  	
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];		
		backbuffer += 4;
		
		b = *(vram_line++);
        backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;		
		b = *(vram_line++);
		backbuffer[4] = supervision_palette[((b >> 8) & 0x03)];
		backbuffer[3] = supervision_palette[((b >> 6) & 0x03)];
		backbuffer[2] = supervision_palette[((b >> 4) & 0x03)];
		backbuffer[1] = supervision_palette[((b >> 2) & 0x03)];
		backbuffer[0] = supervision_palette[((b >> 0) & 0x03)];
		backbuffer += 4;
}
*/

void gpu_render_scanline_fast(uint32 scanline, uint16 *backbuffer)
{
	uint8 *vram_line = &(memorymap_getUpperRamPointer())[(gpu_regs[2] >> 2) + (scanline)];
	uint8 x;
	uint32 *buf = (uint32 *) backbuffer;
	
	for (x =0; x < 160; x += 4)
	{
		uint8 b = *(vram_line++);
		*(buf++) = (supervision_palette[((b >> 2) & 0x03)]<<16) | (supervision_palette[((b) & 0x03)]);
		*(buf++) = (supervision_palette[((b >> 6) & 0x03)]<<16) | (supervision_palette[((b >> 4) & 0x03)]);
	}
}
