#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** TODO
 * Generating expression
 * IF
 * WHILE
 * RETURN TYPE CHECK
 */

// Functions for dynamic string
void string_add_char(dynamic_string_t* string, char c) {
    if (string->usedmem + 1 >= (string->stringmem * 0.9)) {
        string->stringmem = string->stringmem * 2;
        string->string = (char*)realloc(string->string, string->stringmem);
        string->usedmem = 0;
    }
    if (c != '"') {
        string->string[string->usedmem++] = c;
    }
    string->string[string->usedmem] = '\0';
}

void string_add_string(dynamic_string_t* string, char* string2) {
    int string_len = strlen(string2);
    if ((string->usedmem + string_len) >= (string->stringmem * 0.9)) {
        string->stringmem = string->stringmem * 2;
        string->string = (char*)realloc(string->string, string->stringmem);
        string->usedmem = 0;
    }
    string->usedmem += string_len;
    strcat(string->string, string2);
    string->string[string->usedmem] = '\0';
}

void generate_exit_label() {
    printf("# EXIT CODE LABELS\n");
    printf("LABEL $ERROR_SEM_TYPE_CHECK\n");
    printf(
        "DPRINT "
        "string@semantic\\032error:"
        "\\032wrong\\032number\\032or\\032type\\032in\\032function\\032call\n");
    printf("EXIT int@4\n");
    printf("LABEL $ERROR_SEM_RET_EXP\n");
    printf(
        "DPRINT "
        "string@semantic\\032error:"
        "\\032missing\\032expression\\032in\\032function\\032return\n");
    printf("EXIT int@6\n");
    printf("LABEL $ERROR_SEM_OP_TYPES\n");
    printf(
        "DPRINT "
        "string@semantic\\032error:"
        "\\032wrong\\032type\\032in\\032expression\\032operands\n");
    printf("EXIT int@7\n");
    printf("LABEL $ERROR_SEM_OTHER\n");
    printf("DPRINT string@semantic\\032error\n");
    printf("EXIT int@8\n");
}

void generate_header() {
    printf(".IFJcode22\n");
    printf("DEFVAR GF@tmp_var\n");
    printf("JUMP $$main\n");
    generate_exit_label();
    generate_builtin_func();
    printf("# START OF MAIN BODY\n");
    printf("LABEL $$main\n");
}

void generate_end() {
    printf("# end of main\n");
}

void generate_func_header(char* func_id, int scope) {
    printf("JUMP $$main%d\n", scope);
    printf("LABEL $$%s\n", func_id);
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval$1\n");
    printf("MOVE LF@retval$1 nil@nil\n");
}

void generate_func_end(int scope) {
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $$main%d\n", scope);
}

void generate_func_param(htab_item_t* param_data,
                         unsigned long long param_number) {
    printf("DEFVAR LF@%s\n", param_data->key);
    printf("MOVE LF@%s LF@$%llu\n", param_data->key, param_number);
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
void generate_global_var_func_param(unsigned long long index,
                                    char* var_id,
                                    bool is_write) {
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu GF@%s\n", index, var_id);
    } else
        printf("WRITE GF@%s", var_id);
}

void generate_local_var_func_param(unsigned long long index,
                                   char* var_id,
                                   bool is_write) {
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu LF@%s\n", index, var_id);
    } else
        printf("WRITE LF@%s\n", var_id);
}

void generate_int_func_param(unsigned long long index,
                             int value,
                             bool is_write) {
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu int@%d\n", index, value);
    } else
        printf("WRITE int@%d\n", value);
}

void generate_float_func_param(unsigned long long index,
                               double value,
                               bool is_write) {
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu float@%a\n", index, value);
    } else
        printf("WRITE float@%a\n", value);
}

void generate_string_func_param(unsigned long long index,
                                char* string,
                                bool is_write) {
    dynamic_string_t* tmp_string =
        (dynamic_string_t*)malloc(sizeof(dynamic_string_t));
    tmp_string->stringmem = 100;
    tmp_string->usedmem = 0;
    tmp_string->string = (char*)calloc(tmp_string->stringmem, sizeof(char*));
    char c;
    char escape_seq[10];

    for (int i = 0; (c = string[i]) != '\0'; i++) {
        if (c == '#' || c == '\\' || c <= 32) {
            string_add_char(tmp_string, '\\');
            sprintf(escape_seq, "%03d", c);
            string_add_string(tmp_string, escape_seq);
        } else {
            string_add_char(tmp_string, c);
        }
    }
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu string@%s\n", index, tmp_string->string);
    } else
        printf("WRITE string@%s\n", tmp_string->string);
    free(tmp_string);
}

void generate_null_func_param(unsigned long long index, bool is_write) {
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu nil@nil\n", index);
    } else
        printf("WRITE string@\n");
}

void generate_func_call(char* func_id) {
    printf("CALL $$%s\n", func_id);
    printf("MOVE GF@tmp_var TF@retval$1\n");
}

void generate_global_assignment(char* var_id) {
    printf("MOVE GF@%s GF@tmp_var\n", var_id);
}

void generate_local_assignment(char* var_id) {
    printf("MOVE LF@%s GF@tmp_var\n", var_id);
}

void generate_null_assignment() {
    printf("MOVE GF@tmp_var nil@nil\n");
}

void generate_builtin_func() {
    printf("#FLOATVAL\n");
    printf("LABEL $$floatval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $floatval$int LF@type$var string@int\n");

    printf("MOVE LF@retval$1 float@0x0.0p+0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $floatval$int\n");
    printf("INT2FLOAT LF@retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#INTVAL\n");
    printf("LABEL $$intval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $intval$float LF@type$var string@float\n");

    printf("MOVE LF@retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $intval$float\n");
    printf("FLOAT2INT LF@retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#STRVAL\n");
    printf("LABEL $$strval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$1\n");
    printf("POPS LF@$1\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $strval$null LF@type$var nil@nil\n");

    printf("MOVE LF@retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $strval$null\n");
    printf(
        "MOVE LF@retval$1 string@"
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

    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@$1\n");
    printf("JUMPIFEQ $strlen$null LF@type$var nil@nil\n");

    printf("STRLEN LF@retval$1 LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $strlen$null\n");
    printf("MOVE LF@retval$1 int@0\n");
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
    printf("DEFVAR LF@retval$1\n");
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
    printf("CONCAT LF@retval$1 LF@retval$1 LF@$var1\n");
    printf("ADD LF@$i LF@$i int@1\n");
    printf("LT LF@$var2 LF@$i LF@$j\n");
    printf("JUMPIFEQ $substring$notnull LF@$var2 bool@true\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $givenstring$null\n");
    printf(
        "MOVE LF@retval$1 string@"
        "\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $substring$null\n");
    printf("MOVE LF@retval$1 nil@nil\n");
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

    printf("DEFVAR LF@retval$1\n");
    printf("STRI2INT LF@retval$1 LF@$c int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $ord$null\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#CHR\n");
    printf("LABEL $$chr\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@chr$int\n");
    printf("DEFVAR LF@retval$1\n");

    printf("INT2CHAR LF@retval$1 LF@chr$int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READS\n");
    printf("LABEL $$reads\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@retval$1\n");
    printf("READ LF@retval$1 string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READI\n");
    printf("LABEL $$readi\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@retval$1\n");
    printf("READ LF@retval$1 int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#READF\n");
    printf("LABEL $$readf\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@retval$1\n");
    printf("READ LF@retval$1 float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generate_if_then(int scope) {
    printf("# IF THEN %d\n", scope);
    printf("JUMPIFEQ $$if%delse GF@tmp_var bool@false\n", scope);
}

void generate_if_else(int scope) {
    printf("JUMP $$if%dend\n", scope);
    printf("# IF ELSE %d\n", scope);
    printf("LABEL $$if%delse\n", scope);
}

void generate_if_end(int scope) {
    printf("# IF END %d\n", scope);
    printf("LABEL $$if%dend\n", scope);
}

void generate_while_start(int scope) {
    printf("# WHILE START %d\n", scope);
    printf("LABEL $$while%dstart\n", scope);
    printf("JUMPIFEQ $$while%dend GF@tmp_var bool@false\n", scope);
}

void generate_while_end(int scope) {
    printf("JUMP $$while%dstart\n", scope);
    printf("# WHILE END %d\n", scope);
    printf("LABEL $$while%dend\n", scope);
}

void generate_ast(ast_node_t* current) {
    // left
    // right
    switch (current->token.lex) {}
}
