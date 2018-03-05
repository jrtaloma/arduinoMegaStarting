#pragma once
#include <stdint.h>

struct UART1;

// initializes a uart object
// returns 0 on failure
struct UART1*  UART1_init(const char* device, uint32_t baud);

// puts a character in the buffer of a uart
// if the buffer is full, the function waits until
// there is room in the buffer
void UART1_putChar(struct UART1* uart, uint8_t c);

// returns a character from the uart.
// if no character is available, the function waits
uint8_t UART1_getChar(struct UART1* uart);

// returns the size of the rx buffer
int  UART1_rxbufferSize(struct UART1* uart);

// returns the size of the tx buffer
int  UART1_txBufferSize(struct UART1* uart);

// returns the number of characters to be read fron the rx buffer
int UART1_rxBufferFull(struct UART1* uart);

// returns the number of available characters in the tx buffer
int UART1_txBufferFree(struct UART1* uart);

// returns the number of characters to be read fron the rx buffer
int UART1_txBufferFull(struct UART1* uart);
