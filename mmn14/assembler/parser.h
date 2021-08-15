#ifndef PARSER_H
#define PARSER_H

#include "dvec.h"

#define COMMENT_CHAR ';'

enum statement_type {R_3_ARG, R_2_ARG, J_0_ARG, J_ARG_LABEL, I_COND, I_ARITH_LOG_MEM, DIR, LABEL, COMMENT, BLANK, ERROR};


typedef struct parser parser_t;

void ParserFirstPass(parser_t *parser, dvec_t *args, size_t line_number);



#endif /* PARSER_H */



