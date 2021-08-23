#include <stdlib.h> /* size_t */
#include <string.h> /* strcmp() */
#include <assert.h> /* assert() */
#include <stdio.h> /* fprintf() */

#include "instruction_builder.h"

/*
enum statement_type {R_3_ARG, R_2_ARG, J_0_ARG, J_1_ARG, J_1_LABEL, I_COND, I_ARITH_LOG_MEM, DIR_DEC, DIR_ASCII, DIR_QUALIF, LABEL, COMMENT, BLANK, ERROR};
*/

typedef
struct 
{
    unsigned int  unused : 6;
    unsigned int  func : 5;
    unsigned int  rd : 5;
    unsigned int  rt : 5;
    unsigned int  rs : 5;
    unsigned int opcode : 6;
    
}
r_t;

word_t *IBCreateInstructionR3Args(const char *inst,int rs, int rt, int rd)
{
    static const char *add = "add";
    static const char *sub = "sub";
    static const char *and = "and";
    static const char *or = "or";
    static const char *nor = "nor";

    static const int opcode = 0;
    int func = -1;
    static const int unused = 0;
  
    r_t *new_instruction = calloc(1, sizeof(*new_instruction));
    if (!new_instruction)
    { 
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return NULL;
    }

    if (0 == strcmp(inst, add))
    {
        func = 1;
    }
    else if (0 == strcmp(inst, sub))
    {
        func = 2;
    } 
    else if (0 == strcmp(inst, and))
    {
        func = 3;
    }
    else if (0 == strcmp(inst, or))
    {
        func = 4;
    }
    else if  (0 == strcmp(inst, nor))
    {
        func = 5;
    }
    else
    {
        assert(0);
        return NULL;
    }

    new_instruction->unused = unused;
    new_instruction->func = func;
    new_instruction->rd = rd;
    new_instruction->rt = rt;
    new_instruction->rs = rs;
    new_instruction->opcode = opcode;

    return (word_t *)new_instruction;
    
}