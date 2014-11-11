/*
* Router.c
*
* Created: 7/8/2014 1:17:09 PM
*  Author: Eric Rudisill
*/

#include "config.h"

#include "halLed.h"
#include "sysTimer.h"

#include "BitStorm.h"
#include "BaseNode.h"
#include "Router.h"
#include "at.h"

typedef struct RouterId_t
{
	uint64_t id;
	uint16_t age;
	uint8_t is_sent;
	uint8_t rssi;
	uint16_t batt;
	uint16_t temp;
} RouterId_t;

RouterId_t router_id_list[ROUTER_ID_LIST_MAX];
volatile static uint16_t slot_to_send;

#define ROUTER_ID_LIST_EMPTY		0xff00
#define ROUTER_ID_LIST_DEAD			0xff00
#define ROUTER_ID_LIST_FULL			0xfe00
#define ROUTER_ID_LIST_ALIVE		0xfd00
#define ROUTER_ID_INVALID				0xffff
#define ROUTER_ID_IS_SENT				0xff
#define ROUTER_ID_NOT_SENT			0x00


static uint8_t send_router_report;

static void appDataSendingTimerHandler(SYS_Timer_t *timer)
{
	send_router_report = 0xff;
	if (APP_STATE_WAIT_SEND_TIMER == appState)
	{
		appState = APP_STATE_SEND;
	}

	//if (APP_STATE_WAIT_SEND_TIMER == appState)
	//{
		//send_router_report = 0xff;
		//appState = APP_STATE_SEND;
	//}
	//else
	//{
		//send_router_report = 0x00;
		//SYS_TimerStart(&appDataSendingTimer);
	//}
//
	(void)timer;
}

static void routerAgingTimerHandler(SYS_Timer_t *timer)
{
	for (int i=0;i<ROUTER_ID_LIST_MAX;i++)
	{
		// skip empty slots
		if (router_id_list[i].age != ROUTER_ID_LIST_EMPTY)
		{
			// increment age and validate life
			router_id_list[i].age++;
			if (router_id_list[i].age > ROUTER_ID_AGE_MAX)
			{
				// too old ... die!
				router_id_list[i].age = ROUTER_ID_LIST_EMPTY;
			}
		}
	}
	(void)timer;
}


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
	baseNode_initDataSendingTimer(appDataSendingTimerHandler);
	baseNode_initAgingTimer(routerAgingTimerHandler);
	baseNode_initNetwork(NULL);
	
	// Assert RTS (go low)
	DDRF |= (1 << 0);
	PORTF &= ~(1 << 0);
	
	// Setup CTS (input)
	DDRF &= ~(1 << 1);
	
	// Init router ids
	for (int i=0;i<ROUTER_ID_LIST_MAX;i++)
	{
		router_id_list[i].id	= 0xffffffffffffffff;
		router_id_list[i].age = ROUTER_ID_LIST_EMPTY;
	}
	
	slot_to_send = ROUTER_ID_INVALID;
	send_router_report = 0xff;
	
	SYS_TimerStart(&routerAgingTimer);
	
	appState = APP_STATE_SEND;
}

static void onSend(void)
{
	if (send_router_report == 0xff)
	{
		baseNode_initMessage();	
		HAL_LedOn(LED_DATA);
		baseNode_sendData(appDataConf);
		appState = APP_STATE_WAIT_CONF;		
		return;
	}
	
	if (slot_to_send == ROUTER_ID_INVALID)
	{
		appState = APP_STATE_WAIT_SEND_TIMER;
		return;
	}

	appMsg.messageType = 0x02;
	appMsg.extAddr = router_id_list[slot_to_send].id;
	appMsg.rssi = router_id_list[slot_to_send].rssi;
	appMsg.battery = router_id_list[slot_to_send].batt;
	appMsg.temperature = router_id_list[slot_to_send].temp;
	
	HAL_LedOn(LED_DATA);
	baseNode_sendDataCurrent(appDataConf);
	appState = APP_STATE_WAIT_CONF;
}

static void onSendingDone(void)
{
	if (send_router_report == 0xff)
	{
		send_router_report = 0x00;
		SYS_TimerStart(&appDataSendingTimer);
	}
	slot_to_send = ROUTER_ID_INVALID;
	appState = APP_STATE_WAIT_SEND_TIMER;
}

static void onWaitSendTimer(void)
{
	uint16_t slot;
	uint16_t i;
	
	if (appState != APP_STATE_WAIT_SEND_TIMER)
		return;

	if (slot_to_send != ROUTER_ID_INVALID)
		return;
	
	slot = ROUTER_ID_INVALID;
	
	// Find a record that has not been sent
	for (i=0;i<ROUTER_ID_LIST_MAX;i++)
	{
		// Skip empty slots
		if (router_id_list[i].age != ROUTER_ID_LIST_EMPTY)
		{
			// Catch first record which has not been sent
			if (router_id_list[i].is_sent == ROUTER_ID_NOT_SENT)
			{
				slot = i;
				break;
			}
		}
	}

	// Do nothing if no records to send
	if (slot == ROUTER_ID_INVALID)
		return;

  router_id_list[slot].is_sent = ROUTER_ID_IS_SENT;
	slot_to_send = slot;
	
	appState = APP_STATE_SEND;
}


static uint16_t trySetSlot(uint64_t id)
{
	int emptyIndex = ROUTER_ID_INVALID;
	
	// scan for a matching, alive id .. collect an empty slot along the way
	// so we don't have to scan a second time (possibly)
	for (int i=0;i<ROUTER_ID_LIST_MAX;i++)
	{
		if (router_id_list[i].age == ROUTER_ID_LIST_EMPTY)
		{
			// grab the first empty slot
			if (emptyIndex == ROUTER_ID_INVALID)
			emptyIndex = i;
		}
		else if (router_id_list[i].id == id)
		{
			// alive, matched id, so bail (encode index in LSB)
			return ROUTER_ID_LIST_ALIVE | i;
		}
	}
	
	// if we got here, we didn't find an alive match
	// so if we found an empty slot, us it... otherwise bail
	if (emptyIndex == ROUTER_ID_INVALID)
		return ROUTER_ID_LIST_FULL;
	
	router_id_list[emptyIndex].id	 = id;
	router_id_list[emptyIndex].age = 0;
	
	return emptyIndex;
}

void AT_handle_le_packet(char * buffer)
{
	//           1111111111222222222
	// 01234567890123456789012345678
	// |||||||||||||||||||||||||||||
	// *00078072CCB3 C3 5994 63BC 24
	
	uint8_t * num;
	uint8_t msb, lsb, ck, ckx;
	uint8_t rssi;
	uint16_t batt, temp;
	uint64_t mac;
	int i;
	
	// Validate checksum in bytes 27-28
	// Just an XOR of bytes 0-26
	msb = AT_parseNybble(buffer[27]);	lsb = AT_parseNybble(buffer[28]);
	ck = (msb << 4) | lsb;
	ckx = 0;
	for (i=0;i<=26;i++)
		ckx ^= buffer[i];
	if (ck != ckx)
	{
		//AT_debugString("BAD CK ");
		//AT_writeHex(ck);
		//AT_debugString(" ");
		//AT_writeHex(ckx);
		//AT_debugString("\n");
		return;
	}
	
	// MAC address - incoming 48bits
	//
	num = (uint8_t *)&mac;
	num[7] = 0;
	num[6] = 0;
	msb = AT_parseNybble(buffer[1]);	lsb = AT_parseNybble(buffer[2]);
	num[5] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[3]);	lsb = AT_parseNybble(buffer[4]);
	num[4] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[5]);	lsb = AT_parseNybble(buffer[6]);
	num[3] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[7]);	lsb = AT_parseNybble(buffer[8]);
	num[2] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[9]);	lsb = AT_parseNybble(buffer[10]);
	num[1] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[11]);	lsb = AT_parseNybble(buffer[12]);
	num[0] = (msb << 4) | lsb;
	
	// RSSI
	//
	msb = AT_parseNybble(buffer[14]);	lsb = AT_parseNybble(buffer[15]);
	rssi = (msb << 4) | lsb;
	
	
	// Temperature
	//
	num = (uint8_t *)&temp;
	msb = AT_parseNybble(buffer[17]);	lsb = AT_parseNybble(buffer[18]);
	num[0] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[19]);	lsb = AT_parseNybble(buffer[20]);
	num[1] = (msb << 4) | lsb;
	
	// Battery
	//
	num = (uint8_t *)&batt;
	msb = AT_parseNybble(buffer[22]);	lsb = AT_parseNybble(buffer[23]);
	num[0] = (msb << 4) | lsb;
	msb = AT_parseNybble(buffer[24]);	lsb = AT_parseNybble(buffer[25]);
	num[1] = (msb << 4) | lsb;
	
	
	// Try to record the record
	// If the ID is still alive or no empty slot found, bail..nothing we can do.
	int slot = trySetSlot(mac);
	if ((slot & ROUTER_ID_LIST_ALIVE) == ROUTER_ID_LIST_ALIVE)
	{
		// Update cached record with current values
		// We may do retries down the road
		//AT_debugString("*REPT\n");
		slot = slot & 0xff;
		router_id_list[slot].rssi = rssi;
		router_id_list[slot].batt	 = batt;
		router_id_list[slot].temp = temp;
		return;
	}
	else if (slot == ROUTER_ID_LIST_FULL)
	{
		// Nothing we can do .. dropped packet
		//AT_debugString("*FULL\n");
		return;
	}

	// Collect the attributes. Message will be sent next pass.
	router_id_list[slot].rssi = rssi;
	router_id_list[slot].batt	 = batt;
	router_id_list[slot].temp = temp;
	router_id_list[slot].is_sent = ROUTER_ID_NOT_SENT;

	//AT_debugString("*CACH\n");

	return;

}




AppStateEvents_t eventsRouter = {
	onInit,									// APP_STATE_INITIAL
	onSend,									// APP_STATE_SEND
	NULL,										// APP_STATE_WAIT_CONF
	onSendingDone,					// APP_STATE_SENDING_DONE
	onWaitSendTimer,				// APP_STATE_WAIT_SEND_TIMER
	NULL,										// APP_STATE_PREPARE_TO_SLEEP
	NULL,										// APP_STATE_SLEEP
	NULL										// APP_STATE_WAKEUP
};
