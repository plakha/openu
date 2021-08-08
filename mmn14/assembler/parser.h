enum statement_type {R_3_ARG, R_2_ARG, J_0_ARG, J_ARG_LABEL, I_COND, I_ARITH_LOG_MEM, DIR, LABEL, ERROR};

typedef struct parser parser_t;

void ParserFirstPass(parser_t *parser, line_args_arr_t args, size_t line_number);


/* etc */




