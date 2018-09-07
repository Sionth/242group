/* this is the file where hashtables should be implemented */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec {
  int capacity;
  int num_keys;
  int *freq;
  char **keys;
};

/*DOUBLE HASHING USED*/
static unsigned int htable_word_to_int(char *word){
  unsigned int result = 0;
  while(*word != '\0'){
    result = (*word++ + 31 * result);
  }
  return result;
}

static unsigned int htable_step(htable h, unsigned int i_key){
    return 1 + (i_key % (h->capacity-1));
}

htable htable_new(int capacity){
  int i = 0;
  htable h = emalloc(sizeof * h);
  h->capacity = capacity;
  h->num_keys = 0;
  h->freq = emalloc(h->capacity * sizeof h->freq[0]);
  h->keys = emalloc(h->capacity * sizeof h->keys[0]);
  for(i=0;i<h->capacity;i++){
    h->freq[i] = 0;
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
  free(h);
}



int htable_insert(htable h, char *str) {
    int index = htable_word_to_int(str) % h->capacity;
    int col = 0;
    int step = htable_step(h, index);

    while (col < h->capacity) {
        /* If there is nothing in this position */
        if (NULL == h->keys[index]) {
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0]);
            strcpy(h->keys[index], str);
            h->num_keys++;
            h->freq[index]++;
            return 1;
        }
        /* If the same string is in that position */
        if (strcmp(h->keys[index], str) == 0) {
            h->freq[index]++;
            return h->freq[index];
        }
        index = (index + step) % h->capacity;
        col++;
    }
    return 0;
}



void htable_print(htable h, FILE *stream) {
    int i;
    for (i = 0; i < h->capacity; i++) {
        if (h->freq[i] > 0) {
            fprintf(stream, "%d coppies of %s\n", h->freq[i], h->keys[i]);
        }
    }
}


int htable_search(htable h, char *str){
    int col = 0;
    unsigned int index = htable_word_to_int(str);
    unsigned int step = htable_step(h, index);
    index = index % h->capacity;
    while (h->keys[index] != NULL && strcmp(h->keys[index], str) != 0 && col < h->capacity){
        col++;
        index = (index + step) % h->capacity;
    }
    if (col == h->capacity){
        return 0;
    } else{
        return h->freq[index];
    }
}

int main(void){

    return EXIT_SUCCESS;
    
    }
   
