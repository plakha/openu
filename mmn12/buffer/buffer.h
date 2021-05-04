#ifndef DEFAULT_BUF_CAPACITY
#define DEFAULT_BUF_CAPACITY (10)
#endif

typedef struct buffer buffer_t;

/* Receives:
size_of_memb - size 0f each member
initial_capacity - how many members can hold; if 0 - uses DEFAULT_BUF_CAPACITY

Returns - pointer to dynamic buffer that holds copies of the members (TODO the members can be of a primitive type only? )
On failure - returns NULL

TODO use assignment or memcpy? The former is good enough because we will use it for primitive type */
buffer_t *BufCreate(size_t size_of_memb, size_t initial_capacity);

/* Free the memory used by buffer and destroy the buffer.
Use this function when the buffer is no longer needed */
void BufDestroy(buffer_t *buf);

size_t BufGetCapacity(const buffer_t *buf);

void *BufGetItem(const buffer_t *buf, size_t index);
