#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <util/delay.h>
#include <util/atomic.h>

#include "support/uart.h"

volatile uint8_t can_switch = 1;

static void switch_leds() {
	PORTC ^= _BV(PC3);
	PORTC ^= _BV(PC5);
}

ISR(PCINT0_vect) {
	// check that we have a falling edge (i.e. just pressed the button)

	if (!(PINB & _BV(PC0))) {

		if (can_switch) {
			switch_leds();
		}
		can_switch = 0;
	}
}

static void setup_button_ISR() {
	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT0);
}

static void setup_pins() {
	// set PB0 as input
	DDRB &= ~(_BV(DDB0));

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

	uint8_t can_switch_cnt = 0;
	while(1)
	{
		_delay_ms(50);

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			if(!can_switch) {
				can_switch_cnt++;
				if( can_switch_cnt > 2) {
					can_switch = 1;
					can_switch_cnt = 0;
				}
			}
		}

		wdt_reset();
	}

	return 0;
}
