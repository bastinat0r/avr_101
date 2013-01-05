#!/bin/bash

sudo avrdude -v -c usbtiny -p 2343 -U flash:w:7segment.hex
