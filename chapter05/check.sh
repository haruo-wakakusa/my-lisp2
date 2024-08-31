#!/bin/sh

clang -c environment.c helper.c state.c -std=c11 -Wall
rm environment.o helper.o state.o

