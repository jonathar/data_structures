#include "minunit.h"
#include "hash_map.h"

/* static HashMap *map = NULL; */
/* static int *val1 = NULL; */
/* static int *val2 = NULL; */

char *test_create() {
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);

  return NULL;
}

RUN_TESTS(all_tests);
