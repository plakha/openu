/**************
	Data Structure Dobly Linked List
	10.6.19
	Author Andrei
	Reviewer Yael
***************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#include "dll.h"

/*typedef int (*cmp_f)(const void *data, const void *key, const void *params);*/
/*typedef int (*act_f)(void *data, const void *params);*/
/*typedef struct dll dll_t;*/
/*typedef struct dll_node *it_t; */

struct dll
{
	struct dll_node *head;
	struct dll_node *tail;
};

struct dll_node 
{
	struct dll_node *next;
	struct dll_node *prev;
	void *data;
};

static it_t CreateNode(void *data)
{
	it_t new_node = (it_t)malloc(sizeof(*new_node));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->data = (void *)data;
	
	return new_node;
}

dll_t *DLLCreate()
{
	dll_t *new_dll = (dll_t *)malloc(sizeof(*new_dll));
	if(NULL == new_dll)
	{
		return NULL;
	}
	
	new_dll->head = CreateNode((void*)0xDEADBEEF);
	new_dll->tail = CreateNode((void*)0xDEADBEEF);
	if(NULL == new_dll->head || NULL == new_dll->tail)
	{
		free(new_dll);
		free(new_dll->head);
		free(new_dll->tail);
		
		return NULL;
	}
	
	new_dll->head->next = new_dll->tail;
	new_dll->tail->prev = new_dll->head;
	
	return new_dll;
}

void DLLDestroy(dll_t *dll)
{
	it_t iter = NULL;
	
	assert(NULL != dll);

	iter = DLLEnd(dll);
	while(NULL != iter)
	{
		free(iter->next);
		iter = DLLPrev(iter);
	}
	free(dll->head);
	
	free(dll);
}

size_t DLLSize(const dll_t *dll)
{
	size_t size = 0;
	it_t run = NULL;
	
	assert(NULL != dll);

	run = DLLBegin(dll);

	while(NULL != (run = DLLNext(run)))
	{
		++size;
	}

	return size;	
}

int DLLIsEmpty(const dll_t *dll)
{
    assert(NULL != dll);

	return DLLBegin(dll) == DLLEnd(dll);
}

it_t DLLBegin(const dll_t *dll)
{
	assert(NULL != dll);

	return DLLNext(dll->head);
}

it_t DLLEnd(const dll_t *dll)
{
	assert(NULL != dll);

	return dll->tail;
}

it_t DLLNext(const it_t iter)
{
	assert(NULL != iter);

	return iter->next;
}

it_t DLLPrev(const it_t iter)
{
	assert(NULL != iter);

	return iter->prev;
}

void *DLLGetData(const it_t iter)
{
	assert(NULL != iter);

	return iter->data;
}

int DLLIsSameIter(const it_t iter1,const it_t iter2)
{
	assert(NULL != iter1 && NULL != iter2);

	return iter1 == iter2;
}

it_t DLLInsert(dll_t *dll, it_t iter, void *data)
{
	it_t new_node = NULL;

	assert(NULL != dll && NULL != iter);

	assert(iter != DLLEnd(dll));

	new_node = CreateNode(data);
	if(NULL == new_node)
	{
		return DLLEnd(dll);
	}

	new_node->next = DLLNext(iter);
	new_node->prev = iter;
	DLLNext(new_node)->prev = new_node;
	iter->next = new_node;

	return new_node;
}

it_t DLLErase(it_t where)
{
	assert(NULL != where);
	assert(NULL != where->next);

	assert(NULL != where->prev);

	where = DLLNext(where);
	where->prev = DLLPrev(DLLPrev(where));
	free(DLLNext(DLLPrev(where)));
	DLLPrev(where)->next = where;

	return where;
}

it_t DLLPushBack(dll_t *dll, void *data)
{
	assert(NULL != dll);

	return DLLInsert(dll, DLLPrev(DLLEnd(dll)), data);
}

it_t DLLPushFront(dll_t *dll, void *data)
{
	assert(NULL != dll);

	return DLLInsert(dll, dll->head, data);
}

void DLLPopBack(dll_t *dll)
{	
	assert(NULL != dll);
	
	assert(!DLLIsEmpty(dll));

	DLLErase(DLLPrev(DLLEnd(dll)));
}

void DLLPopFront(dll_t *dll)
{
	assert(NULL != dll);
	
	assert(!DLLIsEmpty(dll));

	DLLErase(DLLBegin(dll));
}

int DLLForEach(it_t from, it_t to, act_f action, const void* params)
{
	int ret = 0;

	assert(NULL != from && NULL != to && NULL != action);

	/* while action is successful */
	while((from != to) && (0 == (ret = action(DLLGetData(from), params))))
	{
		from = DLLNext(from);
	}

	return ret;
}

it_t DLLFind(it_t from, it_t to, cmp_f is_found, const void *key, 
												const void *params)
{
	assert(NULL != from && NULL != to && NULL != is_found);
		
	for(; 
		from != to && 0 == is_found(DLLGetData(from), key, params); 
		from = DLLNext(from))
	{;}

	return from;
}

void DLLSpliceBefore(it_t where, it_t from, it_t to)
{
	it_t prev_where = NULL;
	it_t prev_from = NULL;
	it_t prev_to = NULL;

	/* TODO assert or if? implications on SrtLMerge */
	assert(from != to);
/*	if(from == to)*/
/*	{*/
/*	return;*/
/*	}*/

	assert(NULL != where && NULL != from && NULL != to);

	assert(NULL != where->prev && NULL != from->prev && NULL != to->prev);
	
	prev_where = DLLPrev(where);
	prev_from = DLLPrev(from);
	prev_to = DLLPrev(to);
	
	prev_from->next = to;
	from->prev = prev_where;
	prev_where->next = from;
	prev_to->next = where;
	where->prev = prev_to;
	to->prev = prev_from;
}
