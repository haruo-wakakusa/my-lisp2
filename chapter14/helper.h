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
int symbolp(void *obj);
int listp(void *obj);
size_t list_length(void *obj);
int stringp(void *obj);
int all_symbol_p(void *list1);
int symbol_equal(void *symbol1, void *symbol2);
int unique_symbol_list_p(void *obj);
int find_symbol(char *name, void *list1);

#endif

