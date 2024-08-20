#!/bin/sh

gcc -c environment.c helper.c state.c -std=c89 -Wall
rm environment.o helper.o state.o

