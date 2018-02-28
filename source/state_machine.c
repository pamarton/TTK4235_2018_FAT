#include "state_machine.h"
struct elevator_data my_elev;


void new_state(){//state machine
	switch(my_elev.state){
		case state_init:
			func_state_init();
			break;
		case state_stop:
			stop_emergency_stop(&my_elev);
			break;
		case state_wait:
			func_state_wait();
			break;
		case state_drive:
			func_state_drive();
			break;
		case state_door:
			func_state_door();
			break;
	}
	panel_get(&my_elev);
	panel_set_lights(my_elev);
}

void func_state_init(void){
	
	my_elev.stop = 0;
	#if OBSTRUCTION
	my_elev.obstruct = 0;
	#endif
	
	for(int type = 0; type < N_BUTTONTYPES; type++){//set orders for initialization lights
		for(int floor = 0; floor < N_FLOORS; floor++){
			my_elev.orders[type][floor] = 1;
		}	
	}
	panel_set_lights(my_elev);//set the lights during initialization

	elev_set_motor_direction(DIRN_DOWN);
	while(elev_get_floor_sensor_signal() != 0);//wait for elevator to reach the ground floor

	my_elev.dir = DIRN_STOP;
	elev_set_motor_direction(DIRN_STOP);
	my_elev.floor = elev_get_floor_sensor_signal();
	my_elev.last_floor = elev_get_floor_sensor_signal();
	my_elev.temp_floor = (float)my_elev.last_floor+(float)my_elev.dir/2;//initialize temproary possition

	for(int type = 0; type < N_BUTTONTYPES; type++){//clear orders
		for(int floor = 0; floor < N_FLOORS; floor++){
			my_elev.orders[type][floor] = 0;
		}	
	}
	panel_set_lights(my_elev);//clear lights
	my_elev.state = state_wait;//goto state_wait
}



void func_state_wait(void){//wait for order
	//elev_set_motor_direction(DIRN_STOP);
	if(func_check_floor(my_elev.floor)){//if order in floor, open door
		door_open();
		my_elev.state = state_door;
	}else if(func_order_exist()){//else go to state_drive
		my_elev.state = state_drive;
	}
}

int func_state_drive(void){//state where the elevator is moving, and decides the direction
	if(my_elev.dir == DIRN_STOP){//if the direction is 0 then there has been an emergency stop OR initializing
		my_elev.dir = func_startup_dir();
	}else{
		if(my_elev.floor >= 0 && my_elev.floor < N_FLOORS){
			my_elev.last_floor = my_elev.floor;
			//my_elev.temp_floor = my_elev.floor;//remove?
			if(func_check_floor_directional(my_elev.floor)){//should it stop in the current floor?
					elev_set_motor_direction(DIRN_STOP);
				my_elev.state = state_wait;//goto state_wait
				return 0;
			}
		}

		if(!func_order_in_front()){//are there no more orders in the current direction?
			my_elev.dir *= -1; //change direction
			if(!func_order_in_front()){//are there no more orders in the current direction?
				elev_set_motor_direction(DIRN_STOP);
				my_elev.state = state_wait;//goto state_wait
				return 0;
			}
				
		}
	}
	
	my_elev.temp_floor = (float)my_elev.last_floor+(float)my_elev.dir/2; //temproary possition of the elevator (between floors)
	elev_set_motor_direction(my_elev.dir);//set the computed direction
	return 0;
}

void func_state_door(void){//state when the door is open
	#if OBSTRUCTION//obstruction lever implemented, not a part of the task
	if(my_elev.obstruct){
		door_open();
	}
	#endif
	if(my_elev.floor < N_FLOORS && my_elev.floor >= 0){//if in floor
		if(func_check_floor(my_elev.floor)){//check for new button pushed in current floor
			door_open();
			func_clear_current_floor();//clear orders of the current floor
		}else if(door_close()){//close the door if time has passed
			my_elev.state = state_wait;
		}	
	}
}

int func_order_exist(void){//returns 1 if there is an order in the system
	for(int f = 0; f < N_FLOORS; f++){
		if(func_check_floor(f)){
			return 1;
		}	
	}
	return 0;
}

int func_startup_dir(void){//if the elevator is in a undefined direction, we chose one (the first button pushed)
	for(int f = 0; f < N_FLOORS; f++){
		
		if(func_check_floor(f)){
			if(f > my_elev.temp_floor){
				my_elev.last_floor = f-1;
				return DIRN_UP;
			}else if(f < my_elev.temp_floor){
				my_elev.last_floor = f+1;
				return DIRN_DOWN;
			}
		}	
	}
	return DIRN_STOP;
}

int func_check_floor_directional(int floor){//syntax to determine if the elevator should stop in the current floor
	if(my_elev.orders[BUTTON_COMMAND][floor]){//if a "heispanel" order is registered we must serve it
		return 1;
	}	
	if(my_elev.orders[BUTTON_CALL_UP][floor] && my_elev.dir == DIRN_UP){//if a "etasjepanel" order is registered we only serve it if it is in the direction we move - here up
		return 1;
	}
	if(my_elev.orders[BUTTON_CALL_DOWN][floor] && my_elev.dir == DIRN_DOWN){//	if a "etasjepanel" order is registered we only serve it if it is in the direction we move - here down
		return 1;
	}
	if(my_elev.orders[BUTTON_CALL_UP][0] == 1 && floor == 0){//check special case, bottom floor up button
		return 1;
	}
	if(my_elev.orders[BUTTON_CALL_DOWN][N_FLOORS - 1] == 1 && floor == N_FLOORS - 1){//check special case, top floor down button
		return 1;
	}
	return 0;
}

int func_check_floor(int floor){//checks if floor has an order (directionless)
	if(floor < 0 || floor >= N_FLOORS){
		return 0;
	}
	for(int type = 0; type < N_BUTTONTYPES; type++){
		if(my_elev.orders[type][floor]){
			return 1;
		}	
	}
	return 0;
}


void func_clear_current_floor(void){//remove orders from the floor we are in
	for(int type = 0; type < N_BUTTONTYPES; type++){
		my_elev.orders[type][my_elev.floor] = 0;
	}
}

int func_order_in_front(void){//checks if there are an order in the direction the elevator is moving
	if(my_elev.dir == DIRN_UP){
		for(int check_floor = my_elev.last_floor+1; check_floor < N_FLOORS && check_floor >= 0; check_floor++){//checks the next floors in the direction (starting from our pos + 1)
			if(func_check_floor(check_floor)){//if there is an order in that direction, return 1
				return 1;
			}
		}
	}else if(my_elev.dir == DIRN_DOWN){
		for(int check_floor = my_elev.last_floor-1; check_floor < N_FLOORS && check_floor >= 0; check_floor--){//checks the next floors in the direction (starting from our pos + -1)
			if(func_check_floor(check_floor)){//if there is an order in that direction, return 1
				return 1;
			}
		}
	}
	return 0;//default return
}


