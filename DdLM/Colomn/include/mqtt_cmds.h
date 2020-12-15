#ifndef _MQTT_CMDS
#define _MQTT_CMDS

//------------------------------------------------------------------------------------------------------
//                                        OUT COMMANDS
//------------------------------------------------------------------------------------------------------
//stuff     
#define RESET_WEB_UI                    0x200
#define C_PING_RESP                     0x30D

//game
#define HARP_CLEAR						0x400
#define HARP_WIN						0x44F
#define HARP_LASER_1					0x401
#define HARP_LASER_2					0x402
#define HARP_LASER_3					0x404
#define HARP_LASER_4					0x408
#define HARP_LASER_5					0x410
#define HARP_LASER_6					0x420
#define HARP_LASER_7					0x440

#define SMELLS_WIN						0x48F
#define SMELLS_CLEAR					0x480

#define TOUCH_WIN						0x49F
#define TOUCH_CLEAR						0x490

#define WEIGHT_WIN						0x4AF
#define WEIGHT_CLEAR					0x4A0

#define FOG_ENABLE						0x335
#define FOG_DISABLE						0x336

//------------------------------------------------------------------------------------------------------
//                                        IN COMMANDS
//------------------------------------------------------------------------------------------------------
//stuff       
#define SLEEP_CMD                       0x309
#define RESTART_CMD                     0x302
#define RESET_CMD                       0x301
#define START_CMD                       0x306

#define S_PING_REQ                      0x30A

#define EN_SOUNDS                       0x1011
#define FR_SOUNDS                       0x1012

#define LASER_HARP_ON					0x32B
#define LASER_HARP_OFF					0x32C

//loads
#define SMOKE_POWER_ON					0x321
#define SMOKE_POWER_OFF					0x322
#define SMOKE_BTN_ON					0x32D
#define SMOKE_BTN_OFF					0x32E
#define SYMBOL_WEIGHT_ON				0x323
#define SYMBOL_WEIGHT_OFF				0x324
#define SYMBOL_SMELLS_ON				0x325
#define SYMBOL_SMELLS_OFF				0x326
#define SYMBOL_TOUCH_ON					0x327
#define SYMBOL_TOUCH_OFF				0x328
#define SYMBOL_SOUND_ON					0x329
#define SYMBOL_SOUND_OFF				0x32A

//game
#define SOLVE_HARP						0x331
#define SOLVE_SMELLS					0x332
#define SOLVE_TOUCH						0x333
#define SOLVE_WEIGHT					0x334

//------------------------------------------------------------------------------------------------------
#endif  //_MQTT_CMDS