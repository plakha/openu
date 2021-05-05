#ifndef BUFFER_H
#define BUFFER_H

#ifndef DEFAULT_BUF_CAPACITY
#define DEFAULT_BUF_CAPACITY (2) /*TODO 60 */
#endif

typedef struct buffer buffer_t;

/* Receives:
size_of_memb - size 0f each member
initial_capacity - how many members can hold; if 0 - uses DEFAULT_BUF_CAPACITY

Returns - pointer to dynamic buffer that holds copies of the members (TODO the members can be of a primitive type only? )
On failure - returns NULL

TODO use assignment or memcpy? The former is good enough because we will use it for primitive type */
buffer_t *BufCreate();

/* Free the memory used by buffer and destroy the buffer.
Use this function when the buffer is no longer needed */
void BufDestroy(buffer_t *buf);

size_t BufGetCapacity(const buffer_t *buf);

char BufGetItem(const buffer_t *buf, size_t index);

/*pushes the item to the end of the buffer and returns 0. */
/*If no memory has been left to push, returns non-0*/
int BufPush(buffer_t *buf, char item);

#endif /*BUFFER_H*/
