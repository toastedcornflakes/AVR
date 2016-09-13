#include <avr/io.h>

// Sets the UART registers (of UART 0) to the value computed
// in setbaud.h, based on the BAUD macro (defined in the Makefile)
void uart_init0()
{
#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
}

