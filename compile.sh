#!/bin/bash

avr-gcc -mmcu=attiny25 -c $1.c
avr-gcc -mmcu=attiny25 -o $1.elf $1.o
avr-objcopy -R .eeprom -O ihex $1.elf $1.hex