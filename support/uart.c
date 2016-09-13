#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>

#include "uart.h"

#ifndef BAUD
#define BAUD 9600
#endif



// Sets the UART registers (of UART 0) to the value computed
// in setbaud.h, based on the BAUD macro (defined in the Makefile)
void uart_setup()
{
#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
#warning Using the less reliable U2X mode of UART0
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif

	// Enable RX and TX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8data, 2stop bit 
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void uart_transmit(uint8_t data)
{
	// Wait for empty transmit buffer 
	while (!(UCSR0A & (1<<UDRE0)));

	// Put data into buffer, sends the data 
	UDR0 = data;
}

uint8_t uart_receive()
{
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));

	// Get and return received data from buffer 
	return UDR0;
}
