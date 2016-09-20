#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include "support/uart.h"

volatile uint8_t button = 0;

ISR(PCINT0_vect) {
	button = 1;
}

static void switch_leds() {
	PORTC ^= _BV(PC3);
	PORTC ^= _BV(PC5);
}

static void setup_button_ISR() {
	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT0);
}

static void setup_pins() {
	// set PC3 and PC5 as output 
	DDRC |= (_BV(DDC3) | _BV(DDC5));
	
	// set PC5 as high at the start
	PORTC |= _BV(PC5);
}

static void setup() {
	setup_pins();
	setup_button_ISR();
	uart_streams_setup();
	wdt_enable(WDTO_500MS);

	// Enable interrupts only when setup of all handlers is done
	sei();
}

int main()
{
	setup();
	puts("Setup done");

	while(1)
	{
		if(button == 1) {
			puts("Button!");
			switch_leds();
			// Clear only after a delay
			_delay_ms(200);
			button = 0;
		}

		_delay_ms(100);

		wdt_reset();
	}

	return 0;
}
