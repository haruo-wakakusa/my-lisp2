/*
 * printer.h
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include <stdio.h>

void printer_print(FILE *stream, void *obj);

extern int is_printed_in_eval;

#endif

