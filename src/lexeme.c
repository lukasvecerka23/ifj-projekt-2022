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
- implement states for exp lit, string lit, func id, var id and keywords
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
            if (edge == '<')
                return LESS;
            if (edge == '>')
                return GREATER;
            if (isalpha(edge) || edge == '_')
                return ID1;
            if (isspace(edge))
                return START;
            // if (edge == '"') return L_STRING TODO
            if (isdigit(edge))
                return NUMBER;
            if (edge == '=')
                return EQ1;
            if (edge == '!')
                return NEQ1;
            return ERROR;
        case NUMBER:
            if (isdigit(edge))
                return NUMBER;
            if (edge == '.')
                return FLOAT1;
        case FLOAT1:
            if (isdigit(edge))
                return FLOAT2;
            else
                return ERROR;
        case FLOAT2:
            if (isdigit(edge))
                return FLOAT2;
            // if (edge == 'e' || edge == 'E')
        case VARID:
            if (isalnum(edge))
                return VARID;
        case EQ1:
            if (edge == '=')
                return EQ2;
        case EQ2:
            if (edge == '=')
                return EQ3;
        case NEQ1:
            if (edge == '=')
                return NEQ2;
        case NEQ2:
            if (edge == '=')
                return NEQ3;
        case LESS:
            if (edge == '=')
                return LESSEQ;
        case GREATER:
            if (edge == '=')
                return GREATEREQ;
        case ID1:
            if (isalnum(edge))
                return ID1;
        default:
            return ERROR;
    }
    return ERROR;
}
/*
TODO:
- Number with Exp
- String literals
- Identificators
- error handling
- use token data
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
        case ID1:
            // call function for decision between funcid, keyword or type id
            return (lexeme){.lex = L_ID, .string = token};
        case VARID:
            return (lexeme){.lex = L_VARID, .string = token};
        case NUMBER:
            return (lexeme){.lex = L_NUMBER, .string = token};
        case EQ1:
            return (lexeme){.lex = L_SET};
        case EQ3:
            return (lexeme){.lex = L_EQ};
        case NEQ3:
            return (lexeme){.lex = L_NEQ};
        case LESSEQ:
            return (lexeme){.lex = L_LESSEQ};
        case GREATEREQ:
            return (lexeme){.lex = L_GREATEREQ};
        case LESS:
            return (lexeme){.lex = L_LESS};
        case GREATER:
            return (lexeme){.lex = L_GREATER};
        case FLOAT2:
            return (lexeme){.lex = L_FLOAT, .string = token};
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
void print_lex(lexeme lex) {
    switch (lex.lex) {
        case L_LPAR:
            printf("( ( )\n");
            return;
        case L_RPAR:
            printf("( ) )\n");
            return;
        case L_COMMA:
            printf("( , )\n");
            return;
        case L_DOT:
            printf("( . )\n");
            return;
        case L_LCURL:
            printf("( { )\n");
            return;
        case L_RCURL:
            printf("( } )\n");
            return;
        case L_SEMICOL:
            printf("( ; )\n");
            return;
        case L_COLON:
            printf("( : )\n");
            return;
        case LEOF:
            printf("( EOF )\n");
            return;
        case L_PLUS:
            printf("( + )\n");
            return;
        case L_MUL:
            printf("( * )\n");
            return;
        case L_SLASH:
            printf("( / )\n");
            return;
        case L_NUMBER:
            printf("(integer, %s)\n", lex.string);
            return;
        case L_VARID:
            printf("(varid, %s)\n", lex.string);
            return;
        case L_ID:
            printf("(identifier, %s)\n", lex.string);
            return;
        case L_DASH:
            printf("( - )\n");
            return;
        case L_SET:
            printf("( = )\n");
            return;
        case L_EQ:
            printf("( === )\n");
            return;
        case L_NEQ:
            printf("( !== )\n");
            return;
        case L_LESS:
            printf("( < )\n");
            return;
        case L_GREATER:
            printf("( > )\n");
            return;
        case L_LESSEQ:
            printf("( <= )\n");
            return;
        case L_GREATEREQ:
            printf("( >= )\n");
            return;
        case L_FLOAT:
            printf("(float, %s)\n", lex.string);
            return;
        default:
            warning_msg("reached end of file \n");
            return;
    }
    warning_msg(
        "token should have been printed (didnt you forget to add it print "
        "func)");
    printf("ERROR");
    return;
}
