#include <stdbool.h>

void generate_header();
void generate_end();
void generate_func_header(char* func_id);
void generate_func_end();
void generate_func_param();
void generate_global_var(char* var_id);
void generate_local_var(char* var_id);
void generate_tmp_frame();
void generate_global_var_func_param(unsigned long long index, char* var_id);
void generate_local_var_func_param(unsigned long long index, char* var_id);
void generate_int_func_param(unsigned long long index, int value);
void generate_float_func_param(unsigned long long index, double value);
void generate_string_func_param(unsigned long long index, char* string);
void generate_null_func_param(unsigned long long index);
void generate_func_call(char* func_id);
void generate_local_assignment(char* var_id);
void generate_global_assignment(char* var_id);