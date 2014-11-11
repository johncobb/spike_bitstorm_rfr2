/*
 * EndPoint.c
 *
 * Created: 7/8/2014 1:17:23 PM
 *  Author: Eric Rudisill
 */ 

#include "config.h"

#include <avr/io.h>
#include <avr/boot.h>
#include <stdlib.h>
#include "nwk.h"
#include "phy.h"
#include "sysTimer.h"
#include "halLed.h"

#include "BaseNode.h"
#include "Sensor_ADC.h"


NWK_DataReq_t nwkDataReq;
AppMessage_t appMsg;
uint16_t mac_short = 0;

SYS_Timer_t appDataSendingTimer;
SYS_Timer_t routerAgingTimer;

void baseNode_lowPower(void)
{
	//// Low Power
	DDRB = 0;
	DDRD = 0;
	DDRE = 0;
	DDRF = 0;
	DDRG = 0;
	
	PORTB = 0;
	PORTD = 0;
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;
	
	ADCSRA &= ~(1<<ADEN);		// Disable ADC
	ACSR = (1<<ACD);			// Disable the analog comparator
	DIDR0 = 0xFF;				// Disable ADC7:0
	DIDR2 = 0xFF;				// Disable ADC15:8
	DIDR1 = (1<<AIN1D) | (1<<AIN0D);	// Disable AIN1:0
}

void baseNode_sendDataCurrent(appDataConf_ptr_t conf_ptr)
{
#ifdef NWK_ENABLE_ROUTING
	appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
#else
	appMsg.parentShortAddr = 0;
#endif

	nwkDataReq.dstAddr = 0;
	nwkDataReq.dstEndpoint = AppConfig.EndPoint;
	nwkDataReq.srcEndpoint = AppConfig.EndPoint;
	//nwkDataReq.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY;
	nwkDataReq.options = NWK_OPT_ENABLE_SECURITY;
	nwkDataReq.data = (uint8_t *)&appMsg;
	nwkDataReq.size = sizeof(appMsg);
	nwkDataReq.confirm = conf_ptr;

	NWK_DataReq(&nwkDataReq);
}

void baseNode_sendData(appDataConf_ptr_t conf_ptr)
{
#ifdef NWK_ENABLE_ROUTING
	appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
#else
	appMsg.parentShortAddr = 0;
#endif

	uint16_t temp = adc_meastemp();
	uint16_t batt = adc_measevdd();

	appMsg.ackByte		 = AppConfig.AckByte;
	appMsg.battery     = batt;
	appMsg.temperature = temp;

	nwkDataReq.dstAddr = 0;
	nwkDataReq.dstEndpoint = AppConfig.EndPoint;
	nwkDataReq.srcEndpoint = AppConfig.EndPoint;
	//nwkDataReq.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY;
	nwkDataReq.options = NWK_OPT_ENABLE_SECURITY;
	nwkDataReq.data = (uint8_t *)&appMsg;
	nwkDataReq.size = sizeof(appMsg);
	nwkDataReq.confirm = conf_ptr;

	NWK_DataReq(&nwkDataReq);
}

void baseNode_initDevice(void)
{
	baseNode_lowPower();
	HAL_LedInit();
}

void baseNode_initMessage(void)
{
	uint8_t b;

	appMsg.messageType          = 1;
	appMsg.nodeType             = AppConfig.NodeType;
	
	b = boot_signature_byte_get(0x0102);	appMsg.extAddr	 = b;
	b = boot_signature_byte_get(0x0103);	appMsg.extAddr	|= ((uint64_t)b << 8);
	b = boot_signature_byte_get(0x0104);	appMsg.extAddr	|= ((uint64_t)b << 16);
	b = boot_signature_byte_get(0x0105);	appMsg.extAddr	|= ((uint64_t)b << 24);
	b = boot_signature_byte_get(0x0106);	appMsg.extAddr	|= ((uint64_t)b << 32);
	b = boot_signature_byte_get(0x0107);	appMsg.extAddr	|= ((uint64_t)b << 40);
	b = boot_signature_byte_get(0x0108);	appMsg.extAddr	|= ((uint64_t)b << 48);
	b = boot_signature_byte_get(0x0109);	appMsg.extAddr	|= ((uint64_t)b << 56);
	
	b = boot_signature_byte_get(0x0102);	mac_short	 = b;
	b = boot_signature_byte_get(0x0103);	mac_short	|= ((uint64_t)b << 8);
	appMsg.shortAddr = mac_short;
	
	appMsg.routerAddr = appMsg.extAddr;

	//appMsg.softVersion          = 0x01010100;
	//appMsg.channelMask          = (1L << AppConfig.Channel);
	appMsg.panId                = AppConfig.PanId;
	appMsg.workingChannel       = AppConfig.Channel;
	appMsg.parentShortAddr      = 0;
	appMsg.lqi                  = 0;
	appMsg.rssi                 = 0;
	appMsg.ackByte							= AppConfig.AckByte;

	appMsg.battery     = 0;
	appMsg.temperature = 0;
}

void baseNode_initNetwork(appDataInd_ptr_t ind_ptr)
{
	NWK_SetAddr(mac_short);
	NWK_SetPanId(AppConfig.PanId);
	PHY_SetChannel(AppConfig.Channel);
	PHY_SetTxPower(ACK_POWER_val);
	PHY_SetRxState(true);

#ifdef NWK_ENABLE_SECURITY
	NWK_SetSecurityKey((uint8_t *)AppConfig.SecurityKey);
#endif

	NWK_OpenEndpoint(AppConfig.EndPoint, ind_ptr);
	
	// Seed the standard C RNG
	uint16_t r;
	r = PHY_RandomReq();
	r = PHY_RandomReq();
	r = PHY_RandomReq();
	srand(r);
}

void baseNode_initDataSendingTimer(appDataSendingTimerHandler_ptr_t handler_ptr)
{
	appDataSendingTimer.interval = ACK_INTERVAL_val;
	appDataSendingTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appDataSendingTimer.handler = handler_ptr;
}

void baseNode_initAgingTimer(appDataSendingTimerHandler_ptr_t handler_ptr)
{
	routerAgingTimer.interval = ROUTER_ID_AGE_INTERVAL;
	routerAgingTimer.mode = SYS_TIMER_PERIODIC_MODE;
	routerAgingTimer.handler = handler_ptr;
}

void baseNode_setAckByte(uint8_t ack)
{
	if (AppConfig.AckByte != ack)
	{
		AppConfig.AckByte = ack;
		write_config();
		PHY_SetTxPower(ACK_POWER_val);
		appDataSendingTimer.interval = ACK_INTERVAL_val;
	}
}
