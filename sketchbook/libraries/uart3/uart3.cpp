#include "uart3.h"
#include "buffer_utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/atomic.h>

void setBaud57600_UART3(void) {
#define BAUD 57600
#include <util/setbaud.h>
  UBRR3H = UBRRH_VALUE;
  UBRR3L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR3A |= (1<<U2X3);
#endif
#undef BAUD
}

void setBaud115200_UART3(void) {
#define BAUD 115200
#include <util/setbaud.h>
  UBRR3H = UBRRH_VALUE;
  UBRR3L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR3A |= (1<<U2X3);
#endif
#undef BAUD
}

#define UART_BUFFER_SIZE 256

typedef struct UART3 {
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
} UART3;

static UART3 uart_3;

struct UART3* UART3_init(const char* device __attribute__((unused)), uint32_t baud) {
  UART3* uart=&uart_3;
  uart->uart_num=0;

  switch(baud){
  case 57600: setBaud57600_UART3(); break;
  case 115200: setBaud115200_UART3(); break;
  default: return 0;
  }
  
  uart->tx_start=0;
  uart->tx_end=0;
  uart->tx_size=0;
  uart->rx_start=0;
  uart->rx_end=0;
  uart->rx_size=0;

  UCSR3C = _BV(UCSZ31) | _BV(UCSZ30); /* 8-bit data */ 
  UCSR3B = _BV(RXEN3) | _BV(TXEN3) | _BV(RXCIE3);   /* Enable RX and TX */  
  sei();  
  return &uart_3;
}

// returns the free space in the buffer
int UART3_rxbufferSize(struct UART3* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}
 
// returns the free occupied space in the buffer
int  UART3_txBufferSize(struct UART3* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}

// number of chars in rx buffer
int UART3_rxBufferFull(UART3* uart) {
  return uart->rx_size;
}

// number of chars in rx buffer
int UART3_txBufferFull(UART3* uart) {
  return uart->tx_size;
}

// number of free chars in tx buffer
int UART3_txBufferFree(UART3* uart){
  return UART_BUFFER_SIZE-uart->tx_size;
}

void UART3_putChar(struct UART3* uart, uint8_t c) {
  // loops until there is some space in the buffer
  while (uart->tx_size>=UART_BUFFER_SIZE);
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    uart->tx_buffer[uart->tx_end]=c;
    BUFFER_PUT(uart->tx, UART_BUFFER_SIZE);
  }
  UCSR3B |= _BV(UDRIE3); // enable transmit interrupt
}

uint8_t UART3_getChar(struct UART3* uart){
  while(uart->rx_size==0);
  uint8_t c;
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    c=uart->rx_buffer[uart->rx_start];
    BUFFER_GET(uart->rx, UART_BUFFER_SIZE);
  }
  return c;
}


ISR(USART3_RX_vect) {
  uint8_t c=UDR3;
  if (uart_3.rx_size<UART_BUFFER_SIZE){
    uart_3.rx_buffer[uart_3.rx_end] = c;
    BUFFER_PUT(uart_3.rx, UART_BUFFER_SIZE);
  }
}

ISR(USART3_UDRE_vect){
  if (! uart_3.tx_size){
    UCSR3B &= ~_BV(UDRIE3);
  } else {
    UDR3 = uart_3.tx_buffer[uart_3.tx_start];
    BUFFER_GET(uart_3.tx, UART_BUFFER_SIZE);
  }
}
