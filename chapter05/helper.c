/*
 * helper.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "state.h"
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

