#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

void SPI_MasterInit(void){
	DDRB = (1<<5)|(1<<3)|(1<<2); // set SCK, MOSI, SS as Output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // enable SPI, set as Master, prescaler: fck/16
}

void SPI_SlaveInit(void){
	DDRB = (1<<4); // MISO as Output
	SPCR = (1<<SPE); // enable SPI
}

void SPI_MasterTransmit(uint8_t data){
	SPDR = data; // load data into the buffer
	while (!(SPSR & (1<<SPIF))); // wait until transmission complete
}

uint8_t SPI_SlaveReceive(void){
	while (!(SPSR & (1<<SPIF))); // wait until transmission complete
	return SPDR; // return data register
}

void SPI_sendString(char* s){
	for (int i=0; i<strlen(s); i++){
		SPI_MasterTransmit((uint8_t)s[i]);
	}
}
