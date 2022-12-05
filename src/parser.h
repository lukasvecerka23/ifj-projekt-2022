/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file of parser.c
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include "code_gen.h"
#include "error.h"
#include "exp_parser.h"
#include "scanner.h"
#include "symtable.h"

/**
 * @struct Parser struct which obtain necessary data for parsing
 */
typedef struct {
    token_t* token;  // actual token
    htab_t* global_symtable;
    htab_t* local_symtable;                  // actual local symtable
    htab_item_data_t* global_symtable_data;  // actual global symtable data
    htab_item_data_t* local_symtable_data;   // actual local symtable data
    htab_item_data_t*
        declared_function;  // data about function which being declared
    bool in_function;
    bool func_check;
    bool in_while_if;
    size_t scope;  // counter to unify labels in code gen
    size_t param_counter;
} Parser;

/**
 * @brief Global instance of parser structure
 */
Parser parser;

/**
 * Ask scanner for next token and store it to parser structure
 */
void get_next_token();

/**
 * Get next token from scanner and check its type,
 * if token doesn't match token type exit program with
 * given error message
 * @param token_type token type to check
 * @param err_msg error message to print on stderr
 */
void get_token_consume_token(token_type token_type, char* err_msg);

/**
 * Check type of token, if doesn't match exit program
 * with given error message
 * @param token_type token type to check
 * @param err_msg error message to print
 */
void consume_token(token_type token_type, char* err_msg);

/**
 * Check type of token
 * @param token_type token type to check
 * @return true if token match, false if not
 */
bool check_token_type(token_type token_type);

/**
 * Free local/global symtable, exit program with given error code
 * and print message to stderr
 * @param err_code error code to exit
 * @param msg message to print
 */
void clear_and_exit_program(int err_code, char* msg);

/**
 * Load builtin function data to global symtable
 * @param data symtable item data for function
 * @param func_name name of builtin function
 */
void add_builtin_func(htab_item_data_t data, char* func_name);

/**
 * Load all builtin function to symtable
 */
void load_builtin_funcs();

/**
 * Check if all function in table are defined, if not exit program
 * @param table pointer to table
 */
void check_if_all_func_defined(htab_t* table);

/**
 * Create new data for local symtable and store them in parser structure
 */
void create_new_local_data();

/**
 * Create new data for global symtable and store them in parser structure
 */
void create_new_global_data();

/**
 * Check if function param type is one of (INT, STRING, FLOAT),
 * otherwise return false, store param type to local data
 * @return false if param is not one of these types, otherwise true
 */
bool check_param_types();

/**
 * Check if function return type is on of (INT, STRING, FLOAT, VOID),
 * and store return type to global data, otherwise return error
 */
bool check_return_type();

/**
 * Search function_id in global symtable, if found load data to parser struct,
 * otherwise create new record and insert it to symtable
 * @param def_check true = function definition, false = function call
 */
void check_func_id(bool def_check);

/**
 * Looks in global/local symtable, depends if parser is in function
 * declaration or not, if variable is not in symtable create new record
 * or update parser data
 */
void symtable_var_check();

/**
 * Function which gives parsing to expression parser, when expression is parser
 * check which error code expression parser returned
 * 0 - generate code from AST, or if AST's height is 1 generate one operand
 * 2 - exit program with syntactic error
 * 5 - exit program with semantic error
 * @param token pointer to actual token
 * @param token2 for special usecase when we load 2 tokens but have to send
 * both to exp parser ($x + $y)
 * @param check_semicolon false - expression end with { (if/while), true -
 * expression ends with ;
 */
void expression_parser(token_t* token, token_t* token2, bool check_semicolon);

/**
 * Check if function with required return value was correctly returned
 * otherwise exit program with error code 4
 */
void check_func_return();

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
/*END OF FILE*/
