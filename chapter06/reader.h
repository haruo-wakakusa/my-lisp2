/*
 * reader.h
 */

#ifndef READER_H_
#define READER_H_

#include <stdio.h>

void reader_initialize(void);
void reader_free(void);
void *reader_read(FILE *stream);

#endif

