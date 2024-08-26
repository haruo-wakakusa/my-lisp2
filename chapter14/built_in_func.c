/*
 * built_in_func.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../chapter02/type.h"
#include "../chapter05/state.h"
#include "../chapter05/environment.h"
#include "../chapter08/reader.h"
#include "../chapter09/dot.h"
#include "../chapter13/printer.h"
#include "helper.h"
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
        void *p = cdr(args);
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
        void *p = cdr(args);
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
    len = list_length(car(args));
    if (len == -1) {
        state = STATE_ERROR;
        return 0;
    }
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

void *op_block(void *args, void *env_func, void *env_var) {
    void *name;
    void *body;
    void *retval;
    if (args == NIL) {
        fprintf(stderr, "BLOCK: 引数が少なすぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    name = car(args);
    body = cdr(args);
    while (body != NIL) {
        retval = eval(car(body), env_func, env_var);
        if (!retval) {
            if (state == STATE_JUMP_RETURN) {
                if (strcmp(get_symbol_string(block_name_when_return),
                           get_symbol_string(name)) == 0) {
                    state = STATE_NORMAL;
                    return return_value_when_return;
                }
            }
            return 0;
        }
        body = cdr(body);
    }
    return retval;
}

void *op_return_from(void *args, void *env_func, void *env_var) {
    void *retval;
    if (args == NIL) {
        fprintf(stderr, "RETURN-FROM: 引数が少なすぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    if (list_length(args) >= 3) {
        fprintf(stderr, "RETURN_FROM: 引数が多すぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    block_name_when_return = car(args);
    retval = eval(car(cdr(args)), env_func, env_var);
    if (!retval) return 0;
    return_value_when_return = retval;
    state = STATE_JUMP_RETURN;
    return 0;
}

void *op_tagbody(void *args, void *env_func, void *env_var) {
    void *retval;
    void *p = args;
    while (p != NIL) {
    LOOP1:
        if (symbolp(car(p))) {
            p = cdr(p);
            continue;
        }
        retval = eval(car(p), env_func, env_var);
        if (!retval) {
            if (state == STATE_JUMP_GO) {
                p = args;
                while (p != NIL) {
                    if (symbolp(car(p))
                            && strcmp(
                                get_symbol_string(car(p)),
                                get_symbol_string(tag_name_when_go)) == 0) {
                        state = STATE_NORMAL;
                        goto LOOP1;
                    }
                    p = cdr(p);
                }
            }
            return 0;
        }
        p = cdr(p);
    }
    return NIL;
}

void *op_go(void *args, void *env_func, void *env_var) {
    if (list_length(args) != 1) {
        fprintf(stderr, "GO: 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!symbolp(car(args))) {
        fprintf(stderr, "GO: 引数がシンボルではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    state = STATE_JUMP_GO;
    tag_name_when_go = car(args);
    return 0;
}

void *f_prin1(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"PRIN1\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    printer_print(stdout, car(args));
    is_printed_in_eval = 1;
    return car(args);
}

void *f_print(void *args) {
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"PRINT\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    putc('\n', stdout);
    printer_print(stdout, car(args));
    putc(' ', stdout);
    is_printed_in_eval = 1;
    return car(args);
}

void *f_load(void *args) {
    STRING *filename;
    FILE *fp;
    void *obj;
    if (list_length(args) != 1) {
        fprintf(stderr, "FUNCTION \"LOAD\": 引数の数が不正です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!stringp(car(args))) {
        fprintf(stderr, "FUNCTION \"LOAD\": 引数が文字列ではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    filename = (STRING *)car(args);
    fp = fopen(get_string_string(filename), "r");
    if (!fp) return NIL;
    while (1) {
        obj = reader_read(fp);
        if (!obj) {
            if (state == STATE_EXIT) {
                break;
            } else if (state == STATE_ERROR) {
                fclose(fp);
                return 0;
            } else {
                fprintf(stderr, "未実装のコードに到達しました\n");
                fclose(fp);
                return 0;
            }
        }
        fix_to_dotted_list(obj);
        if (!check_single_dot(obj)) {
            fprintf(stderr, "ドットをシンボルとして使用することはできません\n");
            state = STATE_ERROR;
            fclose(fp);
            return 0;
        }
        obj = eval_top(obj);
        if (!obj) {
            if (state == STATE_EXIT) {
                break;
            } else if (state == STATE_ERROR) {
                fclose(fp);
                return 0;
            } else if (state == STATE_JUMP_RETURN || state == STATE_JUMP_GO) {
                fprintf(stderr, "ジャンプに失敗しました\n");
                fclose(fp);
                state = STATE_ERROR;
                return 0;
            } else {
                fprintf(stderr, "未実装のコードに到達しました\n");
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    state = STATE_NORMAL;
    return T;
}

void *op_defun(void *args, void *env_func, void *env_var) {
    void *name = car(args);
    void *arglist = car(cdr(args));
    void *p;
    USER_DEFINED_FUNC *f;
    if (!symbolp(name)) {
        fprintf(stderr, "関数名がシンボルではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!listp(arglist)
            || !all_symbol_p(arglist)
            || !unique_symbol_list_p(arglist)) {
        fprintf(stderr, "引数リストが不正な形式です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (find_symbol("&OPTIONAL", arglist)) {
        fprintf(stderr, "オプション引数は非対応です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (find_symbol("&KEY", arglist)) {
        fprintf(stderr, "キーワード引数は非対応です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (find_symbol("&AUX", arglist)) {
        fprintf(stderr, "&AUXの指定は非対応です\n");
        state = STATE_ERROR;
        return 0;
    }
    if (find_symbol("&ALLOW-OTHER-KEYS", arglist)) {
        fprintf(stderr, "&ALLOW-OTHER-KEYSの指定は非対応です\n");
        state = STATE_ERROR;
        return 0;
    }
    p = arglist;
    while (p != NIL && strcmp("&REST", get_symbol_string(car(p))) != 0)
        p = cdr(p);
    if (p != NIL && list_length(p) != 2) {
        fprintf(stderr, "&RESTが不正な場所にあります\n");
        state = STATE_ERROR;
        return 0;
    }
    f = (USER_DEFINED_FUNC *)malloc(sizeof(USER_DEFINED_FUNC));
    f->h.type = TYPE_USER_DEFINED_FUNC;
    f->body = cdr(args);
    f->env_func = env_func;
    f->env_var = env_var;
    if (environment_exists(env_func_global, name)) {
        environment_modify(env_func_global, name, f);
    } else {
        environment_add(env_func_global, name, f);
    }
    return name;
}

void *op_progn(void *args, void *env_func, void *env_var) {
    void *retval;
    for ( ; args != NIL; args = cdr(args)) {
        retval = eval(car(args), env_func, env_var);
        if (!retval) return 0;
    }
    return retval;
}

void *op_flet(void *args, void *env_func, void *env_var) {
    void *funclist = car(args);
    void *new_env_func = environment_init(env_func);
    void *retval = NIL;
    if (args == NIL) {
        fprintf(stderr, "FLET: 引数が少なすぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!listp(funclist)) {
        fprintf(stderr, "FLET: 関数リストがリストではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    while (funclist != NIL) {
        void *func = car(funclist);
        void *name;
        void *arglist;
        void *p;
        USER_DEFINED_FUNC *f;
        if (!listp(func) || list_length(func) < 2) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        name = car(func);
        arglist = car(cdr(func));
        if (!symbolp(name)
                || !listp(arglist)
                || !all_symbol_p(arglist)
                || !unique_symbol_list_p(arglist)
                || find_symbol("&OPTIONAL", arglist)
                || find_symbol("&KEY", arglist)
                || find_symbol("&AUX", arglist)
                || find_symbol("&ALLOW-OTHER-KEYS", arglist)) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        p = arglist;
        while (p != NIL && strcmp("&REST", get_symbol_string(car(p))) != 0)
            p = cdr(p);
        if (p != NIL && list_length(p) != 2) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        f = (USER_DEFINED_FUNC *)malloc(sizeof(USER_DEFINED_FUNC));
        f->h.type = TYPE_USER_DEFINED_FUNC;
        f->body = cdr(func);
        f->env_func = env_func;
        f->env_var = env_var;
        if (!environment_exists(new_env_func, name)) {
            environment_add(new_env_func, name, f);
        } else {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        funclist = cdr(funclist);
    }
    for (args = cdr(args); args != NIL; args = cdr(args)) {
        retval = eval(car(args), new_env_func, env_var);
        if (!retval) return 0;
    }
    return retval;
}

void *op_labels(void *args, void *env_func, void *env_var) {
    void *funclist = car(args);
    void *new_env_func = environment_init(env_func);
    void *retval = NIL;
    if (args == NIL) {
        fprintf(stderr, "FLET: 引数が少なすぎます\n");
        state = STATE_ERROR;
        return 0;
    }
    if (!listp(funclist)) {
        fprintf(stderr, "FLET: 関数リストがリストではありません\n");
        state = STATE_ERROR;
        return 0;
    }
    while (funclist != NIL) {
        void *func = car(funclist);
        void *name;
        void *arglist;
        void *p;
        USER_DEFINED_FUNC *f;
        if (!listp(func) || list_length(func) < 2) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        name = car(func);
        arglist = car(cdr(func));
        if (!symbolp(name)
                || !listp(arglist)
                || !all_symbol_p(arglist)
                || !unique_symbol_list_p(arglist)
                || find_symbol("&OPTIONAL", arglist)
                || find_symbol("&KEY", arglist)
                || find_symbol("&AUX", arglist)
                || find_symbol("&ALLOW-OTHER-KEYS", arglist)) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        p = arglist;
        while (p != NIL && strcmp("&REST", get_symbol_string(car(p))) != 0)
            p = cdr(p);
        if (p != NIL && list_length(p) != 2) {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        f = (USER_DEFINED_FUNC *)malloc(sizeof(USER_DEFINED_FUNC));
        f->h.type = TYPE_USER_DEFINED_FUNC;
        f->body = cdr(func);
        f->env_func = new_env_func;
        f->env_var = env_var;
        if (!environment_exists(new_env_func, name)) {
            environment_add(new_env_func, name, f);
        } else {
            fprintf(stderr, "FLET: 関数リスト内の記述が不正です\n");
            state = STATE_ERROR;
            return 0;
        }
        funclist = cdr(funclist);
    }
    for (args = cdr(args); args != NIL; args = cdr(args)) {
        retval = eval(car(args), new_env_func, env_var);
        if (!retval) return 0;
    }
    return retval;
}
