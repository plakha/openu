#ifndef BARR_H
#define BARR_H

#define BITS_PER_BYTE (8)

#ifndef BARR_N_BYTES
#define BARR_N_BYTES (16lu)
#endif /* BARR_N_BYTES */

enum {FALSE};

typedef struct barr barr_t;

/* Creates bit array of the size  BARR_N_BYTES * BITS_PER_BYTE */
barr_t *BArrCreate();

/* returns FALSE if the bit on the index is off */
int BArrIsBitOn(const barr_t *barr, size_t index);

/* creates new bit array that is boolean sum of the two arguments */
barr_t *BArrBArrOr(const barr_t *barr1, const barr_t *barr2);

/* creates new bit array that is boolean product of the two arguments */
barr_t *BArrBArrAnd(const barr_t *barr1, const barr_t *barr2);

/* creates new bit array that is boolean invertion of the argument */
barr_t *BArrBArrNot(const barr_t *barr);

/* creates new bit array that is result of XOR of the two arguments */
barr_t *BArrBArrXor(const barr_t *barr1, const barr_t *barr2);

/* creates new bit array that is copy of the argument */
barr_t *BArrBArrCopy(const barr_t *barr);

/* set the bit of the index to 1 */
barr_t *BArrSetBit(barr_t *barr, size_t index);

/* set the bit of the index to 0 */
barr_t *BArrUnsetBit(barr_t *barr, size_t index);

/* destroy the bit array to free the memory */
void BArrDestroy(barr_t *barr);

#endif /* BARR_H */
