#ifndef INSTRUCTION_BUILDER_H
#define INSTRUCTION_BUILDER_H
/* the library name spave will be IB... */
#include <stdlib.h> /* size_t */

#include "mmn14_types.h" /* word_t */

word_t IBCreateInstructionR3Args(const char *inst,int rs, int rt, int rd);

/* rt unused */
word_t *IBCreateInstructionR2Args(const char *inst, int rs, int rd);

/* "stop only" */
word_t IBCreateInstructionJ0Args(const char *inst);

word_t IBCreateInstructionJ1Args(const char *inst, int reg);


/* receives address to which the Label refers */
word_t IBCreateInstructionJ1Label(const char *inst, size_t label_address);

word_t IBCreateInstructionICond(const char *inst, int rs , int rt, size_t label_address);


word_t IBCreateInstructionIArithLogMem(const char *inst, int rs , long immed , int rt);


int IBIsBadInstruction(word_t instruction);

int IBIsSameWord(word_t word1, word_t word2);



#endif /* INSTRUCTION_BUILDER_H */