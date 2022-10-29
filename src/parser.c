#include "parser.h"
#include <stdbool.h>
#include "error.h"

Parser parser;

void get_next_token() {
    parser.token = get_lex_value();
}

void check_token_type(lex token_type) {
    if (parser.token.lex != token_type) {
        warning_msg("syntax error");
        printf("token: %d", parser.token.lex);
        exit(1);
    }
}

void get_token_check_token_type(lex token_type) {
    get_next_token();
    if (parser.token.lex != token_type) {
        warning_msg("syntax error");
        printf("token: %d", parser.token.lex);
        exit(1);
    }
}

bool check_param_type(lex token_type) {
    if (parser.token.lex == token_type) {
        return true;
    }
    return false;
}

bool check_param_types() {
    if (!check_param_type(K_INT) && !check_param_type(K_STRING) &&
        !check_param_type(K_FLOAT)) {
        return false;
    }
    return true;
}
// <type> rule
bool type() {
    get_next_token();
    // ?type (int, string, float)
    if (parser.token.lex == L_VARPREF) {
        get_next_token();
        if (check_param_types()) {
            return true;
        } else {
            warning_msg("wrong type");
            exit(1);
        }
    }
    // type (int, string, float)
    if (check_param_types()) {
        return true;
    }

    return false;
}

// <list_params> rule
bool list_params() {
    if (type()) {
        get_token_check_token_type(L_VARID);
        return false;
    }
    // epsilon
    return true;
}

// <program> rule
bool program() {
    get_next_token();
    if (parser.token.lex == K_FUNCTION) {
        get_token_check_token_type(L_FUNCID);
        get_token_check_token_type(L_LPAR);
        if (list_params()) {
            check_token_type(L_RPAR);
        } else {
            get_token_check_token_type(L_RPAR);
        }
        if (program()) {
            return true;
        }
    } else if (parser.token.lex == LEOF) {
        printf("eof\n");
        return true;
    }

    warning_msg("missing eof");
    return false;
}

// function for prolog
/*void prolog() {
}*/