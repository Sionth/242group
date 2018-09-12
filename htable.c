/**
 * A modular implementation of a hash table data structure.
 * 13/09/18.
 * @author Kelson Sadlier
 * @author Quinn Thorsnes
 * @author Callan Taylor
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"


/**
 * Macro for checking the collision resolution strategy.
 *
 * @param x The given hash table.
 */
#define IS_DHASH(x) (DOUBLE_H == (x)->method)


/**
 * htablerec struct is the blueprint for creating an instance of a hash table.
 */
struct htablerec {
    int capacity;
    int num_keys;
    int *freq;
    int *stats;
    char **keys;
    hashing_t method;
};


/**
 * Converts a word into an integer to be a strings location in the hash table.
 *
 * @param word The string to convert.
 * @return result The integer resulting from the string conversion.
 */
static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;
    while(*word != '\0'){
        result = (*word++ + 31 * result);
    }
    return result;
}


/**
 * Computes the step size to be used based on the collision
 * resoloution method for the given hashtable.
 *
 * @param h The hash table.
 * @param i_key The key in integer form of a string.
 *
 * @return Returns 1 if linear probing is used or if the table
 * capacity is 1. Otherwise returns a step size based on the key value
 * and tables capacity.
 */
static unsigned int htable_step(htable h, unsigned int i_key) {
    if(!IS_DHASH(h) || h->capacity == 1){
        return 1;
    }else{
        return 1 + (i_key % (h->capacity - 1));
    }
}


/**
 * Creates a new empty hashtable, allocates memory and sets defualt values.
 *
 * @param capacity The tables maximum capacity.
 * @param s The collision resoloution method to be used for the table.
 *
 * @return h The hash table.
 */
htable htable_new(int capacity, hashing_t s){
    int i = 0;
    htable h = emalloc(sizeof * h);
    h->capacity = capacity;
    h->num_keys = 0;
    h->method = s;
    h->freq = emalloc(h->capacity * sizeof h->freq[0]);
    h->keys = emalloc(h->capacity * sizeof h->keys[0]);
    h->stats = emalloc(h->capacity * sizeof h->stats[0]);
    for(i=0;i<h->capacity;i++){
        h->freq[i] = 0;
        h->stats[i] = 0;
        h->keys[i] = NULL;
    }
    return h;
}


/**
 * Frees the memory which was allocated to the hash table and its
 * associated fields; stats, freq & keys.
 *
 * @param h The hash table.
 */
void htable_free(htable h){
    int i = 0;
    for(i=0;i<h->capacity;i++){
        free(h->keys[i]);
    }
    free(h->keys);
    free(h->freq);
    free(h->stats);
    free(h);
}


/**
 * Prints the non-empty positions of the hash table
 *
 * @param h the hash table.
 * @param f the function which prints the words and frequency.
 *
 */
void htable_print(htable h, void f(int freq, char *key)) {
    int i;
    for (i = 0; i < h->capacity; i++) {
        if (h->freq[i] > 0) {
            f(h->freq[i], h->keys[i]);
        }
    }
}


/**
 * Inserts a word into the hashtable.
 * Note: This method will use either Linear Probing or
 * Double hashing as a collision resoloution strategy depending
 * on what is specified for the given hash table.
 *
 * @param h The hash table.
 * @param str The word to be inserted.
 *
 * @return Returns 1 if the word is inserted into an empty space
 * in the table, or returns the frequency of the word if it is already been
 * inserted into the table, and 0 if the word fails to be inserted.
 */
int htable_insert(htable h, char *str) {
    unsigned int word_value = htable_word_to_int(str);
    unsigned int index = word_value % h->capacity;
    int collisions = 0;
    unsigned int step = htable_step(h, word_value);

    while(collisions < h->capacity &&
          h->keys[index] != NULL && strcmp(h->keys[index],str) != 0){
        index += step;
        index = index % h->capacity;
        collisions++;
    }

    if (NULL == h->keys[index]){
        h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0]);
        strcpy(h->keys[index], str);
        h->num_keys++;
        h->freq[index]++;
        h->stats[index] = collisions;
        return 1;
    } else if (strcmp(h->keys[index], str) == 0){
        h->freq[index]++;
        return h->freq[index];
    } else {
        return 0;
    }
}


/**
 * Prints out the entire hash table including information about each position.
 *
 * @li Pos - The position in the table.
 * @li Freq - The frequency of the word.
 * @li Stats - The number of collisions that occured at this position.
 * @li Word - The word (key) stored at this position.
 *
 * @param h The hash table.
 * @param stream The stream to print the table out to.
 */
void htable_print_entire_table(htable h, FILE *stream){
    int i;
    fprintf(stream, "%5s %5s %5s   %s\n", "Pos", "Freq", "Stats", "Word");
    fprintf(stream, "----------------------------------------\n");
    for(i = 0; i < h->capacity; i++) {
        if(h->keys[i] != NULL){
            fprintf(stream, "%5d %5d %5d   %s\n"
                    ,i,h->freq[i],h->stats[i],h->keys[i]);
        }else{
            fprintf(stream, "%5d %5d %5d   %s\n",i,h->freq[i],h->stats[i],"");
        }
    }
}


/**
 * Searches for a particular key in the hash table.
 *
 * @param h The hash table.
 * @param The word to search for.
 *
 * @return Returns the frequency of the word or returns 0 if not found.
 */
int htable_search(htable h, char *str){
    int collisions = 0;
    unsigned int word_value = htable_word_to_int(str);
    unsigned int index = word_value % h->capacity;
    unsigned int step = htable_step(h,word_value);

    while (collisions < h->capacity) {
        if (h->keys[index] == NULL) {
            return 0;
        } else if (strcmp(h->keys[index],str) == 0) {
            return h->freq[index];
        }
        index += step;
        index = index % h->capacity;
        collisions++;
    }

    return 0;
}


/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}


/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}
