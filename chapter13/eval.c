/*
 * eval.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../chapter02/type.h"
#include "../chapter04/cons_buffer.h"
#include "../chapter10/helper.h"
#include "../chapter05/environment.h"
#include "../chapter05/state.h"
#include "built_in_func.h"
#include "eval.h"

static void *eval_list(void *obj, void *env_func, void *env_var);
static void *eval_symbol(void *obj, void *env_func, void *env_var);
static void register_built_in_func(char *name, void *(* func)(void *));
static void register_special_operator(char *name,
                                      void *(* op)(void *, void *, void *));

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
    register_built_in_func(">", &f_number_gt);
    register_built_in_func(">=", &f_number_ge);
    register_built_in_func("<", &f_number_lt);
    register_built_in_func("<=", &f_number_le);
    register_built_in_func("REM", &f_rem);
    register_built_in_func("CONS", &f_cons);
    register_built_in_func("LIST", &f_list);
    register_built_in_func("CONSP", &f_consp);
    register_built_in_func("CAR", &f_car);
    register_built_in_func("CDR", &f_cdr);
    register_built_in_func("RPLACA", &f_rplaca);
    register_built_in_func("RPLACD", &f_rplacd);
    register_built_in_func("SYMBOLP", &f_symbolp);
    register_built_in_func("LISTP", &f_listp);
    register_built_in_func("LIST-LENGTH", &f_list_length);
    register_built_in_func("NULL", &f_null);
    register_built_in_func("EXIT", &f_exit);
    register_special_operator("DEFVAR", &op_defvar);
    register_special_operator("SETQ", &op_setq);
    register_special_operator("LET", &op_let);
    register_special_operator("LET*", &op_let_star);
    register_special_operator("IF", &op_if);
    register_special_operator("BLOCK", &op_block);
    register_special_operator("RETURN-FROM", &op_return_from);
    register_special_operator("TAGBODY", &op_tagbody);
    register_special_operator("GO", &op_go);
    register_built_in_func("PRIN1", &f_prin1);
    register_built_in_func("PRINT", &f_print);
    register_built_in_func("LOAD", &f_load);
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
            CONS_BUFFER cbuf = cons_buffer_allocate();
            while (p != NIL) {
                void *retval = eval(car(p), env_func, env_var);
                if (!retval) return 0;
                cons_buffer_add(cbuf, retval);
                p = cdr(p);
            }
            args = cons_buffer_get_list(cbuf);
            cons_buffer_free(cbuf);
            return f->f(args);
        } else if (h->type == TYPE_SPECIAL_OPERATOR) {
            SPECIAL_OPERATOR *op = (SPECIAL_OPERATOR *)obj2;
            return op->op(cdr(obj), env_func, env_var);
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

static void register_special_operator(char *name,
                                      void *(* op)(void *, void *, void *)) {
    SPECIAL_OPERATOR *op2 = (SPECIAL_OPERATOR *)malloc(sizeof(SPECIAL_OPERATOR));
    op2->h.type = TYPE_SPECIAL_OPERATOR;
    op2->op = op;
    environment_add(env_func_global, make_symbol(name), op2);
}

