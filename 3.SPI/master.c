#include "delay.h"
#include "spi.h"
 
int main(void) {
	SPI_MasterInit();                                   
 
	while(1) {
		SPI_sendString("Hello World\n");
		delayMs(1500);
	}
}
