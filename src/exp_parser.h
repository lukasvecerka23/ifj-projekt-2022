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

/**
 * @enum data representing tokens, actions of precedence table
 */
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

/**
 * @struct data of parsed expression in stack
 */
typedef struct stack_el {
    precedence_symbols data;
    token_t* token;
    ast_node_t* tree;
    struct stack_el* next_element;

} * Stack_exp;

/**
 * @struct data representing top element on stack
 */
typedef struct {
    Stack_exp top;
} Stack;

/**
 * Initializes stack
 * @param stack pointer to stack
 */
void stack_init(Stack* stack);

/**
 * finds top token on stack ignoring all terminals
 * @param stack pointer to stack
 * @return enum value representing token type on top, else returns enum value
 * EMPTY
 */
precedence_symbols stack_top_terminal(Stack* stack);

/**
 * finds data of enum value on stack top
 * @param stack pointer to stack
 * @return enum of stack top, in case of empty stack, returns enum value EMPTY
 */
precedence_symbols stack_top(Stack* stack);

/**
 * inserts new value to stack, new value becomes stack top
 * @param stack pointer to stack
 * @param data enum value to be pushed on stack
 */
void stack_push(Stack* stack, precedence_symbols data);

/**
 * inserts symbol representing action "push" in precedence table under current
 * token
 * @param stack pointer to stack
 */
void stack_shift_push(Stack* stack);

/**
 * removes current top value from the stack
 * @param stack pointer to stack
 * @return returns popped data from the stack top
 */
Stack_exp stack_pop(Stack* stack);

/**
 * checks if stack is empty
 * @param stack pointer to stack
 * @return 1 if stack is empty otherwise 0
 */
int stack_empty(Stack* stack);

/**
 * reduce number of nonterminals and terminals on stack based on grammer of
 * expression parser, and calls function for AST generation
 * @param stack pointer to stack
 * @return 1 if reduction was sucessful otherwise 0
 */
int rule_reduction(Stack* stack);

/**
 * generates enum data representing input token
 * @param token input to be converted to expression parser enum data
 * @return enum data representing incoming token
 */
precedence_symbols map_token_to_enum(token_t* token);

/**
 * checks if remaining data in stack are according to the requirement of the
 * algorithm after expression was parsed
 * @param stack
 * @return 1 if state of stack is ok, otherwise 0
 */
int exp_correct_syntax(Stack* stack);

/**
 * checks syntax of given expression during parsing generates its AST.
 * @param used_token pointer to incoming token from parser, to determine
 * difference between condition and expression
 * @param used_token2 pointer second token used by parser, otherwise NULL
 * @param is_expression int which represents if it is expression (1) or
 * condition (0), in order to check correct terminanting character
 * @param tree pointer to generated AST
 * @return  if syntax of parsed expression is correct, returns 0, else returns
 * syntax error
 */
int parse_expression(token_t* used_token,
                     token_t* used_token2,
                     bool is_expression,
                     ast_node_t** tree);

#endif

/*END OF FILE*/
