/**
 * Header file for binary tree implementation.
 * 13/9/18.
 * @author Kelson Sadlier
 * @author Quinn Thorsnes
 * @author Callan Taylor
 *
 */
#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct tree_node *tree;
typedef enum tree_e {BST, RBT} tree_t;

extern tree     tree_new();
extern tree     tree_insert(tree T, char *key);
extern int      tree_search(tree T, char *key);
extern void     tree_inorder(tree T, void f(char *key));
extern void     tree_preorder(tree T, void f(int frequency, char *key));
extern tree     tree_free(tree T);
extern tree     tree_fix_root(tree T);
extern void     tree_print(char *key);
extern void     tree_output_dot(tree t, FILE *out);

#endif
