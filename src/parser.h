/*
Name: IFJ PROJEKT 2022
Authors: xdolez0c, xvecer30, xnespo10, xtomko06
Description: --
*/

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdbool.h>
#include "code_gen.h"
#include "exp_parser.h"
#include "scanner.h"
#include "symtable.h"

typedef struct {
    token_t* token;
    htab_t* global_symtable;
    htab_t* local_symtable;
    htab_item_data_t* global_symtable_data;
    htab_item_data_t* local_symtable_data;
    htab_item_data_t* declared_function;
    bool in_function;
    bool func_check;
    bool in_while;
    size_t scope;
    size_t param_counter;
} Parser;

void get_next_token();
void get_token_consume_token(token_type token_type, char* err_msg);
void consume_token(token_type token_type, char* err_msg);

bool check_param_types();
bool check_return_type();
bool check_data_type(token_type token_type);

// funtions for rule checking
bool type();
bool program();
bool list_params();
bool syntax_analyse();
bool next_parameter();
bool list_input_params();
bool next_input_parameter();
bool term();
bool return_type();
bool statement();
// void prolog();
#endif
