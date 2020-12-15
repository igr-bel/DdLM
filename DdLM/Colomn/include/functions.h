#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>
#include <HCSR04.h>

#include "mqtt_cmds.h"

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

//GPIO
#define LASER_PIN_1						27
#define LASER_PIN_2						26
#define LASER_PIN_3						25
#define LASER_PIN_4						24
#define LASER_PIN_5						23
#define LASER_PIN_6						22
#define LASER_PIN_7						32
#define LASER_POWER_PIN					33
#define LASER_POWER_ON()				digitalWrite(LASER_POWER_PIN, 1)
#define LASER_POWER_OFF()				digitalWrite(LASER_POWER_PIN, 0)
#define concat(a,b)						a ## b

#define SMELLS_BTN_PIN_1				4
#define SMELLS_BTN_PIN_2				10
#define SMELLS_BTN_PIN_3				A12
#define SMELLS_BTN_PIN_4				A13
#define SMELLS_BTN_PIN_ERR				A14
#define SMELLS_BTN_COUNT				5

#define TOUCH_BTN_PIN_1					31
#define TOUCH_BTN_PIN_2					30
#define TOUCH_BTN_PIN_3					29
#define TOUCH_BTN_PIN_ERR				28
#define TOUCH_BTN_COUNT					4

#define WEIGHT_BTN_PIN_1				34
#define WEIGHT_BTN_PIN_2				35
#define WEIGHT_BTN_PIN_ERR				36
#define WEIGHT_BTN_COUNT				3

#define SONAR_TRIG_PIN					37
#define SONAR_ECHO_PIN					38
#define SONAR_ACTIVE_DISTANCE			40	//in centimeters

//GPIO LOADS	
#define WEIGHT_SYMBOL_PIN				8
#define SMELLS_SYMBOL_PIN				7
#define TOUCH_SYMBOL_PIN				6
#define SOUND_SYMBOL_PIN				5
#define SMOKE_POWER_PIN					46
#define SMOKE_BTN_PIN					9

//DF player
#define player_pin_tx1                	2   //TX1
#define player_pin_rx1                	3   //RX1
#ifndef player_pin_rx1
  #define busy_pin_1                    9
#endif

#define VOLUME                          30

//SOUNDS
#define LASER_SOUND_1					1
#define LASER_SOUND_2					2
#define LASER_SOUND_3					3
#define LASER_SOUND_4					4
#define LASER_SOUND_5					5
#define LASER_SOUND_6					6
#define LASER_SOUND_7					7
#define HARP_SOUND_HINT				8

#define HARP_MELODY_LENGTH				7
#define HARP_TIME_ERROR					5000
#define HARP_HINT_INTERVAL				3000
#define SONAR_INTERVAL					1000

//------------------------------------------------------------------------------------------------------
//Class for DF player
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};
//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------
#ifdef player_pin_rx1
  SoftwareSerial mp3_Serial(player_pin_rx1, player_pin_tx1); // RX, TX
#else
  SoftwareSerial mp3_Serial(99, player_pin_tx1); // RX, TX
#endif
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mp3_Serial);

HCSR04 sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN);

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
//loads
enum
{
	SMOKE_POWER,
	SMOKE_BTN,
	SYMBOL_WEIGHT,	
	SYMBOL_SMELLS,
	SYMBOL_TOUCH,
	SYMBOL_SOUND
};
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
    OK,
    ERR
};

enum
{
	LASER_1,
	LASER_2,
	LASER_3,
	LASER_4,
	LASER_5,
	LASER_6,
	LASER_7,
	NUM_OF_LASERS,
	NON_ACTIVE
};

enum
{
	RELEASED,
	PRESSED,
	HOLD
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

//data for UI
bool mqtt_data_is_ready = false;
int mqtt_data;

//laser harp
uint8_t laser_pin[NUM_OF_LASERS];
uint8_t laser_sounds[NUM_OF_LASERS];
uint8_t harp_correct_sequence[HARP_MELODY_LENGTH] = {LASER_4, LASER_6, LASER_3, LASER_1, LASER_2, LASER_5, LASER_7};
bool harp_error = false;
unsigned long harp_start_timer;
static uint8_t harp_melody_counter;
uint8_t prew_harp_state;
unsigned long harp_hint_timer;
unsigned long sonar_timer;

//smells
uint8_t smells_pin[SMELLS_BTN_COUNT] = {SMELLS_BTN_PIN_1, SMELLS_BTN_PIN_2, SMELLS_BTN_PIN_3, SMELLS_BTN_PIN_4, SMELLS_BTN_PIN_ERR};

//touch
uint8_t touch_pin[TOUCH_BTN_COUNT] = {TOUCH_BTN_PIN_1, TOUCH_BTN_PIN_2, TOUCH_BTN_PIN_3, TOUCH_BTN_PIN_ERR};

//weight
uint8_t weight_pin[WEIGHT_BTN_COUNT] = {WEIGHT_BTN_PIN_1, WEIGHT_BTN_PIN_2, WEIGHT_BTN_PIN_ERR};

//------------------------------------------------------------------------------------------------------
//                                        FLAGS
//------------------------------------------------------------------------------------------------------
bool harp_win_flag = false;
bool smells_win_flag = false;
bool touch_win_flag = false;
bool weight_win_flag = false;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//HARDWARE
inline void hardware_init(void);

//LOADS
void loadControl(uint8_t load, uint8_t state);

//MP3
inline void playSound(uint8_t track);
inline void stopSound(void);
inline void mp3_init(void);

//GAME
//laser harp
int8_t laserHarpGame(void);
uint8_t getLaserState(void);

uint8_t checkDistance(void);

//smells
btn_state_T getSmellsBtnState(void);	
uint8_t smellsGame(void);

//touch
btn_state_T getTouchBtnState(void);	
uint8_t touchGame(void);

//weight
btn_state_T getWeightBtnState(void);	
uint8_t weightGame(void);

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
	//laser harp
	laser_pin[0] = LASER_PIN_1;
	laser_pin[1] = LASER_PIN_2;
	laser_pin[2] = LASER_PIN_3;
	laser_pin[3] = LASER_PIN_4;
	laser_pin[4] = LASER_PIN_5;
	laser_pin[5] = LASER_PIN_6;
	laser_pin[6] = LASER_PIN_7;
	for(uint8_t i = 0; i < NUM_OF_LASERS; i++)
	{
		pinMode(laser_pin[i], INPUT);
	}
	pinMode(LASER_POWER_PIN, OUTPUT);

	//smells buttons
	for(uint8_t i = 0; i < SMELLS_BTN_COUNT; i++)
	{
		pinMode(smells_pin[i], INPUT);
	}

	//touch buttons
	for(uint8_t i = 0; i < TOUCH_BTN_COUNT; i++)
	{
		pinMode(touch_pin[i], INPUT);
	}

	//sounds
	laser_sounds[0] = LASER_SOUND_1;
	laser_sounds[1] = LASER_SOUND_2;
	laser_sounds[2] = LASER_SOUND_3;
	laser_sounds[3] = LASER_SOUND_4;
	laser_sounds[4] = LASER_SOUND_5;
	laser_sounds[5] = LASER_SOUND_6;
	laser_sounds[6] = LASER_SOUND_7;

	//loads
	pinMode(WEIGHT_SYMBOL_PIN, OUTPUT);			
	pinMode(SMELLS_SYMBOL_PIN, OUTPUT);			
	pinMode(TOUCH_SYMBOL_PIN, OUTPUT);			
	pinMode(SOUND_SYMBOL_PIN, OUTPUT);			
	pinMode(SMOKE_POWER_PIN, OUTPUT);			
	pinMode(SMOKE_BTN_PIN, OUTPUT);			

    #ifdef DBG
        Serial.println("[DBG] done!");
    #endif
}//hardware_init()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
void playSound(uint8_t track)
{
    mp3.stop();
    delay(5);
    mp3.playMp3FolderTrack(track);

    #ifdef DBG
        Serial.print("[DBG] play track - ");
        Serial.println(track); 
    #endif
  
}//playSound()
//------------------------------------------------------------------------------------------------------
void stopSound(void)
{
    mp3.stop();
    delay(5);
}//stopSound()
//------------------------------------------------------------------------------------------------------
void mp3_init(void)
{
    #ifdef busy_pin_1
        pinMode(busy_pin_1, INPUT);
        #define mp3isBusy !digitalRead(busy_pin_1)
    #endif

    mp3_Serial.begin (9600);
    mp3.begin();
    mp3.setVolume(VOLUME);

}//mp3_init()
//------------------------------------------------------------------------------------------------------
int8_t laserHarpGame(void)
{
	uint8_t active_laser;

	active_laser = getLaserState();
	if(active_laser != NON_ACTIVE && prew_harp_state == NON_ACTIVE)
	{
		playSound(laser_sounds[active_laser]);
		harp_start_timer = millis();
		prew_harp_state = active_laser;
		delay(50);
		if(harp_correct_sequence[harp_melody_counter] == active_laser)
		{
			harp_melody_counter++;
			if(harp_melody_counter == HARP_MELODY_LENGTH)
			{
				return OK;
			}

			mqtt_data_is_ready = true;
			mqtt_data = HARP_CLEAR | (1 << (harp_melody_counter-1));
		}
		else
		{
			if(active_laser == harp_correct_sequence[0])
			{
				harp_melody_counter = 1;
				mqtt_data_is_ready = true;
				mqtt_data = HARP_CLEAR | (1 << (harp_melody_counter-1));
			}
			else
			{
				harp_melody_counter = 0;
				mqtt_data_is_ready = true;
				mqtt_data = HARP_CLEAR;
			}
			return ERR;
		}
	}//if laser is active

	if((millis() - harp_start_timer) > HARP_TIME_ERROR)
	{
		harp_melody_counter = 0;
	}//if over time to wait next note

	prew_harp_state = active_laser;
	return ERR;
}//laserHarpGame()
//------------------------------------------------------------------------------------------------------
uint8_t getLaserState(void)
{
	for(uint8_t i = 0; i < NUM_OF_LASERS; i++)
	{
		if(!digitalRead(laser_pin[i]))
		{
			return i;
		}
	}

	return NON_ACTIVE;
}//getLaserState()
//------------------------------------------------------------------------------------------------------
btn_state_T getSmellsBtnState(void)
{
	uint8_t _pins_data;				//all button pins [x x x 4 3 2 1 0]
	uint8_t _btns_pressed;			//counter of pressed buttons
	static btn_state_T _prew_btn;
	btn_state_T _current_btn;
	_current_btn = {_ALL_BTNS, RELEASED};
	_pins_data = 0x00;
	_btns_pressed = 0;

	for(uint8_t i = 0; i < SMELLS_BTN_COUNT; i++)
	{
		if(digitalRead(smells_pin[i]))
		{
			_pins_data |= (1 << i);
			_btns_pressed++;
		}
		else
		{
			_pins_data &= ~(1 << i);
		}
	}//[for] read all buttons

	/* 
	* ---- error processing
	*/
	//more then one button is pressed ||
	//if wrong button pressed
	//( all wrong buttons pinned to SMELLS_BTN_PIN_ERR )
	if(_btns_pressed > 1 || ((_pins_data >> 4) > 0))	
	{
		_current_btn.btn_number = _BTN_ERR;
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]
		return _prew_btn = _current_btn;
	}

	/*
	*  --- return current pressed button
	*/
	if((_pins_data & 0x0F) == 0x00)
	{
		_current_btn.btn_number = _ALL_BTNS;
		_current_btn.btn_state = RELEASED;
		return _prew_btn = _current_btn;
	}//[if] all buttons not active
	else
	{
		//check the changed state of the button
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]

		//calculate and set number of the current pressed button
		for(uint8_t i = 0; i < 4; i++)
		{
			if((_pins_data >> i) & 0x01)
			{
				_current_btn.btn_number = i;
				return _prew_btn = _current_btn;
			}
		}//[for]
	}//[else]

	//this code doesn't work if function complete correctly
	_current_btn.btn_number = _BTN_READ_ERROR;
	_prew_btn = _current_btn;
	return _current_btn;
}//getSmellsBtnState()
//------------------------------------------------------------------------------------------------------
uint8_t smellsGame(void)
{
	static uint8_t _btn_counter = 0;
	btn_state_T _current_btn;

	_current_btn = getSmellsBtnState();
	if(_current_btn.btn_state == PRESSED)
	{
		if(_current_btn.btn_number == _BTN_ERR)
		{
			_btn_counter = 0;
			mqtt_data_is_ready = true;
			mqtt_data = SMELLS_CLEAR;
			return ERR;
		}

		if(_current_btn.btn_number == _btn_counter)
		{
			_btn_counter++;
			if(_btn_counter == 4)
			{
				_btn_counter = 0;
				return OK;
			}//[if] win

			mqtt_data_is_ready = true;
			mqtt_data = SMELLS_CLEAR | (1 << (_btn_counter-1));
			return ERR;
		}
		else
		{
			if(_current_btn.btn_number == _BTN_1)
			{
				_btn_counter = 1;
				mqtt_data = SMELLS_CLEAR | (1 << (_btn_counter-1));
			}
			else
			{
				_btn_counter = 0;
				mqtt_data = SMELLS_CLEAR;
			}
			mqtt_data_is_ready = true;
			return ERR;
		}
	}//[if] btn pressed

	return ERR;
}//smellsGame()
//------------------------------------------------------------------------------------------------------
btn_state_T getTouchBtnState(void)
{
	uint8_t _pins_data;				//all button pins [x x x x 3 2 1 0]
	uint8_t _btns_pressed;			//counter of pressed buttons
	static btn_state_T _prew_btn;
	btn_state_T _current_btn;
	_current_btn = {_ALL_BTNS, RELEASED};
	_pins_data = 0x00;
	_btns_pressed = 0;

	for(uint8_t i = 0; i < TOUCH_BTN_COUNT; i++)
	{
		if(digitalRead(touch_pin[i]))
		{
			_pins_data |= (1 << i);
			_btns_pressed++;
		}
		else
		{
			_pins_data &= ~(1 << i);
		}
	}//[for] read all buttons

	/* 
	* ---- error processing
	*/
	//more then one button is pressed ||
	//if wrong button pressed
	//( all wrong buttons pinned to SMELLS_BTN_PIN_ERR )
	if(_btns_pressed > 1 || ((_pins_data >> (TOUCH_BTN_COUNT-1)) > 0))	
	{
		_current_btn.btn_number = _BTN_ERR;
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]
		return _prew_btn = _current_btn;
	}

	/*
	*  --- return current pressed button
	*/
	if((_pins_data & 0x07) == 0x00)
	{
		_current_btn.btn_number = _ALL_BTNS;
		_current_btn.btn_state = RELEASED;
		return _prew_btn = _current_btn;
	}//[if] all buttons not active
	else
	{
		//check the changed state of the button
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]

		//calculate and set number of the current pressed button
		for(uint8_t i = 0; i < (TOUCH_BTN_COUNT-1); i++)
		{
			if((_pins_data >> i) & 0x01)
			{
				_current_btn.btn_number = i;
				return _prew_btn = _current_btn;
			}
		}//[for]
	}//[else]

	//this code doesn't work if function complete correctly
	_current_btn.btn_number = _BTN_READ_ERROR;
	_prew_btn = _current_btn;
	return _current_btn;
}//getTouchBtnState()
//------------------------------------------------------------------------------------------------------
uint8_t touchGame(void)
{
	static uint8_t _btn_counter = 0;
	btn_state_T _current_btn;

	_current_btn = getTouchBtnState();
	if(_current_btn.btn_state == PRESSED)
	{
		if(_current_btn.btn_number == _BTN_ERR)
		{
			_btn_counter = 0;
			mqtt_data_is_ready = true;
			mqtt_data = TOUCH_CLEAR;
			return ERR;
		}

		if(_current_btn.btn_number == _btn_counter)
		{
			_btn_counter++;
			if(_btn_counter == (TOUCH_BTN_COUNT - 1))
			{
				_btn_counter = 0;
				return OK;
			}//[if] win

			mqtt_data_is_ready = true;
			mqtt_data = TOUCH_CLEAR | (1 << (_btn_counter-1));
			return ERR;
		}
		else
		{
			if(_current_btn.btn_number == _BTN_1)
			{
				_btn_counter = 1;
				mqtt_data = TOUCH_CLEAR | (1 << (_btn_counter-1));
			}
			else
			{
				_btn_counter = 0;
				mqtt_data = TOUCH_CLEAR;
			}
			mqtt_data_is_ready = true;
			return ERR;
		}
	}//[if] btn pressed

	return ERR;
}//touchGame()
//------------------------------------------------------------------------------------------------------
btn_state_T getWeightBtnState(void)	
{
	uint8_t _pins_data;				//all button pins [x x x x x 2 1 0]
	uint8_t _btns_pressed;			//counter of pressed buttons
	static btn_state_T _prew_btn;
	btn_state_T _current_btn;
	_current_btn = {_ALL_BTNS, RELEASED};
	_pins_data = 0x00;
	_btns_pressed = 0;

	for(uint8_t i = 0; i < WEIGHT_BTN_COUNT; i++)
	{
		if(digitalRead(weight_pin[i]))
		{
			_pins_data |= (1 << i);
			_btns_pressed++;
		}
		else
		{
			_pins_data &= ~(1 << i);
		}
	}//[for] read all buttons

	/* 
	* ---- error processing
	*/
	//more then one button is pressed ||
	//if wrong button pressed
	//( all wrong buttons pinned to SMELLS_BTN_PIN_ERR )
	if(_btns_pressed > 1 || ((_pins_data >> (WEIGHT_BTN_COUNT-1)) > 0))	
	{
		_current_btn.btn_number = _BTN_ERR;
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]
		return _prew_btn = _current_btn;
	}

	/*
	*  --- return current pressed button
	*/
	if((_pins_data & 0x07) == 0x00)
	{
		_current_btn.btn_number = _ALL_BTNS;
		_current_btn.btn_state = RELEASED;
		return _prew_btn = _current_btn;
	}//[if] all buttons not active
	else
	{
		//check the changed state of the button
		switch (_prew_btn.btn_state)
		{
			case RELEASED:
				_current_btn.btn_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:
				_current_btn.btn_state = HOLD;
				break;

			default:
				_current_btn.btn_number = _BTN_READ_ERROR;
				break;
		}//[switch]

		//calculate and set number of the current pressed button
		for(uint8_t i = 0; i < (WEIGHT_BTN_COUNT-1); i++)
		{
			if((_pins_data >> i) & 0x01)
			{
				_current_btn.btn_number = i;
				return _prew_btn = _current_btn;
			}
		}//[for]
	}//[else]

	//this code doesn't work if function complete correctly
	_current_btn.btn_number = _BTN_READ_ERROR;
	_prew_btn = _current_btn;
	return _current_btn;
}//getWeightBtnState()
//------------------------------------------------------------------------------------------------------
uint8_t weightGame(void)
{
	static uint8_t _btn_counter = 0;
	btn_state_T _current_btn;

	_current_btn = getWeightBtnState();
	if(_current_btn.btn_state == PRESSED)
	{
		if(_current_btn.btn_number == _BTN_ERR)
		{
			_btn_counter = 0;
			mqtt_data_is_ready = true;
			mqtt_data = WEIGHT_CLEAR;
			return ERR;
		}

		if(_current_btn.btn_number == _btn_counter)
		{
			_btn_counter++;
			if(_btn_counter == (WEIGHT_BTN_COUNT - 1))
			{
				_btn_counter = 0;
				return OK;
			}//[if] win

			mqtt_data_is_ready = true;
			mqtt_data = WEIGHT_CLEAR | (1 << (_btn_counter-1));
			return ERR;
		}
		else
		{
			if(_current_btn.btn_number == _BTN_1)
			{
				_btn_counter = 1;
				mqtt_data = WEIGHT_CLEAR | (1 << (_btn_counter-1));
			}
			else
			{
				_btn_counter = 0;
				mqtt_data = WEIGHT_CLEAR;
			}
			mqtt_data_is_ready = true;
			return ERR;
		}
	}//[if] btn pressed

	return ERR;
}//weightGame()
//------------------------------------------------------------------------------------------------------
void loadControl(uint8_t load, uint8_t state)
{
	switch (load)
	{
	case SMOKE_POWER:
		digitalWrite(SMOKE_POWER_PIN, state);
		break;

	case SMOKE_BTN:
		digitalWrite(SMOKE_BTN_PIN, state);
		break;

	case SYMBOL_WEIGHT:
		digitalWrite(WEIGHT_SYMBOL_PIN, state);
		break;
		
	case SYMBOL_SMELLS:
		digitalWrite(SMELLS_SYMBOL_PIN, state);
		break;
		
	case SYMBOL_TOUCH:
		digitalWrite(TOUCH_SYMBOL_PIN, state);
		break;
		
	case SYMBOL_SOUND:
		digitalWrite(SOUND_SYMBOL_PIN, state);
		break;
	}//[switch]

}//loadControl()
//------------------------------------------------------------------------------------------------------
uint8_t checkDistance(void)
{
	float _dist;

	_dist = sonar.dist();
	if(_dist <= SONAR_ACTIVE_DISTANCE)
	{
		return OK;
	}
	else
	{
		return ERR;
	}
	
}//getDistance()
//------------------------------------------------------------------------------------------------------

#endif //FUNCTIONS_INCLUDED