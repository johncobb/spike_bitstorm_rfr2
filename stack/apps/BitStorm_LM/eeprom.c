#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>


/*! \brief  Read byte from EEPROM.
 *
 *  This function reads one byte from a given EEPROM address.
 *
 *  \note  The CPU is halted for 4 clock cycles during EEPROM read.
 *
 *  \param  addr  EEPROM address to read from.
 *  \return  The byte read from the EEPROM address.
 */
char EEPROM_GetChar( unsigned int addr )
{
	do {} while( EECR & (1<<EEPE) ); // Wait for completion of previous write.
	EEAR = addr; // Set EEPROM address register.
	EECR = (1<<EERE); // Start EEPROM read operation.
	return EEDR; // Return the byte read from EEPROM.
}



/*! \brief  Write byte to EEPROM.
 *
 *  This function writes one byte to a given EEPROM address.
 *  The differences between the existing byte and the new value is used
 *  to select the most efficient EEPROM programming mode.
 *
 *  \note  The CPU is halted for 2 clock cycles during EEPROM programming.
 *
 *  \note  When this function returns, the new EEPROM value is not available
 *         until the EEPROM programming time has passed. The EEPE bit in EECR
 *         should be polled to check whether the programming is finished.
 *
 *  \note  The EEPROM_GetChar() function checks the EEPE bit automatically.
 *
 *  \param  addr  EEPROM address to write to.
 *  \param  new_value  New EEPROM value.
 */
void EEPROM_PutChar( unsigned int addr, char new_value )
{
	char old_value; // Old EEPROM value.
	char diff_mask; // Difference mask, i.e. old value XOR new value.

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		do {} while( EECR & (1<<EEPE) ); // Wait for completion of previous write.
	
		EEAR = addr; // Set EEPROM address register.
		EECR = (1<<EERE); // Start EEPROM read operation.
		old_value = EEDR; // Get old EEPROM value.
		diff_mask = old_value ^ new_value; // Get bit differences.
	
		// Check if any bits are changed to '1' in the new value.
		if( diff_mask & new_value ) {
			// Now we know that _some_ bits need to be erased to '1'.
		
			// Check if any bits in the new value are '0'.
			if( new_value != 0xff ) {
				// Now we know that some bits need to be programmed to '0' also.
			
				EEDR = new_value; // Set EEPROM data register.
				EECR = (1<<EEMPE) | // Set Master Write Enable bit...
							 (0<<EEPM1) | (0<<EEPM0); // ...and Erase+Write mode.
				EECR |= (1<<EEPE);  // Start Erase+Write operation.
			} else {
				// Now we know that all bits should be erased.

				EECR = (1<<EEMPE) | // Set Master Write Enable bit...
							 (1<<EEPM0);  // ...and Erase-only mode.
				EECR |= (1<<EEPE);  // Start Erase-only operation.
			}
		} else {
			// Now we know that _no_ bits need to be erased to '1'.
		
			// Check if any bits are changed from '1' in the old value.
			if( diff_mask ) {
				// Now we know that _some_ bits need to the programmed to '0'.
			
				EEDR = new_value;   // Set EEPROM data register.
				EECR = (1<<EEMPE) | // Set Master Write Enable bit...
							 (1<<EEPM1);  // ...and Write-only mode.
				EECR |= (1<<EEPE);  // Start Write-only operation.
			}
		}
	}
}


// end of file
