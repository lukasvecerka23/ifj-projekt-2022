/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Expression syntax check and creation of AST tree for expressions.
 */

#include "exp_parser.h"

void stack_init(Stack* stack) {
    stack->top = NULL;
}

precedence_symbols prec_table[TABLE_SIZE][TABLE_SIZE] = {
    /*   +  *  (  )  i  $  -  /  .   <  >  <= >= === !==*/
    {R, S, S, R, S, R, R, S, Er, R, R, R, R, R, R},       // +
    {R, R, S, R, S, R, R, R, Er, R, R, R, R, R, R},       // *
    {S, S, S, W, S, Er, S, S, S, S, S, S, S, S, S},       // (
    {R, R, Er, R, Er, R, R, R, R, R, R, R, R, R, R},      // )
    {R, R, Er, R, Er, R, R, R, R, R, R, R, R, R, R},      // i
    {S, S, S, Er, S, Er, S, S, S, S, S, S, S, S, S},      // $
    {R, S, S, R, S, R, R, S, Er, R, R, R, R, R, R},       // -
    {R, R, S, R, S, R, R, R, Er, R, R, R, R, R, R},       // /
    {Er, E, S, R, S, R, Er, E, R, R, R, R, R, R, R},      // .
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er},  // <
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er},  // >
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er},  // <=
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er},  // >=
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er},  // ===
    {S, S, S, R, S, R, S, S, S, Er, Er, Er, Er, Er, Er}   // !==

};

precedence_symbols map_token_to_enum(token_t* token) {
    switch (token->token_type) {
        case L_LPAR:
            return T_LPAR;
        case L_RPAR:
            return T_RPAR;
        case L_DOT:
            return T_CONCAT;
        case L_MUL:
            return T_MUL;
        case L_SLASH:
            return T_DIV;
        case L_PLUS:
            return T_PLUS;
        case L_DASH:
            return T_MINUS;
        case L_VARID:
            return T_INT;
        case L_NUMBER:
            return T_INT;
        case L_STRING:
            return T_INT;
        case K_NULL:
            return T_INT;
        case L_EQ:
            return T_EQ;
        case L_EXP:
            return T_INT;
        case L_NEQ:
            return T_NEQ;
        case L_LESS:
            return T_LESS;
        case L_GREATER:
            return T_GREATER;
        case L_LESSEQ:
            return T_LESSEQ;
        case L_GREATEREQ:
            return T_GREATEREQ;
        case L_FLOAT:
            return T_INT;
        case L_SEMICOL:
            return $;
        case L_LCURL:
            return $;
        default:
            return T_INVALID;
            break;
    }
    return T_INVALID;
}

precedence_symbols stack_top_nonterminal(Stack* stack) {
    Stack_exp tmp;
    tmp = stack->top;
    while (tmp->data != EMPTY) {
        if (tmp->data >= T_PLUS && tmp->data <= T_NEQ) {
            return tmp->data;
        }

        tmp = tmp->next_element;
    }
    return EMPTY;
}

precedence_symbols stack_top(Stack* stack) {
    return stack->top->data;
}

void stack_push(Stack* stack, precedence_symbols data) {
    Stack_exp new_element = malloc(sizeof(struct stack_el));
    if (new_element == NULL) {
    }
    new_element->data = data;
    if (stack->top != NULL) {
        new_element->next_element = stack->top;
    } else {
        new_element->next_element = NULL;
    }
    stack->top = new_element;
}

void stack_shift_push(Stack* stack) {
    Stack_exp tmp;
    Stack_exp prev;
    tmp = stack->top;
    prev = NULL;

    while (tmp->data != EMPTY) {
        if (tmp->data >= T_PLUS && tmp->data <= T_NEQ) {
            Stack_exp new_element = malloc(sizeof(struct stack_el));
            if (new_element == NULL) {
            }
            new_element->data = S;
            if (prev != NULL) {
                new_element->next_element = prev->next_element;
                prev->next_element = new_element;
                return;
            } else {
                new_element->next_element = tmp;
                stack->top = new_element;
                return;
            }
            return;
        }
        prev = tmp;
        tmp = tmp->next_element;
    }
}

void stack_print_stack(Stack* stack) {
    Stack_exp tmp = stack->top;
    while (tmp != NULL) {
        tmp = tmp->next_element;
    }
}

// pops first element
Stack_exp stack_pop(Stack* stack) {
    if (stack->top->data != EMPTY) {
        Stack_exp to_return = stack->top;
        Stack_exp next = stack->top->next_element;
        stack->top = next;
        return to_return;
    } else {
        return NULL;
    }
}

int stack_empty(Stack* stack) {
    if (stack->top->data == EMPTY) {
        return 1;
    } else {
        return 0;
    }
    return 0;
}

int exp_correct_syntax(Stack* stack) {
    if (stack->top != NULL) {
        if (stack->top->data == E) {
            if (stack->top->next_element != NULL) {
                if (stack->top->next_element->data == $) {
                    if (stack->top->next_element->next_element != NULL) {
                        if (stack->top->next_element->next_element->data ==
                            EMPTY) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int rule_reduction(Stack* stack) {
    Stack_exp stack_data[4];
    for (int j = 0; j < 5; j++) {
        stack_data[j] = malloc(sizeof(Stack_exp));
    }
    for (int i = 0; i <= 3; i++) {
        stack_data[i] = stack_pop(stack);
        if ((stack_data[i])->data == S) {
            break;
        }
    }
    if (stack_data[0]->data == S) {
        return 0;
    }
    if (stack_data[1]->data == S && stack_data[0]->data == T_INT) {  // i -> E
        stack_push(stack, E);
        if (stack_data[0]->token->token_type == L_VARID) {
        }
        stack->top->tree = make_leaf(stack_data[0]->token);
        stack->top->token = stack_data[0]->token;
        return 1;
    }
    if (stack_data[0]->data == T_RPAR && stack_data[1]->data == E &&
        stack_data[2]->data == T_LPAR) {
        stack_push(stack, E);
        stack->top->token = stack_data[1]->token;
        stack->top->tree = stack_data[1]->tree;
        return 1;
    }
    if (stack_data[0]->data == E && stack_data[2]->data == E) {  // E op E
        ast_node_t* tree_ptr;
        token_t* operator=(token_t*) malloc(sizeof(token_t));
        switch (stack_data[1]->data) {
            case T_PLUS:
                operator->token_type = L_PLUS;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_MUL:
                operator->token_type = L_MUL;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_DIV:
                operator->token_type = L_SLASH;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_GREATER:
                operator->token_type = L_GREATER;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_GREATEREQ:
                operator->token_type = L_GREATEREQ;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_LESS:
                operator->token_type = L_LESS;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_LESSEQ:
                operator->token_type = L_LESSEQ;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_MINUS:
                operator->token_type = L_DASH;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_CONCAT:
                operator->token_type = L_DOT;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_EQ:
                operator->token_type = L_EQ;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_NEQ:
                operator->token_type = L_NEQ;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;

            default:
                break;
        }
        stack_push(stack, E);
        stack->top->tree = tree_ptr;
        return 1;
    } else {
        return 0;
    }
}

int parse_expression(token_t* used_token,
                     token_t* used_token2,
                     bool is_expression,
                     ast_node_t** tree) {
    //   stack init
    Stack stack;
    stack_init(&stack);
    stack_push(&stack, EMPTY);
    int first_token_used = 0;
    int second_token = 0;
    if (used_token2 != NULL) {
        second_token = 1;
    }

    token_t* current_token;
    precedence_symbols current_token_enum;
    if (used_token != NULL) {
        current_token_enum = map_token_to_enum(used_token);
        if (current_token_enum == T_INVALID) {
            return 2;
        }
        current_token = used_token;
    } else {
        current_token = get_lex_value();
        current_token_enum = map_token_to_enum(current_token);
        if (current_token_enum == T_INVALID) {
            return 2;
        }
    }
    precedence_symbols top;

    stack_push(&stack, $);

    do {
        stack_print_stack(&stack);
        top = stack_top_nonterminal(&stack);
        switch (prec_table[top][current_token_enum]) {
            case W:  // get next token and push current
                // next token
                stack_push(&stack, current_token_enum);
                stack.top->token = current_token;
                if (used_token2 != NULL && second_token) {
                    current_token = used_token2;
                    second_token = 0;
                } else {
                    current_token = get_lex_value();
                }
                current_token_enum = map_token_to_enum(current_token);
                if (current_token_enum == T_INVALID) {
                    return 2;
                }
                break;
            case S:  // shift
                stack_shift_push(&stack);
                stack_push(&stack, current_token_enum);
                stack.top->token = current_token;
                if (used_token2 != NULL && second_token) {
                    current_token = used_token2;
                    second_token = 0;
                } else {
                    current_token = get_lex_value();
                }
                if (!first_token_used) {
                    first_token_used = 1;
                    if (used_token != NULL) {
                        if (current_token->token_type == L_RPAR &&
                            used_token->token_type == L_LPAR) {
                            current_token = get_lex_value();
                            if (current_token->token_type == L_LCURL) {
                                *tree = NULL;
                                return 0;
                            } else {
                                return 2;
                            }
                        }
                    }
                }
                current_token_enum = map_token_to_enum(current_token);
                if (current_token_enum == T_INVALID) {
                    return 2;
                }
                break;
            case R:  // reduce
                if (!rule_reduction(&stack)) {
                    return 2;
                }
                stack_print_stack(&stack);
                break;
            case Er:
                stack_print_stack(&stack);
                return 2;
                break;
            default:
                return 2;
                break;
        }
    } while ((current_token_enum != $) || (exp_correct_syntax(&stack) == 0));
    stack_print_stack(&stack);
    if (exp_correct_syntax(&stack)) {
        if (is_expression) {
            if (current_token->token_type != L_SEMICOL) {
                return 2;
            }
        } else {
            if (current_token->token_type != L_LCURL) {
                return 2;
            }
        }
        *tree = stack.top->tree;
        return 0;
    }
    return 2;
}

/*END OF FILE*/
