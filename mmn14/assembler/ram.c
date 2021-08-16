#include "stddef.h" /* NULL */

#include "mmn14_types.h"

#include "ram.h"

struct ram
{
    /* data */
    int dummy;

};


ram_t *RAMCreate()
{
    return NULL;
}

void RAMDestroy(ram_t *ram)
{
    (void)ram;
}
