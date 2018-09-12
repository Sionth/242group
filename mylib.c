#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mylib.h"
#include "htable.h"
#include "tree.h"
#include <assert.h>
#include <ctype.h>



double fill_time;
double search_time;
int unknown_words;



void *emalloc(size_t s) {
    void *result = malloc(s);

    if (NULL == result) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}



void *erealloc(void *p, size_t s) {
    void *result = realloc(p, s);

    if (NULL == result) {
        fprintf(stderr, "memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}



int get_word(char *s, int limit, FILE *stream){
  int c;
  char *w = s;
  assert(limit > 0 && s != NULL && stream != NULL);

  /* skip to the start of the word */
  while(!isalnum(c = getc(stream)) && EOF != c)
    ;
  if(EOF == c){
    return EOF;
  }else if(--limit > 0){ /* reduce limit by 1 to allow for the \0 */
    *w++ = tolower(c);
  }
  while(--limit > 0){
    if(isalnum(c = getc(stream))){
      *w++ = tolower(c);
    }else if('\'' == c){
      limit++;
    }else{
      break;
    }
  }
  *w = '\0';
  return w - s;
}



/* Prints out info on how to use each command line argument of asgn.c.
 * Parameter: option signals if user requires help info.
 */
void print_help(int option) {
    if (option == 1) {
        fprintf(stderr, "Need to write new help message\n");
        exit(EXIT_SUCCESS);
    }
}



/* Determines if the number supplied is a prime number.
 * Parameter: c is the candidate number.
 * Returns: 1 if c is prime 0 if not.
 */
int is_prime(int c) {
    int i, prime = 1;
    for (i = 2; i < c; i++) {
        if (c % i== 0) {
            prime = 0;
        }
    }
    return prime;
}



/* Finds the next prime number greater or equal too the size parameter.
 * Parameter: size is the minimum size for a new hashtable.
 * Returns: the next prime number greater than or equal to size.
 */
int get_next_prime(int size) {
    int candidate = size;
    while (is_prime(candidate) == 0) {
        candidate++;
    }
    return candidate;
}



/* Opens a given file for reading.
 * Parameter: filename is the file to be opened.
 * Returns: the opened file.
 * EXITS if the file does not exist.
 */
FILE *open_file(char *filename) {
    FILE *infile = NULL;
    if (NULL == (infile = fopen(filename, "r"))) {
        fprintf(stderr, "Can't find file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return infile;
}



/* Inserts words from a specified file into the given hashtable and mesures the
 * time taken.
 * Parameter: h is the hash table to use.
 * Parameter: infile is the file to read from.
 */
void insert_words_into_htable(htable h, FILE *infile) {
    clock_t start, end;
    char word[256];
    start = clock();
    while (get_word(word, sizeof word, infile) != EOF) {
        htable_insert(h, word);
    }
    end = clock();
    fill_time = (end - start) / (double)CLOCKS_PER_SEC;
}



/* Inserts words from a specified file into the given tree and mesures the
 * time taken.
 * Parameter: t is the tree to use.
 * Parameter: infile is the file to read from.
 */
tree insert_words_into_tree(tree t, FILE *stream) {
    clock_t start, end;
    char word[256];
    start = clock();
    while (get_word(word, sizeof word, stream) != EOF) {
        t = tree_insert(t, word);
    }
    end = clock();
    fill_time = (end - start) / (double)CLOCKS_PER_SEC;
    return t;
}



void search_htable(htable h, FILE *stream) {
    clock_t start, end;
    char word[256];
    unknown_words = 0;
    start = clock();
    while (get_word(word, sizeof word, stream) != EOF) {  
        if (htable_search(h, word) == 0) {
            fprintf(stdout, "%s\n", word);
            unknown_words++;
        }
    }
    end = clock();
    search_time = (end-start) / (double)CLOCKS_PER_SEC;
    
}



void search_tree(tree t, FILE *stream) {
    clock_t start, end;
    char word[256];
    unknown_words = 0;
    start = clock();
    while (get_word(word, sizeof word, stream) != EOF) {
        if (tree_search(t, word) == 0) {
            fprintf(stdout, "%s\n", word);
            unknown_words++;
        }
    }
    end = clock();
    search_time = (end - start) / (double)CLOCKS_PER_SEC;
}



/**
 * Prints the time taken to search and fill the data structure and how many
 * words were not found in the dictionary.
 */
void print_basic_stats() {
    fprintf(stderr,
            "Fill time :\t%f\nSearch time :\t%f\nUnknown words : %d\n",
            fill_time, search_time,unknown_words);
   
}
