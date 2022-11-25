/*
Name: IFJ PROJEKT 2022
Authors: xdolez0c, xvecer30, xnespo10, xtomko06
Description: --
*/

#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** TODO
 * RETURN TYPE CHECK
 * Semantic checks for type incompatibility in arithmetic and relation
 * operations
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

char* formate_string(char* string) {
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
    return tmp_string->string;
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
    printf("LABEL $PROGRAM_GOOD\n");
    printf("EXIT int@0\n");
}

void generate_header() {
    printf(".IFJcode22\n");
    printf("DEFVAR GF@tmp_var\n");

    // ast exp tmp vars
    printf("DEFVAR GF@exp_tmp1\n");
    printf("DEFVAR GF@exp_tmp2\n");
    printf("DEFVAR GF@exp_result1\n");
    printf("DEFVAR GF@exp_result2\n");
    printf("DEFVAR GF@exp_type1\n");
    printf("DEFVAR GF@exp_type2\n");

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
    printf("JUMP $$%s_declare\n", func_id);
    printf("LABEL $$%s_do\n", func_id);
    printf("DEFVAR LF@retval$1\n");
    printf("MOVE LF@retval$1 nil@nil\n");
}

void check_optional_type(char* type) {
    printf("EQ GF@exp_result1 GF@exp_type1 string@%s\n", type);
    printf("EQ GF@exp_result2 GF@exp_type1 string@nil\n");
    printf("PUSHS GF@exp_result1\n");
    printf("PUSHS GF@exp_result2\n");
    printf("ORS\n");
    printf("POPS GF@exp_result1\n");
    printf(
        "JUMPIFEQ $ERROR_SEM_TYPE_CHECK GF@exp_result1 "
        "bool@false\n");
}

void generate_func_end(int scope, htab_item_data_t* func_data) {
    printf("LABEL $$%s_return\n", func_data->name);
    switch (func_data->func_data.ret_type) {
        case RETTYPE_FLOAT:
            printf("TYPE GF@exp_type1 LF@retval$1\n");
            if (func_data->func_data.optional_ret_type)
                check_optional_type("float");
            else
                printf(
                    "JUMPIFNEQ $ERROR_SEM_TYPE_CHECK GF@exp_type1 "
                    "string@float\n");
            break;
        case RETTYPE_INT:
            printf("TYPE GF@exp_type1 LF@retval$1\n");
            if (func_data->func_data.optional_ret_type)
                check_optional_type("int");
            else
                printf(
                    "JUMPIFNEQ $ERROR_SEM_TYPE_CHECK GF@exp_type1 "
                    "string@int\n");
            break;
        case RETTYPE_STRING:
            printf("TYPE GF@exp_type1 LF@retval$1\n");
            if (func_data->func_data.optional_ret_type)
                check_optional_type("string");
            else
                printf(
                    "JUMPIFNEQ $ERROR_SEM_TYPE_CHECK GF@exp_type1 "
                    "string@string\n");
            break;
    }
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $$main%d\n", scope);
}

void generate_var_definition(char* var) {
    printf("DEFVAR LF@%s\n", var);
}

void generate_func_declaration(htab_t* table, char* func_id) {
    printf("LABEL $$%s_return\n");
    printf("LABEL $$%s_declare\n", func_id);
    for (size_t i = 0; i < table->arr_size; i++) {
        htab_item_t* item = table->arr_ptr[i];
        while (item != NULL) {
            if (item->data->type == ID_VAR) {
                generate_var_definition(item->data->name);
            }
            item = item->next;
        }
    }
    printf("JUMP $$%s_do\n", func_id);
}

void generate_func_param(htab_item_t* param_data,
                         unsigned long long param_number,
                         int scope) {
    printf("MOVE LF@%s LF@$%llu\n", param_data->key, param_number);
    printf("DEFVAR LF@%s_type\n", param_data->key);
    printf("TYPE LF@%s_type LF@%s\n", param_data->key, param_data->key);
    switch (param_data->data->var_data.data_type) {
        case DTYPE_FLOAT:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%d%s_notnil LF@%s_type string@nil\n", scope,
                       param_data->key, param_data->key);
                printf("MOVE LF@%s float@0x0p+0\n", param_data->key);
                printf("JUMP $%d%s_continue\n", scope, param_data->key);
                printf("LABEL $%d%s_notnil\n", scope, param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@float\n",
                   param_data->key);
            printf("LABEL $%d%s_continue\n", scope, param_data->key);
            break;
        case DTYPE_INT:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%d%s_notnil LF@%s_type string@nil\n", scope,
                       param_data->key, param_data->key);
                printf("MOVE LF@%s int@0\n", param_data->key);
                printf("JUMP $%d%s_continue\n", scope, param_data->key);
                printf("LABEL $%d%s_notnil\n", scope, param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@int\n",
                   param_data->key);
            printf("LABEL $%d%s_continue\n", scope, param_data->key);
            break;
        case DTYPE_STRING:
            if (param_data->data->var_data.optional_type) {
                printf("JUMPIFNEQ $%d%s_notnil LF@%s_type string@nil\n", scope,
                       param_data->key, param_data->key);
                printf("MOVE LF@%s string@\32\n", param_data->key);
                printf("JUMP $%d%s_continue\n", scope, param_data->key);
                printf("LABEL $%d%s_notnil\n", scope, param_data->key);
            }
            printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@%s_type string@string\n",
                   param_data->key);
            printf("LABEL $%d%s_continue\n", scope, param_data->key);
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
        printf("WRITE GF@%s\n", var_id);
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
    char* tmp_string;
    tmp_string = formate_string(string);
    if (!is_write) {
        printf("DEFVAR TF@$%llu\n", index);
        printf("MOVE TF@$%llu string@%s\n", index, tmp_string);
    } else
        printf("WRITE string@%s\n", tmp_string);
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

void generate_exp_local_assignment(char* var_id) {
    printf("POPS LF@%s\n", var_id);
}

void generate_exp_global_assignment(char* var_id) {
    printf("POPS GF@%s\n", var_id);
}

void generate_null_assignment() {
    printf("MOVE GF@tmp_var nil@nil\n");
}

void generate_builtin_func() {
    // printf("#FLOATVAL\n");
    // printf("LABEL $$floatval\n");
    // printf("PUSHFRAME\n");

    // printf("DEFVAR LF@param1\n");
    // printf("MOVE LF@param1 LF@$1\n");
    // printf("DEFVAR LF@retval$1\n");
    // printf("DEFVAR LF@type$var\n");
    // printf("TYPE LF@type$var LF@param1\n");
    // printf("JUMPIFEQ $floatval$int LF@type$var string@int\n");
    // printf("JUMPIFEQ $floatval$null LF@type$var string@nil\n");

    // printf("MOVE LF@retval$1 LF@param1\n");
    // printf("POPFRAME\n");
    // printf("RETURN\n");

    // printf("LABEL $floatval$null \n");
    // printf("INT2FLOAT LF@retval$1 float@0x0.0p+0\n");
    // printf("POPFRAME\n");
    // printf("RETURN\n");

    // printf("LABEL $floatval$int\n");
    // printf("INT2FLOAT LF@retval$1 LF@param1\n");
    // printf("POPFRAME\n");
    // printf("RETURN\n");

    // INTVAL

    printf("#INTVAL\n");
    printf("LABEL $$intval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@$1\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@param1\n");
    printf("JUMPIFEQ $intval$float LF@type$var string@float\n");
    printf("JUMPIFEQ $intval$null LF@type$var string@nil\n");

    printf("MOVE LF@retval$1 LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $intval$float\n");
    printf("FLOAT2INT LF@retval$1 LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $intval$null \n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#STRVAL\n");
    printf("LABEL $$strval\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@$1\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@param11\n");
    printf("JUMPIFEQ $strval$null LF@type$var string@nil\n");

    printf("MOVE LF@retval$1 LF@param1\n");
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

    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@$1\n");
    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@param1\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");

    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@type$var\n");
    printf("TYPE LF@type$var LF@param1\n");
    printf("JUMPIFEQ $strlen$null LF@type$var string@nil\n");

    printf("STRLEN LF@retval$1 LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $strlen$null\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("#SUBSTRING\n");
    printf("LABEL $$substring\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@param_s\n");
    printf("MOVE LF@param_s LF@$1\n");
    printf("DEFVAR LF@param_i\n");
    printf("MOVE LF@param_i LF@$2\n");
    printf("DEFVAR LF@param_j\n");
    printf("MOVE LF@param_j LF@$3\n");
    printf("DEFVAR LF@retval$1\n");
    printf("MOVE LF@retval$1 string@\n");
    printf("JUMPIFEQ $givenstring$null LF@param_s nil@nil\n");

    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@param_s\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");
    printf("TYPE LF@$type LF@param_i\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int\n");
    printf("TYPE LF@$type LF@param_j\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@int\n");

    printf("DEFVAR LF@$tmp\n");
    printf("DEFVAR LF@$stringlen\n");
    printf("STRLEN LF@$stringlen LF@param_s\n");
    printf("LT LF@$tmp LF@param_i int@0\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("LT LF@$tmp LF@param_j int@0\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("GT LF@$tmp LF@param_i LF@param_j\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");
    printf("LT LF@$tmp LF@param_i LF@$stringlen\n");
    printf("JUMPIFNEQ $substring$null LF@$tmp bool@true\n");
    printf("GT LF@$tmp LF@param_j LF@$stringlen\n");
    printf("JUMPIFEQ $substring$null LF@$tmp bool@true\n");

    printf("DEFVAR LF@$var1\n");
    printf("DEFVAR LF@$var2\n");
    printf("LABEL $substring$notnull\n");
    printf("GETCHAR LF@$var1 LF@param_s LF@param_i\n");
    printf("CONCAT LF@retval$1 LF@retval$1 LF@$var1\n");
    printf("ADD LF@param_i LF@param_i int@1\n");
    printf("LT LF@$var2 LF@param_i LF@param_j\n");
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
    printf("DEFVAR LF@retval$1\n");
    printf("MOVE LF@retval$1 int@0\n");

    printf("DEFVAR LF@param_c\n");
    printf("MOVE LF@param_c LF@$1\n");
    printf("DEFVAR LF@$type\n");
    printf("TYPE LF@$type LF@param_c\n");
    printf("JUMPIFNEQ $ERROR_SEM_TYPE_CHECK LF@$type string@string\n");
    printf("JUMPIFEQ $ord$null LF@param_c string@\n");

    printf("STRI2INT LF@retval$1 LF@param_c int@0\n");
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
}

void generate_while_condition(int scope) {
    printf("JUMPIFEQ $$while%dend GF@tmp_var bool@false\n", scope);
}

void generate_while_end(int scope) {
    printf("JUMP $$while%dstart\n", scope);
    printf("# WHILE END %d\n", scope);
    printf("LABEL $$while%dend\n", scope);
}

void generate_return(char* func_id) {
    printf("POPS LF@retval$1\n");
    printf("JUMP $$%s_return\n", func_id);
}

void generate_exit_program() {
    printf("JUMP $PROGRAM_GOOD\n");
}

void generate_ast(ast_node_t* current, bool in_function) {
    char* tmp_string;
    if (current == NULL) {
        return;
    }
    generate_ast(current->left, in_function);
    generate_ast(current->right, in_function);

    switch (current->token.token_type) {
        case L_DOT:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("TYPE GF@exp_type1 GF@exp_tmp1\n");
            printf("TYPE GF@exp_type2 GF@exp_tmp2\n");
            printf(
                "JUMPIFNEQ $ERROR_SEM_OP_TYPES GF@exp_type1 string@string\n");
            printf(
                "JUMPIFNEQ $ERROR_SEM_OP_TYPES GF@exp_type2 string@string\n");

            printf("CONCAT GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_MUL:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("MUL GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_SLASH:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            // printf("TYPE GF@exp_type1 GF@exp_tmp1\n");
            // printf("TYPE GF@exp_type2 GF@exp_tmp2\n");
            // printf("JUMPIFEQ $type1_to_float%d GF@exp_type1 string@int\n",
            // *cnt); printf("JUMPIFNEQ $ERROR_SEM_OP_TYPES GF@exp_type1
            // string@float\n");
            printf("DIV GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_PLUS:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("ADD GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_DASH:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("SUB GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_NUMBER:
            // printf("");
            printf("PUSHS int@%lld\n", current->token.val);
            break;
        case L_STRING:
            tmp_string = formate_string(current->token.string);
            printf("PUSHS string@%s\n", tmp_string);
            break;
        case L_EQ:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("EQ GF@tmp_var GF@exp_tmp1 GF@exp_tmp2\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_EXP:
            printf("PUSHS float@%a\n", current->token.float_val);
            break;
        case L_NEQ:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("EQ GF@tmp_var GF@exp_tmp1 GF@exp_tmp2\n");
            printf("PUSHS GF@tmp_var\n");
            printf("NOTS\n");
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_LESS:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("LT GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_GREATER:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("GT GF@tmp_var GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_LESSEQ:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("LT GF@exp_result1 GF@exp_tmp2 GF@exp_tmp1\n");
            printf("EQ GF@exp_result2 GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@exp_result1\n");
            printf("PUSHS GF@exp_result2\n");
            printf("ORS\n");
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_GREATEREQ:
            printf("POPS GF@exp_tmp1\n");
            printf("POPS GF@exp_tmp2\n");
            printf("GT GF@exp_result1 GF@exp_tmp2 GF@exp_tmp1\n");
            printf("EQ GF@exp_result2 GF@exp_tmp2 GF@exp_tmp1\n");
            printf("PUSHS GF@exp_result1\n");
            printf("PUSHS GF@exp_result2\n");
            printf("ORS\n");
            printf("POPS GF@tmp_var\n");
            printf("PUSHS GF@tmp_var\n");
            break;
        case L_FLOAT:
            printf("PUSHS float@%a\n", current->token.float_val);
            break;
        case L_VARID:
            if (in_function)
                printf("PUSHS LF@%s\n", current->token.string);
            else
                printf("PUSHS GF@%s\n", current->token.string);
            break;
        case K_NULL:
            printf("PUSHS nil@nil\n");
            break;
    }
}
