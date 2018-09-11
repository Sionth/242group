#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

#define IS_DHASH(x) (DOUBLE_H == (x)->method)

struct htablerec {
  int capacity;
  int num_keys;
  int *freq;
  int *stats;
  char **keys;
  hashing_t method;
};


static unsigned int htable_word_to_int(char *word){
  unsigned int result = 0;
  while(*word != '\0'){
    result = (*word++ + 31 * result);
  }
  return result;
}



static unsigned int htable_step(htable h, unsigned int i_key) {
    if(!IS_DHASH(h) || h->capacity == 1){
      return 1;
    }else{
      return 1 + (i_key % (h->capacity - 1));
    }
}

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



int htable_insert(htable h, char *str) {
    unsigned int word_value = htable_word_to_int(str);
    unsigned int index = word_value % h->capacity; 
    int collisions = 0;
    unsigned int step;

    if (h->method == DOUBLE_H) {
        step = htable_step(h, word_value);
    } else {
        step = 1;
    }

    while(collisions < h->capacity && h->keys[index] != NULL && strcmp(h->keys[index],str) != 0){
      index += step;
      index = index % h->capacity;
      collisions++;
    }

    if(NULL == h->keys[index]){
      h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0]);
      strcpy(h->keys[index], str);
      h->num_keys++;
      h->freq[index]++;
      h->stats[index] = collisions;
      return 1;
    }else if(strcmp(h->keys[index], str) == 0){
      h->freq[index]++;
      return h->freq[index];
    }else{
      return 0;
  }
}




void htable_print_entire_table(htable h, FILE *stream){
  int i;
  fprintf(stream, "%5s %5s %5s   %s\n", "Pos", "Freq", "Stats", "Word");
  fprintf(stream, "----------------------------------------\n");
  for(i = 0; i < h->capacity; i++) {
    if(h->keys[i] != NULL){
      fprintf(stream, "%5d %5d %5d   %s\n",i,h->freq[i],h->stats[i],h->keys[i]);
    }else{
      fprintf(stream, "%5d %5d %5d   %s\n",i,h->freq[i],h->stats[i],"");
    }
  }
}





int htable_search(htable h, char *str){
  int collisions = 0;
  unsigned int word_value = htable_word_to_int(str);
  unsigned int index = word_value % h->capacity;
  unsigned int step;

  if (h->method == DOUBLE_H) {
      step = htable_step(h,word_value);
  } else {
      step = 1;
  }

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






/* Supplied Code*/




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
