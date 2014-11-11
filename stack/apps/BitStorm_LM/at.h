/*
 * at.h
 *
 * Created: 7/14/2014 1:38:19 PM
 *  Author: Eric Rudisill
 */ 


#ifndef AT_H_
#define AT_H_


void AT_init(void);

uint8_t AT_parseNybble(char c);
void AT_handle_le_packet(char *);
void AT_debugString(char * data);
void AT_writeHex(uint8_t b);

#endif /* AT_H_ */