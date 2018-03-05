#pragma once
#include <stdint.h>

struct UART3;

// initializes a uart object
// returns 0 on failure
struct UART3*  UART3_init(const char* device, uint32_t baud);

// puts a character in the buffer of a uart
// if the buffer is full, the function waits until
// there is room in the buffer
void UART3_putChar(struct UART3* uart, uint8_t c);

// returns a character from the uart.
// if no character is available, the function waits
uint8_t UART3_getChar(struct UART3* uart);

// returns the size of the rx buffer
int  UART3_rxbufferSize(struct UART3* uart);

// returns the size of the tx buffer
int  UART3_txBufferSize(struct UART3* uart);

// returns the number of characters to be read fron the rx buffer
int UART3_rxBufferFull(struct UART3* uart);

// returns the number of available characters in the tx buffer
int UART3_txBufferFree(struct UART3* uart);

// returns the number of characters to be read fron the rx buffer
int UART3_txBufferFull(struct UART3* uart);
