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
    char *s;

    s = (char *)symbol;
    return s + sizeof(SYMBOL);
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
    char *s;

    len = buffer_get_size(buf) + 1;
    symbol = malloc(sizeof(SYMBOL) + len);
    symbol->h.type = TYPE_SYMBOL;
    s = (char *)symbol;
    buffer_copy(buf, s + sizeof(SYMBOL));
    s[sizeof(SYMBOL) + len - 1] = '\0';

    return symbol;
}

STRING * make_string_from_buffer(BUFFER buf) {
    size_t len;
    STRING *string;
    char *s;

    len = buffer_get_size(buf) + 1;
    string = malloc(sizeof(STRING) + len);
    string->h.type = TYPE_STRING;
    s = (char *)string;
    buffer_copy(buf, s + sizeof(STRING));
    s[sizeof(STRING) + len - 1] = '\0';

    return string;
}

SYMBOL * make_symbol(char *str1) {
    size_t len;
    SYMBOL *symbol;
    char *s;

    len = strlen(str1) + 1;
    symbol = malloc(sizeof(SYMBOL) + len);
    symbol->h.type = TYPE_SYMBOL;
    s = (char *)symbol;
    strcpy(s + sizeof(SYMBOL), str1);

    return symbol;
}

char * get_string_string(STRING *string) {
    char *s;
    
    s = (char *)string;
    return s + sizeof(STRING);
}

int symbolp(void *obj) {
    HEADER *h = (HEADER *)obj;
    return (h->type == TYPE_SYMBOL);
}

int listp(void *obj) {
    while (obj != NIL) {
        if (!consp(obj)) return 0;
        obj = cdr(obj);
    }
    return 1;
}   
    
size_t list_length(void *obj) {
    size_t len = 0;
    while (obj != NIL) {
        if (!consp(obj)) {
            fprintf(stderr, "プロパーなリストではありません\n");
            return 0;
        }
        obj = cdr(obj);
        len++;
    }   
    return len; 
}

int stringp(void *obj) {
    HEADER *h = (HEADER *)obj;
    return (h->type == TYPE_STRING);
}

int all_symbol_p(void *list1) {
    for ( ; list1 != NIL; list1 = cdr(list1)) {
        if (!symbolp(car(list1))) return 0;
    }
    return 1;
}

int symbol_equal(void *symbol1, void *symbol2) {
    return strcmp(get_symbol_string(symbol1),
                  get_symbol_string(symbol2)) == 0;
}

int unique_symbol_list_p(void *obj) {
    void *p;
    void *q;
    for (p = obj; p != NIL; p = cdr(p)) {
        for (q = cdr(p); q != NIL; q = cdr(q)) {
            if (symbol_equal(car(p), car(q))) return 0;
        }
    }
    return 1;
}

int find_symbol(char *name, void *list1) {
    while (list1 != NIL) {
        if (strcmp(name, get_symbol_string(car(list1))) == 0) return 1;
        list1 = cdr(list1);
    }
    return 0;
}

