#!/bin/sh

gcc -c cons_buffer.c helper.c -std=c89 -Wall
rm cons_buffer.o helper.o

