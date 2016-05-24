#include "hash_map.h"
#include "bstrlib.h"

int default_compare(void *a, void *b) {
  return bstrcmp((bstring) a, (bstring) b);
}

uint32_t default_hash(void *a) {
	size_t len = blength((bstring) a);
	char *key = bdata((bstring) key);
	uint32_t hash = 0;
	uint32_t i = 0;

	for(hash = i = 0; i < len; ++i) {
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

HashMap *HashMapCreate(HashMapCompare compare, HashMapHash hash) {
  HashMap *map = calloc(1, sizeof(HashMap));
  check_mem(map);
  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;
  map->buckets = VectorCreate(sizeof(Vector *), DEFAULT_NUMBER_OF_BUCKETS );
  map->buckets->end = map->buckets->max;
  check_mem(map->buckets);
  
  return map;
error:
  if (map) HashMapDestroy(map);
  return NULL;
}

void HashMapDestroy(HashMap *map) {
	if (map) {
		if (map->buckets) {
			for(int i = 0; i < VectorCount(map->buckets); i++) {
				Vector *bucket = VectorGet(map->buckets, i);
				if (bucket) {
					for(int j = 0; j < VectorCount(bucket); j++) {
						void *item = VectorGet(bucket, j);
						if (item) free(item);
					}
					VectorDestroy(bucket);
				}
			}
		}
		free(map);
	}
}

static inline HashMapNode *HasMapNodeCreate(int hash, void *key, void *data) {
	HashMapNode *node = calloc(1, sizeof(HashMapNode));
	check_mem(node);
	node->hash = hash;
	node->key = key;
	node->data = data;

error:
	return NULL;
}

int HashMapSet(HashMap *map, void *key, void *data) {
  return -1;
}

void *HashMapGet(HashMap *map, void *key) {
  return NULL;
}

int HashMapTraverse(HashMap *map, HashMapTraverseCB traverse_cb) {
  return -1;
}

void *HashMapDelete(HashMap *map, void *key) {
  return NULL;
}
