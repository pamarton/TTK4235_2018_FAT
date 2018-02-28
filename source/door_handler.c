#include "door_handler.h"

int timer_get_timestamp(int wait_time){//get timestamp from <time.h>
	return time(NULL) + wait_time;
}

int timer_compare(int timestamp){//compare timestamp to current time <time.h>
	if(timestamp <= time(NULL)){
		return 1;	
	}
	return 0;
}

int door_status = 0;//global varable for the door timer

void door_open(void){//open door and set timer to WAIT_TIME
	door_status = timer_get_timestamp(WAIT_TIME);
	elev_set_door_open_lamp(1);
}

int door_close(void){//close door if time has run out

	//IF OBSTRUCTION RETURN 0 AND door_open() :)
	if(timer_compare(door_status)){		
		elev_set_door_open_lamp(0);
		return 1;
	}
	return 0;
}
