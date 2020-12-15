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

	lightFirstRoom(OFF);
	lightSecondRoom(OFF);

	lockFirstRoomControl(OPEN);
	lockSecondRoomControl(OPEN);


    return;
}//sleep_state_init()
//------------------------------------------------------------------------------------------------------
void start_variables_init(void)
{
    //start values
	chairs_counter = 0;
	current_chair = prew_chair = {_ALL_RELEASED, RELEASED};

    //state machine
    current_state = IDLE_STATE;

    //flags
    game_start_flag = false;
	sleep_state_done_flag = false;

	watch_win_flag = false;
	watch_start_timer_flag = false;
	watch_game_en_flag = false;

	chairs_win_flag = false;

    //loads
	lightFirstRoom(ON);
	lightSecondRoom(ON);

	lockFirstRoomControl(OPEN);
	lockSecondRoomControl(CLOSED);

	//watch
	setSteppersRandom();

    
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