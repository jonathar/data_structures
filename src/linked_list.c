#include <stdlib.h>
#include "linked_list.h"
#include "dbg.h"

List *ListCreate() {
  return calloc(1, sizeof(List));
}

void ListDestroy(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }

  free(list->last);
  free(list);
}

void ListClear(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    free(cur->value);
  }
}

void ListClearDestroy(List *list) {
  ListClear(list);
  ListDestroy(list);
}

void ListPush(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);
  node->value = value;

  if (list->last) {
    node->prev = list->last;
    list->last->next = node;
    list->last = node;
  } else {
    list->first = node;
    list->last = node;
  }

  list->count++;

error:
  return;
}


void *ListPop(List *list) {
  ListNode *node = list->last;

  return node != NULL ? ListRemove(list, node) : NULL;
}


void ListUnshift(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);
  node->value = value;

  if (list->first) {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  } else {
    list->first = node;
    list->last = node;
  }

  list->count++;

error:
  return;
}


void *ListShift(List *list) {
  ListNode *node = list->first;

  return node != NULL ? ListRemove(list, node) : NULL;
}


void *ListRemove(List *list, ListNode *node) {
  void *result = NULL;

  check(list->first && list->last, "List is empty.");
  check(node, "node can't be NULL");

  if(node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL, "Invalid list. First can't be null.");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev; 
    check(list->last != NULL, "Invalid list. Last can't be null.");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode *before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  if(node) free(node);
  return result;

error:
  return result;
}
