#ifndef _MQTT_PARSER_H
#define _MQTT_PARSER_H


//------------------------------------------------------------------------------------------------------
//                                        INCLUDDES
//------------------------------------------------------------------------------------------------------
#include "main.h"
#include <Ethernet.h>
#include <PubSubClient.h>
#include "mqtt_cmds.h"

//------------------------------------------------------------------------------------------------------
//                                        DEFINES
//------------------------------------------------------------------------------------------------------
//ETHERNET      
#define CLIENT_ID                       "client_main"
#define ETHERNET_SS_PIN                 53

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
uint8_t mac[6] = {0xAB,0x42,0x5D,0xC8,0xF8,0x65};
const char* server = "192.168.0.70";
IPAddress ip(192, 168, 0, 223);
IPAddress myDns(192, 168, 0, 1);

const String sub_topic = "/main_sub";
const String pub_topic = "/main_pub";
const String brdcst_topic = "/broadcast";

//------------------------------------ INPUT ETHERNET DATA
byte msg_payload[10];
uint8_t msg_length;

//--------------------------------------- DATA PARSER
uint16_t command;
String cmd_string;
char *pos = 0;
int base = 16;

//------------------------------------------------------------------------------------------------------
//                                        OBJECTS
//------------------------------------------------------------------------------------------------------
EthernetClient ethClient;
PubSubClient mqttClient;

//------------------------------------------------------------------------------------------------------
//                                        PROTOTYPES
//------------------------------------------------------------------------------------------------------
//ethernet
void subscribeReceive(char* topic, byte* payload, unsigned int length);
void publishData(int data);
void mqttParser(uint16_t command);
inline void setup_ethernet(void);
inline void setup_mqtt(void);

//------------------------------------------------------------------------------------------------------
void mqttParser(uint16_t command)
{
    switch(command)
    {
        case SLEEP_CMD:
            current_state = SLEEP_STATE;
            break;

        case RESTART_CMD:     //restart prop
            current_state = RESTART_STATE;
            break;

        case RESET_CMD:       //reset prop
            current_state = RESET_STATE;
            break;

        case START_CMD:       //start prop
            game_start_flag = true;
            current_state = IDLE_STATE;
            break;

        case S_PING_REQ:
            publishData(C_PING_RESP);
            break;

        //game

        //solve by operator
		case SOLVE_COLORS:
			current_state = COLORS_WIN_STATE;
			break;

		case SOLVE_LASERS:
			current_state = LASER_WIN_STATE;
			break;

		//loads
		case LAMP_1_ON:
			loadControl(LAMP_1, ON);
			break;

		case LAMP_1_OFF:
			loadControl(LAMP_1, OFF);
			break;

		case LAMP_2_ON:
			loadControl(LAMP_2, ON);
			break;

		case LAMP_2_OFF:
			loadControl(LAMP_2, OFF);
			break;

		case LAMP_3_ON:
			loadControl(LAMP_3, ON);
			break;

		case LAMP_3_OFF:
			loadControl(LAMP_3, OFF);
			break;

		case LAMP_4_ON:
			loadControl(LAMP_4, ON);
			break;

		case LAMP_4_OFF:
			loadControl(LAMP_4, OFF);
			break;

		case LAMP_5_ON:
			loadControl(LAMP_5, ON);
			break;

		case LAMP_5_OFF:
			loadControl(LAMP_5, OFF);
			break;

		case HOLE_LIGHT_ON:
			loadControl(HOLE_LIGHT, ON);
			break;
			
		case HOLE_LIGHT_OFF:
			loadControl(HOLE_LIGHT, OFF);
			break;
			
		case FIRST_DOOR_LOCK:
			loadControl(FIRST_DOOR, LOCK);
			break;

		case FIRST_DOOR_UNLOCK:
			loadControl(FIRST_DOOR, UNLOCK);
			break;

		case SECOND_DOOR_LOCK:
			loadControl(SECOND_DOOR, LOCK);
			break;

		case SECOND_DOOR_UNLOCK:
			loadControl(SECOND_DOOR, UNLOCK);
			break;

		case THIRD_DOOR_LOCK:
			loadControl(THIRD_DOOR, LOCK);
			break;

		case THIRD_DOOR_UNLOCK:
			loadControl(THIRD_DOOR, UNLOCK);
			break;

		case OUT_DOOR_LOCK:
			loadControl(OUT_DOOR, LOCK);
			break;

		case OUT_DOOR_UNLOCK:
			loadControl(OUT_DOOR, UNLOCK);
			break;

		case LIGHT_BRIDGE_ON:
			loadControl(LIGHT_BRIDGE, ON);
			break;

		case LIGHT_BRIDGE_OFF:
			loadControl(LIGHT_BRIDGE, OFF);
			break;

		case LIGHT_FIRST_ROOM_ON:
			loadControl(LIGHT_FIRST_ROOM, ON);
			break;

		case LIGHT_FIRST_ROOM_OFF:
			loadControl(LIGHT_FIRST_ROOM, OFF);
			break;

		case LIGHT_SECOND_ROOM_ON:
			loadControl(LIGHT_SECOND_ROOM, ON);
			break;

		case LIGHT_SECOND_ROOM_OFF:
			loadControl(LIGHT_SECOND_ROOM, OFF);
			break;

		case LIGHT_THIRD_ROOM_ON:
			loadControl(LIGHT_THIRD_ROOM, ON);
			break;

		case LIGHT_THIRD_ROOM_OFF:
			loadControl(LIGHT_THIRD_ROOM, OFF);
			break;

		case BRIDGE_LASER_ON:
			loadControl(BRIDGE_LASER, ON);
			break;

		case BRIDGE_LASER_OFF:
			loadControl(BRIDGE_LASER, OFF);
			break;

    }//switch

}//mqttParser()
//------------------------------------------------------------------------------------------------------
void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  //set new data ready flag
  new_data_recieved = true;

  //save new data
  for(uint8_t i=0; i<length; i++)
  {
    msg_payload[i] = payload[i];
  }//for
  msg_length = length;

}//subscribeReceive()
//------------------------------------------------------------------------------------------------------
void publishData(int data)
{
    char msgBuffer[6];  //maximum count of simbols for convert to string ( 0xFFFF = 65535 )
    //int to string //*char 
    uint8_t indx = 0;
    msgBuffer[indx++] = data / 10000 + 48;
    data -= (data / 10000) * 10000;
    msgBuffer[indx++] = data / 1000 + 48;
    data -= (data / 1000) * 1000;
    msgBuffer[indx++] = data / 100 + 48;
    data -= (data / 100) * 100;
    msgBuffer[indx++] = data / 10 + 48;
    data -= (data / 10) * 10;
    msgBuffer[indx++] = (data % 10) + 48;
    msgBuffer[indx] = '\0';
    //drop first zero
    indx = 0;
    while(msgBuffer[0] == 48)
    {
        for(uint8_t i = 0; i < 5; i++)
        {
            msgBuffer[i] = msgBuffer[i+1];
        }
    }

    if(mqttClient.connect(CLIENT_ID)) 
    {
        mqttClient.publish(pub_topic.c_str(), msgBuffer);
    }

    return;
}//publishData()
//------------------------------------------------------------------------------------------------------
void setup_ethernet(void)
{
    //ss pin
    Ethernet.init(ETHERNET_SS_PIN);
    Ethernet.begin(mac, ip, myDns);
    delay(1500);

    // check for Ethernet hardware present
    if(Ethernet.hardwareStatus() == EthernetNoHardware) 
    {
        Serial.println(" ");
        Serial.println("[INF] Ethernet shield was not found.");
        
        while(true) 
        {
            delay(1); // empty loop
        }//while(true)
    }//if
    while(Ethernet.linkStatus() == LinkOFF) 
    {
        Serial.println("[INF] Ethernet cable is not connected.");
        delay(500);
    }//while
    // give the Ethernet shield a second to initialize:
    delay(1000);

}//setup_ethernet()
//------------------------------------------------------------------------------------------------------
void setup_mqtt(void)
{
    mqttClient.setClient(ethClient);
    mqttClient.setServer(server, 1883);

    Serial.println(" ");
    Serial.println("[INF] connecting...");
    while(!mqttClient.connect(CLIENT_ID))
    {
        Serial.println("[INF] Looks like the server connection failed...");
        Serial.println(mqttClient.state());
        delay(1000);
    }
    Serial.println("[INF] Connection has been established, well done!");

    mqttClient.setCallback(subscribeReceive);

    mqttClient.subscribe(sub_topic.c_str());
    mqttClient.subscribe(brdcst_topic.c_str());
}//setup_mqtt()
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
#endif //_MQTT_PARSER_H