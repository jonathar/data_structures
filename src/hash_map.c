#include "hash_map.h"
#include "bstrlib.h"

int default_compare(void *a, void *b) {
  return bstrcmp((bstring) a, (bstring) b);
}

uint32_t default_hash(void *a) {
  size_t len = blength((bstring) a);
  char *key = bdata((bstring) a);
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

static inline HashMapNode *HashMapNodeCreate(int hash, void *key, void *data) {
  HashMapNode *node = calloc(1, sizeof(HashMapNode));
  check_mem(node);
  node->hash = hash;
  node->key = key;
  node->data = data;

  return node;
error:
  return NULL;
}

static inline Vector *HashMapGetBucket(HashMap *map, uint32_t hash, int create) {
  int bucket_num = hash % map->buckets->end;
  check(bucket_num > 0, "Invalid bucket_num found");
  Vector *bucket = VectorGet(map->buckets, bucket_num);

  if (bucket == NULL && create) {
    Vector *new_bucket = VectorCreate(sizeof(HashMapNode), 1);
    VectorSet(map->buckets, bucket_num, new_bucket);
    bucket = new_bucket;
  }

  return bucket;
error:
  return NULL;
}

int HashMapSet(HashMap *map, void *key, void *data) {
  uint32_t hash = map->hash(key);
  Vector *bucket = HashMapGetBucket(map, hash, 1);
  check(bucket, "Can't create bucket.");

  HashMapNode *node = HashMapNodeCreate(hash, key, data);
  check(node, "Failed to create node.");
  VectorPush(bucket, node);

  return 0;
error:
  return -1;
}

void *HashMapGet(HashMap *map, void *key) {
  uint32_t hash = map->hash(key);
  Vector *bucket = HashMapGetBucket(map, hash, 0);

  for(int i = 0; i < bucket->end; i ++) {
    HashMapNode *node = VectorGet(bucket, i);
    if (node->key == key) return node->data;
  }

  return NULL;
error:
  return NULL;
}

int HashMapTraverse(HashMap *map, HashMapTraverseCB traverse_cb) {
  int rc = 0;
  for (int i = 0; i < map->buckets->end; i++) {
    Vector *bucket = VectorGet(map->buckets, i);
    if (bucket) {
      for (int j = 0; j < bucket->end; j++) {
        HashMapNode *node = VectorGet(bucket, j);
        rc = traverse_cb(node);
        if (rc != 0) return rc;
      }
    }
  }
  return 0;
}

void *HashMapDelete(HashMap *map, void *key) {

  uint32_t hash = map->hash(key);
  int bucket_num = hash % map->buckets->end;
  check(bucket_num > 0, "Invalid bucket_num found");

  Vector *bucket = VectorGet(map->buckets, bucket_num);
  for(int i = 0; i < bucket->end; i ++) {
    HashMapNode *node = VectorGet(bucket, i);
    if (node->key == key) {
      void *data = node->data;
      free(node);
      HashMapNode *ending = VectorPop(bucket);
      VectorSet(bucket, i, ending);
      return data;
    }
  }
error:
  return NULL;
}
