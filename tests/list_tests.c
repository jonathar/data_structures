#include "minunit.h"
#include "linked_list.h"
#include <assert.h>

static List *list = NULL;
char *test1 = "test 1 data";
char *test2 = "test 2 data";
char *test3 = "test 3 data";

char *test_create() {
  list = ListCreate();
  mu_assert(list != NULL, "Failed to create list");
  ListDestroy(list);
  return NULL;
}

char *test_destroy() {
  list = ListCreate();
  ListDestroy(list);
  return NULL;
}

char *test_push() {
  list = ListCreate();
  ListPush(list, test1);
  mu_assert(list->count == 1, "Failed to increment count on push 1.");
  mu_assert(list->first->value == test1, "Failed to push element test1.");

  ListPush(list, test2);
  mu_assert(list->count == 2, "Failed to increment count on push 2.");
  mu_assert(list->first->value == test1, "Failed to retain first on push test2");
  mu_assert(list->last->value == test2, "Failed to push element test2.");

  ListPush(list, test3);
  mu_assert(list->count == 3, "Failed to increment count on push 3.");
  mu_assert(list->first->value == test1, "Failed to retain first on push test3");
  mu_assert(list->last->value == test3, "Failed to push element test3.");

  ListDestroy(list);
  return NULL;
}

char *test_pop() {
  list = ListCreate();

  ListPush(list, test1);
  ListPush(list, test2);
  ListPush(list, test3);

  ListPop(list);
  mu_assert(list->count == 2, "Failed to decrement count after pop.");

  ListDestroy(list);

  return NULL;
}

char *test_shift() {
  list = ListCreate();

  ListUnshift(list, test1);
  ListUnshift(list, test2);
  ListUnshift(list, test3);
  mu_assert(list->count == 3, "Failed to add all items.");

  void *item = ListShift(list);
  mu_assert(list->count == 2, "Failed to decrement count after shift.");
  mu_assert(item == test3, "Failed to shift test1");
  mu_assert(list->first->value == test2, "Failed to shift element test1.");

  item = ListShift(list);
  mu_assert(list->count == 1, "Failed to decrement count after shift.");
  mu_assert(item == test2, "Failed to shift test2");
  mu_assert(list->first->value == test1, "Failed to shift element test1.");


  item = ListShift(list);
  mu_assert(list->count == 0, "Failed to decrement count on shift");
  mu_assert(item == test1, "Failed to shift test1");

  ListDestroy(list);
  return NULL;
}

char *test_unshift() {
  list = ListCreate();
  ListUnshift(list, test1);
  mu_assert(list->count == 1, "Failed to increment count on unshift 1.");
  mu_assert(list->first->value == test1, "Failed to unshift element test1.");

  ListUnshift(list, test2);
  mu_assert(list->count == 2, "Failed to increment count on unshift 2.");
  mu_assert(list->first->value == test2, "Failed to retain first on unshift test2");
  mu_assert(list->last->value == test1, "Failed to retain last on unshift element test2.");

  ListUnshift(list, test3);
  mu_assert(list->count == 3, "Failed to increment count on unshift 3.");
  mu_assert(list->first->value == test3, "Failed to retain first on unshift test3");
  mu_assert(list->last->value == test1, "Failed to retain last on unshift element test3.");

  ListDestroy(list);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_destroy);
  mu_run_test(test_push);
  mu_run_test(test_pop);
  mu_run_test(test_shift);
  mu_run_test(test_unshift);

  return NULL;
}

RUN_TESTS(all_tests);
