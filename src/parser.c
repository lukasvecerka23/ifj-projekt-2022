/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Implementation of parser
 */

#include "parser.h"

void get_next_token() {
    token_t* tmp_token = parser.token;
    parser.token = get_lex_value();
    free(tmp_token);
}

void consume_token(token_type token_type, char* err_msg) {
    if (parser.token->token_type != token_type) {
        clear_and_exit_program(2, err_msg);
    }
}

void get_token_consume_token(token_type token_type, char* err_msg) {
    get_next_token();
    if (parser.token->token_type != token_type) {
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
    if (parser.in_function && !parser.local_symtable)
        htab_clear(parser.local_symtable);
    if (!parser.global_symtable)
        htab_clear(parser.global_symtable);
    exit_program(err_code, msg);
}

void add_builtin_func(htab_item_data_t data, char* func_name) {
    htab_item_data_t* func_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));
    if (func_data == NULL)
        clear_and_exit_program(99, "malloc error");
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

    // write
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

void check_if_all_func_defined(htab_t* table) {
    for (size_t i = 0; i < table->arr_size; i++) {
        htab_item_t* item = table->arr_ptr[i];
        while (item != NULL) {
            if (item->data->type == ID_FUNC &&
                item->data->func_data.defined == false)
                clear_and_exit_program(3, "calling undefined function");

            item = item->next;
        }
    }
}

void create_new_local_data() {
    htab_item_data_t* local_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));
    if (local_data == NULL)
        clear_and_exit_program(99, "malloc error");
    parser.local_symtable_data = local_data;
}

void create_new_global_data() {
    htab_item_data_t* global_data =
        (htab_item_data_t*)malloc(sizeof(htab_item_data_t));
    if (global_data == NULL)
        clear_and_exit_program(99, "malloc error");
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

        // init
        parser.global_symtable_data->name = parser.token->string;
        parser.global_symtable_data->type = ID_FUNC;
        parser.global_symtable_data->func_data.param_count = 0;
        parser.global_symtable_data->func_data.optional_ret_type = false;
        parser.global_symtable_data->func_data.returned = false;
        htab_insert_update(parser.global_symtable, parser.token->string,
                           parser.global_symtable_data);
        // check param count disable
        parser.func_check = false;
    } else {
        // update global data
        parser.global_symtable_data = tmp_item->data;
        // if function is not write, change flag of parser to check param count
        if (strcmp(tmp_item->key, "write") != 0) {
            parser.func_check = true;
        } else {
            parser.func_check = false;
        }

        // if there is record check if function is already defined, if yes
        // return error if not update the symtable record
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
    // if in function declaration looks to local symtable, otherwise looks to
    // global
    if (parser.in_function) {
        tmp_item = htab_search(parser.local_symtable, parser.token->string);
        // if var not in symtable create new one
        if (tmp_item == NULL) {
            create_new_local_data();
            // init
            parser.local_symtable_data->name = parser.token->string;
            parser.local_symtable_data->type = ID_VAR;
            htab_insert_update(parser.local_symtable, parser.token->string,
                               parser.local_symtable_data);
        } else {
            // update parser local data
            parser.local_symtable_data = tmp_item->data;
        }

    } else {
        tmp_item = htab_search(parser.global_symtable, parser.token->string);
        // if not in symtable create new record
        if (tmp_item == NULL) {
            create_new_global_data();
            // init
            parser.global_symtable_data->name = parser.token->string;
            parser.global_symtable_data->type = ID_VAR;
            htab_insert_update(parser.global_symtable, parser.token->string,
                               parser.global_symtable_data);
        } else {
            // update parser global data
            parser.global_symtable_data = tmp_item->data;
        }
    }
}

void check_param_count() {
    if (parser.func_check &&
        parser.param_counter !=
            parser.global_symtable_data->func_data.param_count) {
        clear_and_exit_program(4, "wrong param count in function call");
    }
}

void expression_parser(token_t* token, token_t* token2, bool check_semicolon) {
    // tree is given by reference
    ast_node_t* new_tree;
    int err_code;
    err_code = parse_expression(token, token2, check_semicolon, &new_tree);

    switch (err_code) {
        case 0:
            if (check_semicolon == false && ast_height(new_tree) == 1)
                if (parser.in_function)
                    generate_one_operand(new_tree->token, parser.in_function);
                else
                    generate_one_operand(new_tree->token, parser.in_function);
            else if (parser.in_function) {
                parser.scope++;
                generate_ast(new_tree, parser.in_function, &parser.scope);
            } else {
                parser.scope++;
                generate_ast(new_tree, parser.in_function, &parser.scope);
            }
            break;
        case 2:
            clear_and_exit_program(2, "syntax error in expression parser");
            break;
        case 5:
            clear_and_exit_program(5, "undefinded variable in expression");
            break;
    }
}

void check_func_return() {
    if (parser.declared_function->func_data.ret_type != RETTYPE_VOID) {
        if (parser.declared_function->func_data.returned == false &&
            parser.declared_function->func_data.optional_ret_type == false) {
            clear_and_exit_program(
                4, "missing return in function with required return type");
        }
    }
}

// TERM rule
void term() {
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

            return;
        case L_STRING:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_string_func_param(parser.param_counter + 1,
                                           parser.token->string, true);
            } else
                generate_string_func_param(parser.param_counter + 1,
                                           parser.token->string, false);
            return;
        case L_FLOAT:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_float_func_param(parser.param_counter + 1,
                                          parser.token->float_val, true);
            } else
                generate_float_func_param(parser.param_counter + 1,
                                          parser.token->float_val, false);
            return;
        case L_NUMBER:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_int_func_param(parser.param_counter + 1,
                                        parser.token->val, true);
            } else
                generate_int_func_param(parser.param_counter + 1,
                                        parser.token->val, false);
            return;
        case K_NULL:
            if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                generate_null_func_param(parser.param_counter + 1, true);
            } else
                generate_null_func_param(parser.param_counter + 1, false);
            return;
        default:
            clear_and_exit_program(2, "wrong term in function call");
    }
    return;
}

// TYPE rule
void type() {
    // ?type (int, string, float)
    if (parser.token->token_type == L_VARPREF) {
        parser.local_symtable_data->var_data.optional_type = true;
        get_next_token();
        if (check_param_types()) {
            return;
        } else {
            clear_and_exit_program(
                2, "wrong data type of argument in function declaration");
        }
    }
    // type (int, string, float)
    if (check_param_types()) {
        return;
    } else {
        clear_and_exit_program(
            2, "wrong data type of argument in function declaration");
    }
    return;
}

// RETURN_TYPE rule
void return_type() {
    // ?return_type (int, string, float, void)
    if (parser.token->token_type == L_VARPREF) {
        parser.global_symtable_data->func_data.optional_ret_type = true;
        get_next_token();
        if (check_return_type()) {
            return;
        } else {
            clear_and_exit_program(2,
                                   "wrong return type in function declaration");
        }
    }
    // return_type (int, string, float, void)
    if (check_return_type()) {
        return;
    } else {
        clear_and_exit_program(2, "wrong return type in function declaration");
    }
    return;
}

// NEXT_INPUT_PARAM rule
void next_input_param() {
    if (parser.token->token_type != L_RPAR) {
        consume_token(L_COMMA, "missing comma before next input parameter");
        get_next_token();
        term();
        parser.param_counter++;
        // update symtable data with param count
        if (parser.func_check == false)
            parser.global_symtable_data->func_data.param_count =
                parser.param_counter;

        get_next_token();
        next_input_param();

        return;
    }
    // epsilon
    return;
}

// LIST_INPUT_PARAM rule
void list_input_params() {
    parser.param_counter = 0;
    if (parser.token->token_type != L_RPAR) {
        term();
        parser.param_counter++;
        if (parser.func_check == false)
            parser.global_symtable_data->func_data.param_count =
                parser.param_counter;
        get_next_token();
        next_input_param();
        return;
    }
    return;
}

// <statement> rule
void statement() {
    if (parser.token->token_type == L_VARID) {
        // add var to hashtable
        symtable_var_check();
        // store data about variable
        token_t* tmp_var = (token_t*)malloc(sizeof(token_t));
        *tmp_var = *parser.token;
        get_next_token();
        // $a = ...
        if (parser.token->token_type == L_ASSIGN) {
            get_next_token();
            // $a = funcid(...);
            if (parser.token->token_type == L_FUNCID) {
                check_func_id(false);
                get_token_consume_token(L_LPAR,
                                        "missing left paren in function call");
                generate_tmp_frame();

                get_next_token();
                list_input_params();

                // check if param count are correct
                check_param_count();

                consume_token(L_RPAR, "missing right paren in function call");
                get_token_consume_token(L_SEMICOL,
                                        "missing semicolon after statement");

                if (strcmp(parser.global_symtable_data->name, "write") == 0) {
                    generate_null_assignment();
                } else {
                    generate_func_call(parser.global_symtable_data->name);
                }
                // assignment to variable
                if (parser.in_function) {
                    generate_local_assignment(tmp_var->string);
                } else {
                    generate_global_assignment(tmp_var->string);
                }
                get_next_token();
                statement();
                return;
            } else {
                // $x = 5 + ...
                expression_parser(parser.token, NULL, true);
                if (parser.in_function) {
                    generate_exp_local_assignment(tmp_var->string);
                } else {
                    generate_exp_global_assignment(tmp_var->string);
                }
            }
        } else {
            // $x; or $x + ...
            if (!check_token_type(L_SEMICOL))
                expression_parser(tmp_var, parser.token, true);
            else if (parser.in_function)
                generate_one_operand(tmp_var, parser.in_function);
            else
                generate_one_operand(tmp_var, parser.in_function);
        }

        get_next_token();
        statement();
        return;
    }
    // funcid(...);
    if (parser.token->token_type == L_FUNCID) {
        check_func_id(false);  // check funcid in symtable
        get_token_consume_token(L_LPAR, "missing left paren in function call");
        generate_tmp_frame();

        get_next_token();
        list_input_params();

        // check if func param count are correct
        check_param_count();

        consume_token(L_RPAR, "missing right paren in function call");
        get_token_consume_token(L_SEMICOL, "missing semicolon after statement");

        if (strcmp(parser.global_symtable_data->name, "write") != 0) {
            generate_func_call(parser.global_symtable_data->name);
        }

        get_next_token();
        statement();
        return;
    }
    // if(...){...}else{...}
    if (parser.token->token_type == K_IF) {
        parser.scope++;
        int if_scope = parser.scope;
        parser.in_while_if = true;

        get_token_consume_token(L_LPAR, "missing left paren in if statement");

        expression_parser(parser.token, NULL, false);
        generate_if_then(if_scope);

        get_next_token();
        statement();
        // after get back from statement, have to update parser state
        parser.in_while_if = true;

        consume_token(L_RCURL, "missing right curl bracket in if statement");
        get_token_consume_token(K_ELSE, "missing else");

        generate_if_else(if_scope);

        get_token_consume_token(L_LCURL,
                                "missing left curl bracket in if statement");
        get_next_token();
        statement();
        // update parser state
        parser.in_while_if = true;

        consume_token(L_RCURL, "missing right curl bracket in if statement");

        generate_if_end(if_scope);

        get_next_token();
        statement();
        parser.in_while_if = false;

        return;
    }
    // while(...){...}
    if (parser.token->token_type == K_WHILE) {
        parser.scope++;
        int while_scope = parser.scope;
        parser.in_while_if = true;

        get_token_consume_token(L_LPAR,
                                "missing left paren in while statement");
        generate_while_start(while_scope);

        expression_parser(parser.token, NULL, false);

        generate_while_condition(while_scope);

        get_next_token();
        statement();
        // have to update state of parser
        parser.in_while_if = true;

        consume_token(L_RCURL, "missing right curl bracket in if statement");

        generate_while_end(while_scope);

        get_next_token();
        statement();
        parser.in_while_if = false;

        return;
    }
    // return; or return ...;
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
                expression_parser(parser.token, NULL, true);
                generate_return(parser.declared_function->name, false);
            }
        } else {
            get_next_token();
            if (!check_token_type(L_SEMICOL)) {
                expression_parser(parser.token, NULL, true);
            }
            generate_exit_program();
        }

        get_next_token();
        statement();

        return;
    }

    // check all combination of empty statement, then return epsilon
    if (check_token_type(LEOF) || check_token_type(K_FUNCTION) ||
        check_token_type(L_PHPEND) ||
        (check_token_type(L_RCURL) &&
         (parser.in_while_if || parser.in_function)))
        return;

    // if not epsilon try to parse expression
    // <expression>;
    if (check_token_type(L_NUMBER) || check_token_type(L_STRING) ||
        check_token_type(L_FLOAT) || check_token_type(L_LPAR)) {
        expression_parser(parser.token, NULL, true);
        get_next_token();
        statement();
        return;
    }

    clear_and_exit_program(2, "syntax error in statement");
}

// <next_parameter> rule
void next_parameter() {
    int param_scope = parser.scope;
    if (!check_token_type(L_RPAR)) {
        create_new_local_data();
        consume_token(L_COMMA, "missing comma between arguments");

        get_next_token();
        type();

        get_token_consume_token(L_VARID,
                                "missing variable identifier after data type "
                                "in function declaration");

        // check if parameter already exist in symtable
        if (htab_search(parser.local_symtable, parser.token->string))
            clear_and_exit_program(4, "redefinition of function parameter");

        // add variable to symtable and add param to function declaration
        parser.local_symtable_data->name = parser.token->string;
        parser.local_symtable_data->type = ID_VAR;
        htab_insert_update(parser.local_symtable, parser.token->string,
                           parser.local_symtable_data);

        // func_check false - function haven't been called yet, store parameter
        // count to symtable
        // func_chec true - function was called before definition, checking
        // parameter count
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
        return;
    }

    // epsilon
    return;
}

// <list_params> rule
void list_params() {
    parser.param_counter = 0;
    int param_scope = parser.scope;
    if (!check_token_type(L_RPAR)) {
        create_new_local_data();
        type();
        get_token_consume_token(L_VARID,
                                "missing variable identifier after data type");

        // check if parameter already exist in symtable
        if (htab_search(parser.local_symtable, parser.token->string))
            clear_and_exit_program(4, "redefinition of function parameter");

        // add param variable to local symtable
        parser.local_symtable_data->name = parser.token->string;
        parser.local_symtable_data->type = ID_VAR;
        htab_insert_update(parser.local_symtable, parser.token->string,
                           parser.local_symtable_data);

        // if func_check true - function is already in symtable and we need to
        // check if param count is correct,
        // if func_check false - load param count to symtable
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

        return;
    }
    // epsilon
    return;
}

// <program> rule
void program() {
    parser.in_while_if = false;
    parser.in_function = false;

    // eof
    if (parser.token->token_type == LEOF) {
        generate_func_declaration(parser.global_symtable, "main", false);
        generate_end();
        return;
    }
    // phpend
    if (parser.token->token_type == L_PHPEND) {
        get_token_consume_token(LEOF, "missing eof after php epilogue");
        generate_func_declaration(parser.global_symtable, "main", false);
        generate_end();
        return;
    }

    // function funcid(<list_params>): <ret_type>{...}
    if (parser.token->token_type == K_FUNCTION) {
        parser.local_symtable = htab_init(10);  // local table for this function
        parser.in_function = true;
        parser.scope++;
        int function_scope = parser.scope;

        get_token_consume_token(L_FUNCID,
                                "missing function identifier keyword");

        // check func_id in symtable
        check_func_id(true);

        // store declaring function for next parsing
        parser.declared_function = parser.global_symtable_data;

        generate_func_header(parser.global_symtable_data->name, function_scope);

        get_token_consume_token(L_LPAR,
                                "missing left paren in function declaration");

        get_next_token();
        list_params();

        // check if param count is correct
        check_param_count();

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

        // check if function is return correctly
        check_func_return();

        generate_func_declaration(parser.local_symtable,
                                  parser.declared_function->name, true);
        generate_func_end(function_scope, parser.declared_function);

        htab_free(parser.local_symtable);
        parser.in_function = false;
        get_next_token();
        program();

        return;
    }

    // <statement> <program>
    statement();
    program();
    return;
}

// <prolog> rule
void prolog() {
    // <?php declare(strict_types=1);
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
            clear_and_exit_program(2, "strict_type must be set to 1");
        }
        get_token_consume_token(L_RPAR,
                                "missing right paren in declare strict types");
        get_token_consume_token(L_SEMICOL, "missing semicolon after declare");

        generate_header();

        get_next_token();
        program();
        return;
    }
    clear_and_exit_program(2, "missing php head and strict_types declaration");
}

void syntax_analyse() {
    get_next_token();

    parser.global_symtable = htab_init(10);
    parser.scope = 0;

    // load builtin funcs to symtable
    load_builtin_funcs();

    prolog();

    // check if all function are defined
    check_if_all_func_defined(parser.global_symtable);

    htab_free(parser.global_symtable);

    return;
}
/*END OF FILE*/
