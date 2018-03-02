#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "uart.h"
#include "spi.h"

#define BUF_SIZE 100

struct UART* uart; 

void printString(char* s){
	int l=strlen(s);
	for(int i=0; i<l; ++i, ++s)
		UART_putChar(uart, (uint8_t) *s);
}

void readString(char* dest, int size){ 
	int i = 0;
	while(1){
		uint8_t c = SPI_SlaveReceive();
		dest[i++] = c;
		dest[i] = 0;
		if(i == size-1){  //end of dest buffer
			while(c != 0) c = UART_getChar(uart); //read all incoming chars without storing in dest buffer: they are lost
			return;
		}
		else if(c=='\n' || c==0) return;
	}
}

int main(void){	
	uart=UART_init("uart_0",115200);
	SPI_SlaveInit();
	char tx_buffer[BUF_SIZE];
	char rx_buffer[BUF_SIZE];
	rx_buffer[0] = 0;
	
	while(1){
		readString(rx_buffer, BUF_SIZE);
		sprintf(tx_buffer, "Received msg: %s", rx_buffer);
		printString(tx_buffer);
    }
}
