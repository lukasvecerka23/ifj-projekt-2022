#include "lexeme.h"
#include "error.h"

/*


*/

// TODO: dynamically allocated
char string[200] = {0};
char* strings = &string[0];

/*
// TODO:
- add all transitions
- implement states for int lit, string lit, func id, var id and keywords
*/
States FSM(States curr_state, char edge) {
    switch (curr_state) {
        case ERROR:
            fprintf(stderr, "token should have been created.");
        case START:
            if (edge == ')')
                return RPAR;
            if (edge == '(')
                return LPAR;
            if (edge == ',')
                return COMMA;
            if (edge == '{')
                return LCURL;
            if (edge == '}')
                return RCURL;
            if (edge == ':')
                return COLON;
            if (edge == ';')
                return SEMICOLON;
            if (edge == '.')
                return DOT;
            if (edge == '+')
                return PLUS;
            if (edge == '-')
                return DASH;
            if (edge == '/')
                return SLASH;
            if (edge == '$')
                return VARID;
            if (edge == '*')
                return MUL;
            if (isalpha(edge) || edge == '_')
                return FUNCID;
            if (isspace(edge))
                return START;
            // if (edge == '"') return L_STRING TODO
            if (isdigit(edge))
                return NUMBER;
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
/*
TODO:
- add all states
- error handling
- use token argument
*/
lexeme create_lex(States final, char* token) {
    switch (final) {
        case LPAR:
            return (lexeme){.lex = L_LPAR};
        case RPAR:
            return (lexeme){.lex = L_RPAR};
        case COMMA:
            return (lexeme){.lex = L_COMMA};
        case DOT:
            return (lexeme){.lex = L_DOT};
        case LCURL:
            return (lexeme){.lex = L_LCURL};
        case RCURL:
            return (lexeme){.lex = L_RCURL};
        case SEMICOLON:
            return (lexeme){.lex = L_SEMICOL};
        case COLON:
            return (lexeme){.lex = L_COLON};
        case MUL:
            return (lexeme){.lex = L_MUL};
        case SLASH:
            return (lexeme){.lex = L_SLASH};
        case PLUS:
            return (lexeme){.lex = L_PLUS};
        case DASH:
            return (lexeme){.lex = L_DASH};
        case FUNCID:
            return (lexeme){.lex = L_FUNCID};
        case VARID:
            return (lexeme){.lex = L_VARID};
        case NUMBER:
            return (lexeme){.lex = L_NUMBER};
        case ERROR:
            error_exit("reached end of token");
    }
    error_exit("No state implemented for this input");
}
/*
TODO:
- string handling
- dynamical allocation
*/
lexeme get_lex_value() {
    States now = START;
    char* lex_start = string;
    while (true) {
        char edge = getchar();
        if (edge == EOF) {
            if (now == START) {
                return (lexeme){.lex = LEOF};
            }
            return create_lex(now, lex_start);
        }
        States next = FSM(now, edge);
        if (next == ERROR) {
            ungetc(edge, stdin);
            *(strings++) =
                '\0';  // remove just for testing should be implemented better
            return create_lex(now, lex_start);
        }
        *(strings++) = edge;
        if (next == START) {
            strings = lex_start;
        }
        now = next;
    }
}
/*
just for testing
TODO:
    - implement creating token strings
    - uploading var id, func id to symtable
    - sending tokens to syntax analyzer
*/
char* print_lex(lexeme lex) {
    switch (lex.lex) {
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
        case LEOF:
            return "EOF";
        case L_PLUS:
            return "+";
        case L_MUL:
            return "*";
        case L_SLASH:
            return "/";
        case L_NUMBER:
            return "num";
        case L_VARID:
            return "var_id";
        case L_FUNCID:
            return "FUNCID";
        case L_DASH:
            return "-";
            warning_msg("reached end of file \n");
    }
    warning_msg(
        "token should have been printed (didnt you forget to add it print "
        "func)");
    return "ERROR";
}
