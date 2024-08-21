/*
 * reader.c
 */

#include <stdio.h>
#include <ctype.h>
#include "../chapter02/type.h"
#include "helper.h"
#include "../chapter03/buffer.h"
#include "../chapter04/cons_buffer.h"
#include "../chapter05/state.h"
#include "reader.h"

BUFFER buffer;

static void *read_list(FILE *stream);
static void *read_string(FILE *stream);

void reader_initialize() {
    buffer = buffer_allocate();
}

void reader_free() {
    buffer_free(buffer);
}

void *reader_read(FILE *stream) {
    while (1) {
        int c;
LOOP1:
        c = fgetc(stream);
        if (c == EOF) {
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                return retval;
            } else {
                state = STATE_EXIT;
                return 0;
            }
        }
        switch ((char)c) {
        case '(':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                return read_list(stream);
            }
            break;
        case ')':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                fprintf(stderr, "余分な閉じ括弧です\n");
                state = STATE_ERROR;
                return 0;
            }
            break;
        case '"':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                return read_string(stream);
            }
            break;
        case ';':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                while (1) {
                    c = fgetc(stream);
                    if (c == EOF) {
                        state = STATE_EXIT;
                        return 0;
                    } else if (c == '\n') {
                        goto LOOP1;
                    }
                }
            }
            break;
        case '#':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    state = STATE_ERROR;
                    return 0;
                }
                switch (c) {
                case '\'':
                    return cons(make_symbol("FUNCTION"),
                                cons(reader_read(stream), NIL));
                default:
                    fprintf(stderr, "未実装のマクロ文字です\n");
                    state = STATE_ERROR;
                    return 0;
                }
            }
            break;
        case '\'':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                void *expr = reader_read(stream);
                if (!expr) return 0;
                return cons(make_symbol("QUOTE"), cons(expr, NIL));
            }
            break;
        case '`':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                void *expr = reader_read(stream);
                if (!expr) return 0;
                /* QUASIQUOTEはCommon LispではなくSchemeの語彙であることに注意 */
                return cons(make_symbol("QUASIQUOTE"), cons(expr, NIL));
            }
            break;
        case ',':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    state = STATE_ERROR;
                    return 0;
                }
                if (c != '@') {
                    ungetc(c, stream);
                    void *expr = reader_read(stream);
                    if (!expr) return 0;
                    /* UNQUOTEはSchemeの語彙であることに注意 */
                    return cons(make_symbol("UNQUOTE"), cons(expr, NIL));
                } else {
                    void *expr = reader_read(stream);
                    if (!expr) return 0;
                    /* UNQUOTE-SPLICINGはSchemeの語彙であることに注意 */
                    return cons(make_symbol("UNQUOTE-SPLICING"),
                                cons(expr, NIL));
                }
            }
            break;
        case '|':
            while (1) {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    state = STATE_ERROR;
                    return 0;
                }
                if (c == '\\') {
                    c = fgetc(stream);
                    if (c == EOF) {
                        fprintf(stderr, "ファイルの終わりに到達しました\n");
                        state = STATE_ERROR;
                        return 0;
                    }
                    buffer_write_char(buffer, c);
                } else if (c == '|') {
                    goto LOOP1;
                } else {
                    buffer_write_char(buffer, c);
                }
            }
            break;
        case '\\':
            c = fgetc(stream);
            if (c == EOF) {
                fprintf(stderr, "ファイルの終わりに到達しました\n");
                state = STATE_ERROR;
                return 0;
            }
            buffer_write_char(buffer, c);
            break;
        case ' ':
        case '\n':
        case '\t':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            }
            break;
        default:
            buffer_write_char(buffer, toupper(c));
        }
    }
}

static void *read_list(FILE *stream) {
    CONS_BUFFER cbuf = cons_buffer_allocate();
    while (1) {
        int c;
LOOP1:
        c = fgetc(stream);
        if (c == EOF) {
            fprintf(stderr, "ファイルの終わりに到達しました\n");
            cons_buffer_free(cbuf);
            state = STATE_ERROR;
            return 0;
        }
        switch ((char)c) {
        case '(':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            void *list1 = read_list(stream);
            if (!list1) {
                cons_buffer_free(cbuf);
                return 0;
            }
            cons_buffer_add(cbuf, list1);
            break;
        case ')':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            {
                void *retval = cons_buffer_get_list(cbuf);
                cons_buffer_free(cbuf);
                return retval;
            }
            break;
        case '"':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            {
                void *str = read_string(stream);
                if (!str) {
                    cons_buffer_free(cbuf);
                    return 0;
                }
                cons_buffer_add(cbuf, str);
            }
            break;
        case ';':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            while (1) {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    cons_buffer_free(cbuf);
                    state = STATE_ERROR;
                    return 0;
                } else if (c == '\n') {
                    goto LOOP1;
                }
            }
            break;
        case '#':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            c = fgetc(stream);
            if (c == EOF) {
                fprintf(stderr, "ファイルの終わりに到達しました\n");
                cons_buffer_free(cbuf);
                state = STATE_ERROR;
                return 0;
            }
            switch (c) {
            case '\'':
                {
                    void *expr = reader_read(stream);
                    if (!expr) {
                        cons_buffer_free(cbuf);
                        return 0;
                    }
                    cons_buffer_add(cbuf,
                        cons(make_symbol("FUNCTION"),
                            cons(expr, NIL)));
                }
                break;
            default:
                fprintf(stderr, "未実装のマクロ文字です");
                cons_buffer_free(cbuf);
                state = STATE_ERROR;
                return 0;
            }
            break;
        case '\'':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            } else {
                void *expr = reader_read(stream);
                if (!expr) {
                    cons_buffer_free(cbuf);
                    return 0;
                }
                cons_buffer_add(cbuf,
                    cons(make_symbol("QUOTE"), cons(expr, NIL)));
            }
            break;
        case '`':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            } else {
                void *expr = reader_read(stream);
                if (!expr) {
                    cons_buffer_free(cbuf);
                    return 0;
                }
                /* QUASIQUOTEはSchemeの語彙であることに注意 */
                cons_buffer_add(cbuf,
                    cons(make_symbol("QUASIQUOTE"), cons(expr, NIL)));
            }
            break;
        case ',':
            if (buffer_get_size(buffer) != 0) {
                void *retval = make_symbol_from_buffer(buffer);
                buffer_clear(buffer);
                ungetc(c, stream);
                return retval;
            } else {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    state = STATE_ERROR;
                    return 0;
                }
                if (c != '@') {
                    ungetc(c, stream);
                    void *expr = reader_read(stream);
                    if (!expr) {
                        cons_buffer_free(cbuf);
                        return 0;
                    }
                    /* UNQUOTEはSchemeの語彙であることに注意 */
                    cons_buffer_add(cbuf,
                        cons(make_symbol("UNQUOTE"), cons(expr, NIL)));
                } else {
                    void *expr = reader_read(stream);
                    if (!expr) return 0;
                    /* UNQUOTE-SPLICINGはSchemeの語彙であることに注意 */
                    cons_buffer_add(cbuf,
                        cons(make_symbol("UNQUOTE-SPLICING"),
                            cons(expr, NIL)));
                }
            }
            break;
        case '|':
            while (1) {
                c = fgetc(stream);
                if (c == EOF) {
                    fprintf(stderr, "ファイルの終わりに到達しました\n");
                    state = STATE_ERROR;
                    return 0;
                }
                if (c == '\\') {
                    c = fgetc(stream);
                    if (c == EOF) {
                        fprintf(stderr, "ファイルの終わりに到達しました\n");
                        state = STATE_ERROR;
                        return 0;
                    }
                    buffer_write_char(buffer, c);
                } else if (c == '|') {
                    goto LOOP1;
                } else {
                    buffer_write_char(buffer, c);
                }
            }
            break;
        case '\\':
            c = fgetc(stream);
            if (c == EOF) {
                fprintf(stderr, "ファイルの終わりに到達しました\n");
                state = STATE_ERROR;
                return 0;
            }
            buffer_write_char(buffer, c);
            break;
        case ' ':
        case '\n':
        case '\t':
            if (buffer_get_size(buffer) != 0) {
                cons_buffer_add(cbuf, make_symbol_from_buffer(buffer));
                buffer_clear(buffer);
            }
            break;
        default:
            buffer_write_char(buffer, toupper((char)c));
        }
    }
}

static void *read_string(FILE *stream) {
    while (1) {
        int c = fgetc(stream);
        if (c == EOF) {
            fprintf(stderr, "ファイルの終わりに到達しました\n");
            state = STATE_ERROR;
            return 0;
        }
        switch ((char)c) {
        case '"':
            {
                void *retval = make_string_from_buffer(buffer);
                buffer_clear(buffer);
                return retval;
            }
            break;
        case '\\':
            c = fgetc(stream);
            if (c == EOF) {
                fprintf(stderr, "ファイルの終わりに到達しました\n");
                state = STATE_ERROR;
                return 0;
            }
            buffer_write_char(buffer, c);
            break;
        default:
            buffer_write_char(buffer, c);
        }
    }
}

