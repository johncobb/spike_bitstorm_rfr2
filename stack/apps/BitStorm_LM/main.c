/**
 * \file BitStorm_LM.c
 *
 * \brief BitStorm Lightweight Mesh
 *
 * Copyright (C) 2014 CP Handheld Technologies
 *
  */
#define F_CPU		8000000

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/boot.h>
#include "config.h"
#include "hal.h"
#include "sys.h"
#include "halUart.h"
#include "halLed.h"

#ifdef APP_ENABLE_OTA
  #include "otaClient.h"
#endif

#include "BitStorm.h"  
#include "at.h"
#include "Coordinator.h"
#include "Router.h"
#include "EndDevice.h"
#include "eeprom.h"

AppStateEvents_t * currentAppStateEvents = NULL;
AppState_t appState = APP_STATE_INITIAL;

AppConfig_t AppConfig;

bool send_messages_paused = false;

static void read_config()
{
	uint8_t * a;
	a = (uint8_t *)&AppConfig;
	for (uint8_t i=0; i < sizeof(AppConfig_t); i++)
	{
		uint8_t e = EEPROM_GetChar(i);
		*(a + i) = e;
	}
}

void write_config()
{
	uint8_t * a;
	a = (uint8_t *)&AppConfig;
	for (uint8_t i=0; i < sizeof(AppConfig_t); i++)
	{
		EEPROM_PutChar(i, *(a + i));
	}
}



typedef void (*func_ptr_t)(void);

int main(void)
{
	func_ptr_t f = NULL;

	read_config();	
	if (AppConfig.PanId == 0xFFFF)
	{
		// Spoof AppConfig
		//AppConfig.NodeType				= NodeType_Coordinator;
		AppConfig.NodeType				= NodeType_Router;
		AppConfig.PanId						= 0x1973;
		AppConfig.Channel					= 0x16;	// 2460 MHz, channel 22, out of range of LE Adv. Channels
		AppConfig.EndPoint				= 1;
		AppConfig.OtaEndpoint			= 2;
		AppConfig.AckByte					= 0;
		memcpy(AppConfig.SecurityKey, "cPhTechMk1230xyz", 16);

		write_config();
	}
	
	send_messages_paused = false;
	
	// Set type of device
	if (AppConfig.NodeType == NodeType_Coordinator)
		currentAppStateEvents = &eventsCoordinator;
	else if (AppConfig.NodeType == NodeType_Router)
		currentAppStateEvents = &eventsRouter;
	else
		currentAppStateEvents = &eventsEndDevice;
	
  SYS_Init();
  HAL_UartInit(38400);
	AT_init();
	
#ifdef APP_ENABLE_OTA
  OTA_ClientInit();
#endif

  while (1)
  {
    SYS_TaskHandler();
    HAL_UartTaskHandler();

#ifdef APP_ENABLE_OTA
    OTA_ClientTaskHandler();
#endif

		// Grab the function pointer that appState is currently set for and call the app task
		f = *((func_ptr_t*)currentAppStateEvents + (appState));
		if ( f != NULL)
			f();
  }
}
