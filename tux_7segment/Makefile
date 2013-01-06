PROGRAMMER = usbtiny
CPU_GCC = at90s2343
CPU_DUDE = 2343

F_CPU = 1000000UL

CDEFS = -DF_CPU=$(F_CPU)
CFLAGS = -mmcu=$(CPU_GCC) $(CDEFS) -Wall -Os

PROGRAM = 7segment

.phony: clean

all: $(PROGRAM).hex

program: $(PROGRAM).hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:$(PROGRAM).hex

clean:
	rm *.o *.elf *.hex

$(PROGRAM).hex: $(PROGRAM).c
	avr-gcc $(CFLAGS) -c $(PROGRAM).c -o $(PROGRAM).o
	avr-gcc $(CFLAGS) $(PROGRAM).o -o $(PROGRAM).elf
	avr-objcopy -R .eeprom -O ihex $(PROGRAM).elf $(PROGRAM).hex
