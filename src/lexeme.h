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
    TOKEN_END,
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
    FLOAT2,
    VARPREF,
    ONE_L_COMMENT,
    MULT_L_COMMENT,
    STAR_END
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
        L_FUNCID,
        L_NUMBER,
        L_ASSIGN,
        L_EQ,
        L_NEQ,
        L_LESS,
        L_GREATER,
        L_LESSEQ,
        L_GREATEREQ,
        L_FLOAT,
        L_VARPREF,
        K_ELSE,
        K_FUNCTION,
        K_IF,
        K_INT,
        K_NULL,
        K_RETURN,
        K_STRING,
        K_VOID,
        K_WHILE,
        K_FLOAT
    } lex;

    union {
        unsigned long long line_index;
        char* string;
        unsigned long long val;
        int index;
    };

} lexeme;

States FSM(States curr_state, char edge);
lexeme create_lex(States final, char* token);
lexeme get_lex_value();
void print_lex(lexeme lex);
