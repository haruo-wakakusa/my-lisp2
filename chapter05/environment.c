/*
 * environment.c
 */

#include <stdlib.h>
#include <string.h>
#include "../chapter02/type.h"
#include "helper.h"
#include "environment.h"

/* LISPの環境において、通常、parentという言い方はしないことに注意 */
void *environment_init(void *parent) {
    return cons(NIL, parent);
}

void environment_add(void *env, void *key, void *val) {
    void *assoc = cons(key, val);
    void *joint = cons(assoc, car(env));
    rplaca(env, joint);
}

int environment_exists(void *env, void *key) {
    SYMBOL *sym1 = (SYMBOL *)key;
    void *p = car(env);
    while (p != NIL) {
        SYMBOL *sym2 = (SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_get(void *env, void *key, void **val) {
    SYMBOL *sym1 = (SYMBOL *)key;
    void *p = car(env);
    while (p != NIL) {
        SYMBOL *sym2 = (SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            *val = cdr(car(p));
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_modify(void *env, void *key, void *val) {
    SYMBOL *sym1 = (SYMBOL *)key;
    void *p = car(env);
    while (p != NIL) {
        SYMBOL *sym2 = (SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            rplacd(car(p), val);
            return 1;
        }
        p = cdr(p);
    }
    return 0;
}

int environment_remove(void *env, void *key) {
    SYMBOL *sym1 = (SYMBOL *)key;
    SYMBOL *sym2;
    void *p = car(env);
    void *q;
    if (p == NIL) return 0;
    sym2 = (SYMBOL *)car(car(p));
    if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
        rplaca(env, cdr(p));
        return 1;
    }
    q = p;
    p = cdr(p);
    while (p != NIL) {
        sym2 = (SYMBOL *)car(car(p));
        if (strcmp(get_symbol_string(sym2), get_symbol_string(sym1)) == 0) {
            rplacd(q, cdr(p));
            return 1;
        }
        q = p;
        p = cdr(p);
    }
    return 0;
}

int environment_get_recurse(void *env, void *key, void **val) {
    while (env != NIL) {
        if (environment_get(env, key, val)) return 1;
        env = cdr(env);
    }
    return 0;
}

int environment_exists_recurse(void *env, void *key) {
    while (env != NIL) {
        if (environment_exists(env, key)) return 1;
        env = cdr(env);
    }
    return 0;
}

int environment_modify_recurse(void *env, void *key, void *val) {
    while (env != NIL) {
        if (environment_modify(env, key, val)) return 1;
        env = cdr(env);
    }
    return 0;
}

