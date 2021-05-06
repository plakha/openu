#ifndef BUFFER_H
#define BUFFER_H

#ifndef DEFAULT_BUF_CAPACITY
#define DEFAULT_BUF_CAPACITY (60)
#endif

typedef struct buffer buffer_t;

/* Receives:

Returns - pointer to dynamic buffer that holds chars. 
The buffer's initial capacity is DEFAULT_BUF_CAPACITY'
On failure - returns NULL
*/
buffer_t *BufCreate();

/* Free the memory used by buffer and destroy the buffer.
Use this function when the buffer is no longer needed */
void BufDestroy(buffer_t *buf);

/*How many items can be currently held */
size_t BufGetCapacity(const buffer_t *buf);


char BufGetItem(const buffer_t *buf, size_t index);

/*pushes the item to the end of the buffer and returns 0. */
/*If couldn't be pushed, returns non-0*/
int BufPush(buffer_t *buf, char item);

/* return how many items are stored */
size_t BufSize(buffer_t *buf);

#endif /*BUFFER_H*/
