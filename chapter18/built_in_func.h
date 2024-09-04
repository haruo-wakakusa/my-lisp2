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
void *op_if(void *args, void *env_func, void *env_var);
void *op_block(void *args, void *env_func, void *env_var);
void *op_return_from(void *args, void *env_func, void *env_var);
void *op_tagbody(void *args, void *env_func, void *env_var);
void *op_go(void *args, void *env_func, void *env_var);
void *f_prin1(void *args);
void *f_print(void *args);
void *f_load(void *args);
void *op_defun(void *args, void *env_func, void *env_var);
void *op_progn(void *args, void *env_func, void *env_var);
void *op_flet(void *args, void *env_func, void *env_var);
void *op_labels(void *args, void *env_func, void *env_var);
void *op_function(void *args, void *env_func, void *env_var);
void *f_funcall(void *args);
void *f_apply(void *args);
void *op_quote(void *args, void *env_func, void *env_var);
void *f_gensym(void *args);
void *op_defmacro(void *args, void *env_func, void *env_var);
void *op_psetq(void *args, void *env_func, void *env_var);

#endif

