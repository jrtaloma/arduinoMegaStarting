#include "uart.h"
#include <string.h>
#include <stdio.h>

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
		uint8_t c = UART_getChar(uart);
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
	uart = UART_init("uart_0", 115200);
	while(1){
		char tx_message[BUF_SIZE];
		char rx_message[BUF_SIZE];
		rx_message[0]=0;
		while(1){
			readString(rx_message, BUF_SIZE);
			sprintf(tx_message, "Received msg: [%s]\n", rx_message);
			printString(tx_message);
		}
	}
}
