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
#define CLIENT_ID                       "client_1"
#define ETHERNET_SS_PIN                 53

//------------------------------------------------------------------------------------------------------
//                                        GLOBAL VARIABLES
//------------------------------------------------------------------------------------------------------
uint8_t mac[6] = {0x04,0x0C,0xDB,0xA1,0x02,0x42};
const char* server = "192.168.0.70";
IPAddress ip(192, 168, 0, 221);
IPAddress myDns(192, 168, 0, 1);

const String sub_topic = "/client_1_sub";
const String pub_topic = "/client_1_pub";
const String brdcst_topic = "/broadcast";

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
		case SOLVE_WATCHES:
			while(watchGame() != _WIN)
			{
				;
			}
			current_state = WATCH_WIN_STATE;
			break;

		case SOLVE_CHAIRS:
			chairs_win_flag = true;
			publishData(CHAIRS_WIN);
			current_state = GAME_STATE;
			break;

        //stuff
        case FIRST_DOOR_LOCK:
            lockFirstRoomControl(CLOSED);
            break;

        case FIRST_DOOR_UNLOCK:
            lockFirstRoomControl(OPEN);
            break;

        case SECOND_DOOR_LOCK:
            lockSecondRoomControl(CLOSED);
            break;

        case SECOND_DOOR_UNLOCK:
            lockSecondRoomControl(OPEN);
            break;
 
        case FIRST_ROOM_LIGHT_ON:
            lightFirstRoom(ON);
            break;

        case FIRST_ROOM_LIGHT_OFF:
            lightFirstRoom(OFF);
            break;

        case SECOND_ROOM_LIGHT_ON:
            lightSecondRoom(ON);
            break;

        case SECOND_ROOM_LIGHT_OFF:
            lightSecondRoom(OFF);
            break;

 
        default:
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