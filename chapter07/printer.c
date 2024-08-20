/*
 * printer.c
 */

#include <stdio.h>
#include "../chapter02/type.h"
#include "helper.h"
#include "printer.h"

static void print_symbol(FILE *stream, void *obj);
static void print_list(FILE *stream, void *obj);
static void print_string(FILE *stream, void *obj);
static void print_number(FILE *stream, void *obj);
static void print_t(FILE *stream, void *obj);
static void print_null(FILE *stream, void *obj);

void printer_print(FILE *stream, void *obj) {
    HEADER *h;

    if (!obj) {
        fprintf(stream, "[!!!NULL POINTER!!!]");
        return;
    }
    h = (HEADER *)obj;
    switch (h->type) {
    case TYPE_SYMBOL:
        print_symbol(stream, obj);
        break;
    case TYPE_CONS:
        print_list(stream, obj);
        break;
    case TYPE_STRING:
        print_string(stream, obj);
        break;
    case TYPE_NUMBER:
        print_number(stream, obj);
        break;
    case TYPE_T:
        print_t(stream, obj);
        break;
    case TYPE_NULL:
        print_null(stream, obj);
    default:
        fprintf(stderr, "未実装のコードに到達しました\n");
    }
}

static void print_symbol(FILE *stream, void *obj) {
    SYMBOL *symbol = (SYMBOL *)obj;
    fprintf(stream, "%s", get_symbol_string(symbol));
}

static void print_list(FILE *stream, void *obj) {
    CONS *cons1 = (CONS *)obj;
    fputc('(', stream);
    printer_print(stream, cons1->car);
    obj = cons1->cdr;
    while (1) {
        if (obj == NIL) {
            break;
        } else if (!consp(obj)) {
            fprintf(stream, " . ");
            printer_print(stream, obj);
            break;
        }
        cons1 = (CONS *)obj;
        fputc(' ', stream);
        printer_print(stream, cons1->car);
        obj = cons1->cdr;
    }
    fputc(')', stream);
}

static void print_string(FILE *stream, void *obj) {
    STRING *string = (STRING *)obj;
    fputc('"', stream);
    fprintf(stream, "%s", get_string_string(string));
    fputc('"', stream);
}

static void print_number(FILE *stream, void *obj) {
    NUMBER *number = (NUMBER *)obj;
    fprintf(stream, "%lf", number->num);
}

static void print_t(FILE *stream, void *obj) {
    fputc('T', stream);
}

static void print_null(FILE *stream, void *obj) {
    fputs("NIL", stream);
}

