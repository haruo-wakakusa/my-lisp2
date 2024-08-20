/*
 * helper.c
 */

#include <stdlib.h>
#include "helper.h"
#include "../chapter02/type.h"

void * cons(void *car1, void *cdr1) {
    CONS *c = malloc(sizeof(CONS));
    c->h.type = TYPE_CONS;
    c->car = car1;
    c->cdr = cdr1;
    return (void *)c;
}

