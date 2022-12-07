/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file for AST Tree
 */

#ifndef IFJ_AST_H
#define IFJ_AST_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "scanner.h"
#include "symtable.h"

/**
 * @struct AST Node structure
 */
typedef struct ast_node {
    token_t* token;          // token struct
    struct ast_node* left;   // left child
    struct ast_node* right;  // right child
} ast_node_t;

/**
 * Delete whole tree
 *
 * @param tree Pointer on tree node
 */
void ast_dispose(ast_node_t* tree);

/**
 * Create binary tree
 *
 * @param token Pointer to token which will represent root node
 * @param left Pointer to left node of tree
 * @param right Pointer to right node of tree
 *
 * @return pointer to whole tree
 */
ast_node_t* make_tree(token_t* token, ast_node_t* left, ast_node_t* right);

/**
 * Create one tree leaf
 *
 * @param token Pointer to token
 *
 * @return pointer to leaf node
 */
ast_node_t* make_leaf(token_t* token);

/**
 * Return maximum of 2 numbers
 *
 * @param num1 First number
 * @param num2 Second number
 *
 * @return Greater number
 */
int max(int num1, int num2);

/**
 * Return tree height
 *
 * @param tree Pointer to tree
 *
 * @return if tree is NULL return 0, otherwise reuturn height of tree
 */
int ast_height(ast_node_t* tree);

#endif
/*END OF FILE*/
