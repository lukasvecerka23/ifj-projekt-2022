#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    SLASH,
    MUL,
    ID1,
    VARID,
    NUMBER,
    EQ1,
    EQ2,
    EQ3,
    LESS,
    GREATER,
    LESSEQ,
    GREATEREQ,
    NEQ1,
    NEQ2,
    NEQ3,
    FLOAT1,
    FLOAT2
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
        LEOF,
        L_MUL,
        L_SLASH,
        L_PLUS,
        L_DASH,
        L_ID,
        L_VARID,
        L_NUMBER,
        L_SET,
        L_EQ,
        L_NEQ,
        L_LESS,
        L_GREATER,
        L_LESSEQ,
        L_GREATEREQ,
        L_FLOAT
    } lex;

    union {
        char* string;
        int val;
        int index;
    };

} lexeme;

States FSM(States curr_state, char edge);
lexeme create_lex(States final, char* token);
lexeme get_lex_value();
char* print_lex(lexeme lex);
