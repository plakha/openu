#ifndef DLL_H
#define DLL_H

typedef int (*cmp_f)(const void *data, const void *key, const void *params);
typedef int (*act_f)(void *data, const void *params);
typedef struct dll dll_t;
typedef struct dll_node *it_t; 

/*struct dll*/
/*{*/
/*	struct node *head;*/
/*	struct node *tail;*/
/*};*/

/*struct node */
/*{*/
/*	struct node *next;*/
/*	struct node *prev;*/
/*	void *data;*/
/*};*/

dll_t *DLLCreate();

void DLLDestroy(dll_t *dll);

size_t DLLSize(const dll_t *dll);

int DLLIsEmpty(const dll_t *dll);

it_t DLLBegin(const dll_t *dll);

it_t DLLNext(const it_t iter);

/* do not pass DLLBegin ("Begin of dll") as 'iter' */
it_t DLLPrev(const it_t iter); 

it_t DLLEnd(const dll_t *dll);

int DLLIsSameIter(const it_t iter1,const it_t iter2);

void *DLLGetData(const it_t iter);

/* insert node with the data after the iter */
it_t DLLInsert(dll_t *dll, it_t iter, void *data);

 /*returns the node after the one removed*/ 
it_t DLLErase(it_t where);

it_t DLLPushBack(dll_t *dll, void *data);/* TODO lose const */

it_t DLLPushFront(dll_t *dll, void *data);/* TODO lose const */

void DLLPopBack(dll_t *dll);

void DLLPopFront(dll_t *dll);

/* returns 0 on success. On failure return val of a func which returns an iter should be the end_iter*/

int DLLForEach(it_t from, it_t to, act_f action, const void* params);
/*int DLLForEach(it_t from, it_t to, act_f action, void* params);*/

/* returns to if not found */
it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params);
/*it_t DLLFind(it_t from, it_t to, cmp_f is_found, void* params, void* key);*/

/* to is exclusive, from - inclusive.
sending to which is equal to from may result in crash */
void DLLSpliceBefore(it_t where, it_t from, it_t to);

#endif
