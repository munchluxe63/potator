#include <stdlib.h>
#include <stdio.h>
#include "controls.h"


uint8 controls_state;

void controls_reset()
{
	//fprintf(log_get(), "controls: reset\n");
	controls_state = 0x00;
}

uint8 controls_read(uint32 addr)
{
	return(controls_state^0xff); 
}
