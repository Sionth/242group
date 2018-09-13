/**
 * Header file for hash table implementation.
 * 13/9/18.
 * @author Kelson Sadlier
 * @author Quinn Thorsnes
 * @author Callan Taylor
 *
 */
#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct htablerec *htable;
typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;

extern void   htable_free(htable h);
extern int    htable_insert(htable h,char *str);
extern htable htable_new(int capacity, hashing_t s);
extern void   htable_print(htable h, void f(int freq, char *key));
extern void   htable_print_entire_table(htable h, FILE *stream);
extern int    htable_search(htable h, char *str);
extern void   htable_print_stats(htable h, FILE *stream, int num_stats);

#endif
