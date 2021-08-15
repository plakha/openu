#include <stdlib.h> /* size_t */
#include <string.h> /* strlen() */
#include <ctype.h> /* isalnum() */
#include <assert.h> /* assert() */
#include <stdio.h> /* printf() */

#include "mmn14_types.h"
#include "sym_tab.h"
#include "ram.h"

#include "parser.h"

#define CMD_LEN (5)
enum {FALSE = 0, TRUE};

struct  parser /* file parser */
{
    size_t ic;

    size_t cur_line_num;
    
    const char *source_file_name;
    ram_t *ram;
    sym_tab_t *sym_tam;
    int is_file_syntax_corrupt;  /* default FALSE */
};

/******* Static function declarations *********/
static enum statement_type WhatStatement(const dvec_t *args);
/***********************/



parser_t *ParserCreate(const char *source_file_name, ram_t *ram, sym_tab_t *sym_tab)
{
    parser_t *new_parser = NULL;

    assert(source_file_name);
    assert(ram);
    assert(sym_tab);
    
    new_parser = malloc(sizeof(*new_parser));
    if(!new_parser)
    {
        
    }

    new_parser->ic = 0;
    new_parser->source_file_name = source_file_name;
    new_parser->cur_line_num = 0;

    return new_parser;

}

void FirstPassProcessLineArgs(parser_t *parser, dvec_t *args)
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

static int IsStringRegistry(const char *str)
{
    return StringToRegistry(str) != -1;
}




static void UtilPushToSymTab(parser_t *parser_data, sym_tab_t *sym_tab, char *statement[]);



/* e.g. add $1, $4, $1 */
static int IsValidateR3(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_r3 = 6;
    const size_t first_elem_index = is_under_label ? 1 :0;
    char *arg0 = NULL;
    char *arg1 = NULL;
    char *arg2 = NULL;
    char *arg3 = NULL;
    char *arg4 = NULL;
    char *arg5 = NULL;
    char *arg6 = NULL;
   
    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == R_3_ARG);

    arg0 = (char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);
    arg2 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 2);
    arg3 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 3);
    arg4 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 4);
    arg5 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 5);
    arg6 = (char *)DVECGetItemAddress(arg_arr, first_elem_index + 6);

    /*
        validate 6 args 
        Validate arg0 is a legal operator (add, sub, etc) - redundant (assert only)
        validate arg1 legal registry
        validate arg3 is ","
        validate arg4 legal registry
        validate arg5  is ","
        validate arg6 legal registry
    */

    if (DVECSize(arg_arr) - first_elem_index != arg_size_r3 || strcmp(arg3, ",") || strcmp(arg5, ","))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expected 3 comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringRegistry(arg1) || !IsStringRegistry(arg4) || !IsStringRegistry(arg6))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expecting legal registries as arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }

    return TRUE;
}

/* e.g. move/mvhi/mvlo $23, $2 */
static int IsValidateR2(const parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_r3 = 6;
    const size_t first_elem_index = is_under_label ? 1 :0;
   
    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == R_3_ARG);

     /*
        validate 4 args 
        Validate arg0 is a legal operator (move/mvhi/mvlo) - redundant (assert only)
        validate arg1 legal registry
        validate arg3 is ","
        validate arg4 legal registry
        validate arg5  is ","
        validate arg6 legal registry
    */

    return 1;
}


static int IsValidateJ0(const parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    return 1;
}

static int IsValidateJ1(const parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    return 1;
}

static int IsValidateICond(const parser_t *parser, const dvec_t *arg_arr, int is_under_label)
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

static enum statement_type WhatStatement(const dvec_t *args, int is_under_label)
{
    char *args0 = (char *) DVECGetItemAddress(args, is_under_label ? 1 : 0);

    if (!strcmp(args0, "add") || !strcmp(args0, "sub") || !strcmp(args0, "and") || !strcmp(args0, "or") || !strcmp(args0, "nor"))
    {
        return R_3_ARG;
    }

    if (!strcmp(args0, "move") || !strcmp(args0, "mvhi") || !strcmp(args0, "mvlo"))
    {
        return R_2_ARG;
    }


    if (!strcmp(args0, "addi") || !strcmp(args0, "subi") || !strcmp(args0, "andi") 
    || !strcmp(args0, "ori") || !strcmp(args0, "nori") || !strcmp(args0, "lb") || !strcmp(args0, "sb") 
    || !strcmp(args0, "lw") || !strcmp(args0, "sw") || !strcmp(args0, "lh") || !strcmp(args0, "sh"))
    {
        return I_ARITH_LOG_MEM;
    }

    if (!strcmp(args0, "bne") || !strcmp(args0, "beq") || !strcmp(args0, "blt") || !strcmp(args0, "bgt"))
    {
        return I_COND;
    }

    if (!strcmp(args0, "jmp") || !strcmp(args0, "la") || !strcmp(args0, "call"))
    {
        return J_ARG_LABEL;
    }

    if (!strcmp(args0, "stop"))
    {
        return J_0_ARG;
    }

    if (!strcmp(args0, ".db") || !strcmp(args0, ".dw") || !strcmp(args0, ".dh") || !strcmp(args0, ".asciz") 
    || !strcmp(args0, ".entry") || !strcmp(args0, ".extern"))
    {
        return DIR;
    }

    if (IsStringLabel(args0))
    {
        return LABEL;
    }

    return ERROR;

}


static void ValidateLineArgs(parser_t *parser, dvec_t *args, enum statement_type statement_type, int is_under_label)
{
    int is_line_ok = TRUE;
    
    switch (statement_type)
    {
    case R_3_ARG:
        if (!IsValidateR3(parser, args, FALSE))
        {
            is_line_ok = FA;
        }
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
        if (is_under_label)
        {
            const char *label1 = (const char *) DVECGetItemAddress(args, 0); 
            const char *label2 = (const char *) DVECGetItemAddress(args, 1); 

            printf("Error in source file %s, line %ld: the label %s is followed by another label %s, which is not allowed \n", 
            parser->source_file_name, parser->cur_line_num, label1, label2);
        }
        else
        {
            assert(TRUE == is_under_label);

            enum statement_type labeled_statement = WhatStatement(args, is_under_label);
            ValidateLineArgs(parser, args, labeled_statement, is_under_label);
        }
        break;

    case ERROR:
        assert(FALSE);
        break;

    default:
        break;
    }

    if (!parser->is_file_syntax_corrupt)
    {
        parser->is_file_syntax_corrupt = is_line_ok;
    }

}

void ParserFirstPass(parser_t *parser, dvec_t *args, size_t line_number)
{
    enum statement_type statement_type = 0;

    assert(parser);
    assert(args);
    assert(line_number > parser->cur_line_num);

        
    parser->cur_line_num = line_number;
    statement_type = WhatStatement(args, FALSE);

    if (ERROR == statement_type)
    {
        /* print line number: unkown statement */
        parser->is_file_syntax_corrupt = TRUE;
    }

    ValidateLineArgs(parser, args, statement_type, FALSE);

    if (!parser->is_file_syntax_corrupt)
    {
        FirstPassProcessLineArgs(parser, args);
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
