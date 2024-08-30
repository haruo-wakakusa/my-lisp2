/*
 * test.c
 */

#include <stdio.h>
#include "../chapter05/state.h"
#include "../chapter08/reader.h"
#include "../chapter07/printer.h"
#include "dot.h"

int main(void) {
    reader_initialize();
    while (1) {
        printf("> ");
        void *obj = reader_read(stdin);
        if (!obj) {
            if (state == STATE_EXIT) {
                reader_free();
                return 0;
            } else if (state == STATE_ERROR) {
                state = STATE_NORMAL;
                continue;
            } else {
                fprintf(stderr, "未実装のコードに到達しました\n");
                continue;
            }
        }
        fix_to_dotted_list(obj);
        if (!check_single_dot(obj)) {
            fprintf(stderr, "ドットをシンボルとして使用することはできません\n");
            continue;
        }
        printer_print(stdout, obj);
        fputc('\n', stdout);
    }
    return 0;
}

