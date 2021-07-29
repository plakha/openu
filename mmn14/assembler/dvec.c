#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "dvec.h"

#define DEF_DVEC_CAPACITY 4

struct Dvec
{
	void *array;
	size_t size_of_element;
	size_t num_of_elements;
	size_t capacity;/* init 4 elements*/
};

dvec_t *DVECCreate(size_t size_of_element, size_t num_elements)
{
	dvec_t *dvec = NULL;
	void *array = NULL;

	assert(0 < size_of_element);
	
	if(0 == num_elements)
	{
		num_elements = DEF_DVEC_CAPACITY;
	}
	dvec = (dvec_t *)malloc(sizeof(*dvec));
	array = malloc(size_of_element * num_elements);
	if((NULL == array) || (NULL == dvec))
	{
		free(dvec);
		free(array);

		return NULL;
	}

	dvec->array = array;
	dvec->size_of_element = size_of_element;
	dvec->num_of_elements = 0;
	dvec->capacity = num_elements;

	return dvec;
}

void DVECDestroy(dvec_t *dvec)
{
	assert(NULL != dvec);

	free(dvec->array);
	free(dvec);
}

int DVECPushBack(dvec_t *dvec, const void *element)
{
	void *alloc_hold = NULL;

	assert(NULL != dvec);
	
	if((dvec->capacity) <= (dvec->num_of_elements))
	{
		/* not enough capacity */
		return 2;
	}

	memcpy( (void *)((char *)(dvec->array) + dvec->size_of_element 
			* (dvec->num_of_elements)), element, dvec->size_of_element );
	++(dvec->num_of_elements);

	if((dvec->capacity) <= (dvec->num_of_elements))
	{
		alloc_hold = realloc( dvec->array, 
					2 * (dvec->capacity) * (dvec->size_of_element) );
		if(NULL == alloc_hold)
		{
			/* realloc fail */
			return 1;
		}
		dvec->array = alloc_hold;
		dvec->capacity *= 2;
	}

	return 0;
}

void DVECPopBack(dvec_t *dvec)
{
	void *alloc_hold = NULL;

	assert(NULL != dvec);
		
	if( (dvec->capacity) > (4 * (dvec->num_of_elements)) )
	{
		/* free excessive allocation */
		if( NULL != ( alloc_hold = realloc(dvec->array, 
			(dvec->size_of_element) * (dvec->capacity) / 2) ) )
		{
			dvec->array = alloc_hold;
			(dvec->capacity) /= 2;
		}
	}

	if(0 == dvec->num_of_elements)
	{

		return;	
	}
	--(dvec->num_of_elements);	
}

void *DVECGetItemAddress(dvec_t *dvec, size_t index)
{
	assert(NULL != dvec);
	
	if(index >= dvec->num_of_elements)	
	{

		return NULL;
	}

	return (void *)((char *)(dvec->array) + index * (dvec->size_of_element));	
}

size_t DVECSize(const dvec_t *dvec)
{
	assert(NULL != dvec);

	return dvec->num_of_elements;
}

int DVECReserve(dvec_t *dvec, size_t new_capacity)
{
	void *alloc_hold = NULL;

	assert(NULL != dvec);

	if(new_capacity < (dvec->num_of_elements) + 1)
	{
		new_capacity = (dvec->num_of_elements) + 1;
	}

	if(new_capacity == dvec->capacity)
	{
		/* Nothing to change */
		return 0;
	}

	alloc_hold = realloc(dvec->array, new_capacity * dvec->size_of_element);
	if(NULL == alloc_hold)
	{
		/* Capacity unchanged because of realloc failure */
		return 1;
	}

	dvec->array = alloc_hold;
	dvec->capacity = new_capacity;

	return 0;
}

size_t DVECCapacity(const dvec_t *dvec)
{
	assert(NULL != dvec);

	return dvec->capacity;
}


