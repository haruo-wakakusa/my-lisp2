/*
 * state.h
 */

#ifndef STATE_H_
#define STATE_H_

typedef enum {
    STATE_NORMAL,
    STATE_ERROR,
    STATE_EXIT,
    STATE_JUMP
} STATE;

extern STATE state;

#endif

