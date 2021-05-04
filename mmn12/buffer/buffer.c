#include <stdlib.h> /* size_t */
#include <assert.h> /* assert() */

#include "buffer.h"

struct buffer 
{
	void *buf;
	size_t n_memb;
	size_t memb_size;
	size_t capacity; /* int byte */
};


buffer_t *BufCreate(size_t size_of_memb, size_t initial_capacity)
{
	buffer_t *buf = NULL;
	
	if (0 == initial_capacity) {initial_capacity = DEFAULT_BUF_CAPACITY;}
	
	buf = (buffer_t *)(malloc(sizeof(*buf)));
	
	return buf;
}


void Buf_Destroy(buffer_t *buf)
{
	assert(buf);
	
/*	free(buf->buf);*/
/*	buf->buf = NULL;*/
	
	free(buf);
	buf = NULL;	
}
