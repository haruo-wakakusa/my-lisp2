/*
 * eval.h
 */

#ifndef EVAL_H_
#define EVAL_H_

extern void *env_func_global;
extern void *env_var_global;

void eval_init();
void *eval_top(void *obj);
void *eval(void *obj, void *env_func, void *env_var);

#endif

