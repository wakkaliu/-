#!/bin/sh
gcc -pthread -I./ stop.c wheel.c pid.c gpio_mod.c -o stop
