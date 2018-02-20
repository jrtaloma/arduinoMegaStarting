#include "uart.h"
#include <string.h>
#include <stdio.h>

struct UART* uart;
void printString(char* s){
  int l=strlen(s);
  for(int i=0; i<l; ++i, ++s)
    UART_putChar(uart, (uint8_t) *s);
}

int main(void){
  uart=UART_init("uart_0",115200);
  while(1) {
    char tx_message[300];
    char rx_message[300];
    rx_message[0]=0;
    int size=0;
    goto LOOP; // this is needed to avoid the sending of an empty message on startup
    while(1){
      sprintf(tx_message, "Received msg: [%s]\n", rx_message);
      printString(tx_message);
      
      LOOP: while (1) {
		uint8_t c= UART_getChar(uart);
		rx_message[size]=c;
		++size;
		rx_message[size]=0;
		if (c=='\0' || c=='\n' || c=='\r' || c==0) {
			size = 0;
			break;
		}	
		continue;
	  }
    }
  }
}
