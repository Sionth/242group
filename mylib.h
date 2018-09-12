#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdlib.h>
#include "htable.h"
#include "tree.h"
#include <stdio.h>

extern void   *emalloc(size_t s);
extern void   *erealloc(void *p, size_t s);
extern void   print_help(int option);
extern int    is_prime(int c);
extern int    get_next_prime(int size);
extern FILE   *open_file(char *filename);
extern int    get_word(char *s, int limit, FILE *stream);
extern void   insert_words_into_htable(htable h, FILE *infile);
extern tree   insert_words_into_tree(tree t, FILE *infile);
extern void   search_tree(tree t, FILE *stream);
extern void   search_htable(htable h, FILE *stream);
extern void   print_basic_stats();

#endif
