#include "parser.h"
#include <stdbool.h>
#include "error.h"

/** TODO
 * Dodelat rule pro prolog
 * Propojit parser s hashtable - nacitani funkci, promennych a literalu
 * Expression parser - parsovani a vyhodnocovani vyrazu
 * AST - vytvoreni struktury a propojeni s parserem, generovani AST
 * Prepinani mezi top/down bottom/up parsingem, predavani tokenu atd.
 **/

Parser parser;

void get_next_token() {
    parser.token = get_lex_value();
}

void consume_token(lex token_type, char* err_msg) {
    if (parser.token.lex != token_type) {
        exit_program(51, err_msg);
    }
}

void get_token_consume_token(lex token_type, char* err_msg) {
    get_next_token();
    if (parser.token.lex != token_type) {
        exit_program(51, err_msg);
    }
}

bool check_token_type(lex token_type) {
    if (parser.token.lex == token_type) {
        return true;
    }
    return false;
}

bool check_param_types() {
    if (!check_token_type(K_INT) && !check_token_type(K_STRING) &&
        !check_token_type(K_FLOAT)) {
        return false;
    }
    return true;
}

bool check_return_type() {
    if (!check_token_type(K_INT) && !check_token_type(K_STRING) &&
        !check_token_type(K_FLOAT) && !check_token_type(K_VOID)) {
        return false;
    }
    return true;
}
// <term> rule
bool term() {
    switch (parser.token.lex) {
        case L_VARID:
            return true;
        case L_STRING:
            return true;
        case L_FLOAT:
            return true;
        case L_NUMBER:
            return true;
        case K_NULL:
            return true;
        default:
            printf("%d", parser.token.lex);
            exit_program(51, "wrong term in function call");
    }
    return false;
}

// <next_input_param> rule
bool next_input_param() {
    if (parser.token.lex != L_RPAR) {
        consume_token(L_COMMA, "missing comma before next input parameter");
        get_next_token();
        term();
        get_next_token();
        next_input_param();
        return true;
    }
    // epsilon
    return true;
}

// <list_input_params> rule
bool list_input_params() {
    if (parser.token.lex != L_RPAR) {
        term();
        get_next_token();
        next_input_param();
        return true;
    }
    return true;
}

// <statement> rule
bool statement() {
    // <expression>
    // $a = $a + $a  5;
    if (parser.token.lex == L_VARID) {
        // add var to hashtable
        get_next_token();
        if (parser.token.lex == L_ASSIGN) {
            get_next_token();
            if (parser.token.lex == L_FUNCID) {
                get_token_consume_token(L_LPAR,
                                        "missing left paren in function call");
                get_next_token();
                list_input_params();
                consume_token(L_RPAR, "missing right paren in function call");
                get_token_consume_token(L_SEMICOL,
                                        "missing semicolon after statement");
                get_next_token();
                statement();
                return true;
            }
            // expression call
            statement();
            return true;
        }
        // <expression>
    }
    if (parser.token.lex == L_FUNCID) {
        get_token_consume_token(L_LPAR, "missing left paren in function call");
        get_next_token();
        list_input_params();
        consume_token(L_RPAR, "missing right paren in function call");
        get_token_consume_token(L_SEMICOL, "missing semicolon after statement");
        get_next_token();
        statement();
        return true;
    }
    if (parser.token.lex == K_IF) {
        get_token_consume_token(L_LPAR, "missing left paren in if statement");
        // expresion
        get_token_consume_token(L_RPAR, "missing right paren in if statement");
        get_token_consume_token(L_LCURL,
                                "missing left curl bracket in if statement");
        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        get_token_consume_token(K_ELSE, "missing else");
        get_token_consume_token(L_LCURL,
                                "missing left curl bracket in if statement");
        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        get_next_token();
        statement();
        return true;
    }
    if (parser.token.lex == K_WHILE) {
        get_token_consume_token(L_LPAR,
                                "missing left paren in while statement");
        // expression parsing
        get_token_consume_token(L_RPAR,
                                "missing right paren in while statement");
        get_token_consume_token(L_LCURL,
                                "missing left curl bracket in if statement");
        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        get_next_token();
        statement();
    }
    if (parser.token.lex == K_RETURN) {
        // expression parsing
    }
    // epsilon
    return true;
}

// <type> rule
bool type() {
    // ?type (int, string, float)
    if (parser.token.lex == L_VARPREF) {
        get_next_token();
        if (check_param_types()) {
            return true;
        } else {
            exit_program(51,
                         "wrong data type of argument in function declaration");
        }
    }
    // type (int, string, float)
    if (check_param_types()) {
        return true;
    } else {
        exit_program(51, "wrong data type of argument in function declaration");
    }
    return false;
}

// <return_type> rule
bool return_type() {
    // ?return_type (int, string, float, void)
    if (parser.token.lex == L_VARPREF) {
        get_next_token();
        if (check_param_types()) {
            return true;
        } else {
            printf("%d", parser.token.lex);
            exit_program(51, "wrong return type in function declaration");
        }
    }
    // return_type (int, string, float, void)
    if (check_return_type()) {
        return true;
    } else {
        printf("%d", parser.token.lex);
        exit_program(51, "wrong return type in function declaration");
    }
    return false;
}

// <next_parameter> rule
bool next_parameter() {
    if (!check_token_type(L_RPAR)) {
        consume_token(L_COMMA, "missing comma between arguments");
        get_next_token();
        type();
        get_token_consume_token(L_VARID,
                                "missing variable identifier after data type "
                                "in function declaration");
        get_next_token();
        next_parameter();
        return true;
    }
    // epsilon
    return true;
}

// <list_params> rule
bool list_params() {
    if (!check_token_type(L_RPAR)) {
        if (type()) {
            get_token_consume_token(
                L_VARID, "missing variable identifier after data type");

            get_next_token();
            next_parameter();

            return true;
        }
    }

    // epsilon
    return false;
}

// <program> rule
bool program() {
    if (parser.token.lex == LEOF) {
        return true;
    }
    if (parser.token.lex == K_FUNCTION) {
        get_token_consume_token(L_FUNCID,
                                "missing function identifier keyword");
        get_token_consume_token(L_LPAR,
                                "missing left paren in function declaration");

        get_next_token();
        list_params();

        consume_token(L_RPAR, "missing right paren in function declaration");
        get_token_consume_token(L_COLON,
                                "missing colon before function return type");

        get_next_token();
        return_type();

        get_token_consume_token(
            L_LCURL, "missing left curl bracket in function declaration");

        get_next_token();
        statement();
        consume_token(L_RCURL,
                      "missing right curl bracket in function declaration");

        get_next_token();
        program();
        return true;
    }
    if (statement()) {
        program();
        return true;
    }

    // add end prolog

    exit_program(51, "missing eof");

    return false;
}

bool syntax_analyse() {
    get_next_token();
    // prolog();
    program();

    return true;
}

// function for prolog
/*void prolog() {
}*/
