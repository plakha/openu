#include <stdlib.h> /* malloc */
#include <assert.h> 

#include "mmn14_types.h"

#include "ram.h"

struct ram
{
    /* data */
    int dummy;

};


ram_t *RAMCreate()
{
    ram_t *new_ram = malloc(sizeof(*new_ram));

    return new_ram;
}

void RAMDestroy(ram_t *ram)
{
    assert(ram);

    free(ram);
    ram = NULL;
}
