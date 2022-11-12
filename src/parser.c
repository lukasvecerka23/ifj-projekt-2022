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

void create_new_local_data() {
    htab_item_data_t* local_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));
    parser.local_symtable_data = local_data;
}

void create_new_global_data() {
    htab_item_data_t* global_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));

    parser.global_symtable_data = global_data;
}

bool check_param_types() {
    if (!check_token_type(K_INT) && !check_token_type(K_STRING) &&
        !check_token_type(K_FLOAT)) {
        return false;
    }
    // store var type to symtable
    switch (parser.token.lex) {
        case K_INT:
            parser.local_symtable_data->var_data.data_type = DTYPE_INT;
            break;
        case K_STRING:
            parser.local_symtable_data->var_data.data_type = DTYPE_STRING;
            break;
        case K_FLOAT:
            parser.local_symtable_data->var_data.data_type = DTYPE_FLOAT;
            break;
    }
    return true;
}

void add_func_param(htab_var_data_t* var_data) {
    parser.global_symtable_data->func_data
        .params[parser.global_symtable_data->func_data.param_count] = var_data;
    parser.global_symtable_data->func_data.param_count++;
}

bool check_return_type() {
    if (!check_token_type(K_INT) && !check_token_type(K_STRING) &&
        !check_token_type(K_FLOAT) && !check_token_type(K_VOID)) {
        return false;
    }
    // store function return type to symtable
    switch (parser.token.lex) {
        case K_INT:
            parser.global_symtable_data->func_data.ret_type = RETTYPE_INT;
            break;
        case K_STRING:
            parser.global_symtable_data->func_data.ret_type = RETTYPE_STRING;
            break;
        case K_FLOAT:
            parser.global_symtable_data->func_data.ret_type = RETTYPE_FLOAT;
            break;
        case K_VOID:
            parser.global_symtable_data->func_data.ret_type = RETTYPE_VOID;
            break;
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
        parser.local_symtable_data->var_data.optional_type = true;
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
        parser.global_symtable_data->func_data.optional_ret_type = true;
        get_next_token();
        if (check_return_type()) {
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
        create_new_local_data();
        consume_token(L_COMMA, "missing comma between arguments");
        get_next_token();
        type();
        get_token_consume_token(L_VARID,
                                "missing variable identifier after data type "
                                "in function declaration");
        parser.local_symtable_data->type = ID_VAR;
        add_func_param(&parser.local_symtable_data->var_data);
        htab_insert_update(parser.local_symtable, parser.token.string,
                           parser.local_symtable_data);
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
        create_new_local_data();
        if (type()) {
            get_token_consume_token(
                L_VARID, "missing variable identifier after data type");
            parser.local_symtable_data->type = ID_VAR;
            add_func_param(&parser.local_symtable_data->var_data);
            htab_insert_update(parser.local_symtable, parser.token.string,
                               parser.local_symtable_data);
            get_next_token();
            next_parameter();

            // free(var_data);
            return true;
        }
    }

    // epsilon
    return false;
}

// <program> rule
bool program() {
    parser.in_function = false;
    parser.local_symtable = htab_init(10);
    if (parser.token.lex == LEOF) {
        htab_free(parser.local_symtable);
        return true;
    }
    if (parser.token.lex == K_FUNCTION) {
        parser.in_function = true;
        get_token_consume_token(L_FUNCID,
                                "missing function identifier keyword");

        create_new_global_data();
        parser.global_symtable_data->func_data.defined = true;
        parser.global_symtable_data->type = ID_FUNC;

        htab_insert_update(parser.global_symtable, parser.token.string,
                           parser.global_symtable_data);
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
        ht_print_table(parser.local_symtable, "LOCAL");
        htab_free(parser.local_symtable);
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
    parser.global_symtable = htab_init(10);
    // load builtin funcs to symtable
    // prolog();
    program();

    // just for testing
    ht_print_table(parser.global_symtable, "GLOBAL");

    htab_free(parser.global_symtable);

    return true;
}

// function for prolog
/*void prolog() {
}*/
