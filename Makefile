AVRDUDE=avrdude
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
CC=avr-gcc
RM=rm -f

MCU=atmega328p
MCU_AVRDUDE=m328p
F_CPU=16000000UL
BIN_FORMAT=ihex
PORT?=/dev/ttyACM0
BAUD=9600
PROGRAMMER?=usbtiny
CFLAGS=-Wall -Wextra -Wno-unused-parameter -g -Os -DF_CPU=$(F_CPU)\
       -DBAUD=$(BAUD) -mmcu=$(MCU) 

TARGET=main
SRC:=$(TARGET).c 
OBJ:=$(SRC:.c=.o) 

.SUFFIXES: .elf .hex

all: $(TARGET).hex $(TARGET).elf tags

%.elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.hex: %.elf
	$(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

upload: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU_AVRDUDE) -c $(PROGRAMMER)\
		-U flash:w:$(TARGET).hex

setfuses:
	$(AVRDUDE) -v -p $(MCU_AVRDUDE) -c $(PROGRAMMER)\
		-U lfuse:w:e6:m

tags: $(SOURCES)
	@ctags --exclude=.git -R . /usr/avr/include/

clean:
	$(RM) $(TARGET).elf $(TARGET).hex $(OBJ) $(TARGET).lss tags

.PHONY: all clean upload
