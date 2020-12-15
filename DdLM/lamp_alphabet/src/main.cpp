#include "main.h"
#include "mqtt_parser.h"
#include "start_inits.h"
#include "functions.h"

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
	//------------------------------------------------------
current_state = NEW_GAME_STATE;
}//setup()
//------------------------------------------------------------------------------------------------------
//                                        LOOP
//------------------------------------------------------------------------------------------------------
void loop(void)
{
	//------------------------------------------- ALGORITHM >>>

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
				current_state = GAME_STATE_JUAN;
			}//if start game
			break;

		case NEW_GAME_STATE:
			prew_letter.letter_code = current_letter.letter_code = _NONE;
			prew_letter.letter_state = current_letter.letter_state = RELEASED;
			letter_counter = 0;
			current_state = GET_LETTER_STATE;
			break;

		case GET_LETTER_STATE:
			current_letter = getCurrentLetter();
			if(current_letter.letter_code != prew_letter.letter_code)
			{
				estonian_timer = millis();

				if(current_letter.letter_code == _ERR)
				{
					current_state = NEW_GAME_STATE;
				}//if err letter pressed

				if(current_letter.letter_code != _NONE)
				{
					if(letter_counter == 0)
					{
						current_state = GAME_CHECK_STATE;
					}
					else
					{
						current_state = current_game;
					}
				}//if correct letter pressed
				prew_letter = current_letter;
			}//if letter changed

			if((millis() - estonian_timer) > game_reset_time)
			{
				current_state = NEW_GAME_STATE;
			}
			break;

		case GAME_CHECK_STATE:
			switch (current_letter.letter_code)
			{
				case _J:
					current_game = current_state = GAME_STATE_JUAN;
					break;
				
				case _M:
					current_game = current_state = GAME_STATE_MARIA;
					break;

				case _C:
					current_game = current_state = GAME_STATE_CARLA;
					break;

				case _R:
					current_game = current_state = GAME_STATE_RAQUEL;
					break;

				case _D:
					current_game = current_state = GAME_STATE_DIEGO;
					break;

				case _H:
					current_game = current_state = GAME_STATE_HELP;
					break;

				default:
					current_state = NEW_GAME_STATE;
					break;
			}//switch
			break;

		case GAME_STATE_JUAN:
			if(current_letter.letter_code == juan_win_arr[letter_counter])
			{
				if(letter_counter == juan_length - 1)
				{
					current_state = WIN_STATE_JUAN;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case GAME_STATE_MARIA:
			if(current_letter.letter_code == maria_win_arr[letter_counter])
			{
				if(letter_counter == maria_length - 1)
				{
					current_state = WIN_STATE_MARIA;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case GAME_STATE_CARLA:
			if(current_letter.letter_code == carla_win_arr[letter_counter])
			{
				if(letter_counter == carla_length - 1)
				{
					current_state = WIN_STATE_CARLA;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case GAME_STATE_RAQUEL:
			if(current_letter.letter_code == raquel_win_arr[letter_counter])
			{
				if(letter_counter == raquel_length - 1)
				{
					current_state = WIN_STATE_RAQUEL;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case GAME_STATE_DIEGO:
			if(current_letter.letter_code == diego_win_arr[letter_counter])
			{
				if(letter_counter == diego_length - 1)
				{
					current_state = WIN_STATE_DIEGO;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case GAME_STATE_HELP:
			if(current_letter.letter_code == help_win_arr[letter_counter])
			{
				if(letter_counter == help_length - 1)
				{
					current_state = WIN_STATE_HELP;
					break;
				}
				else
				{
					letter_counter++;
					current_state = GET_LETTER_STATE;
				}
			}//right letter from sequence
			else
			{
				current_game = current_state = NEW_GAME_STATE;
			}//error letter
			break;

		case WIN_IDLE_STATE:
			//wait commands from node-red
			delay(1);
			break;

		//send data to server
		case WIN_STATE_JUAN:
			publishData(JUAN);
			current_state = NEW_GAME_STATE;
			break;

		case WIN_STATE_MARIA:
			publishData(MARIA);
			current_state = NEW_GAME_STATE;
			break;

		case WIN_STATE_CARLA:
			publishData(CARLA);
			current_state = NEW_GAME_STATE;
			break;

		case WIN_STATE_RAQUEL:
			publishData(RAQUEL);
			current_state = NEW_GAME_STATE;
			break;

		case WIN_STATE_DIEGO:
			publishData(DIEGO);
			current_state = NEW_GAME_STATE;
			break;

		case WIN_STATE_HELP:
			publishData(HELP);
			current_state = NEW_GAME_STATE;
			break;


	}//switch STATE MACHINE

	//REGULAR TASKS
	mqttClient.loop();

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