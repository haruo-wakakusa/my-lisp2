/*
 * state.h
 */

#ifndef STATE_H_
#define STATE_H_

typedef enum {
    STATE_NORMAL,
    STATE_ERROR,
    STATE_EXIT,
    STATE_JUMP_RETURN,
    STATE_JUMP_GO
} STATE;

extern STATE state;
extern void *block_name_when_return;
extern void *return_value_when_return;
extern void *tag_name_when_go;

#endif

