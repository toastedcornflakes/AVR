#include <stdio.h>
#include "uart.h"

static FILE in;
static FILE out;

static int _putchar(char c, FILE *stream) {
	uart_transmit(c);
	return 0;
}

static int _getchar(FILE *stream) {
	return uart_receive();
}

void streams_setup() {
	fdev_setup_stream(&in, NULL, _getchar, _FDEV_SETUP_READ);
	fdev_setup_stream(&out, _putchar, NULL, _FDEV_SETUP_WRITE);

	stdin = &in;
	stdout = &out;
}
