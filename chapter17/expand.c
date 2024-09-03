/*
 * expand.c
 */

#include <stdio.h>
#include <string.h>
#include "type.h"
#include "../chapter05/state.h"
#include "../chapter05/environment.h"
#include "../chapter14/helper.h"
#include "../chapter16/printer.h"
#include "eval.h"
#include "expand.h"

static void *expand_list(void *obj, void *env_func, void *env_var);
static void *expand_macro(
        void *name, void *macro, void *args, void *env_func, void *env_var);

int expanded = 0;

void *expand_all(void *obj) {
    do {
        obj = expand(obj, env_func_global, env_var_global);
        if (!obj) return 0;
    } while (expanded);
    return obj;
}

void *expand(void *obj, void *env_func, void *env_var) {
    HEADER *h = (HEADER *)obj;
    expanded = 0;
    switch (h->type) {
    case TYPE_CONS:
        return expand_list(obj, env_func, env_var);
    default:
        return obj;
    }
}

static void *expand_list(void *obj, void *env_func, void *env_var) {
    void *car1 = car(obj);
    SYMBOL *symbol;
    char *sym;
    void *obj2;
    void *p;
    if (!listp(obj)) {
        fprintf(stderr, "プロパーなリストではありません");
        state = STATE_ERROR;
        return 0;
    }
    if (!symbolp(car1)) {
        /* 展開中に先頭がシンボルでないリストが出てきた場合、
           スペシャルオペレーターの引数の一部などの可能性がある */
        p = obj;
        while (p != NIL) {
            void *retval = expand(car(p), env_func, env_var);
            if (!retval) return 0;
            rplaca(p, retval);
            p = cdr(p);
        }
        return obj;
    }
    symbol = (SYMBOL *)car1;
    sym = get_symbol_string(symbol);

    if (strcmp(sym, "QUOTE") == 0) {
        return obj;
    }

    if (environment_get_recurse(env_func, symbol, &obj2)) {
        HEADER *h = (HEADER *)obj2;
        if (h->type == TYPE_MACRO) {
            expanded = 1;
            return expand_macro(symbol, obj2, cdr(obj), env_func, env_var);
        }
    }
    p = cdr(obj);
    while (p != NIL) {
        void *retval = expand(car(p), env_func, env_var);
        if (!retval) return 0;
        rplaca(p, retval);
        p = cdr(p);
    }
    return obj;
}

static void *expand_macro(
        void *name, void *macro, void *args, void *env_func, void *env_var) {
    MACRO *m = (void *)macro;
    void *arglist = car(m->body);
    int has_rest_param = find_symbol("&REST", arglist);
    void *p;
    void *new_env_var;
    void *retval;
    if (has_rest_param) {
        if (list_length(args) < list_length(arglist) - 2) {
            fprintf(stderr, "MACRO \"%s\": 引数の数が一致しません\n",
                            get_symbol_string(name));
            state = STATE_ERROR;
            return 0;
        }
    } else {
        if (list_length(args) != list_length(arglist)) {
            fprintf(stderr, "FUNCTION \"%s\": 引数の数が一致しません\n",
                            get_symbol_string(name));
            state = STATE_ERROR;
            return 0;
        }
    }
    new_env_var = environment_init(m->env_var);
    if (has_rest_param) {
        while (strcmp("&REST", get_symbol_string(car(arglist))) != 0) {
            environment_add(new_env_var, car(arglist), car(args));
            args = cdr(args);
            arglist = cdr(arglist);
        }
        environment_add(new_env_var, car(cdr(arglist)), args);
    } else {
        while (arglist != NIL) {
            environment_add(new_env_var, car(arglist), car(args));
            args = cdr(args);
            arglist = cdr(arglist);
        }
    }
    retval = NIL;
    for (p = cdr(m->body); p != NIL; p = cdr(p)) {
        retval = eval(car(p), m->env_func, new_env_var);
        if (!retval) return 0;
    }
    return retval;
}

