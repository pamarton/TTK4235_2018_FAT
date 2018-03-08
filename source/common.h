//	TTK4235 SPRING 2018 : ELEVATOR C-PROJECT : AMUND MARTON & VEBJØRN TANDBERG
#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__

#define N_BUTTONTYPES 3//there are 3 types of buttons; up, down and panel
#ifndef N_FLOORS //if this is not already defined:
#define N_FLOORS 4//number of floors
#endif
#define OBSTRUCTION 0//if this one is set to 1, the obstruction lever will prevent the door from closing
#define N_STATES 5//number of states
#define WAIT_TIME 3//3 second waittime for the door
#define PRINTOUT 0//if this one is set to 1 the elevator prints its position

struct elevator_data{//struct for the elevator data and states
	int state;//current state
	int stop;//stop button
	#if OBSTRUCTION//see common.h to enable/disable
	int obstruct;//if there is an obstruction
	#endif
	int floor;//current floor (if not in floor, -1)
	float temp_floor;//floor in decimal (2.5 = between 2nd and 3rd floor)
	int last_floor;//last floor passed (allways between 0 and N_FLOOR-1)
	int dir;//direction
	int orders[N_BUTTONTYPES][N_FLOORS];//array over all the different button orders
}inputs;

enum elevator_state{//enum with the different states
	state_init = 0,
	state_wait = 2,
	state_stop = 1,
	state_drive = 3,
	state_door = 4,
};

#endif //__INCLUDE_COMMON_H__
