#ifndef IFJ_AST_H
#define IFJ_AST_H

#include <stdbool.h>
#include <stdio.h>
#include "scanner.h"
#include "symtable.h"

// Tree node
typedef struct ast_node {
    token_t token;           // token struct
    htab_item_t h_item;      // hash table item struct
    struct ast_node* left;   // left child
    struct ast_node* right;  // right child
} ast_node_t;

// void init(ast_node_t *tree);
void dispose(ast_node_t* tree);

ast_node_t* make_tree(token_t o, ast_node_t* a, ast_node_t* b);
ast_node_t* make_leaf(token_t ia, htab_item_t hia);

void print_tree_postorder(ast_node_t* tree);

typedef enum direction { left, right, none } direction_t;

void ast_print_subtree(ast_node_t* tree, char* prefix, direction_t from);
void ast_print_tree(ast_node_t* tree);

#endif
