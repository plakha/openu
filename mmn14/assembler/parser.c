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
#define COMMA_STR (",")
#define MAX_LABEL_SIZE (31)

enum {FALSE = 0, TRUE};

struct  parser /* file parser */
{
    size_t ic;

    size_t cur_line_num;
    
    const char *source_file_name;
    ram_t *ram;
    sym_tab_t *sym_tab;
    int is_file_syntax_corrupt;  /* default FALSE */
};

/******* Static function declarations *********/
static enum statement_type WhatStatement(const dvec_t *args, int is_under_label);
static int IsStringLabel(const char *str, int is_declaration);
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
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", __LINE__, __FILE__);

        return NULL;
    }

    new_parser->ic = 0;
    new_parser->source_file_name = source_file_name;
    new_parser->cur_line_num = 1;
    new_parser->ram = ram;
    new_parser->sym_tab = sym_tab;

    return new_parser;
}

void ParserDestroy(parser_t *parser, int should_destroy_passed_entities)
{
    assert(parser);

    if (should_destroy_passed_entities)
    {
        RAMDestroy(parser->ram);
        parser->ram = NULL;

        SymTabDestroy(parser->sym_tab);
        parser->sym_tab = NULL;
    }

    free(parser);
    parser = NULL;
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

static int IsStringNum(const char *str)
{
    const char *runner = str;

    assert(str);

    if ((!isdigit(*runner)) 
    && ('+' != *runner) 
    && ('-' != *runner))
    {
        return FALSE;
    }

    ++runner;

    while (*runner)
    {
        if (!isdigit(*runner))
        {
            return FALSE;
        }

        ++runner;
    }
    
    return TRUE;
}




static void UtilPushToSymTab(parser_t *parser_data, sym_tab_t *sym_tab, char *statement[])
{

}

/* e.g.  .dh 1, 2, 3,3 */
static int IsValidateDirDec(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_min_size_dir_dec = 2;
    const size_t first_elem_index = is_under_label ? 1 :0;
    const char *arg0 = NULL;
    size_t arg_arr_size = 0;
    size_t i = 0;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == DIR_DEC);

    arg_arr_size = DVECSize(arg_arr);
    arg0 = DVECGetItemAddress(arg_arr, first_elem_index);


    /*
    size >= 2
    every even element (except 0th) is ','

    every odd element is number
    last element is number

    */

    if (arg_min_size_dir_dec > arg_arr_size - first_elem_index)
    {
        printf("ERROR in source file %s, line %ld: expecting arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }

    if (!IsStringNum(DVECGetItemAddress(arg_arr, arg_arr_size - 1)))
    {
        printf("ERROR in source file %s, line %ld: missing last numerical arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }

    for (i = 1 + first_elem_index; i < arg_arr_size; i += 2)
    {
        if (!IsStringNum(DVECGetItemAddress(arg_arr, i)))
        {
            printf("ERROR in source file %s, line %ld: expecting numerical arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
        }
    }

    for (i = 2 + first_elem_index; i < arg_arr_size; i += 2)
    {
        if (strcmp(DVECGetItemAddress(arg_arr, i), COMMA_STR))
        {
            printf("ERROR in source file %s, line %ld: expected comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
        }
    }

    return TRUE;
}

/* check if the directive is .extern or .entry */
static int IsValidateDirQualif(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_dir_qualif = 2;
    const size_t first_elem_index = is_under_label ? 1 :0;
    const char *arg0 = NULL;
    size_t arg_arr_size = 0;
    size_t i = 0;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == DIR_QUALIF);

    arg_arr_size = DVECSize(arg_arr);
    arg0 = DVECGetItemAddress(arg_arr, first_elem_index);

    /* 
        First arg .extern or .entry (assertion)
        second arg is a reference to a legal label (without the ':' - terminator)

        size 2 exactrly

        if preceded by a label declaration - warning: the label won't be counted
    */

    if (is_under_label)
    {
        printf("WARNING for source file %s, line %ld: declaration of label %s is illegal and will be ignored: not allowed to label %s directive.\n", 
        parser->source_file_name, parser->cur_line_num, (const char *)DVECGetItemAddress(arg_arr, 0) , arg0);  
    }

    if (arg_size_dir_qualif != arg_arr_size - first_elem_index)
    {
        printf("ERROR in source file %s, line %ld: expecting 1 and only 1 argument for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    } 

    if (!IsStringLabel(DVECGetItemAddress(arg_arr, first_elem_index + 1), FALSE))
    {
        printf("ERROR in source file %s, line %ld: expecting legal label as argument for %s statement\n", 
        parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    } 
    
    return TRUE;
}


static int IsStringLegalAscizArg(const char *str)
{
    /*
        starts with `"`

        ends with `"`, and after it can be a number of space chars
        from `"` to `"` - printable chars only -> check all for isprint(), including subsequent space chars
    */

   size_t i = -1;
   size_t str_len = -1;

    /* length from the initial ti the final `"` */
   size_t asciz_len = -1;

    assert(str);
    assert(!isspace(*str));

    str_len = strlen(str);

    if ('\"' != *str)
    {
        return FALSE;
    }

    for(i = str_len - 1; isspace(str[i]); --i)
    {};
    if ('\"' != str[i])
    {
        return FALSE;
    }
    asciz_len = i + 1;

    for (i = 0; i < asciz_len; ++i)
    {
        if (!isprint(str))
        {
            return FALSE;
        }
    }

    return TRUE;
}

static int IsValidateDirAscii(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    /*
        exactly size 2
        if not under label - warning
        arg 1 is legal ascuz arg 
    */
    static const size_t arg_size_dir_asciz = 2;
    const size_t first_elem_index = is_under_label ? 1 :0;
    const char *arg0 = NULL;
    const char *arg1 = NULL;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == DIR_ASCII); 

    
    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);

    if (!is_under_label)
    {
        printf("WARNING for source file %s, line %ld: unlabeled  %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);
    }

    if((arg_size_dir_asciz != (DVECSize(arg_arr) - first_elem_index)) 
    || !IsStringLegalAscizArg(arg1))
    {
         printf("ERROR in source file %s, line %ld: expected 1 and only one, printable, `\"`-delimited argument for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }

    return TRUE;
}

/* e.g. add $1, $4, $1 */
static int IsValidateR3(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_r3 = 6;
    const size_t first_elem_index = is_under_label ? 1 :0;
    const char *arg0 = NULL;
    const char *arg1 = NULL;
    const char *arg2 = NULL;
    const char *arg3 = NULL;
    const char *arg4 = NULL;
    const char *arg5 = NULL;
   
    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == R_3_ARG);

    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);
    arg2 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 2);
    arg3 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 3);
    arg4 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 4);
    arg5 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 5);

    /*
        validate 6 args 
        Validate arg0 is a legal operator (add, sub, etc) - redundant (assert only)
        validate arg1 legal registry
        validate arg2 is ","
        validate arg3 legal registry
        validate arg4  is ","
        validate arg5 legal registry
    */

    if (DVECSize(arg_arr) - first_elem_index != arg_size_r3 || strcmp(arg2, COMMA_STR) || strcmp(arg4, COMMA_STR))
    {
        printf("Error in source file %s, line %ld: expected 3 comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringRegistry(arg1) || !IsStringRegistry(arg3) || !IsStringRegistry(arg5))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expecting legal registries as arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }

    return TRUE;
}

/* e.g. move/mvhi/mvlo $23, $2 */
static int IsValidateR2(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_r2 = 4; /* incl "," */
    const char *arg0 = NULL;
    const char *arg1 = NULL;
    const char *arg2 = NULL;
    const char *arg3 = NULL;
    const size_t first_elem_index = is_under_label ? 1 :0;
   
    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == R_2_ARG);

    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);
    arg2 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 2);
    arg3 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 3);

     /*
        validate 4 args 
        Validate arg0 is a legal operator (move/mvhi/mvlo) - redundant (assert only)
        validate arg1 legal registry
        validate arg2 is ","
        validate arg3 legal registry
        
    */

    if (DVECSize(arg_arr) - first_elem_index != arg_size_r2 || strcmp(arg2, COMMA_STR))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expected 2 comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringRegistry(arg1) || !IsStringRegistry(arg3))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expecting legal registries as arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

            return FALSE;
    }


    return TRUE;
}

/* stop only*/
static int IsValidateJ0(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_j0 = 1;
    const size_t first_elem_index = is_under_label ? 1 :0;
    const char *arg0 = NULL;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == J_0_ARG);


    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);

    if (DVECSize(arg_arr) - first_elem_index != arg_size_j0)
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expected 0 arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }




    return TRUE;
}

/*  la label ; call label */
static int IsValidateJ1Label(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_j1 = 2;
    const size_t first_elem_index = is_under_label ? 1 :0;

    const char *arg0 = NULL;
    const char *arg1 = NULL;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == J_1_LABEL);


    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);

    if (DVECSize(arg_arr) - first_elem_index != arg_size_j1)
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expected 1 arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringLabel(arg1, FALSE))
    {
        parser->is_file_syntax_corrupt = TRUE;
        printf("Error in source file %s, line %ld: expecting a legal label as argument for %s statement\n", 
        parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    return TRUE;
}

/* jmp only */
static int IsValidateJ1Arg(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_j1 = 2;
    const size_t first_elem_index = is_under_label ? 1 :0;

    const char *arg0 = NULL;
    const char *arg1 = NULL;

    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == J_1_ARG);

    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);

    if (DVECSize(arg_arr) - first_elem_index != arg_size_j1)
    {
        printf("Error in source file %s, line %ld: expected 1 argument for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringLabel(arg1, FALSE) && !IsStringRegistry(arg1))
    {
        printf("Error in source file %s, line %ld: expecting a legal label or register as argument for %s statement\n", 
        parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    return TRUE;
}


/*     if (!strcmp(args0, "bne") || !strcmp(args0, "beq") || !strcmp(args0, "blt") || !strcmp(args0, "bgt"))
e.g blt $5, $24, loop 
 */
static int IsValidateICond(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_i = 6;
    const size_t first_elem_index = is_under_label ? 1 : 0;

    const char *arg0 = NULL;
    const char *arg1 = NULL;
    const char *arg2 = NULL;
    const char *arg3 = NULL;
    const char *arg4 = NULL;
    const char *arg5 = NULL;
    
    /*
    arg0 - operator
    arg1 register
    arg2 comma
    arg3 register
    arg4 comma
    arg5 label

    */


    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == I_COND);

    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);
    arg2 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 2);
    arg3 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 3);
    arg4 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 4);
    arg5 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 5);

    if (DVECSize(arg_arr) - first_elem_index != arg_size_i || strcmp(arg2, COMMA_STR) || strcmp(arg4, COMMA_STR))
    {
        printf("Error in source file %s, line %ld: expected 3 comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringRegistry(arg1) || !IsStringRegistry(arg3) || !IsStringLabel(arg5, FALSE))
    {
        printf("Error in source file %s, line %ld: expecting the following order of arguments for  %s statement:  legal label, legal label, legal register\n", 
        parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }



    return TRUE;
}


/*
if (!strcmp(args0, "addi") || !strcmp(args0, "subi") || !strcmp(args0, "andi") 
    || !strcmp(args0, "ori") || !strcmp(args0, "nori") || !strcmp(args0, "lb") || !strcmp(args0, "sb") 
    || !strcmp(args0, "lw") || !strcmp(args0, "sw") || !strcmp(args0, "lh") || !strcmp(args0, "sh"))

    e.g lh $9, 34, $2

*/
static int IsValidateIArithLogMem(parser_t *parser, const dvec_t *arg_arr, int is_under_label)
{
    static const size_t arg_size_i = 6;
    const size_t first_elem_index = is_under_label ? 1 : 0;

    const char *arg0 = NULL;
    const char *arg1 = NULL;
    const char *arg2 = NULL;
    const char *arg3 = NULL;
    const char *arg4 = NULL;
    const char *arg5 = NULL;
    
    /*
    arg0 - operator
    arg1 register
    arg2 comma
    arg3 int number
    arg4 comma
    arg5 register

    */


    assert(parser);
    assert(arg_arr);
    assert(WhatStatement(arg_arr, is_under_label) == I_ARITH_LOG_MEM);

    arg0 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index);
    arg1 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 1);
    arg2 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 2);
    arg3 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 3);
    arg4 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 4);
    arg5 = (const char *)DVECGetItemAddress(arg_arr, first_elem_index + 5);

    if (DVECSize(arg_arr) - first_elem_index != arg_size_i || strcmp(arg2, COMMA_STR) || strcmp(arg4, COMMA_STR))
    {
        printf("Error in source file %s, line %ld: expected 3 comma-separated arguments for %s statement\n", 
            parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }

    if (!IsStringRegistry(arg1) || !IsStringNum(arg3) || !IsStringRegistry(arg5))
    {
        printf("Error in source file %s, line %ld: expecting the following order of arguments for  %s statement:  legal label, legal label, legal register\n", 
        parser->source_file_name, parser->cur_line_num, arg0);

        return FALSE;
    }
   
    return TRUE;
}


/* on reference, there is no ':' terminator, on declaration - there is. 
DECLAREDLABEL: []

.entry REFERENCEDLABEL
 */
static int IsStringLabel(const char *str, int is_declaration)
{
    size_t str_len = -1;
    size_t i = 0;
    /* TODO add more keywords */
    static const char *keywords[] = {"add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo", "addi", "subi", "andi", "ori", "nori", "bne", "beq", 
    "blt", "bgt", "lb", "sb", "lw", "sw", "lh", "sh", "jmp", "la", "call", "stop", NULL};
    char str_modified_copy[MAX_LABEL_SIZE] = {'\0'};
    
    assert(str);

    strcpy(str_modified_copy, str);
    if (!is_declaration)
    {
        strcat(str_modified_copy, ":");
    }

    printf("orig: %s, modif: %s, is_declaration=%d", str, str_modified_copy, is_declaration);

    str_len = strlen(str_modified_copy);

    /* str_len - 1 because label declaration ends with ':' */
    if (str_len - 1 > MAX_LABEL_SIZE)
    {
        return FALSE;
    }
/*
    if (str_modified_copy[0] < 'A' || str_modified_copy[0] > 'Z')
    {
        return FALSE;
    }
*/
    if (':' != str_modified_copy[str_len - 1])
    {
        return FALSE;
    }

    for (i = 0; i < str_len - 1; ++i)
    {
        if (!isalnum(str_modified_copy[i]))
        {
            return FALSE;
        }
    }

    for (i = 0; NULL != keywords[i] ; ++i)
    {
        const size_t keyword_len = strlen(keywords[i]);
        /* e.g. "add: [...]" is illegal label declaration. strncpm("add", "add:", 3) returns 0 */
        if ((keyword_len == (str_len - 1)) 
        && 0 == strncmp(keywords[i], str_modified_copy, keyword_len))
        {
            return FALSE;
        }
    }

    return TRUE;
    

}

static enum statement_type WhatStatement(const dvec_t *args, int is_under_label)
{
    const char *args0 = NULL;
    size_t len_args = 0;

    assert (args);

    len_args = DVECSize(args) - is_under_label ? 1 : 0;
    if (0 == len_args)
    {
        /* empty statement (e.g "MYLABEL:") */
        return ERROR;
    }

    args0 = (const char *) DVECGetItemAddress(args, is_under_label ? 1 : 0);

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

    if (!strcmp(args0, "la") || !strcmp(args0, "call"))
    {
        return J_1_LABEL;
    }

    if (!strcmp(args0, "jmp"))
    {
        return J_1_ARG;
    }

    if (!strcmp(args0, "stop"))
    {
        return J_0_ARG;
    }

    if (!strcmp(args0, ".entry") || !strcmp(args0, ".extern"))
    {
        return DIR_QUALIF;
    }

    if (!strcmp(args0, ".db") || !strcmp(args0, ".dw") || !strcmp(args0, ".dh"))
    {
        return DIR_DEC;
    }

    if (!strcmp(args0, ".asciz"))
    {
        return DIR_ASCII;
    }


    if (IsStringLabel(args0, TRUE))
    {
        return LABEL;
    }

    if (COMMENT_CHAR == *args0)
    {
        return COMMENT;
    }

    if (isspace(*args0))
    {
        #ifndef NDEBUG
        const char *space_checker = args0;
        while (*space_checker)
        {
            assert(isspace(*space_checker));
            ++space_checker;
        }
        #endif

        return BLANK;
    }

    return ERROR;

}


static void ValidateLineArgs(parser_t *parser, dvec_t *args, enum statement_type statement_type, int is_under_label)
{
    int is_line_ok = TRUE;
    
    switch (statement_type)
    {
    case R_3_ARG:
        if (!IsValidateR3(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;
    
    case R_2_ARG:
     if (!IsValidateR2(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
    break;
    
    case J_0_ARG:
        if (!IsValidateJ0(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case J_1_LABEL:
        if (!IsValidateJ1Label(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case J_1_ARG:
        if (!IsValidateJ1Arg(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case I_COND:
        if (!IsValidateICond(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case I_ARITH_LOG_MEM:
        if (!IsValidateIArithLogMem(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

        break;

    case DIR_QUALIF:
        if (!IsValidateDirQualif(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case DIR_DEC:
        if (!IsValidateDirDec(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case DIR_ASCII:
        if (!IsValidateDirAscii(parser, args, is_under_label))
        {
            is_line_ok = FALSE;
        }
        break;

    case LABEL:       
        if (is_under_label)
        {
            const char *label1 = (const char *) DVECGetItemAddress(args, 0); 
            const char *label2 = (const char *) DVECGetItemAddress(args, 1); 

            printf("ERROR in source file %s, line %ld: the label %s is followed by another label %s, which is not allowed \n", 
            parser->source_file_name, parser->cur_line_num, label1, label2);

            is_line_ok = FALSE;
        }
        else
        {
            enum statement_type labeled_statement = WhatStatement(args, TRUE); /* check statement after the label */
            
            ValidateLineArgs(parser, args, labeled_statement, TRUE);
        }
        break;

    case ERROR:
        is_line_ok = FALSE;

         printf("ERROR in source file %s, line %ld: unknown statement starting from %s\n", 
            parser->source_file_name, parser->cur_line_num, (const char *) DVECGetItemAddress(args, 0));

        break;

    default:
        assert(BLANK == statement_type || COMMENT == statement_type);

        break;
    }

    if (!parser->is_file_syntax_corrupt)
    {
        parser->is_file_syntax_corrupt = is_line_ok;
    }

}

void ParserFirstPass(parser_t *parser, dvec_t *args, size_t line_num)
{
    enum statement_type statement_type = 0;
    
    #ifndef NDEBUG
    {
        int i = 0;
       /* printf("ParserFirstPass, file line %lu\n", parser->cur_line_num);*/
        for (;i < DVECSize(args); ++i)
        {
           printf("%s ", (const char *)DVECGetItemAddress(args, i));
        }
        puts("");
    }
    #endif

    assert(parser);
    assert(args);

    (void)line_num;

    statement_type = WhatStatement(args, FALSE);

    ValidateLineArgs(parser, args, statement_type, FALSE);

    if (!parser->is_file_syntax_corrupt)
    {
        FirstPassProcessLineArgs(parser, args);
    }

    ++(parser->cur_line_num);
}

int ParserIsSyntaxCorrupt(const parser_t * parser)
{
    assert(parser);

    /* Because we only assign enum of 0 or 1 to this lvalue  */
    assert((FALSE == parser->is_file_syntax_corrupt) || (TRUE == parser->is_file_syntax_corrupt));

    return parser->is_file_syntax_corrupt;
}