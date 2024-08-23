/*
 * reader.h
 */

#ifndef READER_H_
#define READER_H_

#include <stdio.h>

void reader_initialize();
void reader_free();
void *reader_read(FILE *stream);

#endif

