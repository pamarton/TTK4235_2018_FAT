#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__

#define N_BUTTONTYPES 3
#ifndef N_FLOORS 
#define N_FLOORS 4
#endif
#define OBSTRUCTION 1
#define N_STATES 5
#define WAIT_TIME 3
#define PRINTOUT 1

struct elevator_data{//struct for the elevator data and states
	int state;
	int stop;
	#if OBSTRUCTION
	int obstruct;
	#endif
	int floor;
	float temp_floor;
	int last_floor;
	int dir;
	int orders[N_BUTTONTYPES][N_FLOORS];
}inputs;

enum elevator_state{//enum with the different states
	state_init = 0,
	state_wait = 2,
	state_stop = 1,
	state_drive = 3,
	state_door = 4,
};

#endif //__INCLUDE_COMMON_H__
