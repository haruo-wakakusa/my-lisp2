/*
 * test.c
 */

#include <stdio.h>
#include "../chapter05/state.h"
#include "../chapter06/reader.h"
#include "printer.h"

int main() {
    reader_initialize();
    while (1) {
        printf("* ");
        void *obj = reader_read(stdin);
        if (!obj) {
            if (state == STATE_EXIT) {
                reader_free();
                return 0;
            } else if (state == STATE_ERROR) {
                state = STATE_NORMAL;
                continue;
            } else if (state == STATE_JUMP) {
                fprintf(stderr, "ジャンプに失敗しました\n");
                state = STATE_NORMAL;
                continue;
            } else {
                fprintf(stderr, "未実装のコードに到達しました\n");
                continue;
            }
        }
        printer_print(stdout, obj);
        fputc('\n', stdout);
    }
    return 0;
}

