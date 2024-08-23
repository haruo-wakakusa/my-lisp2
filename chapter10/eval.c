/*
 * eval.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../chapter02/type.h"
#include "helper.h"
#include "../chapter05/environment.h"
#include "../chapter05/state.h"
#include "built_in_func.h"
#include "eval.h"

static void *eval_list(void *obj, void *env_func, void *env_var);
static void *eval_symbol(void *obj, void *env_func, void *env_var);
static void register_built_in_func(char *name, void *(* func)(void *));

void *env_func_global = 0;
void *env_var_global = 0;

void eval_init() {
    env_func_global = environment_init(NIL);
    env_var_global = environment_init(NIL);

    register_built_in_func("+", &f_add);
    register_built_in_func("-", &f_sub);
    register_built_in_func("*", &f_mul);
    register_built_in_func("/", &f_div);
    register_built_in_func("=", &f_number_equal);
}

void *eval_top(void *obj) {
    return eval(obj, env_func_global, env_var_global);
}

void *eval(void *obj, void *env_func, void *env_var) {
    HEADER *h = (HEADER *)obj;
    switch (h->type) {
    case TYPE_CONS:
        return eval_list(obj, env_func, env_var);
    case TYPE_SYMBOL:
        return eval_symbol(obj, env_func, env_var);
    default:
        return obj;
    }
}

static void *eval_list(void *obj, void *env_func, void *env_var) {
    void *car1 = car(obj);
    SYMBOL *symbol;
    char *sym;
    void *obj2;
    if (!listp(obj)) {
        fprintf(stderr, "プロパーなリストではありません");
        state = STATE_ERROR;
        return 0;
    }
    if (!symbolp(car1)) {
        fprintf(stderr, "不正な関数です\n");
        state = STATE_ERROR;
        return 0;
    }
    symbol = (SYMBOL *)car1;
    sym = get_symbol_string(symbol);

    if (environment_get_recurse(env_func, symbol, &obj2)) {
        HEADER *h = (HEADER *)obj2;
        if (h->type == TYPE_BUILT_IN_FUNC) {
            BUILT_IN_FUNC *f = (BUILT_IN_FUNC *)obj2;
            void *args = cdr(obj);
            void *p = args;
            while (p != NIL) {
                void *retval = eval(car(p), env_func, env_var);
                if (!retval) return 0;
                rplaca(p, retval);
                p = cdr(p);
            }
            return f->f(args);
        } else {
            fprintf(stderr, "未実装のコードに到達しました\n");
            state = STATE_ERROR;
            return 0;
        }
    } else {
        fprintf(stderr, "%sという名前の関数がありません\n", sym);
        state = STATE_ERROR;
        return 0;
    }
}

static void *eval_symbol(void *obj, void *env_func, void *env_var) {
    SYMBOL *symbol = (SYMBOL *)obj;
    char *sym = get_symbol_string(symbol);
    void *retval = 0;
    if (strcmp(sym, "NIL") == 0) {
        return NIL;
    } else if (strcmp(sym, "T") == 0) {
        return T;
    } else if (environment_get_recurse(env_var, symbol, &retval)) {
        return retval;
    } else {
        fprintf(stderr, "%sという名前の変数がありません\n", sym);
        state = STATE_ERROR;
        return 0;
    }
}

static void register_built_in_func(char *name, void *(* func)(void *)) {
    BUILT_IN_FUNC *f = (BUILT_IN_FUNC *)malloc(sizeof(BUILT_IN_FUNC));
    f->h.type = TYPE_BUILT_IN_FUNC;
    f->f = func;
    environment_add(env_func_global, make_symbol(name), f);
}

