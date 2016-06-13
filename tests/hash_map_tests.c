#include "minunit.h"
#include "hash_map.h"
#include "bstrlib.h"

static HashMap *map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");


char *test_create() {
  map = HashMapCreate(NULL, NULL);
  mu_assert(map != NULL, "Could not create HashMap.");

  return NULL;
}

char *test_destroy() {
  HashMapDestroy(map);
  return NULL;
}

char *test_get_set() {
  int rc = HashMapSet(map, &test1, &expect1);
  mu_assert(rc == 0, "Failed to set test 1.");
  bstring result = HashMapGet(map, &test1);
  mu_assert(result == &expect1, "Wrong value for test1.");

  rc = HashMapSet(map, &test2, &expect2);
  mu_assert(rc == 0, "Failed to set test 2.");
  result = HashMapGet(map, &test2);
  mu_assert(result == &expect2, "Wrong value for test2.");

  rc = HashMapSet(map, &test3, &expect3);
  mu_assert(rc == 0, "Failed to set test 3.");
  result = HashMapGet(map, &test3);
  mu_assert(result == &expect3, "Wrong value for test3.");

  return NULL;
}

static int traverse_good_cb(HashMapNode *node) {
  debug("KEY: %s", bdata((bstring) node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail_cb(HashMapNode *node) {
  debug("KEY: %s", bdata((bstring) node->key));
  traverse_called++;

  if(traverse_called == 2) {
    return 1;
  } else {
    return 0;
  }
}

char *test_traverse() {
  int rc = HashMapTraverse(map, traverse_good_cb);
  mu_assert(rc == 0, "Failed to traverse.");
  mu_assert(traverse_called == 3, "Wrong traverse count")

  traverse_called = 0;
  rc = HashMapTraverse(map, traverse_fail_cb);
  mu_assert(rc == 1, "Failed to traverse.");
  mu_assert(traverse_called ==2, "Wrong count traverse for fail.");
  return NULL;
}

char *test_delete() {
  bstring deleted = (bstring) HashMapDelete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect1, "Should get test1");
  bstring result = HashMapGet(map, &test1);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring) HashMapDelete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect2, "Should get test2");
  result = HashMapGet(map, &test2);
  mu_assert(result == NULL, "Should delete.");

  deleted = (bstring) HashMapDelete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete.");
  mu_assert(deleted == &expect3, "Should get test3");
  result = HashMapGet(map, &test3);
  mu_assert(result == NULL, "Should delete.");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
