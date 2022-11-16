#include <stdlib.h>
#include "lexeme.h"

#define TABLE_SIZE 6

typedef enum precedence_symbols {
    EMPTY,
    W,   // = wait
    R,   // > reduce
    S,   // < shift
    Er,  // nothing error
    E,   // non-terminal
    T_FLOAT,
    T_INT,
    T_STRING,
    T_PLUS,
    T_DIV,
    T_LPAR,
    T_RPAR,
    T_MINUS,
    T_LESS,
    T_GREATER,
    T_LESSEQ,
    T_GREATEREQ,
    T_EQ,
    T_NEQ,
    $,
} precedence_symbols;

typedef struct stack_el {
    precedence_symbols data;
    struct stack_el* next_element;

} Stack_exp;

typedef Stack_exp Stack;

void stack_init(Stack_exp* stack) {
    stack->data = EMPTY;
}

precedence_symbols stack_top(Stack_exp* stack) {
    Stack_exp* tmp;
    tmp = stack;
    while (tmp->data != EMPTY) {
        if (tmp->data >= T_FLOAT && tmp->data <= T_NEQ) {
            return tmp->data;
        }

        tmp = tmp->next_element;
    }
    //
}

void stack_push(Stack_exp* stack) {
    Stack_exp* new_element = malloc(sizeof(struct stack_el));
}

// pops first element
precedence_symbols pop(Stack_exp* stack) {
    if (stack->data != EMPTY) {
        precedence_symbols to_return = stack->data;
        stack = stack->next_element;
        return to_return;
    } else {
        return EMPTY;
    }
}

int empty(Stack_exp* stack) {
    if (stack->data == EMPTY) {
        return 1;
    } else {
        return 0;
    }
}

precedence_symbols prec_table[TABLE_SIZE][TABLE_SIZE] = {
    {R, S, S, R, S, R}, {R, R, S, R, S, R}, {S, S, S, W, S, E},
    {R, R, E, R, E, R}, {R, R, E, R, E, R}, {R, R, R, E, S, E}};
/*
 +  *  (  )  i  $
{R, S, S, R, S, R} +
{R, R, S, R, S, R} *
{S, S, S, W, S, E} (
{R, R, E, R, E, R} )
{R, R, E, R, E, R} i
{R, R, R, E, S, E} $
*/

void rule_reduction() {
    /*
    i => {float,int,string}
    E -> i
    E -> (E)
    E -> E op E
    op => {+,-,*,/,}

    - load until < if 4th loaded element is not < then error
    - push new non terminal on stack


    todo ast generate call
    */
}

void expression() {
    Stack stack;
    stack_init(&stack);
    // stack init

    // stack_push($)

    /*



                        SA ALGORITMUS

    while(top != $S and b = $){
        a = top(stack);
        b = get_token();
        if(b == '{' || b == ';'){
            check stav zasobniku else err;
        }
        prec state = prec_table[a][b];
        switch(state)
            case W:
                stack_push(b);
                b = get_token();
            case S:
                stack_push(S);
                stack_push(B);
                b = get_token;
            case R:
                rule_reduction(stack,);
            case Er:
                error();
            default:
                warning("something is very wrong");
    }
    if(stack_top(stack) == E and (token == '{' || token == ';'})){
        return true;
    }
    return false;




    */
}
