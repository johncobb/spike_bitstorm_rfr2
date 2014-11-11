/*
 * EndPoint.c
 *
 * Created: 7/8/2014 1:17:23 PM
 *  Author: Eric Rudisill
 */ 

#include <stdlib.h>
#include "config.h"
#include "halSleep.h"
#include "halLed.h"
#include "BitStorm.h"
#include "BaseNode.h"
#include "EndDevice.h"


static void appDataConf(NWK_DataReq_t *req)
{
	HAL_LedOff(LED_DATA);
	baseNode_setAckByte(req->control);
	appState = APP_STATE_SENDING_DONE;
}


static void onInit(void)
{
	baseNode_initDevice();
	baseNode_initMessage();
	baseNode_initNetwork(NULL);
	appState = APP_STATE_SEND;
}

static void onSend(void)
{
	HAL_LedOn(LED_DATA);
	baseNode_sendData(appDataConf);
	appState = APP_STATE_WAIT_CONF;
}

static void onSendingDone(void)
{
	appState = APP_STATE_PREPARE_TO_SLEEP;
}

static void onPrepareToSleep(void)
{
	if (!NWK_Busy())
	{
		NWK_SleepReq();
		appState = APP_STATE_SLEEP;
	}
}

static void onSleep(void)
{
	uint16_t i,r;
	uint32_t t;
	
	HAL_LedClose();
	baseNode_lowPower();
	
	i = ACK_INTERVAL_val >> 3;
	r = rand();
	t = ((uint32_t)i * (uint32_t)r) / RAND_MAX;
	if (t & 0x01)
		t = ACK_INTERVAL_val - t;
	else
		t = ACK_INTERVAL_val + t;
	HAL_Sleep(t);
	
	appState = APP_STATE_WAKEUP;	
}

static void onWakeup(void)
{
	NWK_WakeupReq();
	HAL_LedInit();
	appState = APP_STATE_SEND;
}





AppStateEvents_t eventsEndDevice = {
	onInit,									// APP_STATE_INITIAL
	onSend,									// APP_STATE_SEND
	NULL,										// APP_STATE_WAIT_CONF
	onSendingDone,					// APP_STATE_SENDING_DONE
	NULL,										// APP_STATE_WAIT_SEND_TIMER
	onPrepareToSleep,				// APP_STATE_PREPARE_TO_SLEEP
	onSleep,								// APP_STATE_SLEEP
	onWakeup								// APP_STATE_WAKEUP
};


