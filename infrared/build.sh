#!/bin/sh
gcc -I ./ bcm2835.c mcp3008 infrared_digital.c controlTest.c -o controlTest 
