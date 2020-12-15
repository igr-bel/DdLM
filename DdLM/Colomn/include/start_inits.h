#ifndef _START_INITS_H
#define _START_INITS_H

#include "main.h"

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//hardware
inline void sleep_state_init(void);
inline void start_variables_init(void);
inline void props_is_ready(void);


//------------------------------------------------------------------------------------------------------
//                                       BEHAVIOR
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
void sleep_state_init(void)
{
    sleep_state_done_flag = true;

	//off all loads
	LASER_POWER_OFF();
	loadControl(SMOKE_POWER, OFF);
	loadControl(SMOKE_BTN, OFF);
	loadControl(SYMBOL_WEIGHT, OFF);
	loadControl(SYMBOL_SMELLS, OFF);
	loadControl(SYMBOL_TOUCH, OFF);
	loadControl(SYMBOL_SOUND, OFF);

    return;
}//sleep_state_init()
//------------------------------------------------------------------------------------------------------
void start_variables_init(void)
{
    //start values
	harp_melody_counter = 0;
	prew_harp_state = NON_ACTIVE;

    //state machine
    current_state = IDLE_STATE;

    //flags
    game_start_flag = false;
	sleep_state_done_flag = false;

	harp_error = false;

	harp_win_flag = false;
	smells_win_flag = false;
	touch_win_flag = false;
	weight_win_flag = false;

    //loads
	//off all loads
	LASER_POWER_OFF();
	loadControl(SMOKE_POWER, OFF);
	loadControl(SMOKE_BTN, OFF);
	loadControl(SYMBOL_WEIGHT, OFF);
	loadControl(SYMBOL_SMELLS, OFF);
	loadControl(SYMBOL_TOUCH, OFF);
	loadControl(SYMBOL_SOUND, OFF);

    return;
}//start_variables_init()
//------------------------------------------------------------------------------------------------------
void props_is_ready(void)
{
    new_data_recieved = false;

    return;
}//props_is_ready()
//------------------------------------------------------------------------------------------------------

#endif //_START_INITS_H