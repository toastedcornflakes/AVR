#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#include "support/uart.h"
#include "support/streams.h"

#define INTERVAL 50000UL


static void switch_led() {
	PORTC ^= _BV(PC3);
}

static void setup() {
	uart_setup();
	// setup the streams used by stdio.h 
	streams_setup();
	
	// set PC3 as output 
	DDRC |= _BV(DDC3);
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
