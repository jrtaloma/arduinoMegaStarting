#include "digio.h"
#include "delay.h"
#include "lcd_lib.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

#define HIGH 1
#define LOW 0

int led = 13;

struct UART* uart;

void printString(char* s){
	int l=strlen(s);
	for(int i=0; i<l; ++i, ++s)
		UART_putChar(uart, (uint8_t) *s);
}

int main(void){
	// 1. initialize the digital pin as an output
	DigIO_setDirection(led, Output);
	uart=UART_init("uart_0", 115200);
	
	// 2. initialize LCD screen
	LCDinit();
	LCDclr();
	LCDcursorOFF();
	
	char* msgOn = "On";
	char* msgOff= "Off";
	
	while(1){
		LCDclr();
		// turn LED on
		DigIO_setValue(led, HIGH);
		printString("On\n");
		LCDstring((uint8_t*)msgOn, strlen(msgOn));
		delayMs(1500);
		
		LCDclr();
		// turn LED off
		DigIO_setValue(led, LOW);
		printString("Off\n");
		LCDstring((uint8_t*)msgOff, strlen(msgOff));
		delayMs(1500);
	}
	
	return 0;
}
