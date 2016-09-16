# Global AVR utilities
AVRDUDE=avrdude
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
CC=avr-gcc
RM=rm -f

# Microcontroller used
MCU=atmega328p
MCU_AVRDUDE=m328p
F_CPU=16000000UL

SERIALPORT?=/dev/ttyUSB0
BAUD=9600
OPEN_SERIAL=picocom $(SERIALPORT) -b $(BAUD) --imap lfcrlf --parity o --stopbits 2

PROGRAMMER?=usbtiny

BIN_FORMAT=ihex
CFLAGS=-Wall -Wextra -Wno-unused-parameter -g -Os -DF_CPU=$(F_CPU)\
       -DBAUD=$(BAUD) -mmcu=$(MCU) 

TARGET=main
SRC:=$(TARGET).c support/uart.c
OBJ:=$(SRC:.c=.o) 

.SUFFIXES: .elf .hex
.SECONDARY: $(OBJS)

all: $(TARGET).hex $(TARGET).elf tags

%.elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.hex: %.elf
	$(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

listing: $(TARGET).lss

upload: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU_AVRDUDE) -c $(PROGRAMMER) -V\
		-U flash:w:$(TARGET).hex

serial:
	$(OPEN_SERIAL)

setfuses:
	$(AVRDUDE) -v -p $(MCU_AVRDUDE) -c $(PROGRAMMER)\
		-U lfuse:w:e6:m

tags: $(SOURCES)
	@ctags --exclude=.git -R . 

clean:
	$(RM) $(TARGET).elf $(TARGET).hex $(OBJ) $(TARGET).lss tags

.PHONY: all clean listing serial setfuses upload 
