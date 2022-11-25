#ifndef IFJ_CODE_GEN_H
#define IFJ_CODE_GEN_H

#include <stdbool.h>
#include "ast.h"
#include "symtable.h"

typedef struct dynamic_string {
    int usedmem;
    int stringmem;
    char* string;
} dynamic_string_t;

void generate_header();
void generate_end();
void generate_func_header(char* func_id, int scope);
void generate_func_end(int scope, htab_item_data_t* func_data);
void generate_func_param(htab_item_t* param_data,
                         unsigned long long param_number,
                         int scope);
void generate_global_var(char* var_id);
void generate_local_var(char* var_id);
void generate_tmp_frame();
void generate_global_var_func_param(unsigned long long index,
                                    char* var_id,
                                    bool is_write);
void generate_local_var_func_param(unsigned long long index,
                                   char* var_id,
                                   bool is_write);
void generate_int_func_param(unsigned long long index,
                             int value,
                             bool is_write);
void generate_float_func_param(unsigned long long index,
                               double value,
                               bool is_write);
void generate_string_func_param(unsigned long long index,
                                char* string,
                                bool is_write);
void generate_null_func_param(unsigned long long index, bool is_write);
void generate_func_call(char* func_id);
void generate_local_assignment(char* var_id);
void generate_global_assignment(char* var_id);
void generate_exp_local_assignment(char* var_id);
void generate_exp_global_assignment(char* var_id);
void generate_builtin_func();
void generate_null_assignment();
void generate_if_then(int scope);
void generate_if_else(int scope);
void generate_if_end(int scope);
void generate_while_start(int scope);
void generate_while_condition(int scope);
void generate_while_end(int scope);
void generate_ast(ast_node_t* current, bool in_function);
void generate_return(char* func_id);
void generate_exit_program();
#endif
