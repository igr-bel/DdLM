#ifndef _MQTT_CMDS
#define _MQTT_CMDS

//------------------------------------------------------------------------------------------------------
//                                        OUT COMMANDS
//------------------------------------------------------------------------------------------------------
//stuff     
#define RESET_WEB_UI                    0x200
#define C_PING_RESP                     0x30D

//game



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

//loads
#define LAMP_1_ON						0x321
#define LAMP_1_OFF						0x322
#define LAMP_2_ON						0x323
#define LAMP_2_OFF						0x324
#define LAMP_3_ON						0x325
#define LAMP_3_OFF						0x326
#define LAMP_4_ON						0x327
#define LAMP_4_OFF						0x328
#define LAMP_5_ON						0x329
#define LAMP_5_OFF						0x32A

#define HOLE_LIGHT_ON					0x32B
#define HOLE_LIGHT_OFF					0x32C

#define FIRST_DOOR_LOCK					0x32D
#define FIRST_DOOR_UNLOCK				0x32E
#define SECOND_DOOR_LOCK				0x331
#define SECOND_DOOR_UNLOCK				0x332
#define THIRD_DOOR_LOCK					0x333
#define THIRD_DOOR_UNLOCK				0x334
#define OUT_DOOR_LOCK					0x335
#define OUT_DOOR_UNLOCK					0x336

#define LIGHT_BRIDGE_ON					0x337
#define LIGHT_BRIDGE_OFF				0x338
#define LIGHT_FIRST_ROOM_ON				0x339
#define LIGHT_FIRST_ROOM_OFF			0x33A
#define LIGHT_SECOND_ROOM_ON			0x33B
#define LIGHT_SECOND_ROOM_OFF			0x33C
#define LIGHT_THIRD_ROOM_ON				0x33D
#define LIGHT_THIRD_ROOM_OFF			0x33E
#define BRIDGE_LASER_ON					0x341
#define BRIDGE_LASER_OFF				0x342

//game
#define LASER_CLEAR						0x600
#define LASER_WIN						0x61F
#define LASER_SOUND_ERR					0x803
#define LASER_SOUND_OK					0x802

#define COLORS_CLEAR					0x400
#define COLORS_WIN						0x40F

#define SOLVE_COLORS					0x501
#define SOLVE_LASERS					0x503

//------------------------------------------------------------------------------------------------------
#endif  //_MQTT_CMDS