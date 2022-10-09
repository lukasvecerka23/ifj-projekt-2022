#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*


*/
typedef unsigned long long size_t;

typedef enum {
    START,
    LPAR,
    RPAR,
    COMMA,
    DOT,
    LCURL,
    PCURL,
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

typedef struct {
    enum lex {
        LPAR,
        RPAR,
        COMMA,
        LBRA,
        RBRA,
        SEMICOL,
        COLON,
        LEOF,
    };

    size_t data;

} lexeme;

States FSM(States curr_state, char edge) {
    switch (curr_state) {
    case ERROR:
        fprinf(stderr,"token should have been created.");
    case START:
        if (edge == ')') return RPAR;
        if (edge == '(') return LPAR;
        if (edge == ',') return COMMA;
        if (edge == '{') return LBRA;
        if (edge == '}') return LBRA;
        if (edge == ':') return COLON;
        if (edge == ';') return SEMICOLON;
        if (edge == '.') return DOT;
        if (edge == '+') return PLUS;
        if (edge == '-') return DASH;
        if (edge == '/') return DIV;
        if (edge == '$') return VARID;
        if (edge == '*') return MUL;
        if (isalpha(edge) || edge == '_') return FUNCID;
        return ERROR;
    case RPAR:
    case COMMA:
    case LBRA:
    case RBRA:
    case LPAR:
        return ERROR;

    
    }
    return ERROR;
}

lexeme get_lex_value(States now) {
    now = START;
}

int main() { return 0; }