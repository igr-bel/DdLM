#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

#include "mqtt_cmds.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
#define DBG

#ifdef DBG
  unsigned long DBG_out_interval = 100;
  unsigned long DBG_time_delay = 0;
  #define printDBG(str)     Serial.print("[DBG] "); Serial.println(str) 
#else 
  #define printDBG(str)     ;
#endif

#define NOP                             do{}while(0)

//GPIO
#define concat(a,b)						a ## b

#define LASER_PIN_1						A6
#define LASER_PIN_2						A7
#define LASER_PIN_3						A8
#define LASER_PIN_4						A9
#define LASER_PIN_5						A10
#define LASER_PIN_ERR					A14

#define COLOR_PIN_1						4
#define COLOR_PIN_2						10
#define COLOR_PIN_3						A12
#define COLOR_PIN_4						A13

//GPIO LOADS
#define LAMP_1_PIN						5
#define LAMP_2_PIN						6
#define LAMP_3_PIN						7
#define LAMP_4_PIN						8
#define LAMP_5_PIN						9
#define HOLE_LIGHT_PIN					11
#define FIRST_DOOR_LOCK_PIN				2
#define SECOND_DOOR_LOCK_PIN			3
#define THIRD_DOOR_LOCK_PIN				18
#define OUT_DOOR_LOCK_PIN				19
#define LIGHT_BRIDGE_PIN	 			20
#define LIGHT_FIRST_ROOM_PIN			28
#define LIGHT_SECOND_ROOM_PIN			29
#define LIGHT_THIRD_ROOM_PIN			30
#define BRIDGE_LASER_PIN				A11


//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
//loads

//game
enum
{
    EN = 1,
    FR
};

enum
{
    CLOSED,
    OPEN
};

enum
{
    OFF,
    ON,
    PULSE
};

enum
{
	UNLOCK,
	LOCK
};

enum
{
    OK,
    ERR
};

enum
{
	NOT_PRESSED, //for function error control
	PRESSED,			
	HOLD,				
	RELEASED			
};

enum
{
	LAMP_1,
	LAMP_2,
	LAMP_3,
	LAMP_4,
	LAMP_5,
	HOLE_LIGHT,
	FIRST_DOOR,
	SECOND_DOOR,
	THIRD_DOOR,
	OUT_DOOR,
	LIGHT_BRIDGE,
	LIGHT_FIRST_ROOM,
	LIGHT_SECOND_ROOM,
	LIGHT_THIRD_ROOM,
	BRIDGE_LASER
};

enum
{
	_BTN_1,
	_BTN_2,
	_BTN_3,
	_BTN_4,
	_BTN_5,

	_NUM_OF_BTNS,

	_BTN_ERR,		//error button is pressed
	_ALL_BTNS,

	_BTN_READ_ERROR	//impossible to define the button
};

struct btn_state_T
{
	 uint8_t btn_number;
	 uint8_t btn_state;
};

enum
{
	LASER_1,
	LASER_2,
	LASER_3,
	LASER_4,
	LASER_5,
	LASER_ERR,

	NUM_OF_LASERS,
	_ERR,
	_NONE
};

//data for UI
bool mqtt_data_is_ready = false;
int mqtt_data;

//lasers
uint8_t laser_pin[NUM_OF_LASERS] = {LASER_PIN_1, LASER_PIN_2, LASER_PIN_3, LASER_PIN_4, LASER_PIN_5, LASER_PIN_ERR};
uint8_t tail_correct_sequence[NUM_OF_LASERS] = {LASER_1, LASER_2, LASER_3, LASER_4, LASER_5};

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool laser_win_flag = false;
bool colors_win_flag = false;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//RESET
void(* resetFunc) (void) = 0;

//HARDWARE
inline void hardware_init(void);

//LOADS
void loadControl(uint8_t load, uint8_t state);


//GAME
//laser tail
uint16_t laserGame(void);
uint8_t getLaserState(void);

//colors
uint8_t colorsGame(void);

//------------------------------------------------------------------------------------------------------
//                                       BEHAVIOR
//------------------------------------------------------------------------------------------------------
inline void hardware_init(void)
{
    #ifdef DBG
        Serial.begin(9600);
        Serial.println("___DBG___");
        Serial.println("[DBG] start init ... ");
    #endif

    //------------------------------------------------------ GPIO
	pinMode(LAMP_1_PIN, OUTPUT);
	pinMode(LAMP_2_PIN, OUTPUT);
	pinMode(LAMP_3_PIN, OUTPUT);
	pinMode(LAMP_4_PIN, OUTPUT);
	pinMode(LAMP_5_PIN, OUTPUT);
	pinMode(HOLE_LIGHT_PIN, OUTPUT);
	pinMode(FIRST_DOOR_LOCK_PIN, OUTPUT);
	pinMode(SECOND_DOOR_LOCK_PIN, OUTPUT);
	pinMode(THIRD_DOOR_LOCK_PIN, OUTPUT);
	pinMode(OUT_DOOR_LOCK_PIN, OUTPUT);
	pinMode(LIGHT_BRIDGE_PIN, OUTPUT);
	pinMode(LIGHT_FIRST_ROOM_PIN, OUTPUT);
	pinMode(LIGHT_SECOND_ROOM_PIN, OUTPUT);
	pinMode(LIGHT_THIRD_ROOM_PIN, OUTPUT);
	pinMode(BRIDGE_LASER_PIN, OUTPUT);

	//colors
	pinMode(COLOR_PIN_1, INPUT);
	pinMode(COLOR_PIN_2, INPUT);
	pinMode(COLOR_PIN_3, INPUT);
	pinMode(COLOR_PIN_4, INPUT);

	//lasers
	for(uint8_t i = 0; i < NUM_OF_LASERS; i++)
	{
		pinMode(laser_pin[i], INPUT);
	}

    #ifdef DBG
        Serial.println("[DBG] done!");
    #endif
}//hardware_init()
//------------------------------------------------------------------------------------------------------
uint8_t getLaserState(void)
{
	for(int8_t i = NUM_OF_LASERS - 1; i >= 0; i--)
	{
		if(digitalRead(laser_pin[i]))
		{
			return i;
		}
	}

	return _NONE;
}//getLaserState()
//------------------------------------------------------------------------------------------------------
uint16_t laserGame(void)
{
	static uint8_t _current_laser = _NONE;
	static uint8_t _prew_laser = _NONE;
	static uint8_t _laser_counter = 0;

	mqtt_data_is_ready = false;

	_current_laser = getLaserState();

	if(_current_laser != _prew_laser)
	{
		_prew_laser = _current_laser;

		if(_current_laser == LASER_ERR)
		{
			_laser_counter = 0;
			mqtt_data_is_ready = true;
			mqtt_data = LASER_CLEAR;
			return ERR;
		}

		if(_current_laser != LASER_ERR && _current_laser != _NONE)
		{
			if(_current_laser == tail_correct_sequence[_laser_counter])
			{
				_laser_counter++;
				if(_laser_counter == 5)
				{
					return LASER_WIN;
				}
				else
				{
					mqtt_data_is_ready = true;
					mqtt_data = LASER_CLEAR | (1 << (_laser_counter-1));
					return ERR;
				}
			}
			else if(_current_laser > tail_correct_sequence[_laser_counter])
			{
				_laser_counter = 0;
				mqtt_data_is_ready = true;
				mqtt_data = LASER_CLEAR;
				return ERR;
			}
			
		}
	}//[if] laser state changed

	_prew_laser = _current_laser;
	return ERR;
}//laserGame()
//------------------------------------------------------------------------------------------------------
uint8_t colorsGame(void)
{
	static uint8_t _prew_colors_state;
	uint8_t _colors_state;

	_colors_state = 0x00;
	
	_colors_state |= (digitalRead(COLOR_PIN_1)) ? (1 << 0) : (0 << 0); 
	_colors_state |= (digitalRead(COLOR_PIN_2)) ? (1 << 1) : (0 << 1); 
	_colors_state |= (digitalRead(COLOR_PIN_3)) ? (1 << 2) : (0 << 2); 
	_colors_state |= (digitalRead(COLOR_PIN_4)) ? (1 << 3) : (0 << 3); 

	mqtt_data_is_ready = false;

	if(_colors_state != _prew_colors_state)
	{
		_prew_colors_state = _colors_state;
		mqtt_data_is_ready = true;
		mqtt_data = COLORS_CLEAR | _colors_state;
		
		if(_colors_state == 0x0F)
		{
			return OK;
		}
	}

	_prew_colors_state = _colors_state;
	return ERR;
}//colorsGame()
//------------------------------------------------------------------------------------------------------
void loadControl(uint8_t load, uint8_t state)
{
	switch (load)
	{
		case LAMP_1:
			digitalWrite(LAMP_1_PIN, state);
			break;

		case LAMP_2:
			digitalWrite(LAMP_2_PIN, state);
			break;

		case LAMP_3:
			digitalWrite(LAMP_3_PIN, state);
			break;

		case LAMP_4:
			digitalWrite(LAMP_4_PIN, state);
			break;

		case LAMP_5:
			digitalWrite(LAMP_5_PIN, state);
			break;

		case HOLE_LIGHT:
			digitalWrite(HOLE_LIGHT_PIN, state);
			break;

		case FIRST_DOOR:
			digitalWrite(FIRST_DOOR_LOCK_PIN, state);
			break;

		case SECOND_DOOR:
			digitalWrite(SECOND_DOOR_LOCK_PIN, state);
			break;

		case THIRD_DOOR:
			digitalWrite(THIRD_DOOR_LOCK_PIN, state);
			break;

		case OUT_DOOR:
			digitalWrite(OUT_DOOR_LOCK_PIN, state);
			break;

		case LIGHT_BRIDGE:
			digitalWrite(LIGHT_BRIDGE_PIN, state);
			break;

		case LIGHT_FIRST_ROOM:
			digitalWrite(LIGHT_FIRST_ROOM_PIN, state);
			break;

		case LIGHT_SECOND_ROOM:
			digitalWrite(LIGHT_SECOND_ROOM_PIN, state);
			break;

		case LIGHT_THIRD_ROOM:
			digitalWrite(LIGHT_THIRD_ROOM_PIN, state);
			break;

		case BRIDGE_LASER:
			digitalWrite(BRIDGE_LASER_PIN, state);
			break;

	}//[switch]

}//loadControl()
//------------------------------------------------------------------------------------------------------

#endif //FUNCTIONS_INCLUDED