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
	//------------------------------------------------------ STEPPERS
	stepperInit(STEPPER_1, STEPPERS_DIR_PIN, STEPPERS_STEP_PIN, STEPPER_EN_PIN_1);
	stepperInit(STEPPER_2, STEPPERS_DIR_PIN, STEPPERS_STEP_PIN, STEPPER_EN_PIN_2);
	stepperInit(STEPPER_3, STEPPERS_DIR_PIN, STEPPERS_STEP_PIN, STEPPER_EN_PIN_3);
	stepperInit(STEPPER_4, STEPPERS_DIR_PIN, STEPPERS_STEP_PIN, STEPPER_EN_PIN_4);
	stepperInit(STEPPER_5, STEPPERS_DIR_PIN, STEPPERS_STEP_PIN, STEPPER_EN_PIN_5);
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
            //reset ui
            publishData(RESET_WEB_UI);
            break;

        case IDLE_STATE:
            if(game_start_flag == true)
            {
				lightSecondRoom(OFF);
				lockFirstRoomControl(CLOSED);
                current_state = GAME_STATE;
            }//if start game
            break;

        case GAME_STATE:
			//watch game
			if(!watch_win_flag)
			{
				if((getTouchSensorsState() == ACTIVE) && (watch_start_timer_flag == false))
				{
					watch_start_timer_flag = true;
				}
				else if(getTouchSensorsState() == NOT_ACTIVE)
				{
					watch_game_timer = millis();
					watch_start_timer_flag = false;
				}

				if((watch_start_timer_flag == true) && ((millis() - watch_game_timer) > WATCH_ENABLE_TIME))
				{
					watch_game_en_flag = true;
				}
				else
				{
					watch_game_en_flag = false;
				}

				if((getTouchSensorsState() == ACTIVE) && (watch_game_en_flag == true))
				{
					if(watchGame() == _WIN)
					{
						current_state = WATCH_WIN_STATE;
					}
				}//if "hands" sensors active more then 4 seconds
			}//if watch still not win

			//chairs game
			if(!chairs_win_flag)
			{
				current_chair = getChairState();
				if(current_chair.chair_number != prew_chair.chair_number)
				{
					if(current_chair.chair_state == PRESSED)
					{
						if(current_chair.chair_number != _ERR)
						{
							if(current_chair.chair_number == chairs_win_sequence[chairs_counter])
							{
								chairs_counter++;
								if(chairs_counter == 5)
								{
									chairs_counter = 0;
									current_state = CHAIRS_WIN_STATE;
								}
								else
								{
									publishData(CHAIRS_OK);	//sound "OK"
								}
							}
							else
							{
								publishData(CHAIRS_ERR);	//sound "error"
								chairs_counter = 0;
							}
						}
						else
						{
							publishData(CHAIRS_ERR);		//sound "error"
							chairs_counter = 0;
						}
					}
					prew_chair = current_chair;
				}//if state is changed
			}//if chairs still not win
            break;

		case WATCH_WIN_STATE:
			publishData(WATCH_WIN);
			watch_win_flag = true;
			current_state = GAME_STATE;
			break;

		case CHAIRS_WIN_STATE:
			publishData(CHAIRS_WIN);
			printDBG("chairs win!");
			// chairs_win_flag = true;	//set by node-red when M-A-R-I-A is win 
			current_state = GAME_STATE;
			break;

        case WIN_STATE:
            current_state = WIN_IDLE_STATE;
            break;

        case WIN_IDLE_STATE:
            //wait commands from node-red
            NOP;
            delay(1);
            break;

        default:
            break;
    }//switch STATE MACHINE

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