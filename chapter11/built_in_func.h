/*
 * built_in_func.h
 */

#ifndef BUILT_IN_FUNC_H_
#define BUILT_IN_FUNC_H_

void *f_add(void *args);
void *f_sub(void *args);
void *f_mul(void *args);
void *f_div(void *args);
void *f_number_equal(void *args);
void *f_number_gt(void *args);
void *f_number_ge(void *args);
void *f_number_lt(void *args);
void *f_number_le(void *args);
void *f_rem(void *args);
void *f_cons(void *args);
void *f_list(void *args);
void *f_consp(void *args);
void *f_car(void *args);
void *f_cdr(void *args);
void *f_rplaca(void *args);
void *f_rplacd(void *args);
void *f_symbolp(void *args);
void *f_listp(void *args);
void *f_list_length(void *args);
void *f_null(void *args);
void *f_exit(void *args);
void *op_defvar(void *args, void *env_func, void *env_var);
void *op_setq(void *args, void *env_func, void *env_var);
void *op_let(void *args, void *env_func, void *env_var);
void *op_let_star(void *args, void *env_func, void *env_var);

#endif

