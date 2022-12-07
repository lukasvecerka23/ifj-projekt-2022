/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file of code_gen.c
 */

#ifndef IFJ_CODE_GEN_H
#define IFJ_CODE_GEN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "error.h"
#include "symtable.h"

/**
 * @struct dynamic string for conversion string literal
 * to IFJcode22 acceptable string
 */
typedef struct dynamic_string {
    int usedmem;
    int stringmem;
    char* string;
} dynamic_string_t;

/**
 * Add char to dynamic string
 * @param string pointer to dynamic string
 * @param c character
 */
void string_add_char(dynamic_string_t* string, char c);

/**
 * Add string to dynamic string
 * @param string pointer to dynamic string
 * @param string2 string which will be added
 */
void string_add_string(dynamic_string_t* string, char* string2);

/**
 * Function to converse string literal to IFJcode22 string
 * transfer special character with ASCII code <= 32 and #
 * to escape sequence
 * @param string string to formate
 * @return pointer to formatted string
 */
char* formate_string(char* string);

/**
 * Generate labels with error print and exit program
 * with specific error code
 */
void generate_exit_label();

/**
 * Generate program header, exit labels
 * and builtin functions
 */
void generate_header();

/**
 * Generate all builtin functions
 */
void generate_builtin_func();

/**
 * Generate floatval builtin function
 */
void builtin_floatval();

/**
 * Generate intval builtin function
 */
void builtin_intval();

/**
 * Generate strval builtin function
 */
void builtin_strval();

/**
 * Generate strlen builtin function
 */
void builtin_strlen();

/**
 * Generate substring builtin function
 */
void builtin_substring();

/**
 * Generate ord builtin function
 */
void builtin_ord();

/**
 * Generate char builtin function
 */
void builtin_char();

/**
 * Generate reads, readi, readf builtin function
 */
void builtin_read();

/**
 * Generate program end
 */
void generate_end();

/**
 * Generate header of function
 * @param func_id identifier of function
 * @param scope scope to jump over function declaration
 */
void generate_func_header(char* func_id, int scope);

/**
 * Take param from temporary frame and check type
 * @param param_data parameter data
 * @param param_number
 * @param scope labels identifier
 */
void generate_func_param(htab_item_t* param_data,
                         unsigned long long param_number,
                         int scope);

/**
 * If function parameter is null, change value of parameter
 * to nil@nil
 * @param scope labels identifier
 * @param param_name
 */
void generate_optional_param_check(int scope, char* param_name);

/**
 * Generate checking of function optional return type
 * @param type data type of function parameter
 */
void check_optional_ret_type(char* type);

/**
 * Generate function return value type check
 * @param scope scope to jump over function declaration
 * @param func_data function data
 */
void generate_func_end(int scope, htab_item_data_t* func_data);

/**
 * Go through given table and declare all variables
 * It prevents from redeclaration in while loops etc.
 * @param table pointer to table
 * @param func_id name of function or main
 * @param local to recognize if we want to define GF or LF
 */
void generate_func_declaration(htab_t* table, char* func_id, bool local);

/**
 * Generate variable declaration
 * @param var name of variable
 * @param local LF/GF
 */
void generate_var_definition(char* var, bool local);

/**
 * Create temporary frame
 */
void generate_tmp_frame();

/**
 * Generate var_id term
 * Move global variable to TF for function call
 * @param index number of parameter
 * @param var_id name of variable
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_global_var_func_param(unsigned long long index,
                                    char* var_id,
                                    bool is_write);

/**
 * Generate var_id term
 * Move local variable to TF for function call
 * @param index number of parameter
 * @param var_id name of variable
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_local_var_func_param(unsigned long long index,
                                   char* var_id,
                                   bool is_write);

/**
 * Generate integer term
 * Move integer value to TF for function call
 * @param index number of parameter
 * @param value integer value
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_int_func_param(unsigned long long index,
                             int value,
                             bool is_write);

/**
 * Generate float term
 * Move float value to TF for function call
 * @param index number of parameter
 * @param value float value
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_float_func_param(unsigned long long index,
                               double value,
                               bool is_write);

/**
 * Generate string term
 * Format string and move it to TF for function call
 * @param index number of parameter
 * @param string
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_string_func_param(unsigned long long index,
                                char* string,
                                bool is_write);

/**
 * Generate null term
 * Move null to TF for function call
 * @param index number of parameter
 * @param is_write true - calling of write function, false - add value to TF
 * param
 */
void generate_null_func_param(unsigned long long index, bool is_write);

/**
 * Generate function call
 * and move function result to GF@tmp_var
 * @param func_id function name
 */
void generate_func_call(char* func_id);

/**
 * Assign function result to local variable
 * @param var_id variable name
 */
void generate_local_assignment(char* var_id);

/**
 * Assign function result to global variable
 * @param var_id variable name
 */
void generate_global_assignment(char* var_id);

/**
 * Assign expression result to local variable
 * @param var_id variable name
 */
void generate_exp_local_assignment(char* var_id);

/**
 * Assign expression result to global variable
 * @param var_id variable name
 */
void generate_exp_global_assignment(char* var_id);

/**
 * Assign null value to GF@tmp_var
 */
void generate_null_assignment();

/**
 * Generate if condition check, check for 0, 0.0, "" and "0"
 * @param scope label identifier
 */
void generate_if_then(int scope);

/**
 * Generate else branch of IF
 * @param scope label identifier
 */
void generate_if_else(int scope);

/**
 * Generate end of IF
 * @param scope label identifier
 */
void generate_if_end(int scope);

/**
 * Generate while start label
 * @param scope label identifier
 */
void generate_while_start(int scope);

/**
 * Generate while condition check, check for 0, 0.0, "" and "0"
 * @param scope label identifier
 */
void generate_while_condition(int scope);

/**
 * Generate end of WHILE cycle
 * @param scope label identifier
 */
void generate_while_end(int scope);

/**
 * Generate return from function
 * @param func_id name of function
 * @param is_void true - return nil@nil, store result of function
 */
void generate_return(char* func_id, bool is_void);

/**
 * Generate return in main scope
 */
void generate_exit_program();

/**
 * Generate one operand expression
 * @param token expression term
 * @param in_func true - working with LF variables, false - working with GF
 * variables
 */
void generate_one_operand(token_t* token, bool in_func);

/**
 * Generate code from AST tree created from expression
 * Generate terms and operators
 * Check types of both operands and do implicit conversion
 * @param current pointer to AST tree root node
 * @param in_function true - working with LF, false - working with GF
 * @param scope pointer to label identifier - have to work with reference
 * to keep updated scope in parser
 */
void generate_ast(ast_node_t* current, bool in_function, int* scope);

#endif
/*END OF FILE*/
