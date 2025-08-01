#!/bin/bash

MCU="atmega328p"
PROGRAMMER="arduino"
PORT="/dev/ttyACM0"
BAUD=115200

make all || { echo "make failed"; exit 1; }

avrdude -F -V -p $MCU -c $PROGRAMMER -P $PORT -b $BAUD -D -U flash:w:build/main.hex:i || { echo "avrdude failed"; exit 1; }
