#pragma once
#include <stdint.h>

// Initialize SPI Master Device
void SPI_MasterInit(void);

// Initialize SPI Slave Device
void SPI_SlaveInit(void);

// Function to send and receive a single data
uint8_t SPI_tranceiver(uint8_t data);

// Function to send a string
void SPI_sendString(char* s);
