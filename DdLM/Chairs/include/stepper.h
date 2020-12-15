#ifndef _STEPPER_H
#define _STEPPER_H

//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include <Arduino.h>


//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
#define HIGH_STEP				1200	//default 1200
#define LOW_STEP				800		//default 800

#define STOPED					0
#define IN_MOVE					1
#define MOVE_FORWARD			2
#define MOVE_BACKWARD			-1
#define STEPPER_MOVE_ERROR		-99

#define ENABLE					1
#define DISABLE					0

#define ALL						255

#define CONTINUOUS_FORWARD		2000000000
#define CONTINUOUS_BACKWARD		-2000000000

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
enum
{
	STEPPER_1,	
	STEPPER_2,	
	STEPPER_3,	
	STEPPER_4,	
	STEPPER_5,	
	STEPPER_6,	
	STEPPER_7,	
	STEPPER_8,	
	STEPPER_9,	
	STEPPER_10,

	_num_of_steppers	
};

static int32_t _steps[_num_of_steppers];
static int8_t _dir[_num_of_steppers];

static uint8_t _step_pin[_num_of_steppers] = {99};
static uint8_t _dir_pin[_num_of_steppers] = {99};
static uint8_t _en_pin[_num_of_steppers] = {99};

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
void stepForward(uint8_t stepper);
void stepBackward(uint8_t stepper);
void setSteps(uint8_t stepper, int32_t steps);
int8_t moveAllSteppers(uint8_t stepper);
int8_t moveStepper(uint8_t stepper);
void stepperPermission(uint8_t stepper, uint8_t permission);
void stepperInit(uint8_t stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin);

//------------------------------------------------------------------------------------------------------
//                                        BEHAVIOR
//------------------------------------------------------------------------------------------------------
void stepForward(uint8_t stepper)
{
	digitalWrite(_dir_pin[stepper], 1);

	digitalWrite(_step_pin[stepper], 1);
	delayMicroseconds(HIGH_STEP);
	digitalWrite(_step_pin[stepper], 0);
	delayMicroseconds(LOW_STEP);

}//stepForward()
//------------------------------------------------------------------------------------------------------
void stepBackward(uint8_t stepper)
{
	digitalWrite(_dir_pin[stepper], 0);

	digitalWrite(_step_pin[stepper], 1);
	delayMicroseconds(HIGH_STEP);
	digitalWrite(_step_pin[stepper], 0);
	delayMicroseconds(LOW_STEP);

}//stepBackward()
//------------------------------------------------------------------------------------------------------
void setSteps(uint8_t stepper, int32_t steps)
{
	if(stepper == ALL)
	{
		for(uint8_t i = 0 ; i < _num_of_steppers; i++)
		{
			_steps[i] = steps;
			_dir[i] = (steps >= 0) ? 1 : -1;		
		}		
	}
	else
	{
		_steps[stepper] = steps;
		_dir[stepper] = (steps >= 0) ? 1 : -1;	
	}

}//moveStep()
//------------------------------------------------------------------------------------------------------
int8_t moveAllSteppers(void)
{
	bool _action;

	_action = false;
	for(uint8_t i = 0; i < _num_of_steppers; i++)
	{
		if(_steps[i] == 0)
		{
			// stepperPermission(i, DISABLE);
			continue;
		}//if
		
		switch(_dir[i])
		{
			case 1:
				// stepperPermission(i, ENABLE);
				stepForward(i);
				_steps[i]--;
				_action = true;
				// stepperPermission(i, DISABLE);
				break;

			case -1:
				// stepperPermission(i, ENABLE);
				stepBackward(i);
				_steps[i]++;
				_action = true;
				// stepperPermission(i, DISABLE);
				break;
		}//switch _dir
	}//for

	if(_action)
		return IN_MOVE;
	else
		return STOPED;
	
	return STEPPER_MOVE_ERROR;
}//moveStepper()
//------------------------------------------------------------------------------------------------------
int8_t moveStepper(uint8_t stepper)
{
	if(_steps[stepper] == 0)
	{
		stepperPermission(stepper, DISABLE);
		return STOPED;
	}//if

	// stepperPermission(stepper, ENABLE);
	switch(_dir[stepper])
	{
		case 1:
			stepForward(stepper);
			_steps[stepper] -= 1;
			// stepperPermission(stepper, DISABLE);
			return MOVE_FORWARD;
			break;

		case -1:
			stepBackward(stepper);
			_steps[stepper] += 1;
			// stepperPermission(stepper, DISABLE);
			return MOVE_BACKWARD;
			break;
	}//switch _dir

	return STEPPER_MOVE_ERROR;
}//moveStepper()
//------------------------------------------------------------------------------------------------------
void stepperPermission(uint8_t stepper, uint8_t permission)
{
	if(stepper == ALL)
	{
		for(uint8_t i = 0 ; i < _num_of_steppers; i++)
		{
			(permission == ENABLE) ? digitalWrite(_en_pin[i], LOW) : digitalWrite(_en_pin[i], HIGH);	
		}
	}
	else
	{
		(permission == ENABLE) ? digitalWrite(_en_pin[stepper], LOW) : digitalWrite(_en_pin[stepper], HIGH);	
	}
	
}//stepperPermission()
//------------------------------------------------------------------------------------------------------
void stepperInit(uint8_t stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin)
{
	_dir_pin[stepper] = dir_pin;
	_step_pin[stepper] = step_pin;
	_en_pin[stepper] = en_pin;

	pinMode(_dir_pin[stepper], OUTPUT);
	pinMode(_step_pin[stepper], OUTPUT);
	pinMode(_en_pin[stepper], OUTPUT);

}//stepperInit()
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------


#endif //_STEPPER_H