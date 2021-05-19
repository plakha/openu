#include <stdlib.h> /* size_t */
#include <assert.h> /* assert() */
#include "barr.h"

typedef unsigned char byte;

enum {FALSE};

struct barr
{
	byte arr[BARR_N_BYTES];	
};

static const byte bit_masks[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};

barr_t *BArrCreate()
{
	return calloc(1, sizeof(barr_t)); /* the bit array is initiated to 0 thanks to calloc() */
}

barr_t *BArrBArrOr(const barr_t *barr1, const barr_t *barr2)
{
	size_t i = 0;
	barr_t *new_barr = NULL;
	
	assert(barr1);
	assert(barr2);
	
	new_barr = BArrCreate();
	if (!new_barr)
	{
		return NULL;
	}
	
	for (i = 0; BARR_N_BYTES > i; ++i)
	{
		(new_barr->arr)[i] = (barr1->arr)[i] | (barr2->arr)[i];
	}
	
	return new_barr;
}

barr_t *BArrBArrAnd(const barr_t *barr1, const barr_t *barr2)
{
	size_t i = 0;
	barr_t *new_barr = NULL;
	
	assert(barr1);
	assert(barr2);
	
	new_barr = BArrCreate();
	if (!new_barr)
	{
		return NULL;
	}
	
	for (i = 0; BARR_N_BYTES > i; ++i)
	{
		(new_barr->arr)[i] = (barr1->arr)[i] & (barr2->arr)[i];
	}
	
	return new_barr;
}

barr_t *BArrBArrNot(const barr_t *barr)
{
	size_t i = 0;
	barr_t *new_barr = NULL;
	
	assert(barr);
	
	new_barr = BArrCreate();
	if (!new_barr)
	{
		return NULL;
	}
	
	for (i = 0; BARR_N_BYTES > i; ++i)
	{
		(new_barr->arr)[i] = ~(barr->arr)[i];
	}
	
	return new_barr;
}

barr_t *BArrBArrCopy(const barr_t *barr)
{
	size_t i = 0;
	barr_t *new_barr = NULL;
	
	assert(barr);
	
	new_barr = BArrCreate();
	if (!new_barr)
	{
		return NULL;
	}
	
	for (i = 0; BARR_N_BYTES > i; ++i)
	{
		(new_barr->arr)[i] = (barr->arr)[i];
	}
	
	return new_barr;
}

barr_t *BArrBArrXor(const barr_t *barr1, const barr_t *barr2)
{
	size_t i = 0;
	barr_t *new_barr = NULL;
	
	assert(barr1);
	assert(barr2);
	
	new_barr = BArrCreate();
	if (!new_barr)
	{
		return NULL;
	}
	
	for (i = 0; BARR_N_BYTES > i; ++i)
	{
		(new_barr->arr)[i] = (barr1->arr)[i] ^ (barr2->arr)[i];
	}
	
	return new_barr;
}

int BArrIsBitOn(const barr_t *barr, size_t index)
{
	assert(barr);
	assert(index < BARR_N_BYTES * BITS_PER_BYTE);
	
/*	all the bits but the masked will be set to 0; if the masked one is 0, 
the whole byte is 0, hence 0 (FALSE) is returned*/
	return (byte)0x0 == ((barr->arr)[index / BITS_PER_BYTE] & bit_masks[index % BITS_PER_BYTE]);
}

barr_t *BArrSetBit(barr_t *barr, size_t index)
{
	assert(barr);
	assert(index < BARR_N_BYTES);

	barr->arr[index / BITS_PER_BYTE] |= bit_masks[index % BITS_PER_BYTE];
	
	return barr;
}

barr_t *BArrUnsetBit(barr_t *barr, size_t index)
{
	assert(barr);
	assert(index < BARR_N_BYTES * BITS_PER_BYTE);
		
	
	barr->arr[index / BITS_PER_BYTE] &= ~(bit_masks[index % BITS_PER_BYTE]);
	
	return barr;
}

void BArrDestroy(barr_t *barr)
{
	assert(barr);
	
	free(barr);
	barr = NULL;
}

