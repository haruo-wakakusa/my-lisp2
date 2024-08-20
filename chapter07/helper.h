/*
 * helper.h
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "../chapter02/type.h"
#include "../chapter03/buffer.h"

void * cons(void *car1, void *cdr1);
char * get_symbol_string(SYMBOL *symbol);
int consp(void *obj);
void *car(void *obj);
void *cdr(void *obj);
void *rplaca(void *cons1, void *obj);
void *rplacd(void *cons1, void *obj);
SYMBOL * make_symbol_from_buffer(BUFFER buf);
STRING * make_string_from_buffer(BUFFER buf);
SYMBOL * make_symbol(char *str1);
char * get_string_string(STRING *string);

#endif

