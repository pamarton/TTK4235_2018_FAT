#ifndef __INCLUDE_STATE_MACHINE_H__
#define __INCLUDE_STATE_MACHINE_H__

#include "elev.h"
#include <stdio.h>
#include "panel_handler.h"
#include "stop_handler.h"

void new_state();//state machine
void func_state_init(void);//initialize state
void func_state_wait(void);//wait for order
int func_state_drive(void);//state where the elevator is moving, and decides the direction
void func_state_door(void);//state when the door is open
int func_order_exist(void);//returns 1 if there is an order in the system
int func_startup_dir(void);//if the elevator is in a undefined direction, we chose one (the first button pushed)
int func_check_floor_directional(int floor);//syntax to determine if the elevator should stop in the current floor
int func_check_floor(int floor);//checks if floor has an order (directionless)
void func_clear_current_floor(void);//remove orders from the floor we are in
int func_order_in_front(void);//checks if there are an order in the direction the elevator is moving
#endif //__INCLUDE_STATE_MACHINE_H__
