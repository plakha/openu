#ifndef LIST_H
#define LIST_H


typedef struct list list_t;
typedef struct node node_t;

list_t *ListCreate();


int ListIsEmpty(const list_t *list);

node_t *ListNodeGetFirst(const list_t *list);

node_t *ListNodeGetNext(const list_t *list, const node_t *node);

int ListNodeIsLast(const list_t *list, const node_t *node);

char ListNodeGetData(const node_t *node);

int ListPushData(list_t *list, char data);

#endif /* LIST_H */
