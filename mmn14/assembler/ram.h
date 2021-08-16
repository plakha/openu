#ifndef RAM_H
#define RAM_H

typedef struct ram ram_t; 

ram_t *RAMCreate();

void RAMDestroy(ram_t *ram);

#endif /* RAM_H */
