/*
 * BitStorm.h
 *
 * Created: 7/8/2014 1:25:10 PM
 *  Author: Eric Rudisill
 */ 


#ifndef BITSTORM_H_
#define BITSTORM_H_

#include "nwk.h"

#define CMD_WAIT_FOR_COMMAND	0x81
#define CMD_BLINK							0x82

/*- Types ------------------------------------------------------------------*/
typedef struct PACK AppMessage_t
{
	uint8_t     messageType;
	uint8_t     nodeType;
	uint64_t    extAddr;
	uint16_t    shortAddr;
	uint64_t		routerAddr;
	//uint32_t    softVersion;
	//uint32_t    channelMask;
	uint16_t    panId;
	uint8_t     workingChannel;
	uint16_t    parentShortAddr;
	uint8_t     lqi;
	int8_t      rssi;
	uint8_t			ackByte;
	
	int32_t   battery;
	int32_t   temperature;

	uint8_t cs;
	
} AppMessage_t;

typedef enum AppState_t
{
	APP_STATE_INITIAL = 0,
	APP_STATE_SEND,
	APP_STATE_WAIT_CONF,
	APP_STATE_SENDING_DONE,
	APP_STATE_WAIT_SEND_TIMER,
	APP_STATE_PREPARE_TO_SLEEP,
	APP_STATE_SLEEP,
	APP_STATE_WAKEUP
} AppState_t;

typedef struct AppStateEvents_t
{
	void (*onInitial)(void);
	void (*onSend)(void);
	void (*onWaitConf)(void);
	void (*onSendingDone)(void);
	void (*onWaitSendTimer)(void);
	void (*onPrepareToSleep)(void);
	void (*onSleep)(void);
	void (*onWakeup)(void);
} AppStateEvents_t;

extern AppState_t appState;

void write_config();

#endif /* BITSTORM_H_ */