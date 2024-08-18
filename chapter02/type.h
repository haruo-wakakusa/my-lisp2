/*
 * type.h
 */

#ifndef TYPE_H_
#define TYPE_H_

typedef unsigned char TYPE;

const TYPE TYPE_SYMBOL = 0;
const TYPE TYPE_CONS = 1;
const TYPE TYPE_STRING = 2;
const TYPE TYPE_NUMBER = 3;
const TYPE TYPE_T = 4;
const TYPE TYPE_NULL = 5;
const TYPE TYPE_BUILT_IN_FUNC = 6;
const TYPE TYPE_USER_DEFINED_FUNC = 7;

/* オブジェクトの型を判別するためのヘッダー */
typedef struct HEADER {
    TYPE type;
} HEADER;

/* シンボル型。シンボル型は可変長のゼロ終端文字列データが直後に付随する */
typedef struct SYMBOL {
    HEADER h;
} SYMBOL;

/* 文字列型。文字列型は可変長のゼロ終端文字列データが直後に付随する */
typedef struct STRING {
    HEADER h;
} STRING;

typedef struct CONS {
    HEADER h;
    void *car;
    void *cdr;
} CONS;

typedef struct NUMBER {
    HEADER h;
    double num;
} NUMBER;

typedef struct T_TYPE {
    HEADER h;
} T_TYPE;

typedef struct NULL_TYPE {
    HEADER h;
} NULL_TYPE;

typedef struct BUILT_IN_FUNC {
    HEADER h;
    void *(*f)(void *, void *, void *);
} BUILT_IN_FUNC;

typedef struct USER_DEFINED_FUNC {
    HEADER h;
    void *body;
    void *func_env;
    void *var_env;
} USER_DEFINED_FUNC;

extern void *T;
extern void *NIL;

#endif
