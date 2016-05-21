#ifndef _linked_list_h
#define _linked_list_h

struct ListNode;

typedef struct ListNode {
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;

typedef struct List {
  int count;
  ListNode *first;
  ListNode *last;
} List;

List *ListCreate();
void ListDestroy(List *list);
void ListClear(List *list);
void ListClearDestory(List *list);

void ListPush(List *list, void *value);
void *ListPop(List *list);
void ListUnshift(List *list, void *value);
void *ListShift(List *list);

void *ListRemove(List *list, ListNode *node);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
  ListNode *V = NULL;\
  for(V = _node = L->S; _node != NULL; V = _node = _node->M)
#endif
