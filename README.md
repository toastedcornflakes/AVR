# AVR 
This project aims to use most features of a barebone Atmega328P. Currently supported features are:

	* LED blinking
	* Bufferized, non-blocking UART output
	* Basic PC interrupts (to i.e. detect a button press)

The UART's `putchar` is non-blocking and writes to a circular buffer. An interrupt triggers when the register used to transmit data is empty, and the next character is pulled from the buffer. This allows for fast, non-blocking transmissions, as long as the buffer is not full. If it is the case `putchar` will simply block until the buffer frees a slot.
