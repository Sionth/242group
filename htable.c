#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

#define IS_DHASH(x) (DOUBLE_H == (x)->strat)

struct htablerec {
  int capacity;
  int num_keys;
  int *freq;
  int *stats;
  char **keys;
  hashing_t strat;
};

/*DOUBLE HASHING USED*/
static unsigned int htable_word_to_int(char *word){
  unsigned int result = 0;
  while(*word != '\0'){
    result = (*word++ + 31 * result);
  }
  return result;
}

static unsigned int double_hash(htable h, int col, int val){
    return ((val + col * (1 + val % (h->capacity-1))) % h->capacity);
}

htable htable_new(int capacity, hashing_t s){
  int i = 0;
  htable h = emalloc(sizeof * h);
  h->capacity = capacity;
  h->num_keys = 0;
  h->strat = s;
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
    int index = htable_word_to_int(str);
    int col = 0;
    int step = htable_step(h, index);
    if(!IS_DHASH(h)){
      while (col < h->capacity) {
        index = index % h->capacity;
        if(NULL == h->keys[index]){
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0]);
            strcpy(h->keys[index], str);
            h->num_keys++;
            h->freq[index]++;
            h->stats[index] = col;
            return 1;
        }
        else if(strcmp(h->keys[index], str) == 0){
            h->freq[index]++;
            return h->freq[index];
        }
        index++;
        col++;
      }
    }else{
      while (col < h->capacity) {
        index = double_hash(h,col,index);
        if(NULL == h->keys[index]){
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0]);
            strcpy(h->keys[index], str);
            h->num_keys++;
            h->freq[index]++;
            h->stats[index] = col;
            return 1;
        }
        else if(strcmp(h->keys[index], str) == 0){
            h->freq[index]++;
            return h->freq[index];
          }
        col++;
      }
    }
    return 0;
  }
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
