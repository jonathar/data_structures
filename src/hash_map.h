#ifndef _hash_map_h
#define _hash_map_h 

#include <stdlib.h>
#include <stdint.h>
#include "vector.h"
#include "dbg.h"

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*HashMapCompare)(void *a, void *b);
typedef uint32_t (*HashMapHash)(void *key);

typedef struct HashMap {
  Vector *buckets;
  HashMapCompare compare;
  HashMapHash hash;
} HashMap;

typedef struct HashMapNode {
  void *key;
  void *data;
  uint32_t hash;
} HashMapNode;

typedef int (*HashMapTraverseCB)(HashMapNode *node);

HashMap *HashMapCreate(HashMapCompare compare, HashMapHash hash);
void HashMapDestroy(HashMap *map);

int HashMapSet(HashMap *map, void *key, void *data);
void *HashMapGet(HashMap *map, void *key);

int HashMapTraverse(HashMap *map, HashMapTraverseCB traverse_cb);

void *HashMapDelete(HashMap *map, void *key);

#endif
