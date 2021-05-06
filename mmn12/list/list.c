#include <assert.h> /* assert() */
#include <stdlib.h> /* size_t */

#include "list.h"

enum {SUCCESS, FAILURE};

struct node
{
	char data;
	struct node *next;
};

struct list
{
	node_t *head;
	node_t *last_stub; /* allows to push in O(1) */
};

static node_t *NodeCreate()
{
	return calloc(1, sizeof(node_t)); /* next is set to NULL, thanks to calloc */
}

list_t *ListCreate()
{
	list_t *list = calloc(1, sizeof(*list));
	if (!list)
	{
		return NULL;
	}
	
	list->head = NULL;
	
	list->last_stub = NodeCreate();
	if (!list->last_stub)
	{
		free(list);
		
		return NULL;
	}
	
	return list;
}


int ListPushData(list_t *list, char data)
{
	node_t *new_node = NULL;
	
	assert(list);
	
	new_node = NodeCreate();
	if (!new_node)
	{
		return FAILURE;
	}
	
	if (ListIsEmpty(list))
	{
		new_node->data = data;
		new_node->next = list->last_stub;
		list->head = new_node;
	}
	else
	{
		list->last_stub->data = data;
		list->last_stub->next = new_node;
		list->last_stub = new_node;
	}
	
	return SUCCESS;
}

int ListIsEmpty(const list_t *list)
{
	assert(list);
	
	return NULL == list->head;
}

node_t *ListNodeGetFirst(const list_t *list)
{
	assert(list);
	assert(!ListIsEmpty(list));
	
	return list->head;
}




node_t *ListNodeGetNext(const node_t *node)
{
	assert(!ListNodeIsLast(node)); /* not last_stub */
	
	return node->next;
}

int ListNodeIsLast(const node_t *node)
{
	assert(node);
	
	return NULL == node->next;
}

char ListNodeGetData(const node_t *node)
{
	assert(node);
	
	return node->data;
}

static void NodesDestroy(node_t *head)
{
	if (!head)
	{
		return;
	}
	
	NodesDestroy(head->next);
	head->next = NULL;
	
	free(head);
	head = NULL;	
}

void ListDestroy(list_t *list)
{

	assert(list);
	
	NodesDestroy(list->head);
	list->head = NULL;
	list->last_stub = NULL;
	
	free(list);
	list = NULL;
}

























	



