/*
 * helper.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "../chapter05/state.h"
#include "../chapter02/type.h"

void * cons(void *car1, void *cdr1) {
    CONS *c = malloc(sizeof(CONS));
    c->h.type = TYPE_CONS;
    c->car = car1;
    c->cdr = cdr1;
    return (void *)c;
}

char * get_symbol_string(SYMBOL *symbol) {
    return symbol->str;
}

int consp(void *obj) {
    return (((HEADER *)obj)->type == TYPE_CONS);
}

void *car(void *obj) {
    if (obj == NIL) return NIL;
    if (!consp(obj)) {
        fprintf(stderr, "CARはアトムに適用できません\n");
        state = STATE_ERROR;
        return 0;
    }
    return ((CONS *)obj)->car;
}

void *cdr(void *obj) {
    if (obj == NIL) return NIL;
    if (!consp(obj)) {
        fprintf(stderr, "CDRはアトムに適用できません\n");
        state = STATE_ERROR;
        return 0;
    }
    return ((CONS *)obj)->cdr;
}

void *rplaca(void *cons1, void *obj) {
    if (!consp(cons1)) {
        fprintf(stderr, "コンスセルではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    ((CONS *)cons1)->car = obj;
    return cons1;
}

void *rplacd(void *cons1, void *obj) {
    if (!consp(cons1)) {
        fprintf(stderr, "コンスセルではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    ((CONS *)cons1)->cdr = obj;
    return cons1;
}

SYMBOL * make_symbol_from_buffer(BUFFER buf) {
    size_t len;
    SYMBOL *symbol;

    len = buffer_get_size(buf) + 1;
    symbol = malloc(sizeof(SYMBOL) + len);
    symbol->h.type = TYPE_SYMBOL;
    buffer_copy(buf, symbol->str);
    symbol->str[len - 1] = '\0';

    return symbol;
}

STRING * make_string_from_buffer(BUFFER buf) {
    size_t len;
    STRING *string;

    len = buffer_get_size(buf) + 1;
    string = malloc(sizeof(STRING) + len);
    string->h.type = TYPE_STRING;
    buffer_copy(buf, string->str);
    string->str[len - 1] = '\0';

    return string;
}

SYMBOL * make_symbol(char *str1) {
    size_t len;
    SYMBOL *symbol;

    len = strlen(str1) + 1;
    symbol = malloc(sizeof(SYMBOL) + len);
    symbol->h.type = TYPE_SYMBOL;
    strcpy(symbol->str, str1);

    return symbol;
}

