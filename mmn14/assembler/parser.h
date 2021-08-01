enum statement {R_3_ARG, R_2_ARG, J_0_ARG, J_ARG_LABEL, I_COND, I_ARITH_LOG_MEM, DIR, LABEL, ERROR};


/* line = {"add", "$1", "$10", "$2"} */
enum expression WhatStatement(const char **statement);

bool IsCorrectR3(const char **line, char *er_buf);
bool IsCorrectR2(const char **line, char *er_buf);
bool IsCorrecJ0(const char **line, char *er_buf);
bool IsCorrectJ1(const char **line, char *er_buf);
bool IsCorrectJLABEL(const char **line, char *er_buf);
//etc




