#include <stdlib.h> /* malloc */
#include <assert.h> /* assert() */
#include <stdio.h> /* fprintf() */

#include "dvec.h" /* dvec_t */

#include "ram.h"

#define INITIAL_RAM_CAPACITY (1000lu)

const size_t RAM_MAX_SIZE_IN_WORDS = RAM_MAX_SIZE / 4; /* 2^25 Bytes = 2^25/4 Words*/ 

struct ram
{
    dvec_t *word_arr;
};


ram_t *RAMCreate()
{
    ram_t *new_ram = malloc(sizeof(*new_ram));
    if (!new_ram)
    {
        fprintf(stderr, 
        "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);
    }
    else{
        new_ram->word_arr = DVECCreate(sizeof(word_t), INITIAL_RAM_CAPACITY);
        if (!new_ram->word_arr)
        {
            free (new_ram);
            new_ram = NULL;
            fprintf(stderr, 
            "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
            __LINE__, __FILE__);
        }
    }

    return new_ram;
}

int RAMPushWord(ram_t *ram, const word_t *word)
{
    assert(ram);
    assert(word);
    assert(ram->word_arr);

    return DVECPushBack(ram->word_arr, word);
}

word_t *RAMGetIthWord(const ram_t *ram, size_t i)
{
    assert(ram);
    assert(RAMSize(ram) > i);
    assert(ram->word_arr);

    return (word_t *)DVECGetItemAddress(ram->word_arr, i);
}

unsigned long RAMSize(const ram_t *ram)
{
    size_t ram_size = -1;

    assert(ram);
    assert(ram->word_arr);

    ram_size = DVECSize(ram->word_arr);

    assert(RAM_MAX_SIZE_IN_WORDS >= ram_size);

    return ram_size;
}


void RAMDestroy(ram_t *ram)
{
    assert(ram);

    DVECDestroy(ram->word_arr);
    ram->word_arr = NULL;
    free(ram);
    ram = NULL;
}
