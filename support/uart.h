#include <stdint.h>

void uart_setup();

void uart_transmit(uint8_t data);
uint8_t uart_receive();
