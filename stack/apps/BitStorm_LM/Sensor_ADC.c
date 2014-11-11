/*
 * Sensor_ADC.c
 *
 * Created: 7/8/2014 2:05:17 PM
 *  Author: Eric Rudisill
 */ 


#include <avr/io.h>
#include "Sensor_ADC.h"


uint16_t adc_meastemp (void)
{
	ADCSRC = 10<<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (9<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);
	do
	{} while( (ADCSRA & (1<<ADSC))); // wait for conversion end
	ADCSRA = 0; // disable the ADC
	return (ADC);
}

uint16_t adc_measevdd (void)
{
	ADCSRC = 10<<ADSUT0; // set start-up time
	ADCSRB = 1<<MUX5; // set MUX5 first
	ADMUX = (3<<REFS0) + (6<<MUX0); // store new ADMUX, 1.6V AREF
	// switch ADC on, set prescaler, start conversion
	ADCSRA = (1<<ADEN) + (1<<ADSC) + (4<<ADPS0);
	do
	{} while( (ADCSRA & (1<<ADSC))); // wait for conversion end
	ADCSRA = 0; // disable the ADC
	return (ADC);
}