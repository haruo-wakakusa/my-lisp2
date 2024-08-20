#!/bin/sh

gcc -c reader.c helper.c -std=c89 -Wall
rm reader.o helper.o

