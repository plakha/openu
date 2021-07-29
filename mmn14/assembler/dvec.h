#ifndef DVEC_H
#define DVEC_H

#include <stddef.h> /* size_t */

typedef struct Dvec dvec_t;

/* May not include null-sized elements */
dvec_t *DVECCreate(size_t size_of_element, size_t num_elements);

/* Can only destroy the dynamic vector once! Do not use free() on 
the dynamic vector. Do not pass NULL argument. Failure to do so
may result in undefined behavior */
void DVECDestroy(dvec_t *dvec);

/* Do not pass NULL argument. */
int DVECPushBack(dvec_t *devc, const void *element);

/* Do not pass NULL argument. */
void DVECPopBack(dvec_t *dvec);

/* Do not pass NULL argument. */
void *DVECGetItemAddress(dvec_t *dvec, size_t index);

/* Do not pass NULL argument. */
size_t DVECSize(const dvec_t *dvec);

/* Do not pass NULL argument. */
int DVECReserve(dvec_t *dvec, size_t new_capacity);

/* Do not pass NULL argument. */
size_t DVECCapacity(const dvec_t *devc);

#endif
