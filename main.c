#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "support/uart.h"
#include "support/streams.h"

#define INTERVAL 50000UL

ISR(INT0_vect) {
	puts("ISR");
}

static void switch_led() {
	PORTC ^= _BV(PC3);
}

static void setup_ISR() {
	// interrupt on falling edge
	EICRA = (1 << ISC10);
	
	// Enable INT0
	EIMSK |= (1 << INTF0);

	// Enable interrupts
	sei();
}

static void setup_pins() {
	// set PC3 as output 
	DDRC |= _BV(DDC3);
}

static void setup() {
	setup_pins();
	uart_setup();

	streams_setup();
	setup_ISR();
}

int main()
{
	uint32_t l = 0;

	setup();

	while(1)
	{
		l = (l + 1) % INTERVAL;
		if(l == 0) {
			switch_led();
			puts("Switching LED\n");
		}
	}

	return 0;
}
