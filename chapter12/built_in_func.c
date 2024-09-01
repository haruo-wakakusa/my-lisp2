/*
 * built_in_func.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../chapter02/type.h"
#include "../chapter05/state.h"
#include "../chapter05/environment.h"
#include "../chapter07/printer.h"
#include "../chapter10/helper.h"
#include "eval.h"
#include "built_in_func.h"

void *f_add(void *args) {
    double sum = 0.0;
    void *p = args;
    NUMBER *num;
    while (p != NIL) {
        HEADER *h = (HEADER *)car(p);
        if (h->type != TYPE_NUMBER) {
            fprintf(stderr, "FUNCTION \"+\": ");
            printer_print(stderr, car(p));
            fprintf(stderr, "は数値型ではありません\n");
            state = STATE_ERROR;
            return 0;
        }
        num = (NUMBER *)car(p);
        sum += num->num;
        p = cdr(p);
    }
    if (!isfinite(sum)) {
        fprintf(stderr, "FUNCTION \"+\": オーバーフローしました\n");
        state = STATE_ERROR;
        return 0;
    }
    num = (NUMBER *)malloc(sizeof(NUMBER));
    num->h.type = TYPE_NUMBER;
    num->num = sum;
    return (void *)num;
}

void *f_sub(void *args) {
    void *p = args;
    if (args == NIL) {
        fprintf(stderr, "FUNCTION \"-\": 引数の数が0です\n");
        state = STATE_ERROR;
        return 0;
    }
    while (p != NIL) {
        HEADER *h = (HEADER *)car(p);
        if (h->type != TYPE_NUMBER) {
            fprintf(stderr, "FUNCTION \"-\": ");
            printer_print(stderr, car(p));
            fprintf(stderr, "は数値型ではありません\n");
            state = STATE_ERROR;
            return 0;
        }
        p = cdr(p);
    }
    if (cdr(args) == NIL) {
        NUMBER *num1 = (NUMBER *)car(args);
        NUMBER *num2 = (NUMBER *)malloc(sizeof(NUMBER));
        num2->h.type = TYPE_NUMBER;
        num2->num = -num1->num;
        return (void *)num2;
    } else {
        NUMBER *num = (NUMBER *)car(args);
        double acc = num->num;
        p = cdr(args);
        while (p != NIL) {
            num = (NUMBER *)car(p);
            acc -= num->num;
            p = cdr(p);
        }
        if (!isfinite(acc)) {
            fprintf(stderr, "FUNCTION \"-\": オーバーフローしました\n");
            state = STATE_ERROR;
            return 0;
        }
        num = (NUMBER *)malloc(sizeof(NUMBER));
        num->h.type = TYPE_NUMBER;
        num->num = acc;
        return (void *)num;
    }
}

void *f_mul(void *args) {
    double product = 1.0;
    void *p = args;
    NUMBER *num;
    while (p != NIL) {
        HEADER *h = (HEADER *)car(p);
        if (h->type != TYPE_NUMBER) {
            fprintf(stderr, "FUNCTION \"*\": ");
            printer_print(stderr, car(p));
            fprintf(stderr, "は数値型ではありません\n");
            state = STATE_ERROR;
            return 0;
        }
        num = (NUMBER *)car(p);
        product *= num->num;
        p = cdr(p);
    }
    if (!isfinite(product)) {
        fprintf(stderr, "FUNCTION \"*\": オーバーフローしました\n");
        state = STATE_ERROR;
        return 0;
    }
    num = (NUMBER *)malloc(sizeof(NUMBER));
    num->h.type = TYPE_NUMBER;
    num->num = product;
    return (void *)num;
}

void *f_div(void *args) {
    void *p = args;
    if (args == NIL) {
        fprintf(stderr, "FUNCTION \"/\": 引数の数が0です\n");
        state = STATE_ERROR;
        return 0;
    }
    while (p != NIL) {
        HEADER *h = (HEADER *)car(p);
        if (h->type != TYPE_NUMBER) {
            fprintf(stderr, "FUNCTION \"/\": ");
            printer_print(stderr, car(p));
            fprintf(stderr, "は数値型ではありません\n");
            state = STATE_ERROR;
            return 0;
        }
        p = cdr(p);
    }
    if (cdr(args) == NIL) {
        NUMBER *num1 = (NUMBER *)car(args);
        double res = 1.0 / num1->num;
        if (!isfinite(res)) {
            fprintf(stderr, "FUNCTION \"/\": ゼロ除算が行われました\n");
            state = STATE_ERROR;
            return 0;
        }
        NUMBER *num2 = (NUMBER *)malloc(sizeof(NUMBER));
        num2->h.type = TYPE_NUMBER;
        num2->num = res;
        return (void *)num2;
    } else {
        NUMBER *num = (NUMBER *)car(args);
        double acc = num->num;
        p = cdr(args);
        while (p != NIL) {
            num = (NUMBER *)car(p);
            acc /= num->num;
            p = cdr(p);
        }
        if (!isfinite(acc)) {
            fprintf(stderr, "FUNCTION \"/\": ゼロ除算が行われました\n");
            state = STATE_ERROR;
            return 0;
        }
        num = (NUMBER *)malloc(sizeof(NUMBER));
        num->h.type = TYPE_NUMBER;
        num->num = acc;
        return (void *)num;
    }
}

void *f_number_equal(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"=\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"=\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"=\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    return (num1->num == num2->num) ? T : NIL;
}

void *f_number_gt(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \">\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \">\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0; 
    }       
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \">\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    return (num1->num > num2->num) ? T : NIL;
}

void *f_number_ge(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \">=\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \">=\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0; 
    }       
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \">=\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    return (num1->num >= num2->num) ? T : NIL;
}

void *f_number_lt(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"<\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"<\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0; 
    }       
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"<\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    return (num1->num < num2->num) ? T : NIL;
}

void *f_number_le(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"<=\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"<=\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0; 
    }       
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"<=\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    return (num1->num <= num2->num) ? T : NIL;
}

void *f_rem(void *args) {
    void *arg1;
    void *arg2;
    NUMBER *num1;
    NUMBER *num2;
    double res;
    NUMBER *num;
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"REM\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    arg1 = car(args);
    arg2 = car(cdr(args));
    if (((HEADER *)arg1)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"REM\": ");
        printer_print(stderr, arg1);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0; 
    }       
    if (((HEADER *)arg2)->type != TYPE_NUMBER) {
        fprintf(stderr, "FUNCTION \"REM\": ");
        printer_print(stderr, arg2);
        fprintf(stderr, "は数値型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    num1 = (NUMBER *)arg1;
    num2 = (NUMBER *)arg2;
    res = fmod(num1->num, num2->num);
    if (!isfinite(res)) {
        fprintf(stderr, "FUNCTION \"REM\": ゼロ除算が行われました\n");
        state = STATE_ERROR;
        return 0;
    }
    num = (NUMBER *)malloc(sizeof(NUMBER));
    num->h.type = TYPE_NUMBER;
    num->num = res;
    return (void *)num;
}

void *f_cons(void *args) {
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"CONS\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return cons(car(args), car(cdr(args)));
}

void *f_list(void *args) {
    return args;
}

void *f_consp(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"CONS\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return consp(car(args)) ? T : NIL;
}

void *f_car(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"CAR\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return car(car(args));
}

void *f_cdr(void *args) {
        if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"CDR\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return cdr(car(args));
}

void *f_rplaca(void *args) {
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"RPLACA\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return rplaca(car(args), car(cdr(args)));
}

void *f_rplacd(void *args) {
    if (list_length(args) != 2) {
        fprintf(stderr, "FUNCTION \"RPLACD\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return rplacd(car(args), car(cdr(args)));
}

void *f_symbolp(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"SYMBOLP\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return symbolp(car(args)) ? T : NIL;
}

void *f_listp(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"LISTP\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return listp(car(args)) ? T : NIL;
}

void *f_list_length(void *args) {
    size_t len;
    NUMBER *num;
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"LIST-LENGTH\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!listp(car(args))) {
        fprintf(stderr, "FUNCTION \"LIST-LENGTH\": プロパーなリストではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    len = list_length(car(args));
    num = (NUMBER *)malloc(sizeof(NUMBER));
    num->h.type = TYPE_NUMBER;
    num->num = (double)len;
    return num;
}

void *f_null(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"NULL\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    return (car(args) == NIL) ? T : NIL;
}

void *f_exit(void *args) {
    /* 本物のCommon Lispとは仕様が異なる */
    if (list_length(args) != 0) {
        fprintf(stderr, "FUNCTION \"EXIT\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    state = STATE_EXIT;
    return 0;
}

void *op_defvar(void *args, void *env_func, void *env_var) {
    void *val;
    if (list_length(args) != 2) {
        fprintf(stderr, "DEFVAR: 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!symbolp(car(args))) {
        fprintf(stderr, "DEFVAR: 引数1がシンボル型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    val = eval(car(cdr(args)), env_func, env_var);
    if (!val) return 0;
    if (!environment_exists(env_var_global, car(args))) {
        environment_add(env_var_global, car(args), val);
    }
    return car(args);
}

void *op_setq(void *args, void *env_func, void *env_var) {
    void *val;
    if (list_length(args) != 2) {
        fprintf(stderr, "SETQ: 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }       
    if (!symbolp(car(args))) { 
        fprintf(stderr, "SETQ: 引数1がシンボル型ではありません\n");
        state = STATE_ERROR;
        return 0;
    }   
    val = eval(car(cdr(args)), env_func, env_var);
    if (!val) return 0;
    if (!environment_exists_recurse(env_var, car(args))) {
        fprintf(stderr, "SETQ: %sという名前の変数はありません\n",
                        get_symbol_string(car(args)));
        state = STATE_ERROR;
        return 0;
    }
    environment_modify_recurse(env_var, car(args), val);
    return val;
}

void *op_let(void *args, void *env_func, void *env_var) {
    void *new_env_var = environment_init(env_var);
    void *var_exp_list = car(args);
    if (!listp(var_exp_list)) {
        fprintf(stderr, "LET: 引数1がリストではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    while (var_exp_list != NIL) {
        void *var_exp = car(var_exp_list);
        if (symbolp(var_exp)) {
            environment_add(new_env_var, var_exp, NIL);
        } else if (listp(var_exp)) {
            size_t len = list_length(var_exp);
            if (len == 1) {
                if (!symbolp(car(var_exp))) {
                    fprintf(stderr, "LET: 引数1が不正な形式です\n");
                    state = STATE_ERROR;
                    return 0;
                }
                environment_add(new_env_var, car(var_exp), NIL);
            } else if (len == 2) {
                void *val;
                if (!symbolp(car(var_exp))) {
                    fprintf(stderr, "LET: 引数1が不正な形式です\n");
                    state = STATE_ERROR;
                    return 0;
                }
                val = eval(car(cdr(var_exp)), env_func, env_var);
                if (!val) return 0;
                environment_add(new_env_var, car(var_exp), val);
            } else {
                fprintf(stderr, "LET: 引数1が不正な形式です\n");
                state = STATE_ERROR;
                return 0;
            }
        } else {
            fprintf(stderr, "LET: 引数1が不正な形式です\n");
            state = STATE_ERROR;
            return 0;
        }
        var_exp_list = cdr(var_exp_list);
    }
    {
        void *p = cdr(args);
        void *retval = NIL;
        while (p != NIL) {
            retval = eval(car(p), env_func, new_env_var);
            if (!retval) return 0;
            p = cdr(p);
        }
        return retval;
    }
}

void *op_let_star(void *args, void *env_func, void *env_var) {
    void *new_env_var = environment_init(env_var);
    void *var_exp_list = car(args);
    if (!listp(var_exp_list)) {
        fprintf(stderr, "LET: 引数1がリストではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    while (var_exp_list != NIL) {
        void *var_exp = car(var_exp_list);
        if (symbolp(var_exp)) {
            environment_add(new_env_var, var_exp, NIL);
        } else if (listp(var_exp)) {
            size_t len = list_length(var_exp);
            if (len == 1) {
                if (!symbolp(car(var_exp))) {
                    fprintf(stderr, "LET: 引数1が不正な形式です\n");
                    state = STATE_ERROR;
                    return 0;
                }
                environment_add(new_env_var, car(var_exp), NIL);
            } else if (len == 2) {
                void *val;
                if (!symbolp(car(var_exp))) {
                    fprintf(stderr, "LET: 引数1が不正な形式です\n");
                    state = STATE_ERROR;
                    return 0;
                }
                val = eval(car(cdr(var_exp)), env_func, new_env_var);
                if (!val) return 0;
                environment_add(new_env_var, car(var_exp), val);
            } else {
                fprintf(stderr, "LET: 引数1が不正な形式です\n");
                state = STATE_ERROR;
                return 0;
            }
        } else {
            fprintf(stderr, "LET: 引数1が不正な形式です\n");
            state = STATE_ERROR;
            return 0;
        }
        var_exp_list = cdr(var_exp_list);
    }
    {
        void *p = cdr(args);
        void *retval = NIL;
        while (p != NIL) {
            retval = eval(car(p), env_func, new_env_var);
            if (!retval) return 0;
            p = cdr(p);
        }
        return retval;
    }
}

void *op_if(void *args, void *env_func, void *env_var) {
    void *test_form = car(args);
    void *then_form = car(cdr(args));
    void *else_form = car(cdr(cdr(args)));
    void *val;
    size_t len = list_length(args);
    if (len <= 1) {
        fprintf(stderr, "IF: 引数が少なすぎます\n");
        state = STATE_ERROR;
        return 0;
    } else if (len >= 4) {
        fprintf(stderr, "IF: 引数が多すぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    val = eval(test_form, env_func, env_var);
    if (!val) return 0;
    if (val != NIL) {
        return eval(then_form, env_func, env_var);
    } else {
        return eval(else_form, env_func, env_var);
    }
}

