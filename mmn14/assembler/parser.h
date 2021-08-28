#ifndef PARSER_H
#define PARSER_H

#include "dvec.h"

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

enum {OK = 0, SYNT_ERR, MEM_ERR};

typedef struct parser parser_t;

parser_t *ParserCreate(const char *source_file_name, ram_t *ram, sym_tab_t *sym_tab);

int ParserFirstPass(parser_t *parser, dvec_t *args);
int ParserSecondPass(parser_t *parser, dvec_t *args);

int ParserIsSyntaxCorrupt(const parser_t * parser);

void ParserDestroy(parser_t *parser, int should_destroy_passed_entities);

unsigned long ParserGetIC(const parser_t *parser);

const char *ParserGetFileName(const parser_t *parser);

/* Force parser into believing syntax is failed. Returns non-zero if it was already failed. Returns 0 otherwise */
int ParserFailParser(parser_t *parser);


unsigned long ParserGetCurLineNum(const parser_t *parser);


void ParserResetLineCount(parser_t *parser);

#endif /* PARSER_H */



