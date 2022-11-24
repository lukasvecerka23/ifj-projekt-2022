#include "exp_parser.h"
// #include "lexeme.h"

void stack_init(Stack* stack) {
    stack->top = NULL;
}

precedence_symbols map_token_to_enum(lexeme token) {
    switch (token.lex) {
        case L_LPAR:
            return T_LPAR;
        case L_RPAR:
            return T_RPAR;
        case L_DOT:
            return T_DOT;
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
        case L_EQ:
            return T_INT;
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
            return T_FLOAT;
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

void print_enum_as_str(precedence_symbols data) {
    switch (data) {
        case T_PLUS:
            printf("T_PLUS");
            break;
        case T_MUL:
            printf("T_MUL");
            break;
        case T_LPAR:
            printf("T_LPAR");
            break;
        case T_RPAR:
            printf("T_RPAR");
            break;
        case T_INT:
            printf("T_INT");
            break;
        case $:
            printf("$");
            break;
        case T_FLOAT:
            printf("T_FLOAT");
            break;
        case T_STRING:
            printf("T_STRING");
            break;
        case T_DIV:
            printf("T_DIV");
            break;
        case T_MINUS:
            printf("T_MINUS");
            break;
        case T_LESS:
            printf("T_LESS");
            break;
        case T_GREATER:
            printf("T_GREATER");
            break;
        case T_LESSEQ:
            printf("T_LESSEQ");
            break;
        case T_GREATEREQ:
            printf("T_GREATEREQ");
            break;
        case T_EQ:
            printf("T_EQ");
            break;
        case T_NEQ:
            printf("T_NEQ");
            break;
        case EMPTY:
            printf("EMPTY");
            break;
        case W:
            printf("W");
            break;
        case R:
            printf("R");
            break;
        case S:
            printf("S");
            break;
        case Er:
            printf("Er");
            break;
        case E:
            printf("E");
            break;
        default:
            printf("not in enum range %d", data);
            break;
    }
    // printf("hit\n");
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
    //
    return EMPTY;
}

precedence_symbols stack_top(Stack* stack) {
    return stack->top->data;
}

void stack_push(Stack* stack, precedence_symbols data) {
    Stack_exp new_element = malloc(sizeof(struct stack_el));
    if (new_element == NULL) {
        printf("error while allocating memory\n");
    }
    new_element->data = data;
    if (stack->top != NULL) {
        new_element->next_element = stack->top;
    } else {
        new_element->next_element = NULL;
    }
    // new_element->next_element = stack->next_element;
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
                printf("error while allocating memory\n");
            }
            new_element->data = S;
            // printf("new_element\n");
            // if (stack->top != NULL) {
            //     new_element->next_element = stack->top;
            // } else {
            //     new_element->next_element = NULL;
            // }
            if (prev != NULL) {
                new_element->next_element = prev->next_element;
                prev->next_element = new_element;
                return;
            } else {
                new_element->next_element = tmp;
                stack->top = new_element;
                return;
            }
            // prev->next_element = new_element;
            // printf("prev ");
            // print_enum_as_str(prev->data);
            // printf(" tmp ");
            // print_enum_as_str(tmp->data);
            // printf("\n");
            // new_element->next_element = tmp;
            // new_element->next_element = stack->next_element;
            // prev->next_element = new_element;
            return;
        }
        prev = tmp;
        tmp = tmp->next_element;
    }
    //
}

void stack_print_stack(Stack* stack) {
    Stack_exp tmp = stack->top;
    printf("top->");
    while (tmp != NULL) {
        // printf("in if\n");
        print_enum_as_str(tmp->data);
        printf("->");
        tmp = tmp->next_element;
    }
    printf("\n");
}

// pops first element
Stack_exp stack_pop(Stack* stack) {
    if (stack->top->data != EMPTY) {
        Stack_exp to_return = stack->top;
        Stack_exp next = stack->top->next_element;
        // free(stack->top);
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

// precedence_symbols prec_table[TABLE_SIZE][TABLE_SIZE] = {
//     {R, S, S, R, S, R}, {R, R, S, R, S, R}, {S, S, S, W, S, E},
//     {R, R, E, R, E, R}, {R, R, E, R, E, R}, {R, R, R, E, S, E}};
// /*
//  +  *  (  )  i  $
// {R, S, S, R, S, R} +
// {R, R, S, R, S, R} *
// {S, S, S, W, S, E} (
// {R, R, E, R, E, R} )
// {R, R, E, R, E, R} i
// {R, R, R, E, S, E} $
// */

void rule_reduction(Stack* stack) {
    // Stack_exp* stack_data = malloc(sizeof(Stack_exp) * 5);
    Stack_exp stack_data[4];
    for (int j = 0; j < 5; j++) {
        stack_data[j] = malloc(sizeof(Stack_exp));
    }
    // Stack_exp* stack_data;

    for (int i = 0; i <= 3; i++) {
        stack_data[i] = stack_pop(stack);
        // printf("popped: ");
        // print_enum_as_str(stack_data[i]->data);
        // printf("\n");
        if ((stack_data[i])->data == S) {
            // printf("break!\n");
            break;
        }
    }
    // printf("hit\n");

    // printf("stack reduced: ");
    // stack_print_stack(stack);
    if (stack_data[0]->data == S) {
        // error
        printf("error < at the top of the stack\n");
        // free(stack_data);
        return;
    }
    if (stack_data[1]->data == S && stack_data[0]->data == T_INT) {  // i -> E
        // stack_pop(stack);
        stack_push(stack, E);
        // stack->top.tree_ptr = create_leaf(stack_data[0]->token);
        htab_item_t data;
        stack->top->tree = make_leaf(stack_data[0]->token, data);
        stack->top->token = stack_data[0]->token;
        // stack_data[0]
        // free(stack_data);
        return;
    }
    if (stack_data[0]->data == T_RPAR && stack_data[1]->data == E &&
        stack_data[2]->data == T_LPAR) {
        stack_push(stack, E);
        // stack_push(stack, E);

        stack->top->token = stack_data[1]->token;
        stack->top->tree = stack_data[1]->tree;
        printf("created tree_ptr %p\n", stack->top->tree);
        // free(stack_data);
        return;
        // E -> (E)
    }
    if (stack_data[0]->data == E && stack_data[2]->data == E) {  // E op E
        printf("rule E -> E op E\n");
        ast_node_t* tree_ptr;
        lexeme operator;
        // operator.value = 0;
        //
        switch (stack_data[1]->data) {
            case T_PLUS:
                printf("AST create PLUS tree\n");
                operator.lex = L_PLUS;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_MUL:
                printf("AST create MUL tree\n");
                operator.lex = L_MUL;
                tree_ptr = make_tree(operator, stack_data[2]->tree,
                                     stack_data[0]->tree);
                break;
            case T_DIV:
                break;
            case T_GREATER:
                break;
            case T_GREATEREQ:
                break;
            case T_LESS:
                break;
            case T_LESSEQ:
                break;
            case T_MINUS:
                break;
            case T_CONCAT:
                break;

            default:
                // someting else == EROR
                printf("no operator matched \n");
                break;
        }
        stack_push(stack, E);
        stack->top->tree = tree_ptr;
    } else {
        // error
        printf("no rule matched \n");
        // free(stack_data);
        return;
    }
    /*
    cases
    < nic => error
    < neco => if neco i => ok else error
    < E op E => ok
    < ( E ) => ok
    else error
    neco bez < => error


    */
    // neco
    //
    //
    //

    /*
    i => {float,int,string}
    E -> i
    E -> (E)
    E -> E op E
    op => {+,-,*,/,}

    - load until < if 4th loaded element is not < then error
    - push new non terminal on stack


    todo ast generate call
    */
    // free(stack_data);
}

void map_token() {}

void stack_test() {
    Stack stack;
    stack_init(&stack);
    stack_push(&stack, EMPTY);
    // printf("stack empty:%d\n", stack_empty(&stack));
    stack_push(&stack, $);
    print_enum_as_str(stack_top_nonterminal(&stack));
    stack_push(&stack, S);
    stack_push(&stack, E);
    stack_push(&stack, T_PLUS);
    stack_push(&stack, E);
    // stack_push(&stack, $);
    stack_print_stack(&stack);
    stack_shift_push(&stack);
    stack_print_stack(&stack);
    // stack_push(&stack, E);
    stack_push(&stack, T_MUL);
    stack_shift_push(&stack);
    stack_print_stack(&stack);
    // printf("Stack non terminal %d\n", stack_top_nonterminal(&stack));
    // printf("stack empty:%d\n", stack_empty(&stack));
    // // printf("hit\n");
    // printf("stack top: %d\n", stack_top(&stack));
    // printf("Stack non terminal %d\n", stack_top_nonterminal(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("stack top: %d\n", stack_top(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("stack top: %d\n", stack_top(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    // printf("popped element %d\n", stack_pop(&stack));
    printf("--------------- end stack test ---------------\n");
}

int parse_expression(lexeme used_token) {
    // stack init
    Stack stack;
    stack_init(&stack);
    stack_push(&stack, EMPTY);
    // lexeme used_token;
    precedence_symbols current_token_enum = map_token_to_enum(used_token);
    lexeme current_token = used_token;
    precedence_symbols top;

    stack_push(&stack, $);

    do {
        stack_print_stack(&stack);
        // current_token = current_token;
        printf("current_token: ");
        print_enum_as_str(current_token_enum);
        printf("\ncurrent top: ");
        top = stack_top_nonterminal(&stack);
        print_enum_as_str(top);
        printf("\n");
        print_enum_as_str(prec_table[top][current_token_enum]);
        printf(" rule chosen \n");
        switch (prec_table[top][current_token_enum]) {
            case W:  // get next token and push current
                // next token
                stack_push(&stack, current_token_enum);
                stack.top->token = current_token;
                current_token = get_lex_value();
                current_token_enum = map_token_to_enum(current_token);
                print_enum_as_str(current_token_enum);

                break;
            case S:  // shift
                // stack_push(&stack, S);
                stack_shift_push(&stack);
                stack_push(&stack, current_token_enum);
                stack.top->token = current_token;
                current_token = get_lex_value();
                current_token_enum = map_token_to_enum(current_token);
                // printf(get_lex_value());
                break;
            case R:  // reduce
                rule_reduction(&stack);
                // printf("stack after reduction ");
                stack_print_stack(&stack);
                // return;
                break;
                // return;
            case Er:
                stack_print_stack(&stack);
                printf("--- SYNTAX ERROR ---\n");
                return 0;
                break;
            default:
                printf("wrong table\n");
                return 0;
                break;
        }
        // printf("hit end\n");
        // print_enum_as_str(current_token);

    } while ((current_token_enum != $) || (exp_correct_syntax(&stack) == 0));
    stack_print_stack(&stack);
    if (exp_correct_syntax(&stack)) {
        // print_tree_postorder(stack.top->tree);
        ast_print_tree(stack.top->tree);
        printf("expression is correct");
        return 0;
        // return tree pointer;
    }
    printf("expression is not correct\n");
    return 1;
}

int main() {
    // stack_test();
    // non_token_expression();
    lexeme test_incoming_token;
    // test_incoming_token.lex = L_NUMBER;
    // test_incoming_token.val = 0;
    test_incoming_token = get_lex_value();
    parse_expression(test_incoming_token);
    return 1;
}