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
    printf("MOVE LF@%s, LF@%llu\n", param_data->key, param_number);
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

void generate_builtin_func() {
    printf("#FLOATVAL");
    printf("LABEL $$floatval");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$1");
    printf("POPS LF@$1");
    printf("DEFVAR LF@%retval$1");
    printf("DEFVAR LF@type$var");
    printf("TYPE LF@type$var LF@$1");
    printf("JUMPIFEQ $floatval$int LF@type$var string@int");

    printf("MOVE LF@%retval$1 float@0.0");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $floatval$int");
    printf("INT2FLOAT LF@%retval$1 LF@$1");
    printf("POPFRAME");
    printf("RETURN");

    printf("#INTVAL");
    printf("LABEL $$intval");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$1");
    printf("POPS LF@$1");
    printf("DEFVAR LF@%retval$1");
    printf("DEFVAR LF@type$var");
    printf("TYPE LF@type$var LF@$1");
    printf("JUMPIFEQ $intval$float LF@type$var string@float");

    printf("MOVE LF@%retval$1 int@0");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $intval$float");
    printf("FLOAT2INT LF@%retval$1 LF@$1");
    printf("POPFRAME");
    printf("RETURN");

    printf("#STRVAL");
    printf("LABEL $$strval");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$1");
    printf("POPS LF@$1");
    printf("DEFVAR LF@%retval$1");
    printf("DEFVAR LF@type$var");
    printf("TYPE LF@type$var LF@$1");
    printf("JUMPIFEQ $strval$null LF@type$var nil@nil");

    printf("MOVE LF@%retval$1 LF@$1");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $strval$null");
    printf(
        "MOVE LF@%retval$1 string@"
        "");
    printf("POPFRAME");
    printf("RETURN");

    printf("#STRLEN");
    printf("LABEL $$strlen");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$1");
    printf("POPS LF@$1");
    printf("DEFVAR LF@$type");
    printf("TYPE LF@$type LF@$1");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string");

    printf("DEFVAR LF@%retval$1");
    printf("DEFVAR LF@type$var");
    printf("TYPE LF@type$var LF@$1");
    printf("JUMPIFEQ $strlen$null LF@type$var nil@nil");

    printf("STRLEN LF@%retval$1 LF@$1");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $strlen$null");
    printf("MOVE LF@%retval$1 int@0");
    printf("POPFRAME");
    printf("RETURN");

    printf("#SUBSTRING");
    printf("LABEL $$substring");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$s");
    printf("POPS LF@$s");
    printf("DEFVAR LF@$i");
    printf("POPS LF@$i");
    printf("DEFVAR LF@$j");
    printf("POPS LF@$j");
    printf("DEFVAR LF@%retval$1");
    printf("JUMPIFEQ $givenstring$null LF@$s nil@nil");

    printf("DEFVAR LF@$type");
    printf("TYPE LF@$type LF@$s");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string");
    printf("TYPE LF@$type LF@$i");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int");
    printf("TYPE LF@$type LF@$j");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int");

    printf("DEFVAR LF@$tmp");
    printf("DEFVAR LF@$stringlen");
    printf("STRLEN LF@stringlen LF@$s");
    printf("LT LF@$tmp LF@$i int@0");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true");
    printf("LT LF@$tmp LF@$j int@0");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true");
    printf("GT LF@$tmp LF@$i LF@$j");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true");
    printf("LT LF@$tmp LF@$i LF@stringlen");
    printf("JUMPIFNEQ $substring$null LF@$tmp bool@true");
    printf("GT LF@$tmp LF@$j LF@stringlen");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true");

    printf("DEFVAR LF@$var1");
    printf("DEFVAR LF@$var2");
    printf("LABEL $substring$notnull");
    printf("GETCHAR LF@$var1 LF@$s LF@$i");
    printf("CONCAT LF@%retval$1 LF@%retval$1 LF@$var1");
    printf("ADD LF@$i LF@$i int@1");
    printf("LT LF@$var2 LF@$i LF@$j");
    printf("JUMPIFEQ $substring$notnull LF@$var2 bool@true");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $givenstring$null");
    printf(
        "MOVE LF@%retval$1 string@"
        "");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $substring$null");
    printf("MOVE LF@%retval$1 nil@nil");
    printf("POPFRAME");
    printf("RETURN");

    printf("#ORD");
    printf("LABEL $$ord");
    printf("PUSHFRAME");

    printf("DEFVAR LF@$c");
    printf("POPS LF@$c");
    printf("DEFVAR LF@$type");
    printf("TYPE LF@$type LF@$c");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string");
    printf("JUMPIFEQ $ord$null LF@$c nil@nil");

    printf("DEFVAR LF@%retval$1");
    printf("STRI2INT LF@%retval$1 LF@$c int@0");
    printf("POPFRAME");
    printf("RETURN");

    printf("LABEL $ord$null");
    printf("MOVE LF@%retval$1 int@0");
    printf("POPFRAME");
    printf("RETURN");

    printf("#CHR");
    printf("LABEL $$chr");
    printf("PUSHFRAME");

    printf("DEFVAR LF@chr@int");
    printf("DEFVAR LF@%retval$1");

    printf("INT2CHAR LF@%retval$1 LF@chr@int");
    printf("POPFRAME");
    printf("RETURN");

    printf("#READS");
    printf("LABEL $$reads");
    printf("PUSHFRAME");

    printf("DEFVAR LF@%retval$1");
    printf("READ LF@%retval$1 string@string");
    printf("POPFRAME");
    printf("RETURN");

    printf("#READI");
    printf("LABEL $$readi");
    printf("PUSHFRAME");

    printf("DEFVAR LF@%retval$1");
    printf("READ LF@%retval$1 string@int");
    printf("POPFRAME");
    printf("RETURN");

    printf("#READF");
    printf("LABEL $$readf");
    printf("PUSHFRAME");

    printf("DEFVAR LF@%retval$1");
    printf("READ LF@%retval$1 string@float");
    printf("POPFRAME");
    printf("RETURN");
}