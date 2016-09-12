#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>


int main()
{
	// set PC3 as input
	DDRC |= _BV(DDC3);

	while(1)
	{
		// set LED on
		PORTC |= _BV(PC3);

		// caveat: the maximal value before overflow is around 260ms
		_delay_ms(250);

		// set LED off
		PORTC &= ~_BV(PC3);

		_delay_ms(250);
	}

	return 0;
}
