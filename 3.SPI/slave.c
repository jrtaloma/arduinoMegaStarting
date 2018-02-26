#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "uart.h"
#include "spi.h"

#define ACK 0x7E
 
struct UART* uart; 

void printString(char* s){
	int l=strlen(s);
	for(int i=0; i<l; ++i, ++s)
		UART_putChar(uart, (uint8_t) *s);
}

int main(void){	
	uart=UART_init("uart_0",115200);
	SPI_SlaveInit();
	unsigned char data;
	char tx_buffer[300];
	char rx_buffer[300];
	rx_buffer[0] = 0;
	int size = 0;
	
	while(1){
		data = SPI_tranceiver(ACK);
		rx_buffer[size] = data;
		++size;
		rx_buffer[size] = 0;
		if (data == '\n') {
			sprintf(tx_buffer, "%s", rx_buffer);
			printString(tx_buffer);
			size = 0;
        }
    }
}
