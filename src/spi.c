#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

void SPI_MasterInit(void){
	DDRB = (1<<DDB2)|(1<<DDB1)|(1<<DDB0); // set SCK, MOSI, SS as Output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // enable SPI, set as Master, prescaler: fck/16
}

void SPI_SlaveInit(void){
	DDRB = (1<<DDB3); // MISO as Output
	SPCR = (1<<SPE); // enable SPI
}

uint8_t SPI_tranceiver(uint8_t data){
	SPDR = data; // load data into the buffer
	while (!(SPSR & (1<<SPIF))); // wait until transmission complete
	return (SPDR); // return received data
}

void SPI_sendString(char* s){
	for (int i=0; i<strlen(s); i++){
		SPI_tranceiver((uint8_t)s[i]);
	}
}
