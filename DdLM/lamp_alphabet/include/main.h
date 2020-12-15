#ifndef _MAIN_H
#define _MAIN_H

//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include <Arduino.h>

#include "functions.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
//STATE MACHINE
#define IDLE_STATE                      0
#define SLEEP_STATE                     1
#define RESET_STATE                     2
#define RESTART_STATE                   3

#define GAME_STATE_JUAN					4
#define GAME_STATE_MARIA				5	
#define GAME_STATE_CARLA				6	
#define GAME_STATE_RAQUEL				7	
#define GAME_STATE_DIEGO				8	
#define GAME_STATE_HELP					9
#define GAME_IDLE_STATE					10
#define GAME_CHECK_STATE				11
#define GET_LETTER_STATE				12
#define NEW_GAME_STATE					13

#define WIN_STATE_JUAN					14
#define WIN_STATE_MARIA					15
#define WIN_STATE_CARLA					16
#define WIN_STATE_RAQUEL				17
#define WIN_STATE_DIEGO					18
#define WIN_STATE_HELP					19

#define WIN_STATE                       21
#define WIN_IDLE_STATE                  22

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
volatile uint16_t current_state;

//-------------------------------------- GAME VARIABLES
letter_T prew_letter;
letter_T current_letter;
uint8_t letter_counter;
uint16_t current_game;

unsigned long estonian_timer;	//timer for reset game
const uint16_t game_reset_time = 5000;

const uint8_t juan_length = 4;
const uint8_t maria_length = 5;
const uint8_t carla_length = 5;
const uint8_t raquel_length = 6;
const uint8_t diego_length = 5;
const uint8_t help_length = 4;

uint8_t juan_win_arr[] = {_J, _U, _A, _N};
uint8_t maria_win_arr[] = {_M, _A, _R, _I, _A};
uint8_t carla_win_arr[] = {_C, _A, _R, _L, _A};
uint8_t raquel_win_arr[] = {_R, _A, _Q, _U, _E, _L};
uint8_t diego_win_arr[] = {_D, _I, _E, _G, _O};
uint8_t help_win_arr[help_length] = {_H, _E, _L, _P};

//------------------------------------------------------------------------------------------------------
//                                        DEBUG
//------------------------------------------------------------------------------------------------------
#ifdef DBG
	unsigned long DBG_out_interval = 100;
	unsigned long DBG_time_delay = 0;
#define printDBG(str)     Serial.print("[DBG] "); Serial.println(str) 
#else 
	#define printDBG(str)     ;
#endif

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool game_start_flag = false;
bool new_data_recieved = false;
bool sleep_state_done_flag = false;

//------------------------------------------------------------------------------------------------------
#endif //_MAIN_H