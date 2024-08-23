/*
 * dot.c
 */

#include <string.h>
#include "../chapter02/type.h"
#include "helper.h"

/*
 * この関数は、リーダーで得られたオブジェクトに対して適用するので、
 * リストはドッティドリストを含まないことを想定している。
 */
void fix_to_dotted_list(void *obj) {
    if (consp(obj)) {
        /* リスト上の要素全てに再帰を適用する */
        {
            void *p = obj;
            while (p != NIL) {
                fix_to_dotted_list(car(p));
                p = cdr(p);
            }
        }
        /* リストの長さを数える */
        {
            size_t len = 0;
            void *p = obj;
            void *q;
            void *r;
            while (p != NIL) {
                len++;
                p = cdr(p);
            }
            /* 最後から３つ目に移動 */
            if (len < 3) return;
            p = obj;
            for (len -= 3; len != 0; len--) {
                p = cdr(p);
            }
            /* 最後から２つ目がドット(.)のシンボルであるかを調べる */
            q = cdr(p);
            r = cdr(q);
            if (!symbolp(car(q)))
                return;
            if (strcmp(get_symbol_string((SYMBOL *)(car(q))), ".") != 0)
                return;
            /* 最後の要素を３番目のcdrに付け替える */
            rplacd(p, car(r));
        }
    }
}

/*
 * 非0ならばOK、0ならばNG
 */
int check_single_dot(void *obj) {
    if (symbolp(obj)) {
        SYMBOL *s = (SYMBOL *)obj;
        return strcmp(get_symbol_string(s), ".");
    }
    if (consp(obj)) {
        void *p = obj;
        while (consp(p)) {
            if (!check_single_dot(car(p))) return 0;
            p = cdr(p);
        }
        if (p != NIL && !check_single_dot(p)) return 0;
    }
    return 1;
}

