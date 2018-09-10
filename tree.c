#include <stdlib.h>
#include "tree.h"
#include <string.h>
#include <stdio.h>

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

struct tree_node *tree {
    char *key;
    tree left;
    tree right;
    int frequency = 1;
    tree_colour colour = RED;
};

/* there should be something about the RBT/BST enum here but need to figure that out */

tree tree_new() {
    return NULL;
}

tree tree_insert(tree T, char *key) {
    /* make insert return a tree to deal with NULL objects */
    if(T == NULL) {
        return new tree_node(key);
    } else {
        if(strcmp(key, T -> key) < 0) {
            T -> left = tree_insert(T -> left, key);
        } else if (strcmp(key, T -> key) >0) {
            T -> right = tree_insert(T -> right, key);
        } else {
            T -> frequency++;
        }
        return T;
    }
}

int tree_search(tree T, char *key) {
    if(T == NULL) { /* key not found */
        return 0; 
    } else if (key == T -> key) { /* key found */
        return T; 
    } else if (strcmp(key, T -> key) < 0) { /* key comes before current */
        return tree_search(T -> left, key);
    } else return tree_search(T -> right, key); /* key comes after current */
}

void tree_inorder(tree T, void f(char *key)) {
    if(T == NULL) { /*stopping case */
        return;
    }
    tree_inorder(T -> left, f); /* not sure about this line */
    f(T -> key);
    tree_inorder(T -> right, f); /* or this one */   
}

void tree_preorder(tree T, void f(char *key)){
    if(T == NULL) {
        return;
    }
    f(T -> key);
    tree_preorder(T -> left, f);
    tree_preorder(T -> right, f);
}

tree right_rotate(tree T) {
    tree temp = T;
    tree root = T -> left;
    temp -> left = root -> right;
    root -> right = temp;
    return root;
}

tree left_rotate(tree T) {
    tree temp = T;
    tree root = T -> right;
    temp -> right = root -> left;
    root -> left = temp;
    return root;
}

tree tree_fix(tree T) {
}

void tree_free(tree T){
    tree_free(T -> left);
    tree_free(T -> right);
    free(T);
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
   fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
   tree_output_dot_aux(t, out);
   fprintf(out, "}\n");
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
   if(t->key != NULL) {
      fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
              t->key, t->key, t->frequency,
              (RBT == tree_type && RED == t->colour) ? "red":"black");
   }
   if(t->left != NULL) {
      tree_output_dot_aux(t->left, out);
      fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
   }
   if(t->right != NULL) {
      tree_output_dot_aux(t->right, out);
      fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
   }
}

