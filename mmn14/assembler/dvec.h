#ifndef DVEC_H
#define DVEC_H

/***********
 * Dynamic Vector impementation.
 * The data is kept by value, not by reference!!
 * Please pass valid arguments
 * **********/
#include <stddef.h> /* size_t */

typedef struct Dvec dvec_t;

/*
    Returns dvec of size_of_element - sized elements,
    initial capacity is num_elements.
    if num_elements - sets a default initial capacity
 May not include null-sized elements
 
 Returns  NULL if memory could not be allocated */
dvec_t *DVECCreate(size_t size_of_element, size_t num_elements);

/* Can only destroy the dynamic vector once! Do not use free() on 
the dynamic vector. Do not pass NULL argument. Failure to do so
may result in undefined behavior */
void DVECDestroy(dvec_t *dvec);

/* Do not pass NULL argument.
    Pushes the data to the end of the vector.
    retuens non-zero in case of memory allocation problem.
    May invalidate the result of previous call to DVECCapacity()
 */
int DVECPushBack(dvec_t *devc, const void *element);

/* Do not pass NULL argument.
erases data from the end of the vector.
May invalidate the result of previous call to DVECCapacity() */
void DVECPopBack(dvec_t *dvec);

/* Do not pass NULL argument.
Returns pointer to the data at index  */
void *DVECGetItemAddress(const dvec_t *dvec, size_t index);

/* Do not pass NULL argument.
retuns number of elements */
size_t DVECSize(const dvec_t *dvec);

/* Do not pass NULL argument.
    Reallocate capacity.
 */
int DVECReserve(dvec_t *dvec, size_t new_capacity);

/* Do not pass NULL argument.
    returns the current capacity
     */
size_t DVECCapacity(const dvec_t *devc);

#endif
