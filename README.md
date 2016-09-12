# AVR 
This is a simple project to blink a LED on barebone Atmega328P.


## Hardware

Not everything is required, and with a bit of creativity you can avoid some of those parts, but this kit will get you going with the most basic features of the AVR.

* AVR μC (I use an [Atmega328P](http://www.atmel.com/devices/atmega328p.aspx))
* External oscillator (16 MHz crystal and two 22pF capacitors)
* Push button with 10k pull-up
* LED and 1k resistor
* ~1 uF capacitor to stabilize the power
* Any avrdude-supported programmer - I chose the 4$ USBTinyISP.

## Connections
### External clock source
The clock source is external. This needs to be configured via fuses on the μC, using avrdude. The connections are pretty straightforward, see "Crystal Oscillator Connections" in the datasheet.

### Reset
Connect the reset pin to VCC via the 10k resistor, and to the ground via the push button.

### Programmer
The 6 pins of the programmer cable (Vcc, ground, MISO, MOSI, SCK and RESET) go to the corresponding pins on the μC. 

### Power
Connect the VCC and AVCC pins of the μC to the power (at this point it can simply be provided by the programmer), and the two ground pins to ground. If you have a ~1uF capacitor laying around, put it with a small resistor between VCC and ground, to stabilize the power.

### LED 
Connect the LED from any free PIN (I chose PC3) to the ground, via a 1k resistor.

With this basic wiring in place, you're now ready to program your first LED-blinker. 

Note that I often redirect you to the datasheet. Understanding this structure and being able to look stuff up on your own will save you a tremendous amount of time.

## Programming
`avrdude` needs to know what programmer and μC you have, and what to upload. The command to upload `-v`erbosely myfile.hex to an Atmega328P, with an USBTinyISP programmer, looks like this:

		avrdude -v -p m328p -c usbtiny -U flash:w:myfile.hex

To get a list of supported parts, type `avrdude -p '?'`, and `avrdude -c '?'` for a list of programmers.


