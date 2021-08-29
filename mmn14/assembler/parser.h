#ifndef PARSER_H
#define PARSER_H

/*************************
 * Parser for assembler project, mmn 14, Summer 2021
 * The parser does the heavy lifting.
 * 
 * 1st pass:
 * Receives an arg vector, checks the statement type, validates statement.
 * Gets the extern symbols and symbol definitions and pushes to symbol table.
 * 
 * 2nd pass:
 * Gets the instructions, sends them to instruction builder amd pushes the resulting words to RAM.
 * Gets the entry qualifiers and sets them to the relevant labels in the symbol table.
 * 
 * Please pass valid arguments.
 * 
 * 
 * 
 * 
 * 
 * 
 * ***********************/

#include "dvec.h" /* dvec_t */

#define COMMENT_CHAR ';'

enum statement_type {
R_3_ARG, /*R-type instruction with 3 args: add, sub, or, nor, and  */
R_2_ARG, /*R-type instruction with 2 args: move, mvhi, mvlo */
J_0_ARG, /*J-type instruction with 0 args: stop */
J_1_ARG,/*J-type instruction with 1 args (label or register): jump */
J_1_LABEL,/*J-type instruction with 1 args (label): la, call */
I_COND,/* I-type conditioned instructions: beq, bne, blt, bgt */
I_ARITH_LOG_MEM,/* I-type other instructions: addi, subi, ori, nori, andi, lb, sb, lw, sb, lh, sh*/
DIR_DEC, /* Directives: .dh, .db, .dw */
DIR_ASCII,/* .asciiz */
DIR_QUALIF,/* .entry, .extern */
LABEL,/* My_Label: [...] */
COMMENT,/* ;[...]*/
BLANK,/* space characters only */
ERROR/* illegal statement */
};

enum 
{
OK = 0,
SYNT_ERR, /* bad syntax instance */
MEM_ERR /* memory alloc problem */
};

typedef struct parser parser_t;

/* receives filename that will be parsed, pointer to ram, pointer to symbol table

The filename is kept as is. It is unsafe for the caller to  and alter it!
returns pointer to parser */
parser_t *ParserCreate(const char *source_file_name, ram_t *ram, sym_tab_t *sym_tab);

/*
    Receives pointer tp parser,  an arg vector (representing a statement), checks the statement type, validates statement.
 Gets the extern symbols and symbol definitions and pushes to symbol table.

 returns MEM_ERR in case of memory allocation issue,
 SYNT_ERR in case of bad syntax
 OK - in other cases
 */
int ParserFirstPass(parser_t *parser, dvec_t *args);


/*
    Receives pinter to parser,  an arg vector (representing a statement), checks the statement type, validates statement.
 * Gets the entry qualifiers and sets them to the relevant labels in the symbol table.

 returns MEM_ERR in case of memory allocation issue,
 SYNT_ERR in case of bad syntax
 OK - in other cases
 */
int ParserSecondPass(parser_t *parser, dvec_t *args);

/* receives pointer to parser, 
returns boolean value: whether there are any syntax errors in the current file. 

*/
int ParserIsSyntaxCorrupt(const parser_t * parser);

/* receives pointer to parser, 
    frees the allocated memory

    the boolean argument denotes whether to free the ram and the symbol table as well
*/
void ParserDestroy(parser_t *parser, int should_destroy_passed_entities);

/*
    Receives pointer to the parser
    Returns the instruction counter for the current file
*/
unsigned long ParserGetIC(const parser_t *parser);


/*
Receives the pointer to parser end returns the filename.
*/
const char *ParserGetFileName(const parser_t *parser);

/* Sets thge syntax corrupt for the current file. Returns non-zero if it was already failed. Returns 0 otherwise */
int ParserFailParser(parser_t *parser);

/*
receives the pointer to parser and returns the current line count for the current filename
*/
unsigned long ParserGetCurLineNum(const parser_t *parser);

/*
    Receives pointer to parser, resets the line count
*/
void ParserResetLineCount(parser_t *parser);

#endif /* PARSER_H */



