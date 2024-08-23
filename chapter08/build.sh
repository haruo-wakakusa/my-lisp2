#!/bin/sh

clang ../chapter02/type.c \
    ../chapter03/buffer.c \
    ../chapter04/cons_buffer.c \
    ../chapter05/state.c \
    ../chapter07/helper.c \
    ../chapter07/printer.c \
    reader.c \
    parser.c \
    test.c \
    -std=c89 -Wall -o test -g3

