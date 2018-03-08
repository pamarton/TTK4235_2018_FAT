//	TTK4235 SPRING 2018 : ELEVATOR C-PROJECT : AMUND MARTON & VEBJØRN TANDBERG
#ifndef __INCLUDE_DOOR_HANDLER_H__
#define __INCLUDE_DOOR_HANDLER_H__

#include <time.h>
#include <stdio.h>
#include "common.h"
#include "elev.h"

int 	timer_get_timestamp(int wait_time);//get timestamp from <time.h>
int 	timer_compare(int timestamp);//compare timestamp to current time <time.h>
void 	door_open(void);//open door and set timer to WAIT_TIME
int 	door_close(void);//close door if time has run out

#endif //__INCLUDE_DOOR_HANDLER_H__
