#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern void *emalloc(size_t s);
extern void *erealloc(void *p, size_t s);
extern int getword(char *s, int limit, FILE *stream);
extern void print_help(int option);
extern int is_prime(int c);
extern int get_next_prime(int size);
extern FILE *open_file(char *filename);

#endif
