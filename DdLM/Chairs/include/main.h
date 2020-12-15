#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include "functions.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
//STATE MACHINE
#define IDLE_STATE                      0
#define SLEEP_STATE                     1
#define RESET_STATE                     2
#define RESTART_STATE                   3

#define GAME_STATE                      4
#define WATCH_WIN_STATE                 5

#define CHAIRS_WIN_STATE                6

#define WIN_STATE                       20
#define WIN_IDLE_STATE                  21
#define FINAL_STATE                     22


//LEDS / ANIMATION / TIMERS / DELAYS
#define WATCH_ENABLE_TIME				3000

//OFFSETS

//SHIFT MASKS

//HINTS

//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------

//------------------------------------ INPUT ETHERNET DATA
byte msg_payload[10];
uint8_t msg_length;

//--------------------------------------- DATA PARSER
uint16_t command;
String cmd_string;
char *pos = 0;
int base = 16;

//-------------------------------------- STATE MACHINE
volatile uint8_t current_state;


//-------------------------------------- GAME
chairs_state_T current_chair;
chairs_state_T prew_chair;
uint8_t chairs_counter;
uint8_t chairs_win_sequence[5] = {_M, _A, _R, _I, _A};

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool game_start_flag = false;
bool new_data_recieved = false;
bool sleep_state_done_flag = false;

//------------------------------------------------------------------------------------------------------
#endif //MAIN_H_INCLUDED