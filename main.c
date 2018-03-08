/*#####################*\
###    ####     ###    ##	TTK4235 SPRING 2018
## ########### ### ######  	ELEVATOR C-PROJECT
## #   #####    ##    ###	AMUND MARTON
## ### ###### #### ### ##	VEBJØRN TANDBERG
###    ###### #####   ###
\*#####################*/

#include "source/state_machine.h"
#include <stdio.h>

int main() {
    	// Initialize hardware
    	if (!elev_init()) {
        	printf("Unable to initialize elevator hardware!\n");
        	return 1;
    	}
	


    	while (1) {//loop forever
		new_state();//state machine
    	}
    	return 0;
}

//end
