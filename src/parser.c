/*
Name: IFJ PROJEKT 2022
Authors: xdolez0c, xvecer30, xnespo10, xtomko06
Description: --
*/

#include "parser.h"
#include <stdio.h>
#include "error.h"
/** TODO
 * Dodelat semanticke kontroly
 **/

Parser parser;

void get_next_token() {
    token_t* tmp_token = parser.token;
    parser.token = get_lex_value();
    free(tmp_token);
}

void consume_token(token_type token_type, char* err_msg) {
    if (parser.token->token_type != token_type) {
        htab_free(parser.global_symtable);
        clear_and_exit_program(2, err_msg);
    }
}

void get_token_consume_token(token_type token_type, char* err_msg) {
    get_next_token();
    if (parser.token->token_type != token_type) {
        htab_free(parser.global_symtable);
        clear_and_exit_program(2, err_msg);
    }
}

bool check_token_type(token_type token_type) {
    if (parser.token->token_type == token_type) {
        return true;
    }
    return false;
}

void clear_and_exit_program(int err_code, char* msg) {
    if (parser.in_function)
        htab_clear(parser.local_symtable);
    htab_clear(parser.global_symtable);
    free(parser.token);
    exit_program(err_code, msg);
}

void add_builtin_func(htab_item_data_t data, char* func_name) {
    htab_item_data_t* func_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));
    *func_data = data;
    htab_insert_update(parser.global_symtable, func_name, func_data);
}

void load_builtin_funcs() {
    htab_item_data_t data;
    // reads
    data = (htab_item_data_t){.name = "reads",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = true,
                                            .param_count = 0,
                                            .ret_type = RETTYPE_STRING}};
    add_builtin_func(data, "reads");
    // readi
    data = (htab_item_data_t){.name = "readi",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = true,
                                            .param_count = 0,
                                            .ret_type = RETTYPE_INT}};
    add_builtin_func(data, "readi");
    // readf
    data = (htab_item_data_t){.name = "readf",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = true,
                                            .param_count = 0,
                                            .ret_type = RETTYPE_FLOAT}};
    add_builtin_func(data, "readf");

    // write - vyresit alokovani n poctu termu v parametrech
    data = (htab_item_data_t){.name = "write",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 0,
                                            .ret_type = RETTYPE_VOID}};
    add_builtin_func(data, "write");

    // floatval
    data = (htab_item_data_t){.name = "floatval",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_FLOAT}};
    add_builtin_func(data, "floatval");

    // intval
    data = (htab_item_data_t){.name = "intval",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_INT}};
    add_builtin_func(data, "intval");

    // strval
    data = (htab_item_data_t){.name = "strval",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_STRING}};
    add_builtin_func(data, "strval");

    // strlen

    data = (htab_item_data_t){.name = "strlen",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_INT}};
    add_builtin_func(data, "strlen");

    // substring
    data = (htab_item_data_t){.name = "substring",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = true,
                                            .param_count = 3,
                                            .ret_type = RETTYPE_STRING}};
    add_builtin_func(data, "substring");

    // ord

    data = (htab_item_data_t){.name = "ord",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_INT}};
    add_builtin_func(data, "ord");

    // chr
    data = (htab_item_data_t){.name = "chr",
                              .type = ID_FUNC,
                              .func_data = {.defined = true,
                                            .optional_ret_type = false,
                                            .param_count = 1,
                                            .ret_type = RETTYPE_STRING}};
    add_builtin_func(data, "chr");
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
    switch (parser.token->token_type) {
        case K_INT:
            parser.local_symtable_data->var_data.data_type = DTYPE_INT;
            break;
        case K_STRING:
            parser.local_symtable_data->var_data.data_type = DTYPE_STRING;
            break;
        case K_FLOAT:
            parser.local_symtable_data->var_data.data_type = DTYPE_FLOAT;
            break;
        default:
            break;
    }
    return true;
}

void check_func_id(bool def_check) {
    htab_item_t* tmp_item =
        htab_search(parser.global_symtable, parser.token->string);

    // if record about func is not in symtable, create new record
    if (tmp_item == NULL) {
        create_new_global_data();
        if (def_check) {
            parser.global_symtable_data->func_data.defined = true;
        } else {
            if (parser.in_function)
                parser.global_symtable_data->func_data.defined = false;
            else
                clear_and_exit_program(
                    3,
                    "calling undefined function outside of function "
                    "declaration");
        }
        parser.global_symtable_data->name = parser.token->string;
        parser.global_symtable_data->type = ID_FUNC;
        parser.global_symtable_data->func_data.param_count = 0;
        parser.global_symtable_data->func_data.optional_ret_type = false;
        parser.global_symtable_data->func_data.returned = false;
        htab_insert_update(parser.global_symtable, parser.token->string,
                           parser.global_symtable_data);
        parser.func_check = false;
    } else {
        // if there is record check if function is already defined, if yes
        // return error if not update the symtable record
        parser.global_symtable_data = tmp_item->data;
        if (strcmp(tmp_item->key, "write") != 0) {
            parser.func_check = true;
        } else {
            parser.func_check = false;
        }

        if (def_check) {
            if (tmp_item->data->func_data.defined == true) {
                clear_and_exit_program(3, "redefinition of function");
            }
            parser.global_symtable_data->func_data.defined = true;
        }
    }
}

void symtable_var_check() {
    htab_item_t* tmp_item;
    if (parser.in_function) {
        tmp_item = htab_search(parser.local_symtable, parser.token->string);
        if (tmp_item == NULL) {
            create_new_local_data();
            parser.local_symtable_data->var_data.init = false;
            parser.local_symtable_data->name = parser.token->string;
            parser.local_symtable_data->type = ID_VAR;
            htab_insert_update(parser.local_symtable, parser.token->string,
                               parser.local_symtable_data);
        } else {
            parser.local_symtable_data = tmp_item->data;
        }
    } else {
        tmp_item = htab_search(parser.global_symtable, parser.token->string);
        if (tmp_item == NULL) {
            create_new_global_data();
            parser.global_symtable_data->var_data.init = false;
            parser.global_symtable_data->name = parser.token->string;
            parser.global_symtable_data->type = ID_VAR;
            htab_insert_update(parser.global_symtable, parser.token->string,
                               parser.global_symtable_data);
        } else {
            parser.global_symtable_data = tmp_item->data;
        }
    }
}

void var_init() {
    if (parser.in_function) {
        parser.local_symtable_data->var_data.init = true;
    } else {
        parser.global_symtable_data->var_data.init = true;
    }
}

void expression_parser(token_t* token, bool is_cond) {
    ast_node_t* new_tree;
    int err_code;
    err_code = parse_expression(token, is_cond, &new_tree);

    switch (err_code) {
        case 0:
            // ast_print_tree(new_tree);
            if (is_cond == false && ast_height(new_tree) == 1)
                if (parser.in_function)
                    generate_one_operand(new_tree->token, parser.in_function,
                                         parser.local_symtable);
                else
                    generate_one_operand(new_tree->token, parser.in_function,
                                         parser.global_symtable);
            else if (parser.in_function) {
                generate_ast(new_tree, parser.in_function,
                             parser.local_symtable);
            } else {
                generate_ast(new_tree, parser.in_function,
                             parser.global_symtable);
            }
            // ast_dispose(new_tree);
            break;
        case 2:
            ast_dispose(new_tree);
            clear_and_exit_program(2, "syntax error in expression parser");
            break;
        case 5:
            ast_dispose(new_tree);
            clear_and_exit_program(5, "undefinded variable in expression");
            break;
    }
}

void check_func_return() {
    switch (parser.declared_function->func_data.ret_type) {
        case RETTYPE_STRING:
            if (parser.declared_function->func_data.returned == false &&
                parser.declared_function->func_data.optional_ret_type ==
                    false) {
                clear_and_exit_program(
                    4, "missing return in function with required return type");
            }
            break;
        case RETTYPE_FLOAT:
            if (parser.declared_function->func_data.returned == false &&
                parser.declared_function->func_data.optional_ret_type ==
                    false) {
                clear_and_exit_program(
                    4, "missing return in function with required return type");
            }
            break;
        case RETTYPE_INT:
            if (parser.declared_function->func_data.returned == false &&
                parser.declared_function->func_data.optional_ret_type ==
                    false) {
                clear_and_exit_program(
                    4, "missing return in function with required return type");
            }
            break;
    }
}

bool check_return_type() {
    if (!check_token_type(K_INT) && !check_token_type(K_STRING) &&
        !check_token_type(K_FLOAT) && !check_token_type(K_VOID)) {
        return false;
    }
    // store function return type to symtable
    switch (parser.token->token_type) {
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
        default:
            break;
    }
    return true;
}
// <term> rule
bool term() {
    switch (parser.token->token_type) {
        case L_VARID:
            if (parser.in_function) {
                if (!htab_search(parser.local_symtable, parser.token->string)) {
                    clear_and_exit_program(
                        5, "undefined variable as function input param");
                }
                if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                    generate_local_var_func_param(parser.param_counter + 1,
                                                  parser.token->string, true);
                } else
                    generate_local_var_func_param(parser.param_counter + 1,
                                                  parser.token->string, false);
            } else {
                if (!htab_search(parser.global_symtable,
                                 parser.token->string)) {
                    clear_and_exit_program(
                        5, "undefined variable as function input param");
                }
                if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                    generate_global_var_func_param(parser.param_counter + 1,
                                                   parser.token->string, true);
                } else
                    generate_global_var_func_param(parser.param_counter + 1,
                                                   parser.token->string, false);
            }

            return true;
        case L_STRING:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_string_func_param(parser.param_counter + 1,
                                           parser.token->string, true);
            } else
                generate_string_func_param(parser.param_counter + 1,
                                           parser.token->string, false);
            return true;
        case L_FLOAT:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_float_func_param(parser.param_counter + 1,
                                          parser.token->float_val, true);
            } else
                generate_float_func_param(parser.param_counter + 1,
                                          parser.token->float_val, false);
            return true;
        case L_NUMBER:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_int_func_param(parser.param_counter + 1,
                                        parser.token->val, true);
            } else
                generate_int_func_param(parser.param_counter + 1,
                                        parser.token->val, false);
            return true;
        case K_NULL:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_null_func_param(parser.param_counter + 1, true);
            } else
                generate_null_func_param(parser.param_counter + 1, false);
            return true;
        default:
            clear_and_exit_program(2, "wrong term in function call");
    }
    return false;
}

// <next_input_param> rule
bool next_input_param() {
    if (parser.token->token_type != L_RPAR) {
        consume_token(L_COMMA, "missing comma before next input parameter");
        get_next_token();
        term();
        parser.param_counter++;
        if (parser.func_check == false)
            parser.global_symtable_data->func_data.param_count =
                parser.param_counter;
        get_next_token();
        next_input_param();
        return true;
    }
    // epsilon
    return true;
}

// <list_input_params> rule
bool list_input_params() {
    parser.param_counter = 0;
    if (parser.token->token_type != L_RPAR) {
        term();
        parser.param_counter++;
        if (parser.func_check == false)
            parser.global_symtable_data->func_data.param_count =
                parser.param_counter;
        get_next_token();
        next_input_param();
        return true;
    }
    return true;
}

// <statement> rule
bool statement() {
    if (parser.token->token_type == L_VARID) {
        // add var to hashtable
        symtable_var_check();
        token_t* tmp_var = (token_t*)malloc(sizeof(token_t));
        *tmp_var = *parser.token;
        get_next_token();
        if (parser.token->token_type == L_ASSIGN) {
            var_init();
            get_next_token();
            if (parser.token->token_type == L_FUNCID) {
                check_func_id(false);
                get_token_consume_token(L_LPAR,
                                        "missing left paren in function call");
                generate_tmp_frame();
                get_next_token();
                list_input_params();

                if (parser.func_check &&
                    parser.param_counter !=
                        parser.global_symtable_data->func_data.param_count) {
                    clear_and_exit_program(
                        4, "wrong param count in function call");
                }

                consume_token(L_RPAR, "missing right paren in function call");
                get_token_consume_token(L_SEMICOL,
                                        "missing semicolon after statement");
                if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                    generate_null_assignment();
                } else {
                    generate_func_call(parser.global_symtable_data->name);
                }
                if (parser.in_function) {
                    generate_local_assignment(tmp_var->string);
                } else {
                    generate_global_assignment(tmp_var->string);
                }
                get_next_token();
                statement();
                return true;
            } else {
                expression_parser(parser.token, true);
                if (parser.in_function) {
                    generate_exp_local_assignment(tmp_var->string);
                } else {
                    generate_exp_global_assignment(tmp_var->string);
                }
            }
        } else {
            if (!check_token_type(L_SEMICOL))
                expression_parser(tmp_var, true);
            else if (parser.in_function)
                generate_one_operand(tmp_var, parser.in_function,
                                     parser.local_symtable);
            else
                generate_one_operand(tmp_var, parser.in_function,
                                     parser.global_symtable);
        }
        get_next_token();
        statement();
        return true;
    }
    if (parser.token->token_type == L_FUNCID) {
        check_func_id(false);
        get_token_consume_token(L_LPAR, "missing left paren in function call");
        generate_tmp_frame();
        get_next_token();
        list_input_params();

        if (parser.func_check &&
            parser.param_counter !=
                parser.global_symtable_data->func_data.param_count) {
            clear_and_exit_program(4, "wrong param count in function call");
        }

        consume_token(L_RPAR, "missing right paren in function call");
        get_token_consume_token(L_SEMICOL, "missing semicolon after statement");
        if (strcmp(parser.global_symtable_data->name, "write") != 0) {
            generate_func_call(parser.global_symtable_data->name);
        }
        get_next_token();
        statement();
        return true;
    }
    if (parser.token->token_type == K_IF) {
        int if_scope = parser.scope + 1;
        parser.scope++;
        get_token_consume_token(L_LPAR, "missing left paren in if statement");
        expression_parser(parser.token, false);
        // consume_token(L_RPAR, "missing right paren in if statement");
        generate_if_then(if_scope);
        // consume_token(L_LCURL, "missing left curl bracket in if statement");
        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        get_token_consume_token(K_ELSE, "missing else");
        generate_if_else(if_scope);
        get_token_consume_token(L_LCURL,
                                "missing left curl bracket in if statement");
        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        generate_if_end(if_scope);
        get_next_token();
        statement();
        return true;
    }
    if (parser.token->token_type == K_WHILE) {
        parser.in_while = true;
        int while_scope = parser.scope + 1;
        parser.scope++;
        get_token_consume_token(L_LPAR,
                                "missing left paren in while statement");
        generate_while_start(while_scope);
        expression_parser(parser.token, false);
        // get_token_consume_token(L_RPAR,
        //                         "missing right paren in while statement");
        generate_while_condition(while_scope);
        // get_token_consume_token(L_LCURL,
        //                         "missing left curl bracket in if statement");
        // consume_token(L_LCURL, "missing left curl bracket in while
        // statement");

        get_next_token();
        statement();
        consume_token(L_RCURL, "missing right curl bracket in if statement");
        generate_while_end(while_scope);
        parser.in_while = false;
        get_next_token();
        statement();
        return true;
    }
    if (parser.token->token_type == K_RETURN) {
        if (parser.in_function) {
            if (parser.declared_function->func_data.ret_type == RETTYPE_VOID) {
                get_next_token();
                if (!check_token_type(L_SEMICOL)) {
                    clear_and_exit_program(
                        6, "return in void function contain expression");
                }
                generate_return(parser.declared_function->name, true);
            } else {
                get_next_token();
                if (check_token_type(L_SEMICOL)) {
                    clear_and_exit_program(
                        6, "return empty expression in non void function");
                }
                parser.declared_function->func_data.returned = true;
                expression_parser(parser.token, true);
                generate_return(parser.declared_function->name, false);
            }
        } else {
            get_next_token();
            if (!check_token_type(L_SEMICOL)) {
                expression_parser(parser.token, true);
            }
            generate_exit_program();
        }
        get_next_token();
        statement();
        return true;
    }
    if (/*!check_token_type(L_SEMICOL) &&*/
        (check_token_type(L_NUMBER) || check_token_type(L_STRING) ||
         check_token_type(L_FLOAT))) {
        expression_parser(parser.token, true);
        get_next_token();
        statement();
        return true;
    }
    if (check_token_type(L_SEMICOL)) {
        clear_and_exit_program(2, "empty semicolon");
    }

    // epsilon
    return true;
}

// <type> rule
bool type() {
    // ?type (int, string, float)
    if (parser.token->token_type == L_VARPREF) {
        parser.local_symtable_data->var_data.optional_type = true;
        get_next_token();
        if (check_param_types()) {
            return true;
        } else {
            clear_and_exit_program(
                2, "wrong data type of argument in function declaration");
        }
    }
    // type (int, string, float)
    if (check_param_types()) {
        return true;
    } else {
        clear_and_exit_program(
            2, "wrong data type of argument in function declaration");
    }
    return false;
}

// <return_type> rule
bool return_type() {
    // ?return_type (int, string, float, void)
    if (parser.token->token_type == L_VARPREF) {
        parser.global_symtable_data->func_data.optional_ret_type = true;
        get_next_token();
        if (check_return_type()) {
            return true;
        } else {
            clear_and_exit_program(2,
                                   "wrong return type in function declaration");
        }
    }
    // return_type (int, string, float, void)
    if (check_return_type()) {
        return true;
    } else {
        clear_and_exit_program(2, "wrong return type in function declaration");
    }
    return false;
}

// <next_parameter> rule
bool next_parameter() {
    int param_scope = parser.scope;
    if (!check_token_type(L_RPAR)) {
        create_new_local_data();
        consume_token(L_COMMA, "missing comma between arguments");
        get_next_token();
        type();
        get_token_consume_token(L_VARID,
                                "missing variable identifier after data type "
                                "in function declaration");
        // add variable to symtable and add param to function declaration
        parser.local_symtable_data->var_data.init = true;
        parser.local_symtable_data->name = parser.token->string;
        parser.local_symtable_data->type = ID_VAR;
        htab_insert_update(parser.local_symtable, parser.token->string,
                           parser.local_symtable_data);

        if (parser.func_check == false) {
            parser.global_symtable_data->func_data.param_count++;
            generate_func_param(
                htab_search(parser.local_symtable, parser.token->string),
                parser.global_symtable_data->func_data.param_count,
                param_scope);
        } else {
            parser.param_counter++;
            generate_func_param(
                htab_search(parser.local_symtable, parser.token->string),
                parser.param_counter, param_scope);
        }

        get_next_token();
        next_parameter();
        return true;
    }

    // epsilon
    return true;
}

// <list_params> rule
bool list_params() {
    parser.param_counter = 0;
    int param_scope = parser.scope;
    if (!check_token_type(L_RPAR)) {
        create_new_local_data();
        if (type()) {
            get_token_consume_token(
                L_VARID, "missing variable identifier after data type");

            // add param variable to local symtable
            parser.local_symtable_data->var_data.init = true;
            parser.local_symtable_data->name = parser.token->string;
            parser.local_symtable_data->type = ID_VAR;
            htab_insert_update(parser.local_symtable, parser.token->string,
                               parser.local_symtable_data);

            // checking param count
            if (parser.func_check == false) {
                parser.global_symtable_data->func_data.param_count++;
                generate_func_param(
                    htab_search(parser.local_symtable, parser.token->string),
                    parser.global_symtable_data->func_data.param_count,
                    param_scope);
            } else {
                parser.param_counter++;
                generate_func_param(
                    htab_search(parser.local_symtable, parser.token->string),
                    parser.param_counter, param_scope);
            }

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
    parser.in_while = false;
    parser.in_function = false;
    parser.local_symtable = htab_init(10);
    if (parser.token->token_type == LEOF) {
        // check if all function call are defined
        generate_func_declaration(parser.global_symtable, "main", false);
        generate_end();
        htab_free(parser.local_symtable);
        return true;
    }
    if (parser.token->token_type == L_PHPEND) {
        // check if all function call are defined
        get_token_consume_token(LEOF, "missing eof after php epilogue");
        generate_func_declaration(parser.global_symtable, "main", false);
        generate_end();
        htab_free(parser.local_symtable);
        return true;
    }
    if (parser.token->token_type == K_FUNCTION) {
        parser.in_function = true;
        int function_scope = parser.scope;
        get_token_consume_token(L_FUNCID,
                                "missing function identifier keyword");

        check_func_id(true);

        parser.declared_function = parser.global_symtable_data;

        generate_func_header(parser.global_symtable_data->name, function_scope);

        get_token_consume_token(L_LPAR,
                                "missing left paren in function declaration");

        get_next_token();
        list_params();

        if (parser.func_check &&
            parser.param_counter !=
                parser.global_symtable_data->func_data.param_count) {
            clear_and_exit_program(4, "wrong param count in function call");
        }

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
        check_func_return();
        // ht_print_table(parser.local_symtable, "LOCAL");
        generate_func_declaration(parser.local_symtable,
                                  parser.declared_function->name, true);
        generate_func_end(function_scope, parser.declared_function);
        parser.scope++;

        htab_free(parser.local_symtable);

        get_next_token();
        program();
        return true;
    }
    if (statement()) {
        // get_next_token();
        program();
        return true;
    }

    clear_and_exit_program(2, "missing eof or php epilog");

    return false;
}

// <prolog> rule
bool prolog() {
    if (parser.token->token_type == L_PHPSTART) {
        get_token_consume_token(K_DECLARE, "missing declare after php head");
        get_token_consume_token(L_LPAR,
                                "missing left paren if strict type declare");
        get_token_consume_token(K_STRICTTYPES,
                                "missing strict_type declaration");
        get_token_consume_token(L_ASSIGN, "missing assignment to strict_types");
        get_token_consume_token(
            L_NUMBER, "value assigned to strict_types must be integer");
        if (parser.token->val != 1) {
            clear_and_exit_program(2, "strict_type muset be set to 1");
        }
        get_token_consume_token(L_RPAR,
                                "missing right paren in declare strict types");
        get_token_consume_token(L_SEMICOL, "missing semicolon after declare");

        generate_header();

        get_next_token();
        program();
        return true;
    }
    htab_free(parser.global_symtable);
    clear_and_exit_program(2, "missing php head and strict_types declaration");
    return false;
}

bool syntax_analyse() {
    get_next_token();

    parser.global_symtable = htab_init(10);
    parser.scope = 0;

    // load builtin funcs to symtable
    load_builtin_funcs();

    prolog();

    // just for testing
    // ht_print_table(parser.global_symtable, "GLOBAL");

    htab_free(parser.global_symtable);

    return true;
}
