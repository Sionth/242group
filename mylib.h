#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern void *emalloc(size_t s);
extern void *erealloc(void *p, size_t s);
extern int getword(char *s, int limit, FILE *stream);

#endif
