#include "parser.h"
#inclide "mmn14_types"

enum {FALSE = 0, TRUE};

struct  fp /* file parser
{
    /* data */
    size_t ic;
    ram_t *ram;
    sym_tab_t *sym_tam;
    int is_file_syntax_corrupt; /* default FALSE */
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

static void UtilPushToSymTab(parser_data_t *parser_data, sym_tab_t *sym_tab, char *statement[]);

static int ValidateInstruction(char *instructive[]);
static int ValidateDirective(char *directive[]]);

FPFirstPass(fp_t *fp, line_args_arr_t args)
{
    fp->is_file_syntax_corrupt = fp->is_file_syntax_corrupt ? fp->is_file_syntax_corrupt : ValidateLineArgs(args);
    if (!fp->is_file_syntax_corrupt)
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
