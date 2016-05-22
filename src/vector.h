#ifndef _vector_h
#define _vector_h

#include <stdlib.h>
#include <assert.h>
#include "dbg.h"

typedef struct Vector {
  int end;
  int max;
  size_t element_size;
  size_t expand_rate;
  void **contents;
} Vector;


Vector *VectorCreate(size_t element_size, size_t initial_max);

void VectorDestroy(Vector *vec);

void VectorClear(Vector *vec);

int VectorExpand(Vector *vec);

int VectorContract(Vector *vec);

int VectorPush(Vector *vec, void *el);

void *VectorPop(Vector *vec);

void VectorClearDestroy(Vector *vec);

#define VectorLast(A) ((A)->contents[(A)->end - 1])
#define VectorFirst(A) ((A)->contents[0])
#define VectorEnd(A) ((A)->end)
#define VectorCount(A) VectorEnd(A)
#define VectorMax(A) ((A)->max)

#define DEFAULT_EXPAND_RATE 300

static inline void VectorSet(Vector *vec, int i, void *el) {
  check(i < vec->max, "Vector attempted to set out of bounds.");
  if (i > vec->end) vec->end = i;
  vec->contents[i] = el;
error:
  return;
}

static inline void *VectorGet(Vector *vec, int i) {
  check(i < vec->max, "Vector attempted to get out of bounds.");
  return vec->contents[i];
error:
  return NULL;
}

static inline void *VectorRemove(Vector *vec, int i) {
  check(i < vec->max, "Vector attempted to remove out of bounds.");
  void *el = vec->contents[i];
  vec->contents[i] = NULL;
  return el;
error:
  return NULL;
}

static inline void *VectorNew(Vector *vec) {
  check(vec->element_size > 0, "Can't use VectorNew on 0 size vectors.");
  return calloc(1, vec->element_size);
error:
  return NULL;
}

#define VectorFree(E) free((E))

#endif
