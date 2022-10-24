#include "lexeme.h"
#include <string.h>
#include "error.h"

/*


*/

// TODO: dynamically allocated
char string[2000] = {0};
char* string_start = &string[0];
unsigned long long line_num = 1;

/*
// TODO:
- add all transitions
- implement states for exp lit, string lit, func id, var id and keywords
*/

lexeme isKeyword(char* keywd) {
    if (strcmp(keywd, "else") == 0)
        return (lexeme){.lex = K_ELSE};
    if (strcmp(keywd, "float") == 0)
        return (lexeme){.lex = K_FLOAT};
    if (strcmp(keywd, "if") == 0)
        return (lexeme){.lex = K_IF};
    if (strcmp(keywd, "int") == 0)
        return (lexeme){.lex = K_INT};
    if (strcmp(keywd, "null") == 0)
        return (lexeme){.lex = K_NULL};
    if (strcmp(keywd, "return") == 0)
        return (lexeme){.lex = K_RETURN};
    if (strcmp(keywd, "void") == 0)
        return (lexeme){.lex = K_VOID};
    if (strcmp(keywd, "while") == 0)
        return (lexeme){.lex = K_WHILE};
    if (strcmp(keywd, "string") == 0)
        return (lexeme){.lex = K_STRING};
    if (strcmp(keywd, "function") == 0)
        return (lexeme){.lex = K_FUNCTION};
    return (lexeme){.lex = L_FUNCID, .string = keywd};
}

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
            if (edge == '?')
                return VARPREF;
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
        case SLASH:
            // printf("edge: %c \n", edge);
            if (edge == '/')
                return ONE_L_COMMENT;
        case ONE_L_COMMENT:
            if (edge == '\n' || edge == EOF)
                return ERROR;
            else
                return ONE_L_COMMENT;
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
            // error_exit("chyba");
            printf("hit\n");
        case ID1:
            if (isalnum(edge))
                return ID1;
        case GREATER:
            if (edge == '=')
                return GREATEREQ;
        default:
            return ERROR;
    }
    return ERROR;
}
/*
TODO:
- Number with Exp
- String literals
- Type identificator with ?
- error handling
- store data to lexeme depends on its type
- uploading var id, func id to symtable
- sending tokens to syntax analyzer
*/
lexeme create_lex(States final, char* token) {
    // (lexeme){.line_index = line_num};
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
            return isKeyword(token);
        case VARID:
            return (lexeme){.lex = L_VARID, .string = token};
        case NUMBER:
            return (lexeme){.lex = L_NUMBER, .string = token};
        case EQ1:
            return (lexeme){.lex = L_ASSIGN};
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
        case VARPREF:
            return (lexeme){.lex = L_VARPREF};
        case ERROR:
            error_exit("reached end of token");
        case ONE_L_COMMENT:
            warning_msg("komentar\n");
            break;
    }
    warning_msg("No state implemented for this input");
}
/*
TODO:
- string handling
- dynamical allocation
*/
lexeme get_lex_value() {
    States now = START;
    char* lex_start = string_start;
    while (true) {
        char edge = getchar();
        if (edge == '\n') {
            line_num++;
        }
        if (edge == EOF) {
            if (now == START) {
                return (lexeme){.lex = LEOF};
            }
            // printf(" line num: %llu", line_num);
            return create_lex(now, lex_start);
        }
        States next = FSM(now, edge);
        // printf("state: %d\n", next);
        if (next == ERROR) {
            ungetc(edge, stdin);
            *(string_start++) = '\0';  // remove just for testing should be
                                       // implemented better
            // printf(" line num: %llu\n", line_num);
            return create_lex(now, lex_start);
        }
        *(string_start++) = edge;
        if (next == START) {
            string_start = lex_start;
        }
        now = next;
    }
}
/*
just for testing
*/
void print_lex(lexeme lex) {
    // printf("line: %uul", lex.line_index);
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
        case L_VARPREF:
            printf("( ? )\n");
            return;
        case L_ID:
            printf("(identifier, %s)\n", lex.string);
            return;
        case L_DASH:
            printf("( - )\n");
            return;
        case L_ASSIGN:
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
        case K_ELSE:
            printf("( else )\n");
            return;
        case K_FUNCTION:
            printf("( function )\n");
            return;
        case K_IF:
            printf("( if )\n");
            return;
        case K_INT:
            printf("( int )\n");
            return;
        case K_NULL:
            printf("( null )\n");
            return;
        case K_RETURN:
            printf("( return )\n");
            return;
        case K_STRING:
            printf("( string )\n");
            return;
        case K_VOID:
            printf("( void )\n");
            return;
        case K_WHILE:
            printf("( while )\n");
            return;
        case K_FLOAT:
            printf("( float )\n");
            return;
        case L_FUNCID:
            printf("( funcid, %s )\n", lex.string);
            return;
        default:
            warning_msg("did not match any token \n");
            return;
    }
    warning_msg(
        "token should have been printed (didnt you forget to add it print "
        "func)");
    printf("ERROR");
    return;
}
