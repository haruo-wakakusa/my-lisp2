/*
 * type.c
 */

#include "type.h"

T_TYPE T_BODY = { { TYPE_T } };
NULL_TYPE NIL_BODY = { { TYPE_NULL } };

void *T = (void *)&T_BODY;
void *NIL = (void *)&NIL_BODY;

