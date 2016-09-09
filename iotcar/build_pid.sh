#!/bin/sh
gcc -pthread main.c wheel.c bcm2835.c mcp3008.c pid.c infrared_digital.c -I ./ -o main
