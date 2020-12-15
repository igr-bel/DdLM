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
	loadControl(LAMP_1, OFF);
	loadControl(LAMP_2, OFF);
	loadControl(LAMP_3, OFF);
	loadControl(LAMP_4, OFF);
	loadControl(LAMP_5, OFF);
	loadControl(HOLE_LIGHT, OFF);
	loadControl(FIRST_DOOR, UNLOCK);
	loadControl(SECOND_DOOR, UNLOCK);
	loadControl(THIRD_DOOR, UNLOCK);
	loadControl(OUT_DOOR, UNLOCK);
	loadControl(LIGHT_BRIDGE, OFF);
	loadControl(LIGHT_FIRST_ROOM, OFF);
	loadControl(LIGHT_SECOND_ROOM, OFF);
	loadControl(LIGHT_THIRD_ROOM, OFF);
	loadControl(BRIDGE_LASER, OFF);

    return;
}//sleep_state_init()
//------------------------------------------------------------------------------------------------------
void start_variables_init(void)
{
    //start values


    //state machine
    current_state = IDLE_STATE;

    //flags
    game_start_flag = false;
	sleep_state_done_flag = false;

	laser_win_flag = false;
	colors_win_flag = false;

    //loads
	loadControl(LAMP_1, OFF);
	loadControl(LAMP_2, OFF);
	loadControl(LAMP_3, OFF);
	loadControl(LAMP_4, OFF);
	loadControl(LAMP_5, OFF);

	loadControl(FIRST_DOOR, UNLOCK);
	loadControl(SECOND_DOOR, LOCK);
	loadControl(THIRD_DOOR, LOCK);
	loadControl(OUT_DOOR, LOCK);

	loadControl(HOLE_LIGHT, ON);
	
	loadControl(LIGHT_FIRST_ROOM, ON);
	loadControl(LIGHT_SECOND_ROOM, ON);
	loadControl(LIGHT_THIRD_ROOM, ON);

	loadControl(LIGHT_BRIDGE, OFF);
	loadControl(BRIDGE_LASER, OFF);

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