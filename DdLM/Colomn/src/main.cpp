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
	//------------------------------------------------------ SOUNDS
	mp3_init();

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
            //reset ui
            publishData(RESET_WEB_UI);
            break;

        case IDLE_STATE:
            if(game_start_flag == true)
            {
                current_state = GAME_STATE;
            }//if start game
            break;

        case GAME_STATE:
			// laser harp game 
			if(!harp_win_flag)
			{
				if(laserHarpGame() == OK)
				{
					current_state = HARP_WIN_STATE;
				}
				if(mqtt_data_is_ready)
				{
					mqtt_data_is_ready = false;
					publishData(mqtt_data);
					sonar_timer = millis();
					harp_hint_timer = millis();
				}
			}
			// play hint sound
			if((millis() - sonar_timer) > SONAR_INTERVAL)
			{
				sonar_timer = millis();
				if((millis() - harp_hint_timer) > HARP_HINT_INTERVAL)
				{
					if(checkDistance() == OK)
					{
						harp_hint_timer = millis();
						sonar_timer = millis();
						playSound(HARP_SOUND_HINT);
					}
				}
			}

			// touch game
			if(!touch_win_flag)
			{
				if(touchGame() == OK)
				{
					current_state = TOUCH_WIN_STATE;
				}
				if(mqtt_data_is_ready)
				{
					mqtt_data_is_ready = false;
					publishData(mqtt_data);
				}
			}

			// smells game
			if(!smells_win_flag)
			{
				if(smellsGame() == OK)
				{
					current_state = SMELLS_WIN_STATE;
				}
				if(mqtt_data_is_ready)
				{
					mqtt_data_is_ready = false;
					publishData(mqtt_data);
				}
			}

			//weight game
			if(!weight_win_flag)
			{
				if(weightGame() == OK)
				{
					current_state = WEIGHT_WIN_STATE;
				}
				if(mqtt_data_is_ready)
				{
					mqtt_data_is_ready = false;
					publishData(mqtt_data);
				}
			}

			//end game
			if(harp_win_flag && touch_win_flag
				&& smells_win_flag && weight_win_flag)
			{
				current_state = WIN_IDLE_STATE;
			}//all game win
            break;

		//games win states
		case HARP_WIN_STATE:
			harp_win_flag = true;
			publishData(HARP_WIN);
			printDBG("harp win!!!");
			current_state = GAME_STATE;
			break;

		case SMELLS_WIN_STATE:
			smells_win_flag = true;
			publishData(SMELLS_WIN);
			printDBG("smells win!!!");
			current_state = GAME_STATE;
			break;

		case TOUCH_WIN_STATE:
			touch_win_flag = true;
			publishData(TOUCH_WIN);
			printDBG("touch win!");
			current_state = GAME_STATE;
			break;

		case WEIGHT_WIN_STATE:
			weight_win_flag = true;
			publishData(WEIGHT_WIN);
			printDBG("weight win!");
			current_state = GAME_STATE;
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