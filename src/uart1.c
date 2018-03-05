#include "uart1.h"
#include "buffer_utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/atomic.h>

void setBaud57600_UART1(void) {
#define BAUD 57600
#include <util/setbaud.h>
  UBRR1H = UBRRH_VALUE;
  UBRR1L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR1A |= (1<<U2X1);
#endif
#undef BAUD
}

void setBaud115200_UART1(void) {
#define BAUD 115200
#include <util/setbaud.h>
  UBRR1H = UBRRH_VALUE;
  UBRR1L = UBRRL_VALUE;

#ifdef USE_2X
  UCSR1A |= (1<<U2X1);
#endif
#undef BAUD
}

#define UART_BUFFER_SIZE 256

typedef struct UART1 {
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
} UART1;

static UART1 uart_1;

struct UART1* UART1_init(const char* device __attribute__((unused)), uint32_t baud) {
  UART1* uart=&uart_1;
  uart->uart_num=0;

  switch(baud){
  case 57600: setBaud57600_UART1(); break;
  case 115200: setBaud115200_UART1(); break;
  default: return 0;
  }
  
  uart->tx_start=0;
  uart->tx_end=0;
  uart->tx_size=0;
  uart->rx_start=0;
  uart->rx_end=0;
  uart->rx_size=0;

  UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); /* 8-bit data */ 
  UCSR1B = _BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1);   /* Enable RX and TX */  
  sei();  
  return &uart_1;
}

// returns the free space in the buffer
int UART1_rxbufferSize(struct UART1* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}
 
// returns the free occupied space in the buffer
int  UART1_txBufferSize(struct UART1* uart __attribute__((unused))) {
  return UART_BUFFER_SIZE;
}

// number of chars in rx buffer
int UART1_rxBufferFull(UART1* uart) {
  return uart->rx_size;
}

// number of chars in rx buffer
int UART1_txBufferFull(UART1* uart) {
  return uart->tx_size;
}

// number of free chars in tx buffer
int UART1_txBufferFree(UART1* uart){
  return UART_BUFFER_SIZE-uart->tx_size;
}

void UART1_putChar(struct UART1* uart, uint8_t c) {
  // loops until there is some space in the buffer
  while (uart->tx_size>=UART_BUFFER_SIZE);
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    uart->tx_buffer[uart->tx_end]=c;
    BUFFER_PUT(uart->tx, UART_BUFFER_SIZE);
  }
  UCSR1B |= _BV(UDRIE1); // enable transmit interrupt
}

uint8_t UART1_getChar(struct UART1* uart){
  while(uart->rx_size==0);
  uint8_t c;
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    c=uart->rx_buffer[uart->rx_start];
    BUFFER_GET(uart->rx, UART_BUFFER_SIZE);
  }
  return c;
}


ISR(USART1_RX_vect) {
  uint8_t c=UDR1;
  if (uart_1.rx_size<UART_BUFFER_SIZE){
    uart_1.rx_buffer[uart_1.rx_end] = c;
    BUFFER_PUT(uart_1.rx, UART_BUFFER_SIZE);
  }
}

ISR(USART1_UDRE_vect){
  if (! uart_1.tx_size){
    UCSR1B &= ~_BV(UDRIE1);
  } else {
    UDR1 = uart_1.tx_buffer[uart_1.tx_start];
    BUFFER_GET(uart_1.tx, UART_BUFFER_SIZE);
  }
}
