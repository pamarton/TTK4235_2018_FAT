#ifndef __INCLUDE_PANEL_HANDLER_H__
#define __INCLUDE_PANEL_HANDLER_H__

#include "elev.h"
#include "common.h"
#include <stdio.h>



void panel_get(struct elevator_data * inputmap);//reads all the inputs from then panel, and changes the state if stop is pressed
void panel_set_lights(struct elevator_data inputmap);//reads from the elevator_data struct and dispays the corresponding lights
#if PRINTOUT
void panel_print(struct elevator_data inputmap);//help function that prints the elevator_data struct
#endif
#endif //__INCLUDE_PANEL_HANDLER_H__
