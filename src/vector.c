#include "vector.h"
#include <assert.h>


Vector *VectorCreate(size_t element_size, size_t initial_max) {
  Vector *vec = malloc(sizeof(Vector));
  check_mem(vec);
  vec->end = 0;
  vec->max = initial_max;
  check(vec->max > 0, "You must set an initial_max > 0.");
  vec->contents = calloc(initial_max, sizeof(void *));
  check_mem(vec->contents);

  vec->element_size = element_size;
  vec->expand_rate = DEFAULT_EXPAND_RATE;

  return vec;
error:
  if(vec) free(vec);
  return NULL;
}


void VectorDestroy(Vector *vec) {
  if (vec) {
    if (vec->contents) free(vec->contents);
    free(vec);
  }
}


void VectorClear(Vector *vec) {
  if (vec->element_size > 0) {
    for(int i = 0; i < vec->max; i++) {
      if (vec->contents[i] != NULL) {
        free(vec->contents[i]);
      }
    }
  }
}


static inline int VectorResize(Vector *vec, size_t newsize) {
  vec->max = newsize;
  check(vec->max > 0, "The newsize must by > 0.");

  void *contents = realloc(vec->contents, vec->max * sizeof(void *));

  check_mem(contents);

  vec->contents = contents;
  return 0;
error:
  return -1;
}


int VectorExpand(Vector *vec) {
  int new_size = vec->max + vec->expand_rate;
  check(VectorResize(vec, new_size) == 0, "Failed to expand array to new size: %d", new_size);
  return 0;
error:
  return -1;
}


int VectorContract(Vector *vec) {
  int new_size = vec->end < (int) vec->expand_rate ? (int) vec->expand_rate : vec->end;
  return VectorResize(vec, new_size + 1);
}


int VectorPush(Vector *vec, void *el) {
  VectorSet(vec, vec->end, el);
  vec->end++;

  if (vec->end >= vec->max) {
    return VectorExpand(vec);
  } else {
    return 0;
  }
}


void *VectorPop(Vector *vec) {
  check(vec->end - 1 >= 0, "Attempt to pop from empty vector.");
  void *el = VectorRemove(vec, vec->end - 1);
  vec->end--;
  if (vec->end > (int) vec->expand_rate && vec->end % vec->expand_rate) {
    VectorContract(vec);
  }
  return el;
error:
  return NULL;
}


void VectorClearDestroy(Vector *vec) {
  VectorClear(vec);
  VectorDestroy(vec);
}
