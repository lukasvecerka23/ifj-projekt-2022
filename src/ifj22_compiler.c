#include "error.h"
#include "parser.h"

/*
just for testing
*/
void print_lex(token_t lex) {
    switch (lex.token_type) {
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
            printf("( exp, %f)\n", lex.float_val);
            // printf("( exp, %s)\n", lex.string);
            return;
        case L_NUMBER:
            printf("(integer, %lld)\n", lex.val);
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
            printf("(float, %f)\n", lex.float_val);
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
        case L_PHPEND:
            printf("( php end )\n");
            return;
        case L_PHPSTART:
            printf("( php start )\n");
            return;
        case K_STRICTTYPES:
            printf("( strict_types )\n");
            return;
        case K_DECLARE:
            printf("( declare )\n");
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

int main() {
    // token_t l = {0};
    // while (l.token_type != LEOF) {
    //     l = get_lex_value();
    //     print_lex(l);
    // }

    syntax_analyse();

    return 0;
}
