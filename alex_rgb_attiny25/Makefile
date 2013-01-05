PROGRAMMER = avrisp2
CPU_GCC = attiny25
CPU_DUDE = t25

F_CPU = 8000000

CDEFS = -DF_CPU=$(F_CPU)
CFLAGS = -mmcu=$(CPU_GCC) $(CDEFS) -Wall -Os

.phony: clean

all: rgb.hex

program: rgb.hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:rgb.hex

clean:
	rm *.o *.elf *.hex

rgb.hex: rgb.c
	avr-gcc $(CFLAGS) -c rgb.c -o rgb.o
	avr-gcc $(CFLAGS) rgb.o -o rgb.elf
	avr-objcopy -R .eeprom -O ihex rgb.elf rgb.hex
