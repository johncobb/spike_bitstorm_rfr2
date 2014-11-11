/*
* Coordinator.c
*
* Created: 7/8/2014 1:17:01 PM
*  Author: Eric Rudisill
*/

#include "config.h"

#include <stdlib.h>
#include "nwk.h"
#include "sysTimer.h"
#include "halLed.h"
#include "halUart.h"

#include "BitStorm.h"
#include "BaseNode.h"
#include "Coordinator.h"
#include "Sensor_ADC.h"


//#define ENABLE_COORDINATOR_MESSAGES


uint8_t cobs_buffer[sizeof(AppMessage_t) + 8];

static size_t cobsEncode(uint8_t *input, uint8_t length, uint8_t *output)
{
	size_t read_index = 0;
	size_t write_index = 1;
	size_t code_index = 0;
	uint8_t code = 1;

	while(read_index < length)
	{
		if(input[read_index] == 0)
		{
			output[code_index] = code;
			code = 1;
			code_index = write_index++;
			read_index++;
		}
		else
		{
			output[write_index++] = input[read_index++];
			code++;
			if(code == 0xFF)
			{
				output[code_index] = code;
				code = 1;
				code_index = write_index++;
			}
		}
	}

	output[code_index] = code;

	return write_index;
}

static void appSendMessage(uint8_t *data, uint8_t size)
{
	uint8_t cobs_size = 0;
	AppMessage_t * msg = (AppMessage_t *)data;

	if (send_messages_paused)
		return;

	// Calculate checksum...Couples us to AppMessage_t, but whatever...
	msg->cs = 0;
	for (uint8_t i=0;i<sizeof(AppMessage_t)-1;i++)
	{
		uint8_t x = ((uint8_t*)msg)[i];
		msg->cs ^= x;
	}

	// Stuff bytes (remove all zeros)
	cobs_size = cobsEncode(data, size, cobs_buffer);

	for (uint8_t i=0; i < cobs_size; i++)
	{
		//HAL_LedToggle(LED_DATA);
		HAL_UartWriteByte(cobs_buffer[i]);
	}
	

	// Zero becomes EOR marker
	HAL_UartWriteByte(0x00);

	//HAL_LedOff(LED_DATA);
}


static bool appDataInd(NWK_DataInd_t *ind)
{
//	AppMessage_t *msg = (AppMessage_t *)ind->data;
//	msg->lqi = ind->lqi;
//	msg->rssi = ind->rssi;

	appSendMessage(ind->data, ind->size);

	NWK_SetAckControl(AppConfig.AckByte);
	
	return true;
}

#ifdef ENABLE_COORDINATOR_MESSAGES
static void appDataSendingTimerHandler(SYS_Timer_t *timer)
{
	if (APP_STATE_WAIT_SEND_TIMER == appState)
		appState = APP_STATE_SEND;
	else
		SYS_TimerStart(&appDataSendingTimer);

	(void)timer;
}
#endif

static void onInit(void)
{
	baseNode_initDevice();

	baseNode_initMessage();
	// Coordinator is short address 0
	mac_short = 0x0000;
	appMsg.shortAddr = mac_short;
	
#ifdef ENABLE_COORDINATOR_MESSAGES	
	baseNode_initDataSendingTimer(appDataSendingTimerHandler);
#endif
	
	baseNode_initNetwork(appDataInd);
	
	HAL_LedOn(LED_DATA);
	
	appState = APP_STATE_SEND;
}

static void onSend(void)
{
#ifdef ENABLE_COORDINATOR_MESSAGES	
	appMsg.parentShortAddr = 0;
	uint16_t temp = adc_meastemp();
	uint16_t batt = adc_measevdd();

	appMsg.sensors.battery     = batt;
	appMsg.sensors.temperature = temp;
	appMsg.sensors.light       = rand() & 0xff;

	appSendMessage((uint8_t *)&appMsg, sizeof(appMsg));
	
	SYS_TimerStart(&appDataSendingTimer);
	appState = APP_STATE_WAIT_SEND_TIMER;
#endif	
}

static void onSendingDone(void)
{
#ifdef ENABLE_COORDINATOR_MESSAGES	
	SYS_TimerStart(&appDataSendingTimer);
	appState = APP_STATE_WAIT_SEND_TIMER;
#endif	
}







AppStateEvents_t eventsCoordinator = {
	onInit,									// APP_STATE_INITIAL
	onSend,									// APP_STATE_SEND
	NULL,										// APP_STATE_WAIT_CONF
	onSendingDone,					// APP_STATE_SENDING_DONE
	NULL,										// APP_STATE_WAIT_SEND_TIMER
	NULL,										// APP_STATE_PREPARE_TO_SLEEP
	NULL,										// APP_STATE_SLEEP
	NULL										// APP_STATE_WAKEUP
};
