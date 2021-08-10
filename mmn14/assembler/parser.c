#include <stdlib.h> /* size_t */
#include <string.h> /* strlen() */
#include <ctype.h> /* isalnum() */
#include <assert.h> /* assert() */

#include "mmn14_types.h"
#include "sym_tab.h"
#include "ram.h"

#include "parser.h"

#define CMD_LEN (5)
enum {FALSE = 0, TRUE};

struct  parser /* file parser */
{
    size_t ic;
    

    ram_t *ram;
    sym_tab_t *sym_tam;
    int is_file_syntax_corrupt;  /* default FALSE */
};



void ProcessLineArgs(parser_t *parser, line_args_arr_t args)
{

}

/* 
    return registry index (0 to 31, incl.) 
-1 if not registry */
static int StringToRegistry(const char *str)
{
    int is_reg = TRUE;
    size_t i = 0;
    const size_t str_len = strlen(str);
    int reg = -1;

    if ('$' != str[0])
    {
        is_reg = FALSE;
    }

    for (i = 1; is_reg && str_len > i; ++i)
    {
        if (!isdigit(str[i]))
        {
            is_reg = FALSE;
        }
    }

    if (is_reg)
    {
        reg = atoi(str + 1);
    }

    return reg;
}




static void UtilPushToSymTab(parser_t *parser_data, sym_tab_t *sym_tab, char *statement[]);



/* add $1, $4, $1 */
static int IsValidateR3(const line_args_arr_t arg_arr, size_t line_num)
{
    
    return 1;
}

static int IsValidateR2(const line_args_arr_t arg_arr, size_t line_num)
{
    return 1;
}


static int IsValidateJ0(const line_args_arr_t arg_arr, size_t line_num)
{
    return 1;
}

static int IsValidateJ1(const line_args_arr_t arg_arr, size_t line_num)
{
    return 1;
}

static int IsValidateICond(const line_args_arr_t arg_arr, size_t line_num)
{
    return 1;
}




static int IsStringLabel(char *str)
{
    const size_t str_len = strlen(str);
    size_t i = 0;
    /* TODO add keywords */
    static const char *keywords[] = {"add", "addi", NULL};

    if (str[0] < 'A' || str[0] > 'Z')
    {
        return FALSE;
    }

    if (':' != str[str_len - 1])
    {
        return FALSE;
    }

    for (i = 1; i < str_len - 1; ++i)
    {
        if (!isalnum(str[i]))
        {
            return FALSE;
        }
    }

    for (i = 0; NULL != keywords + i ; ++i)
    {
        size_t keyword_len = strlen(keywords[i]);
        if (!strncmp(keywords[i], str, keyword_len) && keyword_len == (str_len + 1))
        {
            return FALSE;
        }
    }
    

    return TRUE;
    

}

static enum statement_type WhatStatement(line_args_arr_t args)
{
    if (!strcmp(args[0], "add") || !strcmp(args[0], "sub") || !strcmp(args[0], "and") || !strcmp(args[0], "or") || !strcmp(args[0], "nor"))
    {
        return R_3_ARG;
    }

    if (!strcmp(args[0], "move") || !strcmp(args[0], "mvhi") || !strcmp(args[0], "mvlo"))
    {
        return R_2_ARG;
    }


    if (!strcmp(args[0], "addi") || !strcmp(args[0], "subi") || !strcmp(args[0], "andi") 
    || !strcmp(args[0], "ori") || !strcmp(args[0], "nori") || !strcmp(args[0], "lb") || !strcmp(args[0], "sb") 
    || !strcmp(args[0], "lw") || !strcmp(args[0], "sw") || !strcmp(args[0], "lh") || !strcmp(args[0], "sh"))
    {
        return I_ARITH_LOG_MEM;
    }

    if (!strcmp(args[0], "bne") || !strcmp(args[0], "beq") || !strcmp(args[0], "blt") || !strcmp(args[0], "bgt"))
    {
        return I_COND;
    }

    if (!strcmp(args[0], "jmp") || !strcmp(args[0], "la") || !strcmp(args[0], "call"))
    {
        return J_ARG_LABEL;
    }

    if (!strcmp(args[0], "stop"))
    {
        return J_0_ARG;
    }

    if (!strcmp(args[0], ".db") || !strcmp(args[0], ".dw") || !strcmp(args[0], ".dh") || !strcmp(args[0], ".asciz") 
    || !strcmp(args[0], ".entry") || !strcmp(args[0], ".extern"))
    {
        return DIR;
    }

    if (IsStringLabel(args[0]))
    {
        return LABEL;
    }

    return ERROR;

}


static void ValidateLineArgs(parser_t *parser, line_args_arr_t args, enum statement_type statement_type, size_t line_num)
{
    int is_line_ok = TRUE;
    
    switch (statement_type)
    {
    case R_3_ARG:
        break;
    
    case R_2_ARG:
    break;
    
    case J_0_ARG:
        break;

    case J_ARG_LABEL:
        break;

    case I_COND:
        break;

    case I_ARITH_LOG_MEM:
        break;

    case DIR:
        break;

    case LABEL:
        break;

    default:
        break;
    }

    if (!parser->is_file_syntax_corrupt)
    {
        parser->is_file_syntax_corrupt = is_line_ok;
    }

}

void ParserFirstPass(parser_t *parser, line_args_arr_t args, size_t line_number)
{
    enum statement_type statement_type = WhatStatement(args);

    if (ERROR == statement_type)
    {
        /* print line number: unkown statement */
        parser->is_file_syntax_corrupt = TRUE;
    }

    ValidateLineArgs(parser, args, statement_type, line_number);

    if (!parser->is_file_syntax_corrupt)
    {
        ProcessLineArgs(parser, args);
    }
}

/*
void FirstPass(char *args[], size_t line_num, int *file_status, sym_tab_t *sym_tab)
{
    enum statement kind_of_statement = CheckStatementKind(args);

    switch (kind_of_statement)
    {
    case LABEL:
        enum statement next_kind_of_statement CheckStatementKind(args + 1);
        if (next_kind_of_statement == ERROR || next_kind_of_statement == LABEL);
        {
            *file_status = 1; 
        }
        else if (next_kind_of_statement == DIR)
        {
            *file_status = ValidateDirective(args);
        }
        else
        {
            *file_status = ValidateInstruction();
        }
        
        if (0 == file_status)
        {
            UtilPushToSymTab(sym_tab, args + 1);
            FirstPass(args + 1);
        }

        break;

    case DIR:

        break;


    case ERROR:

        break;


    
    
    default:
        if (0 == (*file_status = ValidateInstruction(args)))
        {
            PushInstructionToRam();
        }
        break;
    }
}
*/
