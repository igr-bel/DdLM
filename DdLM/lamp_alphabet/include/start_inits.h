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
	return;
}//sleep_state_init()
//------------------------------------------------------------------------------------------------------
void start_variables_init(void)
{
	//start values
	letter_counter = 0;

	//state machine
	current_state = IDLE_STATE;

	//flags
	sleep_state_done_flag = false;
	game_start_flag = false;

	new_data_recieved = false;


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