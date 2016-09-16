#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#include <stdio.h>


#ifndef BAUD
#define BAUD 9600
#endif

static FILE out;

#define STREAM_BUFFER_SIZE 16
static char ring_buffer[STREAM_BUFFER_SIZE];
static uint8_t buffer_consumer_position = 0; 
static uint8_t buffer_n_elements = 0; 


// puts c in the ring buffer; wait if the buffer is full
static int _putchar(char c, FILE *stream) {
start:
	// start critical section
	ATOMIC_BLOCK(ATOMIC_FORCEON) {

		// Say we want an interrupt to trigger as soon as 
		// UDR0 is empty
		UCSR0B |= (1 << UDRIE0);

		// Check if buffer is full
		if (buffer_n_elements >= STREAM_BUFFER_SIZE) {
			goto spinwait;
		} 

		// Put data in the buffer
		ring_buffer[(buffer_consumer_position + buffer_n_elements) % STREAM_BUFFER_SIZE] = c;
		buffer_n_elements++;

		// end critical section
	}
	return 0;

spinwait:
	sei();
	__asm("nop");
	goto start;
}

// UART is ready to transmit a character
ISR(USART_UDRE_vect) {
	// TX data if buffer isn't empty
	if (buffer_n_elements > 0) {
		UDR0 = ring_buffer[buffer_consumer_position++];
		buffer_consumer_position %= STREAM_BUFFER_SIZE;
		buffer_n_elements--;
	} 

	// If the buffer is now empty, disable the interrupt
	// (putchar will re-enable if needed)
	if (buffer_n_elements == 0) {
		// Disable the interrupt if the buffer is empty
		UCSR0B &= ~(1 << UDRIE0);
	}
}

// Sets the UART registers to the value computed
// in setbaud.h, based on the BAUD macro (defined in the Makefile)
static void uart_setup() {
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

	// Set frame format: 8 data, 1 odd parity, 2 stop bit 
	UCSR0C = (1 << USBS0) | (3 << UPM00) | (3 << UCSZ00);
}

void streams_setup() {
	fdev_setup_stream(&out, _putchar, NULL, _FDEV_SETUP_WRITE);

	stdout = &out;

	// putchar bufferized implementation needs interrupts to work
	// make sure to call it later!
}

void uart_streams_setup() {
	uart_setup();
	streams_setup();
}
