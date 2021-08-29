#define I_INSTR_MAX_JUMP ((1l << 15) - 1)
#define I_INSTR_MIN_JUMP (-(1l << 15))

#ifndef INSTRUCTION_BUILDER_H
#define INSTRUCTION_BUILDER_H
/* the library name space will be IB... */
#include <stdlib.h> /* size_t */

#include "mmn14_types.h" /* word_t */

/* This function gets the instruction name ("add", "sub", "and", "or", "nor") */ 
/* The function also accepts 3 additional parameters rs, rt, rd in the order listed */
/* The function returns bit array of the instruction according to the R instruction structure given for the maman14 */
word_t IBCreateInstructionR3Args(const char *inst,int rs, int rt, int rd);

/* This function gets the instruction name ("move", "mvhi", "mvlo") */ 
/* The function also accepts 2 additional parameters rs, rd in the order listed */
/* rt unused */
/* The function returns bit array of the instruction according to the R instruction structure given for the maman14 */
word_t IBCreateInstructionR2Args(const char *inst, int rs, int rd);

/* This function gets the instruction name only ("stop") */
/* The function returns bit array of the instruction according to the J instruction structure given for the maman14 */
/* "stop only" */
word_t IBCreateInstructionJ0Args(const char *inst);

/* This function gets the instruction name ("jmp") */
/* In addition the function gets reg and "address_or_reg" */
/* "address_or_reg" contains the address to which to jump or the register containing such address
    is_reg tells which it is
   */

/* is_reg is a Boolean value: if true "address_or_reg" holds register,
if false - address
    */
/* The function returns bit array of the instruction according to the J instruction structure given for the maman14 */
word_t IBCreateInstructionJ1Args(const char *inst, int is_reg, size_t address_or_reg);

/* This function gets the instruction name ("la", "call") */
/* In addition the function get "label_address" */

/* "label_address" -  address to which the Label refers  */

/* The function returns bit array of the instruction according to the J instruction structure given for the maman14 */
word_t IBCreateInstructionJ1Label(const char *inst, size_t label_address);

/* This function gets the instruction name ("beq", "bne", "blt", "bgt") */
/* The function also accepts 3 additional parameters rs, rt and "jump_distance" in the order listed */

/* "jump_distance" - between I_INSTR_MIN_JUMP and I_INSTR_MAX_JUMP - signed 16 bit integer (2's complement)  */

/* The function returns bit array of the instruction according to the I instruction structure given for the maman14 */
word_t IBCreateInstructionICond(const char *inst, int rs , int rt, long jump_distance);

/* This function gets the instruction name ("addi", "subi", "andi", "ori", "nori", "lb", "sb", "lw", "sw", "lh", "sh") */
/* The function also accepts 3 additional parameters rs, immed, rt in the order listed */
/* The immed parameter is a constant value (integer) represented by a 16-bit width in the complementary method to 2 */
/* The function returns bit array of the instruction according to the I instruction structure given for the maman14 */
word_t IBCreateInstructionIArithLogMem(const char *inst, int rs , long immed , int rt);

/*
    returns illegal instruction
*/
word_t IBBadInstruction();

/*
    Return boolean: if the received instraction is "Bad"
*/
int IBIsBadInstruction(word_t instruction);

/*
    receives to word, returns boolean: are they equal?
*/
int IBIsSameWord(word_t word1, word_t word2);



#endif /* INSTRUCTION_BUILDER_H */
