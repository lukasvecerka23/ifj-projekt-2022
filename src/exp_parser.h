/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file for Expression Parser
 */

#ifndef EXP_PARSER_H
#define EXP_PARSER_H

#include <stdlib.h>
#include "ast.h"
#include "scanner.h"
#define TABLE_SIZE 15

typedef enum precedence_symbols {
    T_PLUS,
    T_MUL,
    T_LPAR,
    T_RPAR,
    T_INT,
    $,
    T_MINUS,
    T_DIV,
    T_CONCAT,
    T_LESS,
    T_GREATER,
    T_LESSEQ,
    T_GREATEREQ,
    T_EQ,
    T_NEQ,
    T_FLOAT,
    T_STRING,
    EMPTY,
    W,   // = wait
    R,   // > reduce
    S,   // < shift
    Er,  // error
    E,   // non-terminal
    T_INVALID,
    T_END_EXP
} precedence_symbols;

typedef struct stack_el {
    precedence_symbols data;
    token_t* token;
    ast_node_t* tree;
    struct stack_el* next_element;

} * Stack_exp;

typedef struct {
    Stack_exp top;
} Stack;

void stack_init(Stack* stack);

precedence_symbols stack_top_nonterminal(Stack* stack);

precedence_symbols stack_top(Stack* stack);

void stack_push(Stack* stack, precedence_symbols data);

// // pops first element
Stack_exp stack_pop(Stack* stack);

int stack_empty(Stack* stack);

int rule_reduction(Stack* stack);

int parse_expression(token_t* used_token,
                     token_t* used_token2,
                     bool is_expression,
                     ast_node_t** tree);

#endif

/*END OF FILE*/
