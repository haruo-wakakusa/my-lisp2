/*
 * parser.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../chapter07/helper.h"
#include "../chapter03/buffer.h"
#include "../chapter05/state.h"

static int has_colon(char *str) {
    while (*str) {
        if (*str == ':') return 1;
        str++;
    }
    return 0;
}

static int integerp(char *str) {
    if (*str == '+' || *str == '-') str++;
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

static int ratiop(char *str) {
    if (!strchr(str, '/')) return 0;
    if (*str == '+' || *str == '-') str++;
    if (*str == '/') return 0;
    while (*str != '/') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    str++;
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

static int floatp(char *str) {
    int no_numerators;
    if (!strchr(str, '.')) return 0;
    if (*str == '+' || *str == '-') str++;
    no_numerators = (*str == '.');
    while (*str != '.') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    str++;
    if (no_numerators && *str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

void *parser_parse(BUFFER buf) {
    size_t len = buffer_get_size(buf) + 1;
    char *str = (char *)malloc(len);
    buffer_copy(buf, str);
    str[len - 1] = '\0';
    void *retval = 0;
    if (has_colon(str)) {
        fprintf(stderr, "パッケージは非対応です\n");
        state = STATE_ERROR;
        retval = 0;
    } else if (ratiop(str)) {
        fprintf(stderr, "有理数は非対応です\n");
        state = STATE_ERROR;
        retval = 0;
    } else if (integerp(str) || floatp(str)) {
        double d;
        if (sscanf(str, "%lf", &d) != 1 || !isfinite(d)) {
            fprintf(stderr, "オーバーフローしました\n");
            state = STATE_ERROR;
            retval = 0;
        } else {
            NUMBER *num = (NUMBER *)malloc(sizeof(NUMBER));
            num->h.type = TYPE_NUMBER;
            num->num = d;
            retval = num;
        }
    } else {
        retval = make_symbol(str);
    }
    free(str);
    return retval;
}

