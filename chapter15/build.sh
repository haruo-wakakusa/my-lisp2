#!/bin/sh

clang ../chapter02/type.c \
    ../chapter03/buffer.c \
    ../chapter04/cons_buffer.c \
    ../chapter05/state.c \
    ../chapter05/environment.c \
    ../chapter08/reader.c \
    ../chapter08/parser.c \
    ../chapter09/dot.c \
    ../chapter14/helper.c \
    printer.c \
    built_in_func.c \
    eval.c \
    test.c \
    -std=c89 -Wall -o test -g3

