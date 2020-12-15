#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include <PCF8574.h>
#include <Wire.h>

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
#define DBG

//GPIO
#define ERROR_LETTER_PIN				3

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------
//		 7 6 5 4 3 2 1 0
//		+---------------+
// pcf1 |A|D|E|G|H|I|J|L|
//		+---------------+
// pcf2 |C|U|R|Q|P|O|N|M|	
//		+---------------+
//
PCF8574 pcf1(0x21);
PCF8574 pcf2(0x22);

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
enum
{
    EN = 1,
    FR
};

uint8_t hint_lang = EN;

enum
{
	_A,
	_C,	
	_D,
	_E,
	_G,
	_H,
	_I,
	_J,
	_L,
	_U,
	_R,	
	_Q,	
	_P,	
	_O,	
	_N,	
	_M,	
	_ERR,
	_NONE
};

enum
{
	NOT_PRESSED, //for function error control
	PRESSED,			
	HOLD,				
	RELEASED			
};

struct letter_T 
{
	uint8_t letter_code;
	uint8_t letter_state;
};

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//HARDWARE
inline void hardware_init(void);

//RESET
void(* resetFunc) (void) = 0;

letter_T getCurrentLetter(void);
uint8_t getRightLetter(void);

//------------------------------------------------------------------------------------------------------
//                                       BEHAVIOR
//------------------------------------------------------------------------------------------------------
void hardware_init(void)
{
    #ifdef DBG
        Serial.begin(9600);
        Serial.println("___DBG___");
        Serial.println("[DBG] start init ... ");
    #endif

    //------------------------------------------------------ GPIO
	pinMode(ERROR_LETTER_PIN, INPUT_PULLUP);

    //------------------------------------------------------ I2C SHIFT IN
	pcf1.begin();
	pcf2.begin();

    #ifdef DBG
        Serial.println("[DBG] done!");
    #endif
}//hardware_init()
//------------------------------------------------------------------------------------------------------
letter_T getCurrentLetter(void)
{
	static letter_T _prew_letter = {_NONE, RELEASED};
	letter_T _current_letter = {_NONE, RELEASED};

	//if error letter is active
	if(!digitalRead(ERROR_LETTER_PIN))
	{
		_current_letter.letter_code = _ERR;
		
		switch(_prew_letter.letter_state)
		{
			case RELEASED:
				_current_letter.letter_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:		
				_current_letter.letter_state = HOLD;
				break;
		}//switch
		
		_prew_letter = _current_letter;
		return _current_letter;
	}
	else if(_prew_letter.letter_code == _ERR)
	{
		if(_prew_letter.letter_state == PRESSED 
			|| _prew_letter.letter_state == HOLD)
		{
			_current_letter.letter_state = RELEASED;
			_current_letter.letter_code = _prew_letter.letter_code;
			_prew_letter = _current_letter;
			return _current_letter;
		}
		else
		{
			_current_letter.letter_state = RELEASED;
			_current_letter.letter_code = _NONE;
			_prew_letter = _current_letter;
			return _current_letter;	
		}	
	}

	//check right letters
	_current_letter.letter_code = getRightLetter();
	if(_current_letter.letter_code != _NONE)
	{
		switch(_prew_letter.letter_state)
		{
			case RELEASED:
				_current_letter.letter_state = PRESSED;
				break;

			case PRESSED:
			case HOLD:		
				_current_letter.letter_state = HOLD;
				break;
		}//switch
		
		_prew_letter = _current_letter;
		return _current_letter;
	}
	else if(_prew_letter.letter_code != _NONE)
	{
		if(_prew_letter.letter_state == PRESSED 
			|| _prew_letter.letter_state == HOLD)
		{
			_current_letter.letter_state = RELEASED;
			_current_letter.letter_code = _prew_letter.letter_code;
			_prew_letter = _current_letter;
			return _current_letter;
		}
		else
		{
			_current_letter.letter_state = RELEASED;
			_current_letter.letter_code = _NONE;
			_prew_letter = _current_letter;
			return _current_letter;	
		}	
	}
	else
	{
		_current_letter.letter_state = RELEASED;
		_current_letter.letter_code = _NONE;
		_prew_letter = _current_letter;
		return _current_letter;
	}
	
	//only if function does not work correctly
	_current_letter.letter_code = _NONE;
	_current_letter.letter_state = NOT_PRESSED;
	return _current_letter;
}//getCurrentLetter()
//------------------------------------------------------------------------------------------------------
uint8_t getRightLetter(void)
{
	uint8_t _buff_1, _buff_2;

	_buff_1 = ~(pcf1.read8());
	_buff_2 = ~(pcf2.read8());

	switch(_buff_1)
	{
		case 0x80:
			return _A;
			break;

		case 0x40:
			return _D;
			break;

		case 0x20:
			return _E;
			break;

		case 0x10:
			return _G;
			break;

		case 0x08:
			return _H;
			break;

		case 0x04:
			return _I;
			break;

		case 0x02:
			return _J;
			break;

		case 0x01:
			return _L;
			break;
	}//switch pcf1

	switch(_buff_2)
	{
		case 0x80:
			return _C;
			break;

		case 0x40:
			return _U;
			break;

		case 0x20:
			return _R;
			break;

		case 0x10:
			return _Q;
			break;

		case 0x08:
			return _P;
			break;

		case 0x04:
			return _O;
			break;

		case 0x02:
			return _N;
			break;

		case 0x01:
			return _M;
			break;
	}//switch pcf2

	return _NONE;
}//getRightLetterState()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
#endif //_FUNCTIONS_H