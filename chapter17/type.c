/*
 * type.c
 */

#include "type.h"

char T_BODY[] = { TYPE_SYMBOL, 'T', '\0' };
char NIL_BODY[] = { TYPE_SYMBOL, 'N', 'I', 'L', '\0' };

void *T = (void *)&T_BODY;
void *NIL = (void *)&NIL_BODY;

