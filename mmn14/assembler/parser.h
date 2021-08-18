#ifndef PARSER_H
#define PARSER_H

#include "dvec.h"

#define COMMENT_CHAR ';'

enum statement_type {R_3_ARG, R_2_ARG, J_0_ARG, J_1_ARG, J_1_LABEL, I_COND, I_ARITH_LOG_MEM, DIR_DEC, DIR_ASCII, DIR_QUALIF, LABEL, COMMENT, BLANK, ERROR};
/*
DIR_DEC, DIR_ASCII, DIR_QUALIF:
add as return values to WhatStatement
Add as cases to Validate
add Validators
*/

typedef struct parser parser_t;

parser_t *ParserCreate(const char *source_file_name, ram_t *ram, sym_tab_t *sym_tab);

void ParserFirstPass(parser_t *parser, dvec_t *args, size_t line_number);

int ParserIsSyntaxCorrupt(const parser_t * parser);

void ParserDestroy(parser_t *parser, int should_destroy_passed_entities);

#endif /* PARSER_H */



