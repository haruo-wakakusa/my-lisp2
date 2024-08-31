#!/bin/sh

clang -c reader.c helper.c -std=c11 -Wall
rm reader.o helper.o

