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

void generate_builtin_func() {
    printf("#FLOATVAL\n");
    printf("LABEL $$floatval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@%retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $floatval$int LF@type$var string@int\n");

    printf("MOVE LF@%retval$1 float@0.0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $floatval$int\n");
    printf("INT2FLOAT LF@%retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#INTVAL\n");
    printf("LABEL $$intval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@%retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $intval$float LF@type$var string@float\n");

    printf("MOVE LF@%retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $intval$float\n");
    printf("FLOAT2INT LF@%retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#STRVAL\n");
    printf("LABEL $$strval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@%retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $strval$null LF@type$var nil@nil\n");

    printf("MOVE LF@%retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $strval$null\n");
    printf(
        "MOVE LF@%retval$1 string@"
        "\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#STRLEN\n");
    printf("LABEL $$strlen\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@$1\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");

    printf("DEFVAR LF@%retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $strlen$null LF@type$var nil@nil\n");

    printf("STRLEN LF@%retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $strlen$null\n");
    printf("MOVE LF@%retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#SUBSTRING\n");
    printf("LABEL $$substring\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$s\n");
    printf("POPS LF@$s\n");
    printf("DEFVAR LF@$i\n");
    printf("POPS LF@$i\n");
    printf("DEFVAR LF@$j\n");
    printf("POPS LF@$j\n");
    printf("DEFVAR LF@%retval$1\n");
    printf("JUMPIFEQ $givenstring$null LF@$s nil@nil\n");

    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@$s\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");
    printf("TYPE LF@$type LF@$i\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int\n");
    printf("TYPE LF@$type LF@$j\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int\n");

    printf("DEFVAR LF@$tmp\n");
    printf("DEFVAR LF@$stringlen\n");
    printf("STRLEN LF@stringlen LF@$s\n");
    printf("LT LF@$tmp LF@$i int@0\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("LT LF@$tmp LF@$j int@0\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("GT LF@$tmp LF@$i LF@$j\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("LT LF@$tmp LF@$i LF@stringlen\n");
    printf("JUMPIFNEQ $substring$null LF@$tmp bool@true\n");
    printf("GT LF@$tmp LF@$j LF@stringlen\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");

    printf("DEFVAR LF@$var1\n");
    printf("DEFVAR LF@$var2\n");
    printf("LABEL $substring$notnull\n");
    printf("GETCHAR LF@$var1 LF@$s LF@$i\n");
    printf("CONCAT LF@%retval$1 LF@%retval$1 LF@$var1\n");
    printf("ADD LF@$i LF@$i int@1\n");
    printf("LT LF@$var2 LF@$i LF@$j\n");
    printf("JUMPIFEQ $substring$notnull LF@$var2 bool@true\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $givenstring$null\n");
    printf(
        "MOVE LF@%retval$1 string@"
        "\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $substring$null\n");
    printf("MOVE LF@%retval$1 nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#ORD\n");
    printf("LABEL $$ord\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$c\n");
    printf("POPS LF@$c\n");
    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@$c\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");
    printf("JUMPIFEQ $ord$null LF@$c nil@nil\n");

    printf("DEFVAR LF@%retval$1\n");
    printf("STRI2INT LF@%retval$1 LF@$c int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $ord$null\n");
    printf("MOVE LF@%retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#CHR\n");
    printf("LABEL $$chr\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@chr@int\n");
    printf("DEFVAR LF@%retval$1\n");

    printf("INT2CHAR LF@%retval$1 LF@chr@int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READS\n");
    printf("LABEL $$reads\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@%retval$1\n");
    printf("READ LF@%retval$1 string@string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READI\n");
    printf("LABEL $$readi\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@%retval$1\n");
    printf("READ LF@%retval$1 string@int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READF\n");
    printf("LABEL $$readf\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@%retval$1\n");
    printf("READ LF@%retval$1 string@float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}