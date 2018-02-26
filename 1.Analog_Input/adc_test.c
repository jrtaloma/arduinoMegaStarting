#include "digio.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define HIGH 1
#define LOW 0

// pin 13 has an LED connected on most Arduino boards
int led = 13;

struct UART* uart;

volatile uint16_t adc_value;

void printString(char* s){
	int l=strlen(s);
	for(int i=0; i<l; ++i, ++s)
		UART_putChar(uart, (uint8_t) *s);
}

// the ISR will be called when an A2D conversion completes
ISR(ADC_vect) {
	if (adc_value < 128) {
		DigIO_setValue(led, HIGH);
		char buffer[32];
		sprintf(buffer, "adc_value: %d\n", adc_value);
		printString(buffer);
		delayMs(2000);
	}
	else {
		DigIO_setValue(led, LOW);
	}
}

int main(void){
	
	DigIO_setDirection(led, Output); // initialize the digital pin as an output

	uart=UART_init("uart_0", 115200);
	
	adc_init(); // initialize adc
	
	adc_enableInterrupt(); // enable ADC interrupt
	
	sei(); // enable global interrupts
	
	while(1){
		adc_value = adc_read(7); // read adc value at ADC7	
	}
	
	return 0;
}
