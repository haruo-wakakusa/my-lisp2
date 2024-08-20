/*
 * cons_buffer.c
 */

#include <stdlib.h>
#include "cons_buffer.h"
#include "../chapter02/type.h"
#include "helper.h"

typedef struct {
    CONS *top;
    CONS *last;
} CBUF_HEADER;

CONS_BUFFER cons_buffer_allocate() {
    CBUF_HEADER *h = (CBUF_HEADER *)malloc(sizeof(CBUF_HEADER));
    h->top = NIL;
    h->last = NIL;
    return (void *)h;
}

void cons_buffer_add(CONS_BUFFER buf, void *obj) {
    CBUF_HEADER *h = (CBUF_HEADER *)buf;
    if (h->top == NIL) {
        h->top = (CONS *)cons(obj, NIL);
        h->last = h->top;
    } else {
        h->last->cdr = (CONS *)cons(obj, NIL);
        h->last = h->last->cdr;
    }
}

void *cons_buffer_get_list(CONS_BUFFER buf) {
    return (void *) ((CBUF_HEADER *)buf)->top;
}

void cons_buffer_free(CONS_BUFFER buf) {
    free(buf);
}

