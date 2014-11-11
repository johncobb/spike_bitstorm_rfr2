/*
 * at.c
 *
 * Created: 7/14/2014 1:38:10 PM
 *  Author: Eric Rudisill
 */ 

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include "phy.h"
#include "halUart.h"

#include "BitStorm.h"
#include "at.h"

char HEX_DIGITS[] = "0123456789abcdef";
char at_buffer[AT_BUFFER_MAX+1];
uint8_t at_index = 0;


#define RESET_MCU		{ wdt_enable(WDTO_15MS); while(1); }

// Function Pototype
// http://www.atmel.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_softreset.html
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
// Function Implementation
void wdt_init(void)
{
	MCUSR = 0;
	wdt_disable();

	return;
}


void AT_writeHex(uint8_t b)
{
	char c;
	
	c = HEX_DIGITS[(b >> 4) & 0x0F];
	HAL_UartWriteByte(c);
	c = HEX_DIGITS[b & 0x0F];
	HAL_UartWriteByte(c);
}

uint8_t AT_parseNybble(char c)
{
	if (c >= 'A' && c <= 'F')
		c = c | 0x20;
	for (uint8_t i=0; i<16; i++)
	{
		if (HEX_DIGITS[i] == c)
			return i;
	}
	return 0x80;
}

static void writeString(char * data)
{
	uint16_t i = 0;
	while (data[i])
	{
		HAL_UartWriteByte(data[i++]);
	}
}

void AT_debugString(char * data)
{
	writeString(data);
	HAL_UartWriteByte(0);
}

static void setConfigParam(void * param, size_t size)
{
	uint8_t n1, n2, n3, n4;
	uint16_t t;
	
	n1 = AT_parseNybble(at_buffer[3]);
	n2 = AT_parseNybble(at_buffer[4]);
	
	n3 = 0;
	n4 = 0;
	if (size == sizeof(uint16_t))
	{
		n3 = AT_parseNybble(at_buffer[5]);
		n4 = AT_parseNybble(at_buffer[6]);
	}
	
	t = n1 | n2 | n3 | n4;
	
	if ((t & 0x80) == 0x80)
	{
		writeString("ERR\n");		
		return;
	}
	else
	{
		if (size == sizeof(uint16_t))
		{
			*((uint16_t *)param) = ((uint16_t)n1 << 12) | ((uint16_t)n2 << 8) | (n3 << 4) | (n4);
		}
		else
		{
			*((uint8_t *)param) = (n1 << 4) | n2;
		}
		write_config();
	}	
}


static void handleCommand(void)
{
	char c;
	bool good;
	
	good = false;

	if ((at_buffer[0] | 0x20) == 'a' && (at_buffer[1] | 0x020) == 't')
	{
		good = true;
		if (at_buffer[2] == 0)
		{
			// Keep this here so compiler doesn't optimize
			good = true;
		}
		else 
		{
			c = (at_buffer[2] | 0x20);
			
			///////////////////////////////////////////////////////////// ATX - Pause send messages
			if (c == 'x')
			{
				if (at_buffer[3] == '?')
				{
					// Keep this here so compiler doesn't optimize
					good = true;
				}
				else if (at_buffer[3] == '1')
				{
					send_messages_paused = true;
				}
				else
				{
					send_messages_paused = false;
				}
				writeString("X:");
				send_messages_paused ? HAL_UartWriteByte('1') : HAL_UartWriteByte('0');
			}
			
		
			///////////////////////////////////////////////////////////// ATZ - Reset
			else if (c == 'z')
			{
				RESET_MCU;
			}


			///////////////////////////////////////////////////////////// ATN - Node Type
			else if (c == 'n')
			{
				if (at_buffer[3] == '?')
				{
					writeString("N:0x");
					AT_writeHex(AppConfig.NodeType);
				}
				else if(at_buffer[3] == '0')
				{
					AppConfig.NodeType = NodeType_Coordinator;
					write_config();
					RESET_MCU;
				}
				else if(at_buffer[3] == '1')
				{
					AppConfig.NodeType = NodeType_Router;
					write_config();
					RESET_MCU;
				}
				else if(at_buffer[3] == '2')
				{
					AppConfig.NodeType = NodeType_EndDevice;
					write_config();
					RESET_MCU;
				}
				else
				{
					writeString("ERR\n");
				}
			}


			///////////////////////////////////////////////////////////// ATA - ACK BYTE
			else if (c == 'a')
			{
				if (at_buffer[3] == '?')
				{
					good = true;
				}
				else 
				{
					setConfigParam(&AppConfig.AckByte, sizeof(AppConfig.AckByte));
				}
				writeString("A:0x");
				AT_writeHex(AppConfig.AckByte);
			}
			

			///////////////////////////////////////////////////////////// ATP - PAN ID
			else if (c == 'p')
			{
				if (at_buffer[3] == '?')
				{
					good = true;
				}
				else
				{
					setConfigParam(&AppConfig.PanId, sizeof(AppConfig.PanId));
				}
				writeString("P:0x");
				AT_writeHex(AppConfig.PanId >> 8);
				AT_writeHex(AppConfig.PanId & 0x00FF);
			}
			
			

			///////////////////////////////////////////////////////////// ATC - CHANNEL ID
			else if (c == 'c')
			{
				if (at_buffer[3] == '?')
				{
					good = true;
				}
				else
				{
					setConfigParam(&AppConfig.Channel, sizeof(AppConfig.Channel));
				}
				writeString("C:0x");
				AT_writeHex(AppConfig.Channel);
			}
			
			
			
			///////////////////////////////////////////////////////////// ATR - RANDOM NUMBER - TEST TEST TEST
			else if (c == 'r')
			{				
				uint16_t i,r;
				uint32_t t;
				i = ACK_INTERVAL_val >> 3;
				r = rand();
				t = ((uint32_t)i * (uint32_t)r) / RAND_MAX;
				writeString("I:0x");
				AT_writeHex(i >> 8);
				AT_writeHex(i & 0x00FF);
				writeString("\nR:0x");
				AT_writeHex(r >> 8);
				AT_writeHex(r & 0x00FF);
				writeString("\nT:0x");
				AT_writeHex((t >> 24) & 0x000000ff);
				AT_writeHex((t >> 16) & 0x000000ff);
				AT_writeHex((t >>  8) & 0x000000ff);
				AT_writeHex(t & 0x000000FF);
			}
			

			else
			{
				// Unknown command
				good = false;
			}
		}
	}

	if (good)
	{
		writeString("\nOK\n");
	}
	else
	{
		writeString("?\n");
	}
	
	// Trigger gateway that writing is done
	HAL_UartWriteByte(0);
}


void HAL_UartBytesReceived(uint16_t bytes)
{
	uint8_t c;
		
	if (at_index >= AT_BUFFER_MAX)
		return;
	
	for (uint16_t i=0; i < bytes; i++)
	{
		c = (char)HAL_UartReadByte();
		if (c == '\r' || c == '\n')		
		{
			if (at_buffer[0] == '*')
			{
				//writeString("LEPACKET\n");
				//HAL_UartWriteByte(0);
				AT_handle_le_packet(at_buffer);
			}
			else				
			{
				//writeString("COMMAND\n");
				//HAL_UartWriteByte(0);
				handleCommand();
			}
			memset(at_buffer, 0, AT_BUFFER_MAX);
			at_index = 0;
		}
		else
		{
			at_buffer[at_index] = c;
			at_index++;
			if (at_index >= AT_BUFFER_MAX)
				return;
		}
	}
}

void AT_init(void)
{
	memset(at_buffer, 0, AT_BUFFER_MAX+1);
	at_index = 0;
	writeString("BitStorm\n");
	HAL_UartWriteByte(0);
}

