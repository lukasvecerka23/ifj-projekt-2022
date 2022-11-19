#include "ast.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

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

ast_node_t* make_tree(lexeme o, ast_node_t* a, ast_node_t* b) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
        // nevim presne jak toto osetrit
    } else {
        new->token = o;
        new->left = a;
        new->right = b;

        return new;
    }
}

/*
 * MakeLeaf(i.a) vytvori novy uzel i.a
 * (i.a je adresa do tabulky symbolu)
 * a vrati ukazatel na tento uzel
 *
 * kde a.i je hodnota z tabulky symbolu
 */

ast_node_t* make_leaf(lexeme ia) {
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL) {
        exit_program(99, "memory allocation failed");
        // nevim presne jak toto osetrit
    } else {
        new->token = ia;
        new->left = NULL;
        new->right = NULL;

        return new;
    }
}