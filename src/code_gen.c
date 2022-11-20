#include "code_gen.h"
#include <stdio.h>
#include "symtable.h"

/** TODO
 * Functions for generate functino calls
 * Checking params in function declaration
 * Generating builtin functions
 * Generating expression
 */

void generate_exit_label() {
    printf("# EXIT CODE LABELS\n");
    printf("LABEL $ERROR_SEM_TYPE_CHECK\n");
    printf("EXIT 4\n");
    printf("LABEL $ERROR_SEM_RET_EXP\n");
    printf("EXIT 6\n");
    printf("LABEL $ERROR_SEM_OP_TYPES\n");
    printf("EXIT 7\n");
    printf("LABEL $ERROR_SEM_OTHER\n");
    printf("EXIT 9\n");
}

void generate_header() {
    printf(".IFcode22\n");
    printf("DEFVAR GF@tmp_var\n");
    generate_exit_label();
    printf("JUMP $$main\n");
    printf("LABEL $$main\n");
}

void generate_end() {
    printf("# end of main\n");
}

void generate_func_header(char* func_id) {
    printf("LABEL $$%s\n", func_id);
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval1\n");
    printf("MOVE LF@retval1 nil@nil\n");
}

void generate_func_end() {
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generate_func_param(htab_item_t* param_data,
                         unsigned long long param_number) {
    printf("DEFVAR LF@%s\n", param_data->key);
    printf("MOVE LF@%s LF@%llu\n", param_data->key, param_number);
    printf("DEFVAR LF@%s_type\n", param_data->key);
    printf("TYPE LF@%s_type LF@%s\n", param_data->key, param_data->key);
    switch (param_data->data->var_data.data_type) {
        case DTYPE_FLOAT:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%s_notnil LF@%s_type string@nil\n",
                       param_data->key, param_data->key);
                printf("MOVE LF@%s float@0x0p+0\n", param_data->key);
                printf("JUMP $%s_continue\n", param_data->key);
                printf("LABEL $%s_notnil\n", param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@float\n",
                   param_data->key);
            printf("LABEL $%s_continue\n", param_data->key);
            break;
        case DTYPE_INT:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%s_notnil LF@%s_type string@nil\n",
                       param_data->key, param_data->key);
                printf("MOVE LF@%s int@0\n", param_data->key);
                printf("JUMP $%s_continue\n", param_data->key);
                printf("LABEL $%s_notnil\n", param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@int\n",
                   param_data->key);
            printf("LABEL $%s_continue\n", param_data->key);
            break;
        case DTYPE_STRING:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%s_notnil LF@%s_type string@nil\n",
                       param_data->key, param_data->key);
                printf("MOVE LF@%s string@\32\n", param_data->key);
                printf("JUMP $%s_continue\n", param_data->key);
                printf("LABEL $%s_notnil\n", param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@string\n",
                   param_data->key);
            printf("LABEL $%s_continue\n", param_data->key);
            break;
        default:
            break;
    }
}

void generate_global_var(char* var_id) {
    printf("DEFVAR GF@%s\n", var_id);
}

void generate_local_var(char* var_id) {
    printf("DEFVAR LF@%s\n", var_id);
}

void generate_tmp_frame() {
    printf("CREATEFRAME\n");
}
void generate_global_var_func_param(unsigned long long index, char* var_id) {
    printf("DEFVAR TF@%llu\n", index);
    printf("MOVE TF@%llu GF@%s\n", index, var_id);
}

void generate_local_var_func_param(unsigned long long index, char* var_id) {
    printf("DEFVAR TF@%llu\n", index);
    printf("MOVE TF@%llu LF@%s\n", index, var_id);
}

void generate_int_func_param(unsigned long long index, int value) {
    printf("DEFVAR TF@%llu\n", index);
    printf("MOVE TF@%llu int@%d\n", index, value);
}

void generate_float_func_param(unsigned long long index, double value) {
    printf("DEFVAR TF@%llu\n", index);
    printf("MOVE TF@%llu float@%f\n", index, value);
}

void generate_string_func_param(unsigned long long index, char* string) {
    printf("DEFVAR TF@%llu\n", index);
    // create function which transfer special character to escape sequences
    printf("MOVE TF@%llu string@%s\n", index, string);
}

void generate_null_func_param(unsigned long long index) {
    printf("DEFVAR TF@%llu\n", index);
    printf("MOVE TF@%llu nil@nil\n", index);
}

void generate_func_call(char* func_id) {
    printf("CALL $$%s\n", func_id);
}

void generate_global_assignment(char* var_id) {
    printf("MOVE GF@%s TF@retval1\n", var_id);
}

void generate_local_assignment(char* var_id) {
    printf("MOVE LF@%s TF@retval1\n", var_id);
}