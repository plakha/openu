#ifndef LIST_H
#define LIST_H


typedef struct list list_t;
typedef struct node node_t;

/* returns pointer to list that can store chars*/
list_t *ListCreate();


int ListIsEmpty(const list_t *list);


node_t *ListNodeGetFirst(const list_t *list);

node_t *ListNodeGetNext(const node_t *node);

int ListNodeIsLast(const node_t *node);

char ListNodeGetData(const node_t *node);

int ListPushData(list_t *list, char data);

void ListDestroy(list_t *list);

#endif /* LIST_H */
