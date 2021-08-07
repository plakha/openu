#include <stdlib.h> /* size_t */

#include "mmn14_types.h"
#include "sym_tab.h"
#include "ram.h"

#include "parser.h"


enum {FALSE = 0, TRUE};

struct  parser /* file parser */
{
    long ic;
    
    ram_t *ram;
    sym_tab_t *sym_tam;
    int is_file_syntax_corrupt;  /* default FALSE */
};



static int IsRegistry(const char *arg)
{
    return 0;
}


static int IsLabel(const char *arg)
{
    return 0;
}

/* chael syntax of .dh, .asciz */
static int IsDirective(const char *arg)
{
    return 0;
}

static void UtilPushToSymTab(parser_t *parser_data, sym_tab_t *sym_tab, char *statement[]);

static int ValidateInstruction(char *instructive[])
{
    return 1;
}
static int ValidateDirective(char *directive[])
{
    return 1;
}

/* e.g., statement = {"add", "$1", "$10", "$2"} */
enum statement_type WhatStatement(const char **statement)
{
    return ERROR;
}

int IsCorrectR3(const char **line, char *er_buf)
{
    return 1;
}

int IsCorrectR2(const char **line, char *er_buf)
{
    return 1;
}


int IsCorrecJ0(const char **line, char *er_buf)
{
    return 1;
}

int IsCorrectJ1(const char **line, char *er_buf)
{
    return 1;
}

int IsCorrectJLABEL(const char **line, char *er_buf)
{
    return 1;
}




void FPFirstPass(parser_t *parser, line_args_arr_t args)
{
    parser->is_file_syntax_corrupt = parser->is_file_syntax_corrupt ? parser->is_file_syntax_corrupt : ValidateLineArgs(args);
    if (!parser->is_file_syntax_corrupt)
    {
        ProcessLineArgs(fp, args);
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
