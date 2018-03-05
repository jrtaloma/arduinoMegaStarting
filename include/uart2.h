#pragma once
#include <stdint.h>

struct UART2;

// initializes a uart object
// returns 0 on failure
struct UART2*  UART2_init(const char* device, uint32_t baud);

// puts a character in the buffer of a uart
// if the buffer is full, the function waits until
// there is room in the buffer
void UART2_putChar(struct UART2* uart, uint8_t c);

// returns a character from the uart.
// if no character is available, the function waits
uint8_t UART2_getChar(struct UART2* uart);

// returns the size of the rx buffer
int  UART2_rxbufferSize(struct UART2* uart);

// returns the size of the tx buffer
int  UART2_txBufferSize(struct UART2* uart);

// returns the number of characters to be read fron the rx buffer
int UART2_rxBufferFull(struct UART2* uart);

// returns the number of available characters in the tx buffer
int UART2_txBufferFree(struct UART2* uart);

// returns the number of characters to be read fron the rx buffer
int UART2_txBufferFull(struct UART2* uart);
