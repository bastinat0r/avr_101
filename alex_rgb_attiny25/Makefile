PROGRAMMER = avrisp2
CPU_GCC = attiny25
CPU_DUDE = t25

.phony: clean

all: rgb.hex

program: rgb.hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:rgb.hex

clean:
	rm *.o *.elf *.hex

rgb.hex: rgb.c
	avr-gcc -mmcu=$(CPU_GCC) -c rgb.c -o rgb.o
	avr-gcc -mmcu=$(CPU_GCC) rgb.o -o rgb.elf
	avr-objcopy -R .eeprom -O ihex rgb.elf rgb.hex
