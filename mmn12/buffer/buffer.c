#include <stdlib.h> /* size_t */
#include <assert.h> /* assert() */

#include "buffer.h"

struct buffer 
{
	void *arr;
	size_t n_memb;
	size_t memb_size;
	size_t capacity; /* num of member */
};


buffer_t *BufCreate(size_t memb_size, size_t initial_capacity)
{
	buffer_t *buf = NULL;
	
	assert(memb_size);
		
	buf = (buffer_t *)(malloc(sizeof(*buf)));
	if (!buf) {return NULL;}
	
	buf->arr = calloc(initial_capacity, memb_size);
	if (!buf->arr)
	{
		free(buf);
		buf = NULL;
		
		return NULL;
	}
	
	buf->n_memb = 0;
	buf->memb_size = memb_size;
	buf->capacity = 0 == initial_capacity ? DEFAULT_BUF_CAPACITY : initial_capacity;
	
	return buf;
}


void BufDestroy(buffer_t *buf)
{
	assert(buf);
	
	free(buf->arr);
	buf->buf = NULL;
	
	free(buf);
	buf = NULL;	
}

size_t BufGetCapacity(const buffer_t *buf)
{
	assert(buf);
	
	return buf->capacity;
}

void *BufGetItem(const buffer_t *buf, size_t index)
{
	assert(buf);
	
	return (buf->arr) + index * (buf->memb_size)m
}
