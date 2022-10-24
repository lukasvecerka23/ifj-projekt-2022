#include "lexeme.h"
#include <string.h>
#include "error.h"

/*


*/

// TODO: dynamically allocated
char string[2000] = {0};
char* string_start = &string[0];
unsigned long long line_num = 1;
int err_flag = 0;
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
        case TOKEN_END:
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
            if (edge == '"')
                return STRING_LIT_E;
            if (isalpha(edge) || edge == '_')
                return ID1;
            if (isspace(edge))
                return START;
            if (isdigit(edge))
                return NUMBER;
            if (edge == '=')
                return EQ1;
            if (edge == '!')
                return NEQ1;
            return TOKEN_END;
        case STRING_LIT_END:
            return TOKEN_END;
        case STRING_LIT_E:
            if (edge == '"') {
                return STRING_LIT_END;
            }
            if (edge == EOF) {
                err_flag = 1;
                return TOKEN_END;
            }
            if (edge == '\\') {
                printf("escape sequence function should've been called\n");
                return STRING_LIT_E;
            }
            return STRING_LIT_E;
        case SLASH:
            if (edge == '/')
                return ONE_L_COMMENT;
            if (edge == '*')
                return MULT_L_COMMENT;
            return TOKEN_END;
        case MULT_L_COMMENT:
            if (edge == EOF) {
                err_flag = 1;
                return TOKEN_END;
            }
            if (edge == '*')
                return STAR_END;
            else
                return MULT_L_COMMENT;
        case STAR_END:
            if (edge == '/')
                return TOKEN_END;
            else {
                return MULT_L_COMMENT;
            }
        case ONE_L_COMMENT:
            if (edge == '\n' || edge == EOF)
                return TOKEN_END;
            else
                return ONE_L_COMMENT;
        case NUMBER:
            if (isdigit(edge))
                return NUMBER;
            if (edge == '.')
                return FLOAT1;
            if (edge == 'e' || edge == 'E')
                return EXP_1;
            else
                return TOKEN_END;
        case FLOAT1:
            if (isdigit(edge))
                return FLOAT2;
            else {
                return err_flag = 1;
                return TOKEN_END;
            }
        case FLOAT2:
            if (isdigit(edge))
                return FLOAT2;
            if (edge == 'E' || edge == 'e')
                return EXP_1;
            else
                return TOKEN_END;
        case EXP_1:
            if (edge == '+' || edge == '-' || isdigit(edge))
                return EXP_2;
            else {
                err_flag = 1;
                TOKEN_END;
            }
        case EXP_2:
            if (isdigit(edge))
                return EXP_2;
            else
                return TOKEN_END;
        case VARID:
            if (isalnum(edge))
                return VARID;
            return TOKEN_END;
        case EQ1:
            if (edge == '=')
                return EQ2;
            return TOKEN_END;
        case EQ2:
            if (edge == '=')
                return EQ3;
            err_flag = 1;
            return TOKEN_END;
        case NEQ1:
            if (edge == '=')
                return NEQ2;
            err_flag = 1;
            return TOKEN_END;
        case NEQ2:
            if (edge == '=')
                return NEQ3;
            err_flag = 1;
            return TOKEN_END;
        case LESS:
            if (edge == '=')
                return LESSEQ;
        case ID1:
            if (isalnum(edge))
                return ID1;
            return TOKEN_END;
        case GREATER:
            if (edge == '=')
                return GREATEREQ;
            return TOKEN_END;
        default:
            return TOKEN_END;
    }
    return TOKEN_END;
}
/*
TODO:
- Number with Exp
- String literals
- Type identificator with ?
- TOKEN_END handling
- store data to lexeme depends on its type
- uploading var id, func id to symtable
- sending tokens to syntax analyzer
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
        case STRING_LIT_END:
            return (lexeme){.lex = L_STRING, .string = token};
        case EXP_2:
            return (lexeme){.lex = L_EXP, .string = token};
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
        case TOKEN_END:
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
            return create_lex(now, lex_start);
        }
        States next = FSM(now, edge);
        if (next == TOKEN_END) {
            ungetc(edge, stdin);
            *(string_start++) = '\0';
            if (!err_flag) {
                return create_lex(now, lex_start);
            } else {
                warning_msg("error when creating token");
                err_flag = 0;
                next = START;
            }
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
        case L_EXP:
            printf("( exp, %s )\n", lex.string);
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
        case L_STRING:
            printf("(string, %s)\n", lex.string);
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
    return;
}
