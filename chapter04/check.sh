#!/bin/sh

clang -c cons_buffer.c helper.c -std=c11 -Wall
rm cons_buffer.o helper.o

