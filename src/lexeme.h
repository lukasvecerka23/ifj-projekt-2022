#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    START,
    LPAR,
    RPAR,
    COMMA,
    DOT,
    LCURL,
    RCURL,
    COLON,
    SEMICOLON,
    ERROR,
    PLUS,
    DASH,
    DIV,
    VARID,
    MUL,
    FUNCID,
} States;
// TODO: add all states




typedef struct {
    enum {
        L_LPAR,
        L_RPAR,
        L_COMMA,
        L_SEMICOL,
        L_COLON,
        L_RCURL,
        L_LCURL,
        L_DOT,
        LEOF
    } lex;

    size_t data;

} lexeme;


States FSM(States curr_state, char edge);

lexeme create_lex(States final, char *token);

lexeme get_lex_value();

char* print_lex(lexeme lex);