#include <stdlib.h>
#include "tree.h"
#include <string.h>
#include <stdio.h>

/* note: everything is bst at this point, rbt options will be added after */
struct tree_node *tree {
    char *key;
    tree left;
    tree right;
    int frequency = 1;
};

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

