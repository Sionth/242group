/**
 * Implementations of helper functions used in the program.
 * 13/09/18.
 * @author Kelson Sadlier
 * @author Quinn Thorsnes
 * @author Callan Taylor
 *
 */
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


/**
 * Error checking memory allocation function ensures the all memory is
 * allocated without issues. If error is detected an error message is printed
 * and the program exits.
 *
 * @param s is the size of memory required for allocation.
 *
 * @return result A pointer to the position in memory allocated.
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}


/**
 * Gets a word from input for the reading of the dictionary and document files.
 *
 * @param s A pointer to the word to read.
 * @param limit The number of letters.
 * @param stream The input file.
 *
 * @return The number of characters left in the file.
 */
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



/**
 * Prints out info on the ussage of the program as a whole, including all of
 * possible command line arguments.
 *
 * @param option Signals if user requires help info.
 */
void print_help(int option) {
    if (option == 1) {
        fprintf(stderr,"Usage: ./asgn [OPTIONS]... <STDIN> \n\n\
Perform tasks using a hash table or binary tree. By defualt, words\n\
read from stdin are added to the data structure before printing\n\
them, along with thier frequencies, to stdout.\n\n\
 -T \t      Uses a tree data structure (default is hash table)\n\
 -c FILENAME  Check spelling of words in FILENAME using words\n\
              from stdin as dictionary. Print unknown words to\n\
              stdout, timing info ect to stderr (ignore -o & -p)");
        fprintf(stderr, "\n\
 -d \t      Use double hahsing (linear probing is defualt)\n\
 -e \t      Display entire contents of hash table on stderr\n\
 -o \t      Output the tree in DOT form to file 'tree-view.dot'\n\
 -p \t      Print hash table stats instead of frequencies & words\n\
 -r \t      Make the tree an RBT (defualt is BST)\n\
 -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n\
 -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n\
 -h \t      Display this message\n\n");
        exit(EXIT_SUCCESS);
    }
}


/**
 * Determines if the number supplied is a prime number.
 *
 * @param c The candidate number.
 *
 * @return 1 if c is prime 0 if not.
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



/**
 * Finds the next prime number greater or equal too the size parameter.
 *
 * @param size The minimum size for a new hashtable.
 *
 * @return the next prime number greater than or equal to size.
 */
int get_next_prime(int size) {
    int candidate = size;
    while (is_prime(candidate) == 0) {
        candidate++;
    }
    return candidate;
}



/**
 * Opens a given file for reading or exits if the file does not exist.
 *
 * @param filename The file to be opened.
 *
 * @return the opened file.
 */
FILE *open_file(char *filename) {
    FILE *infile = NULL;
    if (NULL == (infile = fopen(filename, "r"))) {
        fprintf(stderr, "Can't find file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return infile;
}



/**
 * Inserts words from a specified file into the given hashtable and mesures the
 * time taken.
 *
 * @param h The hash table to use.
 * @param infile The file to read from.
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



/**
 * Inserts words from a specified file into the given tree and mesures the
 * time taken.
 *
 * @param t The tree to use.
 * @param infile The file to read from.
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


/**
 * Searches a hash table for all words in the input stream, if the words are not
 * found then it increments the number of unknown words and prints that word
 * to stdout. Mesures the time taken to find each word.
 *
 * @param h The hash table in which to seach for the words.
 * @param stream The input stream containing all the words to look for.
 */
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


/**
 * Searches a binary tree for all words in the input stream, if the words
 * are not found then it increments the number of unknown words and prints
 * that word to stdout. Mesures the time taken to find each word.
 *
 * @param h The binary treee in which to seach for the words.
 * @param stream The input stream containing all the words to look for.
 */
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
            "Fill time     : %f\nSearch time   : %f\nUnknown words = %d\n",
            fill_time, search_time,unknown_words);
   
}
