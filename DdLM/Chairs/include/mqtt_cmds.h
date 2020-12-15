#ifndef _MQTT_CMDS
#define _MQTT_CMDS

//------------------------------------------------------------------------------------------------------
//                                        OUT COMMANDS
//------------------------------------------------------------------------------------------------------
//stuff     
#define RESET_WEB_UI                    0x200

#define C_PING_RESP                     0x30D

#define WATCH_SENSORS_MASK				0x400
#define TOUCH_SENSORS_MASK				0x420
#define CHAIRS_SENSORS_MASK				0x600

//game
#define WATCH_WIN						0x433
#define WATCH_MOVE						0x431
#define WATCH_STOPED					0x433
#define CHAIRS_WIN						0x801
#define CHAIRS_ERR						0x803
#define CHAIRS_OK						0x802


// #define ALL_WIN                         0x0000

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

#define FIRST_DOOR_LOCK                 0x501
#define FIRST_DOOR_UNLOCK               0x502
#define SECOND_DOOR_LOCK                0x503
#define SECOND_DOOR_UNLOCK              0x504
#define FIRST_ROOM_LIGHT_ON             0x703
#define FIRST_ROOM_LIGHT_OFF            0x704
#define SECOND_ROOM_LIGHT_ON            0x705
#define SECOND_ROOM_LIGHT_OFF           0x706


//game
#define SOLVE_WATCHES					0x901
#define SOLVE_CHAIRS					0x902

//------------------------------------------------------------------------------------------------------
#endif  //_MQTT_CMDS