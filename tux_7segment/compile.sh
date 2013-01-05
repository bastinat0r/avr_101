#!/bin/bash

PART=at90s2343

avr-gcc -mmcu=$PART -c 7segment.c -o 7segment.o
avr-gcc -mmcu=$PART -o 7segment.elf 7segment.o
avr-objcopy -R .eeprom -O ihex 7segment.elf 7segment.hex
