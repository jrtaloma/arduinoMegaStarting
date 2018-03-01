#pragma once
#include <stdint.h>

void SPI_MasterInit(void);

void SPI_SlaveInit(void);

void SPI_MasterTransmit(uint8_t data);

uint8_t SPI_SlaveReceive(void);

// Function to send a string
void SPI_sendString(char* s);
