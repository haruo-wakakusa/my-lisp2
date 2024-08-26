/*
 * type.h
 */

#ifndef TYPE_H_
#define TYPE_H_

typedef unsigned char TYPE;

#define TYPE_SYMBOL 0
#define TYPE_CONS 1
#define TYPE_STRING 2
#define TYPE_NUMBER 3
#define TYPE_BUILT_IN_FUNC 4
#define TYPE_USER_DEFINED_FUNC 5
#define TYPE_SPECIAL_OPERATOR 6

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

typedef struct BUILT_IN_FUNC {
    HEADER h;
    void *(*f)(void *);
} BUILT_IN_FUNC;

typedef struct USER_DEFINED_FUNC {
    HEADER h;
    void *body;
    void *env_func;
    void *env_var;
} USER_DEFINED_FUNC;

typedef struct SPECIAL_OPERATOR {
    HEADER h;
    void *(*op)(void *, void *, void *);
} SPECIAL_OPERATOR;

extern void *T;
extern void *NIL;

#endif
