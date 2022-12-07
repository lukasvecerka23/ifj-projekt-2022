/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Function implementation of AST Tree
 */

#include "ast.h"

void ast_dispose(ast_node_t* tree) {
    if (tree != NULL) {
        ast_dispose(tree->left);
        ast_dispose(tree->right);
        free(tree);

        tree = NULL;
    }
}

ast_node_t* make_tree(token_t* token, ast_node_t* left, ast_node_t* right) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
    }
    new->token = token;
    new->left = left;
    new->right = right;

    return new;
}

ast_node_t* make_leaf(token_t* token) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
    }
    new->token = token;
    new->left = NULL;
    new->right = NULL;

    return new;
}

int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}

int ast_height(ast_node_t* tree) {
    if (tree != NULL) {
        return max(ast_height(tree->left), ast_height(tree->right)) + 1;
    } else {
        return 0;
    }
}
/*END OF FILE*/
