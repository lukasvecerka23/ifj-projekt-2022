#ifndef IFJ_AST_H
#define IFJ_AST_H

#include <stdbool.h>

// Uzol stromu
typedef struct ast_node {
  char val;               // hodnota
  struct ast_node *left;  // left child
  struct ast_node *right; // right child
} ast_node_t;

// void init(ast_node_t *tree);
void dispose(ast_node_t *tree);

ast_node_t* make_tree(char o, ast_node_t *a, ast_node_t *b);
ast_node_t* make_leaf(char ia);

void postorder(ast_node_t *tree);
void preorder(ast_node_t *tree);
void inorder(ast_node_t *tree);


void print_node(ast_node_t *node);

#endif