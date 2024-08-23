/*
 * built_in_func.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../chapter02/type.h"
#include "../chapter05/state.h"
#include "../chapter07/printer.h"
#include "helper.h"
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

