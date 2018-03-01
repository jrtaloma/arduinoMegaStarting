#include "adc.h"
#include <avr/io.h>

/* ADC enable and prescaler of 128 ----> 16 MHz/128 = 125 kHz */
void adc_init(void) {
	ADMUX = (1<<REFS0); // AREF = AVcc
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/* ADC enable and set prescaler */
void adc_init_with_prescaler(uint8_t prescaler) {
	ADMUX = (1<<REFS0);
	
	switch(prescaler) {
		case 2: ADCSRA |= (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0); break;
		case 4: ADCSRA |= (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); break;
		case 8: ADCSRA |= (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break;
		case 16: ADCSRA |= (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0); break;
		case 32: ADCSRA |= (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0); break;
		case 64: ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); break;
		case 128: ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break;
		default: ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); break;
	}
	
	ADCSRA |= (1<<ADEN);
}

void adc_enableInterrupt(void) {
	ADCSRA |= (1<<ADIE);
}

/* https://bennthomsen.wordpress.com/arduino/peripherals/analogue-input/ */
uint16_t adc_read(uint8_t ch) {
	ADMUX &= 0xE0; // clear bits MUX0-4
	ADMUX |= ch&0x07; // defines the new ADC channel to be read by setting bits MUX0-2
	ADCSRB = ch&(1<<3); // set MUX5
	ADCSRA |= (1<<ADSC); // starts a new conversion
	while(ADCSRA & (1<<ADSC)); // wait until the conversion is done
	return ADCW; // returns the ADC value of the chosen channel
}
