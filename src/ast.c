#include "ast.h"
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

ast_node_t* make_tree(char o, ast_node_t *a, ast_node_t *b){
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL){
        exit(99);
        //nevim presne jak toto osetrit
    } else {
        new->val = o;
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

ast_node_t* make_leaf(char ia){
    ast_node_t* new = malloc(sizeof(struct ast_node));
    if (new == NULL){
        exit(99);
        //nevim presne jak toto osetrit
    } else {
        new->val = ia;
        new->left = NULL;
        new->right = NULL;

        return new;
    }
}

void print_node(ast_node_t *node) {
  printf("[%c]", node->val);
}