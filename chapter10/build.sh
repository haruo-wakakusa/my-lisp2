#!/bin/sh

if [ "$(uname)" != 'Darwin' ]; then
    lm="-lm"
fi

clang ../chapter02/type.c \
    ../chapter03/buffer.c \
    ../chapter04/cons_buffer.c \
    ../chapter05/state.c \
    ../chapter05/environment.c \
    ../chapter07/printer.c \
    ../chapter08/reader.c \
    ../chapter08/parser.c \
    ../chapter09/dot.c \
    built_in_func.c \
    eval.c \
    helper.c \
    test.c \
    -std=c11 -Wall $lm -o test -g3

