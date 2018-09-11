#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>

typedef struct tree_node *tree;
typedef enum tree_e {BST, RBT} tree_t;

extern tree     tree_new();
extern tree     tree_insert(tree T, char *key);
extern int      tree_search(tree T, char *key);
extern void     tree_inorder(tree T, void f(char *key));
extern void     tree_preorder(tree T, void f(char *key));
extern void     tree_free(tree T);

#endif
