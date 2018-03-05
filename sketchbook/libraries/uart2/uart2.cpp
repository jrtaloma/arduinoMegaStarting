#include "uart2.h"
#include "buffer_utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/atomic.h>

void setBaud57600_UART2(void) {
#define BAUD 57600
#include <util/setbaud.h>
  UBRR2H = UBRRH_VALUE;
  UBRR2L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR2A |= (1<<U2X2);
#endif
#undef BAUD
}

void setBaud115200_UART2(void) {
#define BAUD 115200
#include <util/setbaud.h>
  UBRR2H = UBRRH_VALUE;
  UBRR2L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR2A |= (1<<U2X2);
#endif
#undef BAUD
}

#define UART_BUFFER_SIZE 256

typedef struct UART2 {
  int tx_buffer[UART_BUFFER_SIZE];
  volatile uint8_t tx_start;
  volatile uint8_t tx_end;
  volatile uint8_t tx_size;

  int rx_buffer[UART_BUFFER_SIZE];
  volatile uint8_t rx_start;
  volatile uint8_t rx_end;
  volatile uint8_t rx_size;
  
  int baud;
  int uart_num; // hardware uart;
} UART2;

static UART2 uart_2;

struct UART2* UART2_init(const char* device __attribute__((unused)), uint32_t baud) {
  UART2* uart=&uart_2;
  uart->uart_num=0;

  switch(baud){
  case 57600: setBaud57600_UART2(); break;
  case 115200: setBaud115200_UART2(); break;
  default: return 0;
  }
  
  uart->tx_start=0;
  uart->tx_end=0;
  uart->tx_size=0;
  uart->rx_start=0;
  uart->rx_end=0;
  uart->rx_size=0;

  UCSR2C = _BV(UCSZ21) | _BV(UCSZ20); /* 8-bit data */ 
  UCSR2B = _BV(RXEN2) | _BV(TXEN2) | _BV(RXCIE2);   /* Enable RX and TX */  
  sei();  
  return &uart_2;
}

// returns the free space in the buffer
int UART2_rxbufferSize(struct UART2* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}
 
// returns the free occupied space in the buffer
int  UART2_txBufferSize(struct UART2* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}

// number of chars in rx buffer
int UART2_rxBufferFull(UART2* uart) {
  return uart->rx_size;
}

// number of chars in rx buffer
int UART2_txBufferFull(UART2* uart) {
  return uart->tx_size;
}

// number of free chars in tx buffer
int UART2_txBufferFree(UART2* uart){
  return UART_BUFFER_SIZE-uart->tx_size;
}

void UART2_putChar(struct UART2* uart, uint8_t c) {
  // loops until there is some space in the buffer
  while (uart->tx_size>=UART_BUFFER_SIZE);
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    uart->tx_buffer[uart->tx_end]=c;
    BUFFER_PUT(uart->tx, UART_BUFFER_SIZE);
  }
  UCSR2B |= _BV(UDRIE2); // enable transmit interrupt
}

uint8_t UART2_getChar(struct UART2* uart){
  while(uart->rx_size==0);
  uint8_t c;
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    c=uart->rx_buffer[uart->rx_start];
    BUFFER_GET(uart->rx, UART_BUFFER_SIZE);
  }
  return c;
}


ISR(USART2_RX_vect) {
  uint8_t c=UDR2;
  if (uart_2.rx_size<UART_BUFFER_SIZE){
    uart_2.rx_buffer[uart_2.rx_end] = c;
    BUFFER_PUT(uart_2.rx, UART_BUFFER_SIZE);
  }
}

ISR(USART2_UDRE_vect){
  if (! uart_2.tx_size){
    UCSR2B &= ~_BV(UDRIE2);
  } else {
    UDR2 = uart_2.tx_buffer[uart_2.tx_start];
    BUFFER_GET(uart_2.tx, UART_BUFFER_SIZE);
  }
}
