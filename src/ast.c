#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

void dispose(ast_node_t* tree) {
    if (tree != NULL) {
        dispose(tree->left);
        dispose(tree->right);
        free(tree);

        tree = NULL;
    }
}

/*
 * MakeTree(o, a, b) vytvori novy uzel o
 * navaze leveho syna a, praveho syna b
 * a vrati ukazatel na uzel o
 */

ast_node_t* make_tree(token_t o, ast_node_t* a, ast_node_t* b) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
        // nevim presne jak toto osetrit
    }
    new->token = o;
    new->left = a;
    new->right = b;

    return new;
}

/*
 * MakeLeaf(i.a) vytvori novy uzel i.a
 * (i.a je adresa do tabulky symbolu)
 * a vrati ukazatel na tento uzel
 *
 * kde a.i je hodnota z tabulky symbolu
 */

ast_node_t* make_leaf(token_t ia, htab_item_t hia) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
        // nevim presne jak toto osetrit
    }
    new->token = ia;
    new->h_item = hia;
    new->left = NULL;
    new->right = NULL;

    return new;
}

/*
 * Debug funkce která tiskne vytvořený strom
 */
void print_tree_postorder(ast_node_t* tree) {
    if (tree != NULL) {
        print_tree_postorder(tree->left);
        print_tree_postorder(tree->right);

        printf("%d ->", tree->token.token_type);
        return;
    }
    // printf("\n");
}

const char* subtree_prefix = "  |";
const char* space_prefix = "   ";

void print_node(ast_node_t* tree) {
    print_lex(tree->token);
}

char* make_prefix(char* prefix, const char* suffix) {
    char* result = (char*)malloc(strlen(prefix) + strlen(suffix) + 1);
    strcpy(result, prefix);
    result = strcat(result, suffix);
    return result;
}

void ast_print_subtree(ast_node_t* tree, char* prefix, direction_t from) {
    if (tree != NULL) {
        char* current_subtree_prefix = make_prefix(prefix, subtree_prefix);
        char* current_space_prefix = make_prefix(prefix, space_prefix);

        if (from == left) {
            printf("%s\n", current_subtree_prefix);
        }

        ast_print_subtree(
            tree->right,
            from == left ? current_subtree_prefix : current_space_prefix,
            right);

        printf("%s  +-", prefix);
        print_node(tree);
        printf("\n");

        ast_print_subtree(
            tree->left,
            from == right ? current_subtree_prefix : current_space_prefix,
            left);

        if (from == right) {
            printf("%s\n", current_subtree_prefix);
        }

        free(current_space_prefix);
        free(current_subtree_prefix);
    }
}

void ast_print_tree(ast_node_t* tree) {
    printf("Binary tree structure:\n");
    printf("\n");
    if (tree != NULL) {
        ast_print_subtree(tree, "", none);
    } else {
        printf("Tree is empty\n");
    }
    printf("\n");
}