#ifndef __MAP_H__
#define __MAP_H__

#include <sys/types.h>
#include <stdlib.h>

#define TABLE_SIZE 1024

#define MAP_CHECK(exp) do { \
  const mapError_t err = (exp); \
  if (err != mapSuccess) { \
    if (err < 0) { \
      fprintf(stderr, "%s:%d Map error: %s\n", __FILE__, __LINE__, mapGetErrorString(err)); \
      fflush(stderr); \
      exit(EXIT_FAILURE); \
    } else { \
      fprintf(stderr, "%s:%d Map warning: %s\n", __FILE__, __LINE__, mapGetErrorString(err)); \
      fflush(stderr); \
    } \
  } \
} while(0)

#define hash(key) ({ \
    unsigned long hash = 5381; \
    const unsigned char *s = (const unsigned char *)(key); \
    int c; \
    while ((c = *s++)) \
        hash = ((hash << 5) + hash) + c; \
    hash & (TABLE_SIZE - 1); \
})

typedef struct HashNode {
  const char* key;
  size_t value;
  struct HashNode *next;
} HashNode;

typedef struct HashTable {
  HashNode *buckets[TABLE_SIZE];
} HashTable;

typedef enum {
  mapKeyAlreadyPresentError = 2,
  mapHashNodeMemoryAllocationError = -1,
  mapSuccess = 1,
} mapError_t; 


static inline const char* mapGetErrorString(mapError_t mapError) {
  switch (mapError) {
    case mapKeyAlreadyPresentError:
      return "path is already in map";
    case mapHashNodeMemoryAllocationError:
      return "hash node memory allocation error";
    default:
      return "unknown error";
  }
}

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


/*
* Function - insert
* @info - Insert the value in the table
* @param - table: HashTable, key: size_t, value: char*
*/
static inline mapError_t insert(HashTable *table, const char* key) {
  size_t idx = hash(key);
  if (table->buckets[idx] != NULL) {
    return mapKeyAlreadyPresentError;
  }

  HashNode* node = (HashNode*)malloc(sizeof(HashNode));
  if (node == NULL) {
    return mapHashNodeMemoryAllocationError;
  }
  node->key = key;
  node->value = 1;
  node->next = table->buckets[idx];
  table->buckets[idx] = node;
  return mapSuccess;
}


/*
* Function - free_table
* @info - The function is used to free the table
* @param - HashTable is the map with fixed map_size
*/ 
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

