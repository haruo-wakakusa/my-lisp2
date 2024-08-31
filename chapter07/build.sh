#!/bin/sh

clang ../chapter02/type.c \
    ../chapter03/buffer.c \
    ../chapter04/cons_buffer.c \
    ../chapter05/state.c \
    ../chapter06/reader.c \
    helper.c \
    printer.c \
    test.c \
    -std=c11 -Wall -o test -g3

