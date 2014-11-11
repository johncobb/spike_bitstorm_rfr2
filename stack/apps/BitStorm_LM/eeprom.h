/*
 * eeprom.h
 *
 * Created: 7/14/2014 12:57:42 PM
 *  Author: Eric Rudisill
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


char EEPROM_GetChar( unsigned int addr );
void EEPROM_PutChar( unsigned int addr, char new_value );


#endif /* EEPROM_H_ */