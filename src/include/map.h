#ifndef __MAP_H__
#define __MAP_H__

#include <cstdlib>
#include <sys/types.h>
#include <stdlib.h>

#define TABLE_SIZE 512
#define hash(key) (((key) % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE)

typedef struct HashNode {
  size_t key;
  char* value;
  struct HashNode *next;
} HashNode;

typedef struct HashTable {
  HashNode *buckets[TABLE_SIZE];
} HashTable;


/*
* Function - initTable
* @info - The function is used to initialized the table
* @param - HashTable is a map with fixed map_size
*/
static inline void initTable(HashTable *table) {
  for(int i = 0; i < TABLE_SIZE; ++i) {
    table->buckets[i] = NULL;
  }
}


static inline void free_table(HashTable *table) {
  for(int i = 0; i < TABLE_SIZE; ++i) {
    HashNode *node = table->buckets[i];
    while (node) {
      HashNode *temp = node;
      node = node->next;
      free(temp);
    }
  }
}

#endif //__MAP__H__

