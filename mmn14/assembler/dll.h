#ifndef DLL_H
#define DLL_H

/********
 * Implementation of doubly linked list abstract data structure
 * 
 * *********/


/* compare function: the data in a node , the given key, to which we compare, the params (optional) */
typedef int (*cmp_f)(const void *data, const void *key, const void *params);

/* the data upon which to act */
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

/*
Returns instance of dll

*/
dll_t *DLLCreate();

/*
    receives dll and free the memory 
*/
void DLLDestroy(dll_t *dll);

/*
    receives dll and returns its size(number of nodes)
*/
size_t DLLSize(const dll_t *dll);

/*
receives dll and returns boolean: whether the dll is empty
*/
int DLLIsEmpty(const dll_t *dll);

/* receives the dll and returns first node (iterator) */
it_t DLLBegin(const dll_t *dll);

/* receives iterator and returns next iterator  */
it_t DLLNext(const it_t iter);

/* 
    returns previous iterator
do not pass DLLBegin ("Begin of dll") as 'iter' !*/
it_t DLLPrev(const it_t iter); 

/*
    return "end" node = "bad", empty node
*/
it_t DLLEnd(const dll_t *dll);

/*
    receives two iterators and returns boolean: whether the are one and the same
*/
int DLLIsSameIter(const it_t iter1,const it_t iter2);

/*
    returns the pointer to data od this iterator (node)
*/
void *DLLGetData(const it_t iter);

/* insert node with the data after the iter */
it_t DLLInsert(dll_t *dll, it_t iter, void *data);

 /*
    erases the given node
 returns the node after the one removed
 do not pass end node!!
 */ 
it_t DLLErase(it_t where);

/*
    Push data to the end of the list
*/
it_t DLLPushBack(dll_t *dll, void *data);

/*
    push data to the beginning of the list.
    Note: it invalidates the previous result of DLLBegin()
*/
it_t DLLPushFront(dll_t *dll, void *data);

/*
    Erase the last node of the list
*/
void DLLPopBack(dll_t *dll);

/*
    Erase the first node of the list
    Note: it invalidates the previous result of DLLBegin()
*/
void DLLPopFront(dll_t *dll);

/* returns 0 on success. 
On failure (action returns non-zero), stops and returns the action result*/
int DLLForEach(it_t from, it_t to, act_f action, const void* params);

/*
Returns iterator that was found using the cmp_f function.
 returns to if not found */
it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params);
/*it_t DLLFind(it_t from, it_t to, cmp_f is_found, void* params, void* key);*/


#endif
