#include "minunit.h"
#include "vector.h"

static Vector *vec = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

char *test_create() {
  vec = VectorCreate(sizeof(int), 100);
  mu_assert(vec != NULL, "Failed to create vector.");
  mu_assert(vec->contents != NULL, "Contents should not be NULL.");
  mu_assert(vec->end == 0, "End should be at 0.");
  mu_assert(vec->element_size == sizeof(int), "Element size is wrong.");
  mu_assert(vec->max == 100, "wrong max length on initial sizem");
  return NULL;
}

char *test_destroy() {
  vec = VectorCreate(sizeof(int), 100);
  VectorDestroy(vec);
  return NULL;
}

char *test_new(){
  val1 = VectorNew(vec);
  mu_assert(val1 != NULL, "failed to make a new element.");

  val2 = VectorNew(vec);
  mu_assert(val2 != NULL, "falied to make a new element.");

  return NULL;
}

char *test_set() {
  VectorSet(vec, 0, val1);
  VectorSet(vec, 1, val2);

  return NULL;
}

char *test_get() {
  mu_assert(VectorGet(vec, 0) == val1, "wrong first value.");
  mu_assert(VectorGet(vec, 1) == val2, "wrong second value.");

  return NULL;
}

char *test_remove() {
  int *val_check = VectorRemove(vec, 0);
  mu_assert(val_check != NULL, "Should not get NULL");
  mu_assert(*val_check == *val1, "Should get first value.");
  mu_assert(VectorGet(vec, 0) == NULL, "Should be gone.");
  VectorFree(val_check);

  val_check = VectorRemove(vec, 1);
  mu_assert(val_check != NULL, "Should not get NULL");
  mu_assert(*val_check == *val2, "Should get second value.");
  mu_assert(VectorGet(vec, 1) == NULL, "Should be gone.");
  VectorFree(val_check);

  return NULL;
}

char *test_expand_contract() {
  int old_max = vec->max;
  VectorExpand(vec);
  mu_assert((unsigned int) vec->max == old_max + vec->expand_rate, "Wrong size after expand.");

  VectorContract(vec);
  mu_assert((unsigned int) vec->max == vec->expand_rate + 1, "Should stay at least at expand rate.");

  VectorContract(vec);
  mu_assert((unsigned int) vec->max == vec->expand_rate + 1, "Should stay at least at expand rate.");

  return NULL;
}

char *test_push_pop() {
  for(int i = 0; i < 1000; i++) {
    int *val = VectorNew(vec);
    *val = i * 333;
    VectorPush(vec, val);
  }

  mu_assert(vec->max == 1201, "Wrong max size.");

  for(int i = 999; i >= 0; i--) {
    int *val = VectorPop(vec);
    mu_assert(val != NULL, "Shouldn't get a NULL.");
    mu_assert(*val == i * 333, "Wrong value.");
    VectorFree(val);
  }

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_new);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_remove);
  mu_run_test(test_set);
  mu_run_test(test_get);
  mu_run_test(test_expand_contract);
  mu_run_test(test_push_pop);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
