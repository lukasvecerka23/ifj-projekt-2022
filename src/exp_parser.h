// #include "lexeme.c"
#include <stdlib.h>
#include "lexeme.h"
#define TABLE_SIZE 6

// typedef struct {
//     lexeme token;
//     // htab_t* global_symtable;
//     // htab_t* local_symtable;
//     // htab_item_data_t* global_symtable_data;
//     // htab_item_data_t* local_symtable_data;
//     bool in_function;
// } Exp_parser;

typedef enum precedence_symbols {
    T_PLUS,
    T_MUL,
    T_LPAR,
    T_RPAR,
    T_INT,
    $,
    T_CONCAT,
    T_FLOAT,
    T_STRING,
    T_DIV,
    T_MINUS,
    T_LESS,
    T_GREATER,
    T_LESSEQ,
    T_GREATEREQ,
    T_EQ,
    T_DOT,
    T_NEQ,
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
    lexeme token;
    // ast_node_t* tree;
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

precedence_symbols prec_table[TABLE_SIZE][TABLE_SIZE] = {
    {R, S, S, R, S, R},   {R, R, S, R, S, R},   {S, S, S, W, S, Er},
    {R, R, Er, R, Er, R}, {R, R, Er, R, Er, R}, {S, S, S, Er, S, Er}};
/*
 +  *  (  )  i  $
{R, S, S, R, S, R} +
{R, R, S, R, S, R} *
{S, S, S, W, S, E} (
{R, R, E, R, E, R} )
{R, R, E, R, E, R} i
{R, R, R, E, S, E} $
*/

void rule_reduction();

void expression();
