#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include <Arduino.h>

#include "stepper.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
// #define DBG                   

#ifdef DBG
  unsigned long DBG_out_interval = 100;
  unsigned long DBG_time_delay = 0;
  #define printDBG(str)     Serial.print("[DBG] "); Serial.println(str) 
#else 
  #define printDBG(str)     ;
#endif

#define NOP                             do{}while(0)

#define _WIN							1
#define _NOT_WIN						0

//GPIO
#define LIGHT_1_ROOM_PIN                47
#define LIGHT_2_ROOM_PIN                46

#define LETTER_PIN_M					34
#define LETTER_PIN_A					35
#define LETTER_PIN_R					36
#define LETTER_PIN_I					37
#define LETTER_PIN_ERR1					38
#define LETTER_PIN_ERR2					39
#define LETTER_PIN_ERR3					40
#define LETTER_PIN_ERR4					41

//STEPPERS
#define STEPPER_EN_PIN_1				28
#define STEPPER_EN_PIN_2				31
#define STEPPER_EN_PIN_3				30
#define STEPPER_EN_PIN_4				33
#define STEPPER_EN_PIN_5				32

#define STEPPERS_DIR_PIN				23
#define STEPPERS_STEP_PIN				22

#define NUM_OF_STEPPERS					5

#define WATCH_1							0
#define WATCH_2							1
#define WATCH_3							2
#define WATCH_4							3
#define WATCH_5							4
#define ALL_WATCHES						7

//IN SHIFT_REGISTER     
#define latch_pin_in                    4
#define data_pin_in                     A12
#define clock_pin_in                    A14
#define NUM_INPUT_REGS                  1

//OUT SHIFT_REGISTER        
#define latch_pin_out                   24
#define data_pin_out                    23
#define clock_pin_out                   22
#define output_enable                   25
#define NUM_OUTPUT_REGS                 1
#ifdef output_enable
    #define outputEnable() digitalWrite(output_enable, 0)
    #define outputDisable() digitalWrite(output_enable, 1)
#endif

//SHIFT & OFFSET MASK
#define DOR_1_LOCK_OFFSET               0
#define DOR_2_LOCK_OFFSET               1

#define WATCH_SENSOR_OFFSET_1			0
#define WATCH_SENSOR_OFFSET_2			1
#define WATCH_SENSOR_OFFSET_3			2
#define WATCH_SENSOR_OFFSET_4			3
#define WATCH_SENSOR_OFFSET_5			4

#define WATCH_TOUCH_SENSOR_OFFSET_1		5
#define WATCH_TOUCH_SENSOR_OFFSET_2		6

//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
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
    OK,
    ERR
};

enum
{
	NOT_ACTIVE,
	ACTIVE
};

enum
{
	RELEASED,
	PRESSED,
	HOLD
};

enum
{
	_M,
	_A,
	_R,
	_I,
	_ERR_1,
	_ERR_2,
	_ERR_3,
	_ERR_4,

	_NUM_OF_CHAIRS,
	_ERR,
	_ALL_RELEASED
};


// shift registers data
uint8_t shift_input_buff[NUM_INPUT_REGS] = {0};
uint8_t shift_output_buff[NUM_OUTPUT_REGS] = {0};

//game states
uint64_t watch_game_timer;

//chairs
struct chairs_state_T  
{
	uint8_t chair_number;
	uint8_t chair_state;
};

uint8_t chairs_pins[_NUM_OF_CHAIRS] = {LETTER_PIN_M, LETTER_PIN_A, 
										LETTER_PIN_R, LETTER_PIN_I, 
										LETTER_PIN_ERR1, LETTER_PIN_ERR2, 
										LETTER_PIN_ERR3, LETTER_PIN_ERR4};

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool watch_win_flag = false;
bool watch_start_timer_flag = false;
bool watch_game_en_flag = false;

bool chairs_win_flag = false;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//HARDWARE
inline void hardware_init(void);

//SHIFT
inline int8_t shiftInData(uint8_t *data_buff, uint32_t len_of_data);
inline int8_t shiftOutData(uint8_t *data_buff, uint32_t len_of_data);

//GAME
//controls
void lightFirstRoom(uint8_t state);
void lightSecondRoom(uint8_t state);
void lockFirstRoomControl(uint8_t state);
void lockSecondRoomControl(uint8_t state);

int8_t getWatchSensorsState(uint8_t watch_number);
int8_t watchGame(void);
int8_t getTouchSensorsState(void);
void setSteppersRandom(void);

//chairs
chairs_state_T getChairState(void);


//RESET
void(* resetFunc) (void) = 0;

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
    // chairs
	for(uint8_t i = 0; i < _NUM_OF_CHAIRS; i++)
	{
		pinMode(chairs_pins[i], INPUT);
	}

    pinMode(LIGHT_1_ROOM_PIN, OUTPUT);
    pinMode(LIGHT_2_ROOM_PIN, OUTPUT);

    // shift registers init
    pinMode(data_pin_in, INPUT_PULLUP);
    pinMode(clock_pin_in, OUTPUT);
    pinMode(latch_pin_in, OUTPUT);

    pinMode(data_pin_out, OUTPUT);
    pinMode(clock_pin_out, OUTPUT);
    pinMode(latch_pin_out, OUTPUT);

    #ifdef output_enable
        pinMode(output_enable, OUTPUT);
        outputEnable();
    #endif


    #ifdef DBG
        Serial.println("[DBG] done!");
    #endif
}//hardware_init()
//------------------------------------------------------------------------------------------------------
inline int8_t shiftInData(uint8_t *data_buff, uint32_t len_of_data)
{
    if(data_buff == nullptr)
    {
        return -1;  
    }
    else if(len_of_data == 0)
    {
        return -2;  
    }

    // write data from parallel inputs
    digitalWrite(latch_pin_in, LOW);
    digitalWrite(latch_pin_in, HIGH);

    // serial read data from 74HC165
    for(uint8_t i = 0; i < len_of_data; i++)
    {
        data_buff[i] = 0;
        for(int8_t j = 7; j >=0 ; j--)
        {
            digitalWrite(clock_pin_in, LOW);
            data_buff[i] |= (digitalRead(data_pin_in)) ? (0 << j) : (1 << j);
            digitalWrite(clock_pin_in, HIGH);
        }//for_inner
    }//for

    return 0;
}//shiftInData()
//------------------------------------------------------------------------------------------------------
inline int8_t shiftOutData(uint8_t *data_buff, uint32_t len_of_data)
{
    if(data_buff == nullptr)
    {
        return -1;  
    }
    else if(len_of_data == 0)
    {
        return -2;  
    }

    digitalWrite(latch_pin_out, LOW);

    for(int8_t i = len_of_data - 1; i >= 0; i--)
    {
        shiftOut(data_pin_out, clock_pin_out, MSBFIRST, data_buff[i]);
    }
    digitalWrite(latch_pin_out, HIGH);

    return 0;
}//shiftOutData()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
void lightFirstRoom(uint8_t state)
{
    digitalWrite(LIGHT_1_ROOM_PIN, state);
}//lightFirstRoom()
//------------------------------------------------------------------------------------------------------
void lightSecondRoom(uint8_t state)
{
    digitalWrite(LIGHT_2_ROOM_PIN, state);
}//lightSecondRoom()
//------------------------------------------------------------------------------------------------------
void lockFirstRoomControl(uint8_t state)
{
    switch(state)
    {
        case CLOSED:
            shift_output_buff[0] |= (1 << DOR_1_LOCK_OFFSET);   
            break;

        case OPEN:
            shift_output_buff[0] &= ~(1 << DOR_1_LOCK_OFFSET);
            break;
    }
    shiftOutData(shift_output_buff, NUM_OUTPUT_REGS);
}//lockInControl()
//------------------------------------------------------------------------------------------------------
void lockSecondRoomControl(uint8_t state)
{
    switch(state)
    {
        case CLOSED:
            shift_output_buff[0] |= (1 << DOR_2_LOCK_OFFSET);   
            break;

        case OPEN:
            shift_output_buff[0] &= ~(1 << DOR_2_LOCK_OFFSET);
            break;
    }
    shiftOutData(shift_output_buff, NUM_OUTPUT_REGS);
}//lockSecondRoomControl()
//------------------------------------------------------------------------------------------------------
int8_t getWatchSensorsState(uint8_t watch_number = ALL_WATCHES)
{
	shiftInData(shift_input_buff, 1);

	if(watch_number == ALL_WATCHES)
	{
		if((shift_input_buff[0] & 0x1F) == 0x1F)
		{
			return ACTIVE;
		}
		else
		{
			return NOT_ACTIVE;
		}
	}

	if((shift_input_buff[0] >> watch_number) & 0x01)
	{
 		return ACTIVE;
	}
	else
	{
		return NOT_ACTIVE;
	}
	
}//getWatchSensorsState()
//------------------------------------------------------------------------------------------------------
int8_t watchGame(void)
{
	setSteps(ALL, CONTINUOUS_FORWARD);
	moveStepper(STEPPER_1);					//all watch plaged to same _dir & _step pins
	
	if(getWatchSensorsState(WATCH_1) == ACTIVE)
		stepperPermission(STEPPER_1, DISABLE);
	if(getWatchSensorsState(WATCH_2) == ACTIVE)
		stepperPermission(STEPPER_2, DISABLE);
	if(getWatchSensorsState(WATCH_3) == ACTIVE)
		stepperPermission(STEPPER_3, DISABLE);
	if(getWatchSensorsState(WATCH_4) == ACTIVE)
		stepperPermission(STEPPER_4, DISABLE);
	if(getWatchSensorsState(WATCH_5) == ACTIVE)
		stepperPermission(STEPPER_5, DISABLE);

	if(getWatchSensorsState() == ACTIVE)	//all watch stoped
	{
		return _WIN;
	}
	else
	{
		return _NOT_WIN;
	}
	
}//watchGame()
//------------------------------------------------------------------------------------------------------
int8_t getTouchSensorsState(void)
{
	shiftInData(shift_input_buff, 1);

	if( ((shift_input_buff[0] >> WATCH_TOUCH_SENSOR_OFFSET_1) & 0x01) 
	 && ((shift_input_buff[0] >> WATCH_TOUCH_SENSOR_OFFSET_2) & 0x01) )
	{
		return ACTIVE;
	}
	else
	{
		return NOT_ACTIVE;
	}
	
}//getWatchTouchSensorsState()
//------------------------------------------------------------------------------------------------------
void setSteppersRandom(void)
{
	stepperPermission(ALL, ENABLE);
	setSteps(ALL, CONTINUOUS_FORWARD);
	for(uint8_t i = 0; i < 255; i++)
	{
		moveStepper(STEPPER_1);
	}
	stepperPermission(ALL, DISABLE);
	stepperPermission(STEPPER_2, ENABLE);
	setSteps(STEPPER_2, 450);
	do
	{
		;
	} while(moveStepper(STEPPER_2) != STOPED);
	stepperPermission(ALL, ENABLE);
}//setSteppersRandom()
//------------------------------------------------------------------------------------------------------
chairs_state_T getChairState(void)
{
	static chairs_state_T _prew_chair;
	chairs_state_T _current_chair;

	uint8_t _pins_state;
	uint8_t _pins_is_up;
	_pins_state = 0;
	_pins_is_up = 0;	//counter of active chairs
	for(uint8_t i = 0; i < _NUM_OF_CHAIRS; i++)
	{
		if(digitalRead(chairs_pins[i]))
		{
			_pins_state |= (1 << i);
			_pins_is_up++;
		}
		else
		{
			_pins_state &= ~(1 << i);
		}
	}//for

	//errors
	if(_pins_is_up > 1)	//more then one chair is active
	{
		_current_chair.chair_number = _ERR;
		_current_chair.chair_state = PRESSED;
		return _prew_chair = _current_chair;
	}
	//if wrong letter is pressed
	//all wrong letters (chairs) is up then 4 bit of _pins_state
	if((_pins_state >> 4) > 0)	
	{
		_current_chair.chair_number = _ERR;
		_current_chair.chair_state = PRESSED;
		return _prew_chair = _current_chair;
	}

	//return current chair state
	if((_pins_state & 0x0F) == 0x00)
	{
		_current_chair.chair_number = _ALL_RELEASED;
		_current_chair.chair_state = RELEASED;
		return _prew_chair = _current_chair;
	}//if all sensor not active
	else if(_prew_chair.chair_state == RELEASED)
	{
		_current_chair.chair_state = PRESSED;
		for (uint8_t i = 0; i < 4; i++)
		{
			if((_pins_state >> i) & 0x01)
			{
				_current_chair.chair_number = i;
				return _prew_chair = _current_chair;
			}
		}//for
	}//if first time pressed
	else if(_prew_chair.chair_state == PRESSED || _prew_chair.chair_state == HOLD)
	{
		_current_chair.chair_state = HOLD;
		for (uint8_t i = 0; i < 4; i++)
		{
			if((_pins_state >> i) & 0x01)
			{
				_current_chair.chair_number = i;
				return _prew_chair = _current_chair;
			}
		}//for
	}//if sensor still pressed

	//only if function dont work correct
	_current_chair.chair_number = _ERR;
	_current_chair.chair_state = RELEASED;
	return _prew_chair = _current_chair;
}//getChairState()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------

#endif //FUNCTIONS_INCLUDED