#!/bin/bash

avr-gcc -mmcu=attiny25 -c 7segment.c
avr-gcc -mmcu=attiny25 -o 7segment.elf 7segment.o
avr-objcopy -R .eeprom -O ihex 7segment.elf 7segment.hex
