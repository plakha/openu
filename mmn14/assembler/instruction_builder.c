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
r_instr_t;

typedef
struct 
{
    unsigned int immed : 16;
    unsigned int rt : 5;
    unsigned int rs : 5;
    unsigned int opcode : 6;
    
}
i_instr_t;

typedef
struct 
{
    
    unsigned int address : 25;
    unsigned int reg : 1;
    unsigned int opcode : 6;
    
}
j_instr_t;

const word_t BAD_INSTRUCTION = {{0x0}}; /* all set to 0 */

word_t IBBadInstruction()
{
    return BAD_INSTRUCTION;
}


int IBIsBadInstruction(word_t instruction)
{
    return IBIsSameWord(BAD_INSTRUCTION, instruction);
}

int IBIsSameWord(word_t word1, word_t word2)
{   
    unsigned int i = 0;
    int ret = 1;

    for (;i < SIZE_OF_WORD; ++i)
    {
        ret = (ret && (word1.bytes[i] == word2.bytes[i]));
    }
    
    return ret;
}

word_t IBCreateInstructionR3Args(const char *inst,int rs, int rt, int rd)
{
    static const char *add = "add";
    static const char *sub = "sub";
    static const char *and = "and";
    static const char *or = "or";
    static const char *nor = "nor";

    static const int opcode = 0;
    int func = -1;
    static const int unused = 0;
  
    r_instr_t new_instruction = {0};

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
        return BAD_INSTRUCTION;
    }

    new_instruction.unused = unused;
    new_instruction.func = func;
    new_instruction.rd = rd;
    new_instruction.rt = rt;
    new_instruction.rs = rs;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}


word_t IBCreateInstructionR2Args(const char *inst, int rs, int rd)
{
    static const char *move = "move";
    static const char *mvhi = "mvhi";
    static const char *mvlo = "mvlo";

    static const int opcode = 1;
    int func = -1;
    static const int rt = 0;
    static const int unused = 0;
  
    r_instr_t new_instruction = {0};

    if (0 == strcmp(inst, move))
    {
        func = 1;
    }
    else if (0 == strcmp(inst, mvhi))
    {
        func = 2;
    } 
    else if (0 == strcmp(inst, mvlo))
    {
        func = 3;
    }
    else
    {
        assert(0);
        return BAD_INSTRUCTION;
    }

    new_instruction.unused = unused;
    new_instruction.func = func;
    new_instruction.rd = rd;
    new_instruction.rt = rt;
    new_instruction.rs = rs;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}

word_t IBCreateInstructionICond(const char *inst, int rs, int rt, long jmp_distance)
{
    static const char *beq = "beq";
    static const char *bne = "bne";
    static const char *blt = "blt";
    static const char *bgt = "bgt";


    int opcode = -1;
  
    i_instr_t new_instruction = {0};

    if (0 == strcmp(inst, beq))
    {
        opcode = 16;
    }
    else if (0 == strcmp(inst, bne))
    {
        opcode = 15;
    } 
    else if (0 == strcmp(inst, blt))
    {
        opcode = 17;
    }
    else if (0 == strcmp(inst, bgt))
    {
        opcode = 18;
    }
    else
    {
        puts(inst);
        printf("%d\n", strcmp(inst, "bgt"));
        assert(0);
        return BAD_INSTRUCTION;
    }

    new_instruction.immed = jmp_distance;
    new_instruction.rt = rt;
    new_instruction.rs = rs;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}

word_t IBCreateInstructionIArithLogMem(const char *inst, int rs, long immed, int rt)
{
    static const char *addi = "addi";
    static const char *subi = "subi";
    static const char *andi = "andi";
    static const char *ori = "ori";
    static const char *nori = "nori";
    static const char *lb = "lb";
    static const char *sb = "sb";
    static const char *lw = "lw";
    static const char *sw = "sw";
    static const char *lh = "lh";
    static const char *sh = "sh";
    
    int opcode = -1;
  
    i_instr_t new_instruction = {0};

    if (0 == strcmp(inst, addi))
    {
        opcode = 10;
    }
    else if (0 == strcmp(inst, subi))
    {
        opcode = 11;
    } 
    else if (0 == strcmp(inst, andi))
    {
        opcode = 12;
    }
    else if (0 == strcmp(inst, ori))
    {
        opcode = 13;
    }
    else if (0 == strcmp(inst, nori))
    {
        opcode = 14;
    }
    else if (0 == strcmp(inst, lb))
    {
        opcode = 19;
    } 
    else if (0 == strcmp(inst, sb))
    {
        opcode = 20;
    }
    else if (0 == strcmp(inst, lw))
    {
        opcode = 21;
    }
    else if (0 == strcmp(inst, sw))
    {
        opcode = 22;
    }
    else if (0 == strcmp(inst, lh))
    {
        opcode = 23;
    } 
    else if (0 == strcmp(inst, sh))
    {
        opcode = 24;
    }
    else
    {
        assert(0);
        return BAD_INSTRUCTION;
    }

    new_instruction.immed = immed;
    new_instruction.rt = rt;
    new_instruction.rs = rs;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}

word_t IBCreateInstructionJ0Args(const char *inst)
{
    static const char *stop = "stop";
   
    static const int reg = 0;
    static const size_t address = 0;
    int opcode = -1;
  
    j_instr_t new_instruction = {0};

    if (0 == strcmp(inst, stop))
    {
        opcode = 63;
    }
    else
    {
        assert(0);
        return BAD_INSTRUCTION;
    }

   
    new_instruction.reg = reg;
    new_instruction.address = address;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}

word_t IBCreateInstructionJ1Args(const char *inst, int is_reg, size_t address_or_reg)
{
    static const char *jmp = "jmp";
    int opcode = -1;
  
    j_instr_t new_instruction = {0};

    if (0 == strcmp(inst, jmp))
    {
        opcode = 30;
    }
    else
    {
        assert(0);
        return BAD_INSTRUCTION;
    }

    new_instruction.reg = is_reg ? 1 : 0;
    new_instruction.address = address_or_reg;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction;
    
}

word_t IBCreateInstructionJ1Label(const char *inst, size_t label_address)
{
    static const char *la = "la";
    static const char *call = "call";
    static const int reg = 0;
    
    int opcode = -1;
  
    j_instr_t new_instruction = {0};

    if (0 == strcmp(inst, la))
    {
        opcode = 31;
    }
    else if (0 == strcmp(inst, call))
    {
        opcode = 32;
    }
    else
    {
        assert(0);
        return BAD_INSTRUCTION;
    }

   
    new_instruction.reg = reg;
    new_instruction.address = label_address ;
    new_instruction.opcode = opcode;

    return *(word_t *)&new_instruction; 
}





