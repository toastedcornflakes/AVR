#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "support/uart.h"

#define INTERVAL 10000UL

ISR(INT0_vect) {
	PORTC ^= _BV(PC5);
}

static void switch_led() {
	PORTC ^= _BV(PC3);
}

static void setup_button_ISR() {
	// interrupt on falling edge
	EICRA = (1 << ISC10);

	// Enable INT0
	EIMSK |= (1 << INTF0);

	// Enable interrupts
	sei();
}

static void setup_pins() {
	// set PC3 and PC5 as output 
	DDRC |= (_BV(DDC3) | _BV(DDC5));
}

static void setup() {
	setup_pins();
	setup_button_ISR();
	uart_streams_setup();
}

int main()
{
	uint32_t l = 0;
	uint8_t i = 0;

	setup();

	while(1)
	{
		l = (l + 1) % INTERVAL;
		if(l == 0) {
			switch_led();
			printf("Switching LED %d\n", i++);
		}
	}

	return 0;
}
