#!/bin/sh
gcc -pthread main.c wheel.c bcm2835.c mcp3008.c infrared_digital.c tcp_client_mod.c -I ./ -o main
