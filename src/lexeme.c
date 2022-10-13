#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*


*/

// TODO: dynamically allocated
char string[200] = {0};
char *strings = &string[0];

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

States FSM(States curr_state, char edge) {
    switch (curr_state) {
    case ERROR:
        fprintf(stderr,"token should have been created.");
    case START:
        if (edge == ')') return RPAR;
        if (edge == '(') return LPAR;
        if (edge == ',') return COMMA;
        if (edge == '{') return LCURL;
        if (edge == '}') return RCURL;
        if (edge == ':') return COLON;
        if (edge == ';') return SEMICOLON;
        if (edge == '.') return DOT;
        if (edge == '+') return PLUS;
        if (edge == '-') return DASH;
        if (edge == '/') return DIV;
        if (edge == '$') return VARID;
        if (edge == '*') return MUL;
        if (isalpha(edge) || edge == '_') return FUNCID;
        if (isspace(edge)) return START;
        return ERROR;
    case RPAR:
    case COMMA:
    case LCURL:
    case RCURL:
    case LPAR:
    default:
        return ERROR;

    
    }
    return ERROR;
}
lexeme create_lex(States final, char *token){
    switch(final){
        case LPAR:
            return (lexeme){.lex=L_LPAR};
        case RPAR:
            return (lexeme){.lex=L_RPAR};
        case COMMA:
            return (lexeme){.lex=L_COMMA};
        case DOT:
            return (lexeme){.lex=L_DOT};
        case LCURL:
            return (lexeme){.lex=L_LCURL};
        case RCURL:
            return (lexeme){.lex=L_RCURL};
        case SEMICOLON:
            return (lexeme){.lex=L_SEMICOL};
        case COLON:
            return (lexeme){.lex=L_COLON};
        case ERROR:
            exit(1); // create error handling
    }
    exit(99);
}

lexeme get_lex_value() {
    States now = START;
    char *lex_start = string;
    while (true) {
        char edge = getchar();
        if (edge == EOF){
            if (now == START) {
                return (lexeme){.lex=LEOF};
            }
            return create_lex(now, lex_start);
        }
        States next = FSM(now, edge);
        if (next == ERROR){
            ungetc(edge, stdin);
            *(strings++) = '\0';
            return create_lex(now, lex_start);
        }
        if (next == START){
            strings = lex_start;
        }
        *(strings++) = edge;
        now = next;
    }
}

char* print_lex(lexeme lex){
    switch(lex.lex){
        case L_LPAR:
            return "(";
        case L_RPAR:
            return ")";
        case L_COMMA:
            return ",";
        case L_DOT:
            return ".";
        case L_LCURL:
            return "{";
        case L_RCURL:
            return "}";
        case L_SEMICOL:
            return ";";
        case L_COLON:
            return ":";
    }
    return "ERROR";
}

int main() {
    lexeme l = {0};
    while (l.lex != LEOF){
        l = get_lex_value();
        puts(print_lex(l));
    }
    return 0; 
}
