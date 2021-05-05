#include <stdlib.h> /* size_t */
#include <assert.h> /* assert() */
#include <string.h> /* memcpy() */

#include "buffer.h"

#ifndef GROWTH_FACTOR
#define GROWTH_FACTOR (2)
#endif


enum {SUCCESS, FAIL};
enum {FALSE};

struct buffer 
{
	char *arr;
	size_t n_memb;
	size_t capacity; /* num of member */
};


buffer_t *BufCreate()
{
	buffer_t *buf = NULL;
	
	buf = (buffer_t *)(malloc(sizeof(*buf)));
	if (!buf) {return NULL;}
	
	buf->arr = calloc(DEFAULT_BUF_CAPACITY, sizeof(*(buf->arr)));
	if (!buf->arr)
	{
		free(buf);
		buf = NULL;
		
		return NULL;
	}
	
	buf->n_memb = 0;
	buf->capacity = DEFAULT_BUF_CAPACITY;
	
	return buf;
}

void BufDestroy(buffer_t *buf)
{
	assert(buf);
	
	free(buf->arr);
	buf->arr = NULL;
	
	free(buf);
	buf = NULL;	
}

size_t BufGetCapacity(const buffer_t *buf)
{
	assert(buf);
	
	return buf->capacity;
}

char BufGetItem(const buffer_t *buf, size_t index)
{
	assert(buf);
	assert(index < buf->n_memb);
	
	return (buf->arr)[index];
}

int BufPush(buffer_t *buf, char item)
{
	char *hold_buf = buf->arr;
	if (buf->n_memb >= buf->capacity)
	{
		if (NULL != (buf->arr = realloc(buf->arr, GROWTH_FACTOR * buf->capacity * sizeof(*(buf->arr)))))
		{
			buf->capacity *= GROWTH_FACTOR;
		}
		else 
		{
			buf->arr = hold_buf;
			
			return FAIL;
		}
	}
	
/*	*(buf->arr + buf->n_memb) = item;*/
/*	memcpy((char *)(buf->arr) + buf->n_memb, &item, buf->memb_size);*/
	(buf->arr)[buf->n_memb] = item;
	++(buf->n_memb);
		
	return SUCCESS;
}



