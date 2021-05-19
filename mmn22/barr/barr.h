#ifndef BARR_H
#define BARR_H

#define BITS_PER_BYTE (8)

#ifndef BARR_N_BYTES
#define BARR_N_BYTES (16lu)
#endif /* BARR_N_BYTES */



typedef struct barr barr_t;


barr_t *BArrCreate();

int BArrIsBitOn(const barr_t *barr, size_t index);

barr_t *BArrBArrOr(const barr_t *barr1, const barr_t *barr2);
barr_t *BArrBArrAnd(const barr_t *barr1, const barr_t *barr2);
barr_t *BArrBArrNot(const barr_t *barr);
barr_t *BArrBArrXor(const barr_t *barr1, const barr_t *barr2);

barr_t *BArrBArrCopy(const barr_t *barr);

barr_t *BArrSetBit(barr_t *barr, size_t index);

barr_t *BArrUnsetBit(barr_t *barr, size_t index);

void BArrDestroy(barr_t *barr);


#endif /* BARR_H */
