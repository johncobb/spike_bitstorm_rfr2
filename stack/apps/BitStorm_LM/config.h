/**
 * \file config.h
 *
 * \brief WSNDemo application and stack configuration
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 * $Id: config.h 7863 2013-05-13 20:14:34Z ataradov $
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stdint.h"
#include "stdbool.h"

/*- Definitions ------------------------------------------------------------*/
#define HAL_UART_CHANNEL        1
#define HAL_UART_RX_FIFO_SIZE   1
#define HAL_UART_TX_FIFO_SIZE   100

#define LED_NETWORK       1
#define LED_DATA          0

#define SYS_SECURITY_MODE                   0

#define NWK_BUFFERS_AMOUNT                  10
#define NWK_DUPLICATE_REJECTION_TABLE_SIZE  50
#define NWK_DUPLICATE_REJECTION_TTL         2000 // ms
#define NWK_ROUTE_TABLE_SIZE                100
#define NWK_ROUTE_DEFAULT_SCORE             3
#define NWK_ACK_WAIT_TIME                   1000 // ms
#define NWK_GROUPS_AMOUNT                   3
#define NWK_ROUTE_DISCOVERY_TABLE_SIZE      5
#define NWK_ROUTE_DISCOVERY_TIMEOUT         1000 // ms

#define NWK_ENABLE_ROUTING
//#define NWK_ENABLE_SECURITY
//#define NWK_ENABLE_ROUTE_DISCOVERY

#define PHY_ENABLE_RANDOM_NUMBER_GENERATOR


#define AT_BUFFER_MAX	256

#define ROUTER_ID_LIST_MAX	50
#define ROUTER_ID_AGE_INTERVAL 250
#define ROUTER_ID_AGE_MAX		2

enum  
{
	NodeType_Coordinator = 0,
	NodeType_Router,
	NodeType_EndDevice	
};

typedef struct AppConfig_t
{
	uint8_t		NodeType;
	uint16_t	PanId;
	uint8_t		Channel;
	uint8_t		EndPoint;
	uint8_t		OtaEndpoint;
	char			SecurityKey[16];
	uint8_t		AckByte;
} AppConfig_t;

extern AppConfig_t AppConfig;

#define ACK_INTERVAL_mask	0x1f
#define ACK_INTERVAL_bp		0
#define ACK_INTERVAL_mult	10000
#define ACK_INTERVAL_val	((((AppConfig.AckByte & ACK_INTERVAL_mask) >> ACK_INTERVAL_bp) + 1) * ACK_INTERVAL_mult)

#define ACK_POWER_mask		0xe0
#define ACK_POWER_bp			5
#define ACK_POWER_mult		2
#define ACK_POWER_val			(((AppConfig.AckByte & ACK_POWER_mask) >> ACK_POWER_bp) * ACK_POWER_mult)

extern bool send_messages_paused;



#endif // _CONFIG_H_
