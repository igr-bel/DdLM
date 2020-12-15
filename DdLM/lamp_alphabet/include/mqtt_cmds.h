#ifndef _MQTT_CMDS
#define _MQTT_CMDS

//------------------------------------------------------------------------------------------------------
//                                        OUT COMMANDS
//------------------------------------------------------------------------------------------------------
//stuff     
#define RESET_WEB_UI                    0x200
#define C_PING_RESP                     0x30D

//game
#define HELP							0x412
#define JUAN							0x413
#define MARIA							0x414
#define CARLA							0x415
#define RAQUEL							0x416
#define DIEGO							0x417


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




//game


//------------------------------------------------------------------------------------------------------
#endif  //_MQTT_CMDS