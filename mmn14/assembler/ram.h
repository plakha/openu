#ifndef RAM_H
#define RAM_H

#include "mmn14_types.h" /* word_t */

#define RAM_START_ADDRESS (100lu)

typedef struct ram ram_t; 

#define RAM_MAX_SIZE (1lu << 25)

extern const size_t RAM_MAX_SIZE_IN_WORDS;

ram_t *RAMCreate();

int RAMPushWord(ram_t *ram, const word_t *word);

word_t *RAMGetIthWord(const ram_t *ram, size_t i);

size_t RAMSize(const ram_t *ram);

void RAMDestroy(ram_t *ram);

#endif /* RAM_H */
