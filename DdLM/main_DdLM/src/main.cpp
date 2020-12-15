#include "main.h"
#include "functions.h"
#include "mqtt_parser.h"
#include "start_inits.h"

//======================================================================================================
//------------------------------------------------------------------------------------------------------
//                                        SETUP
//------------------------------------------------------------------------------------------------------
void setup(void)
{
    //------------------------------------------------------ GAME INITS
    hardware_init();
    //------------------------------------------------------ GAME INITS
    start_variables_init();
    //------------------------------------------------------ ETHERNET
    setup_ethernet();
    //------------------------------------------------------ setup mqtt client
    setup_mqtt();  

}//setup()
//------------------------------------------------------------------------------------------------------
//                                        LOOP
//------------------------------------------------------------------------------------------------------
void loop(void)
{
    //REGULAR TASKS
    mqttClient.loop();

  //------------------------------------------- ALGORITHM >>>
  //------------------------------------------------------------------------------------
  //STATE MACHINE
    switch(current_state)
    {
		case SLEEP_STATE:
			if(!sleep_state_done_flag)
			{
				sleep_state_init();
			}
			break;

		case RESET_STATE:
			resetFunc();
			break;

		case RESTART_STATE:
			start_variables_init();
			publishData(RESET_WEB_UI);             //reset ui
			break;

		case IDLE_STATE:
			if(game_start_flag == true)
			{
				loadControl(LIGHT_THIRD_ROOM, OFF);
				loadControl(LIGHT_SECOND_ROOM, OFF);
				loadControl(HOLE_LIGHT, OFF);

				loadControl(FIRST_DOOR, LOCK);

			    current_state = GAME_STATE;
			}//if start game
			break;

		case GAME_STATE:
			if(colorsGame() == OK)
			{
				current_state = COLORS_WIN_STATE;
			}

			if(mqtt_data_is_ready)
			{
				publishData(mqtt_data);
			}

			if(colors_win_flag)
			{
				loadControl(LIGHT_BRIDGE, ON);
				loadControl(THIRD_DOOR, UNLOCK);
				loadControl(BRIDGE_LASER, ON);
				current_state = LASER_GAME_STATE;
			}
			break;

		case LASER_GAME_STATE:
			//laser game
			if(!laser_win_flag)
			{
				if(laserGame() == LASER_WIN)
				{
					current_state = LASER_WIN_STATE;
				}
				
				if(mqtt_data_is_ready)
				{
					publishData(mqtt_data);
				}

				if(mqtt_data_is_ready)
				{
					publishData(mqtt_data);
					if(mqtt_data == LASER_CLEAR)
					{
						publishData(LASER_SOUND_ERR);
					}
				}
			}
            break;

		//games win states
		case COLORS_WIN_STATE:
			publishData(COLORS_WIN);
			printDBG("colors win!");
			colors_win_flag = true;
			current_state = GAME_STATE;
			break;

		case LASER_WIN_STATE:
			printDBG("laser win!");
			publishData(LASER_WIN);
			laser_win_flag = true;
			loadControl(BRIDGE_LASER, OFF);
			current_state = WIN_IDLE_STATE;
			break;

		case WIN_IDLE_STATE:
			//wait commands from node-red
			NOP;
			delay(1);
			break;

		default:
			break;
	}//[switch] STATE MACHINE

	//------------------------------------------- <<< ALGORITHM

	//------------------------------------------- COMMUNICATION >>>
	//parse new data
	if(new_data_recieved)
	{
		new_data_recieved = false;

		//string to command code
		command = 0;
		uint8_t shft = (msg_length == 5) ? 8 : 12;
		for(uint8_t indx = 2; indx < msg_length; indx++)
	    {
			command |= (msg_payload[indx] > 64) ? (msg_payload[indx] - 55) << shft : (msg_payload[indx] - 48) << shft;
			shft -= 4;
	    }//for

		mqttParser(command);

		mqttClient.loop();

	}//if new data recieved  
  //------------------------------------------- <<< COMMUNICATION

  //------------------------------------------- RECONNECT >>>
	if(mqttClient.state() != MQTT_CONNECTED)
	{
		Serial.println("[ERR] Reconnect ... ");
		while(!mqttClient.connect(CLIENT_ID))
		{
			setup_ethernet();
			delay(1000);
		}
		setup_mqtt();  
	}//if connection is lost
  //------------------------------------------- <<< RECONNECT

  //------------------------------------------------------------------------------------

	//---DBG info
	#ifdef DBG
	if(millis() - DBG_time_delay >= DBG_out_interval)
	{
		DBG_time_delay = millis();

		// printDBG("");
	}
	#endif
  
}//loop()